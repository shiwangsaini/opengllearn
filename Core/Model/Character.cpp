#include "Character.h"
#include "BPS/Dungeon.h"      // pulls in InputHandler, ShaderType, glm

#include <algorithm>
#include <cmath>
#include <iostream>

Character::Character(const Config& cfg)
	: mCfg(cfg)
{
	// model 0 = idle, models 1..N = walk frames
	mModels.LoadModel(cfg.idleModel);
	for (const auto& p : cfg.walkModels)
		mModels.LoadModel(p);
	mWalkFrames = (int)cfg.walkModels.size();

	if (mModels.ModelCount() != (size_t)(1 + mWalkFrames))
		std::cerr << "Character: some frames failed to load, frame indices are shifted\n";

	// every frame uses the idle frame's extents so the feet never jump between frames
	Model::BoundingBox bb = mModels.GetModelBoundingBox(0);
	mSize   = bb.maxBounds - bb.minBounds;
	mCenter = (bb.maxBounds + bb.minBounds) * 0.5f;

	mRadius = cfg.radius      > 0.f ? cfg.radius      : 0.5f * std::max(mSize.x, mSize.y);
	mCycle = cfg.cycleLength > 0.f
		? cfg.cycleLength
		: cfg.speed / std::max(cfg.cyclesPerSecond, 0.05f);   // independent of model scale

	std::cout << "Character: size " << mSize.x << " " << mSize.y << " " << mSize.z
		<< "  cycle " << mCycle << "\n";

	for (int i = 0; i <= mWalkFrames; ++i)
		mModels.InstanceModel(glm::vec3(0.f), i);
}

void Character::Spawn(const glm::vec3& feetPos)
{
	mPos      = feetPos;
	mVelZ     = 0.f;
	mDistance = 0.f;
	mMoving   = false;
	mGrounded = true;
}

void Character::Jump()
{
	if (!mGrounded) return;
	mVelZ     = mCfg.jumpSpeed;
	mGrounded = false;
}

void Character::Update(const Dungeon& dungeon, glm::vec2 dir, float dt)
{
	// ---------------- horizontal: walls, props, edge of the map ----------------
	mMoving = false;
	const float len = glm::length(dir);
	if (len > 1e-4f) {
		dir /= len;

		// split big steps so a slow frame can't tunnel through a wall
		const float total   = mCfg.speed * dt;
		const float maxStep = std::max(mRadius * 0.5f, 0.01f);
		const int   steps   = std::max(1, (int)std::ceil(total / maxStep));
		const glm::vec2 stepDelta = dir * (total / steps);

		float moved = 0.f;
		for (int i = 0; i < steps; ++i) {
			glm::vec2 cur(mPos.x, mPos.y);
			glm::vec2 nxt = dungeon.MoveEntity(cur, stepDelta, mRadius);
			moved += glm::length(nxt - cur);
			mPos.x = nxt.x;
			mPos.y = nxt.y;
		}

		// distance only grows by what was ACTUALLY travelled
		if (moved > 1e-5f) {
			mMoving    = true;
			mDistance += moved;
		}
		// face the direction you are trying to go, even when pressed against a wall
		mYaw = std::atan2(dir.x, -dir.y) + mCfg.facingOffset;
	}

	// ---------------- vertical: floor ----------------
	// MoveEntity keeps us over floor cells, so the ground height is the floor top.
	const float ground = dungeon.GroundZ();
	mVelZ  -= mCfg.gravity * dt;
	mPos.z += mVelZ * dt;
	if (mPos.z <= ground) {
		mPos.z    = ground;
		mVelZ     = 0.f;
		mGrounded = true;
	} else {
		mGrounded = false;
	}
}

// idle -> model 0, walking -> model 1..N picked from distance travelled
int Character::CurrentModel() const
{
	if (!mMoving || !mGrounded || mWalkFrames == 0) return 0;

	const float cycle = std::max(mCycle, 0.001f);
	float phase = std::fmod(mDistance, cycle) / cycle;           // 0..1 through the cycle
	int   frame = std::min((int)(phase * mWalkFrames), mWalkFrames - 1);
	return 1 + frame;
}

void Character::Render(InputHandler& input)
{
	static int last = -1;
	const int id = CurrentModel();
	if (id != last) {
		std::cout << "frame " << id << " moving=" << mMoving
			<< " grounded=" << mGrounded
			<< " walkFrames=" << mWalkFrames
			<< " dist=" << mDistance << "\n";
		last = id;
	}

	// put the feet centre on mPos
	glm::vec3 offset(mPos.x - mCenter.x,
	                 mPos.y - mCenter.y,
	                 mPos.z - (mCenter.z - mSize.z * 0.5f));
	mModels.InstanceModel(offset, id);

	// yaw is applied in the vertex shader (instances are position-only)
	input.mShaders.setUniform1Float("uYaw", mYaw, ShaderType::OBJECT);
	input.mShaders.setUniformVec3("uPivot", glm::vec3(mCenter.x, mCenter.y, 0.f), ShaderType::OBJECT);

	mModels.DrawInstance(input.mShaders, ShaderType::OBJECT, id);

	input.mShaders.setUniform1Float("uYaw", 0.f, ShaderType::OBJECT);   // don't rotate the dungeon next frame
}
