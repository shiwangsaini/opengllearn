#pragma once

#include <string>
#include <vector>

#include "ModelLoad.h"

class Dungeon;
class InputHandler;

// A character made of OBJ "frames": one idle model plus the frames of ONE walk cycle.
// The walk frame is chosen from the DISTANCE actually travelled, so feet never slide
// and the animation stops when you push against a wall.
class Character
{
public:
	struct Config {
		std::string              idleModel;
		std::vector<std::string> walkModels;     // frames of one full cycle, in order
		std::vector<std::string> runModels;     // frames of one full cycle, in order
		float speed        = 3.0f;               // world units / second
		float cycleLength  = 0.0f;               // distance covered by one full cycle (0 = 80% of height)
		float radius       = 0.0f;               // collision radius (0 = from model size)
		float gravity      = 20.0f;
		float jumpSpeed    = 7.0f;
		float facingOffset = 0.0f;               // radians: 0 if model faces +X, -PI/2 if it faces +Y
		float cyclesPerSecond = 1.2f;    // full walk cycles per second at normal speed
	};

	explicit Character(const Config& cfg);

	void Spawn(const glm::vec3& feetPos);

	// dir = desired world-space XY direction (any length, zero = stand still)
	void Update(const Dungeon& dungeon, glm::vec2 dir, float dt);
	void Jump();
	void Render(InputHandler& input);

	const glm::vec3& Position() const { return mPos; }   // feet centre
	float Radius() const              { return mRadius; }
	float Distance() const            { return mDistance; }
	bool  IsMoving() const            { return mMoving; }
	float Height() const { return mSize.z; }

private:
	int CurrentModel() const;

	Config         mCfg;
	Model::Manager mModels;            // own manager so the dungeon never draws these by accident
	int            mWalkFrames = 0;

	glm::vec3 mSize{ 1.f }, mCenter{ 0.f };   // measured from the idle frame, used for ALL frames
	float     mRadius = 0.3f;
	float     mCycle  = 1.0f;

	glm::vec3 mPos{ 0.f };
	float     mVelZ     = 0.f;
	float     mYaw      = 0.f;
	float     mDistance = 0.f;
	bool      mMoving   = false;
	bool      mGrounded = true;
};
