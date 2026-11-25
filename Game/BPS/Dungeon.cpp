#include "Dungeon.h"

#include "CollisionSystem/Collision.h"

unsigned int RoomSize  = 5;
unsigned int MinWidth  = 3;
unsigned int MinHeight = 3;
constexpr int HallSize = 1;

// ============================================================================
// Construction
// ============================================================================

Dungeon::Dungeon(InputHandler& manager)
	: m_minRoomSize(2 * RoomSize)
	, m_manager(manager)
	, mEnemy(manager,  manager.getCameraPos() + glm::vec3(0.f, 4.f, 0.f))
	, mEnemy1(manager, manager.getCameraPos() + glm::vec3(1.f, 4.f, 0.f))
{
	root = Room(-m_minRoomSize, -m_minRoomSize, m_minRoomSize, m_minRoomSize);

	// order must match enum ModelId. A failed load shifts every index after it,
	// so check the console for "ASSIMP:" errors.
	static const char* paths[M_COUNT] = {
		"Core/Assets/floor.obj",
		"Core/Assets/Vwall.obj",       "Core/Assets/Hwall.obj",
		"Core/Assets/VwallWindow.obj", "Core/Assets/HwallWindow.obj",
		"Core/Assets/barrel.obj",      "Core/Assets/Chest.obj",
		"Core/Assets/Spikes.obj"
	};

	for (int i = 0; i < M_COUNT; ++i) {
		mModelManager.LoadModel(paths[i]);
		Model::BoundingBox bb = mModelManager.GetModelBoundingBox(i);
		mExt[i].size   = bb.maxBounds - bb.minBounds;
		mExt[i].center = (bb.maxBounds + bb.minBounds) * 0.5f;
	}

	m_tile   = mExt[M_FLOOR].size; 
	m_wallThick = glm::max(mExt[M_VWALL].size.x, mExt[M_HWALL].size.y);
	m_radius = m_wallThick + 0.2f;

	Generate();

	mModelManager.InstanceModel(mRooms, M_FLOOR);
	
	Character::Config cfg;
	cfg.idleModel = "Core/Assets/idel.obj";
	for (int i = 1; i <= 30; ++i) {                      // however many frames you exported
		char path[96];
		std::snprintf(path, sizeof path, "Core/Assets/Models/CharacterMovement/walk%04d.obj", i);   // adjust to Blender's naming
		cfg.walkModels.push_back(path);
	}
	mHero = std::make_unique<Character>(cfg);

	mHero->Spawn(SpawnPoint());
	for (int i = 1; i < M_COUNT; ++i)
		mModelManager.InstanceModel(mInst[i], i);       // empty vectors are fine

	// put the camera at eye height above the floor surface
	const Extent& t = mExt[M_FLOOR];
	float topZ = t.center.z + t.size.z * 0.5f;
	m_manager.mCamera.mPos.z = topZ + mExt[M_VWALL].size.z * 0.8f;
}

// ============================================================================
// BSP generation
// ============================================================================

void Dungeon::DivideRooms(Room* room)
{
	if (room == nullptr) return;
	if (room->Divide(MinWidth, MinHeight)) {
		DivideRooms(room->left);
		DivideRooms(room->right);
	}
}

void Dungeon::FindNeighbour()
{
	for (auto& r : rooms) {
		for (auto& o : rooms) {
			if (r == o) continue;

			if (r->qv.w == o->qv.x) {
				if (std::max(r->qv.y, o->qv.y) < std::min(r->qv.h, o->qv.h))
					r->HNeighbour.push_back(o);
			}
			if (r->qv.h == o->qv.y) {
				if (std::max(r->qv.x, o->qv.x) < std::min(r->qv.w, o->qv.w))
					r->VNeighbour.push_back(o);
			}
		}
	}
}

void Dungeon::Shrink()
{
	roombounds.clear();
	root.Shrink(MinWidth, MinHeight, roombounds);
}

// connects rooms together (all values are in grid cells)
void Dungeon::AddHalls()
{
	for (auto& r : rooms) {
		for (auto& n : r->HNeighbour) {
			int lo = (int)std::ceil(std::max(r->qv.y, n->qv.y));
			int hi = (int)std::floor(std::min(r->qv.h, n->qv.h)) - HallSize;
			if (hi < lo) continue;

			std::uniform_int_distribution<int> d(lo, hi);
			int Y = d(r->m_gen);
			r->HHall.push_back(Hall(r->qv.w, Y, n->qv.x, Y + HallSize));
		}
		for (auto& n : r->VNeighbour) {
			int lo = (int)std::ceil(std::max(r->qv.x, n->qv.x));
			int hi = (int)std::floor(std::min(r->qv.w, n->qv.w)) - HallSize;
			if (hi < lo) continue;

			std::uniform_int_distribution<int> d(lo, hi);
			int X = d(r->m_gen);
			r->VHall.push_back(Hall(X, r->qv.h, X + HallSize, n->qv.y));
		}
	}
}

void Dungeon::FillMap(Room* room, const glm::vec3& tile)
{
	if (room == nullptr) return;

	if (room->left == nullptr && room->right == nullptr) {
		FillRoom(room->qv.x, room->qv.w, room->qv.y, room->qv.h, tile);
		return;
	}
	FillMap(room->left,  tile);
	FillMap(room->right, tile);
}

// rooms are stored in grid cells; multiply by the tile size for world space
void Dungeon::FillRoom(const float& x0, const float& x1, const float& y0, const float& y1, const glm::vec3& tile)
{
	for (int x = (int)x0; x < (int)x1; ++x)
		for (int y = (int)y0; y < (int)y1; ++y)
			mRooms.push_back(glm::vec3(x * tile.x, y * tile.y, 0.f));
}

void Dungeon::Generate()
{
	DivideRooms(&root);

	rooms.clear();
	root.GetLastRoom(rooms);

	FindNeighbour();
	Shrink();
	AddHalls();

	mRooms.clear();
	FillMap(&root, m_tile);
	const size_t roomCount = mRooms.size();

	for (auto& room : rooms) {
		for (auto& h : room->HHall) h.FillHall(mRooms, m_tile);
		for (auto& h : room->VHall) h.FillHall(mRooms, m_tile);
	}

	// dedupe into grid cells (rooms and halls overlap) and remember hall cells
	std::set<Cell> cells;
	mHallCells.clear();
	for (size_t i = 0; i < mRooms.size(); ++i) {
		Cell c{ (int)std::round(mRooms[i].x / m_tile.x),
		        (int)std::round(mRooms[i].y / m_tile.y) };
		cells.insert(c);
		if (i >= roomCount) mHallCells.insert(c);
	}
	mCells.assign(cells.begin(), cells.end());

	mRooms.clear();
	m_Floor.clear();
	for (auto& c : mCells) {
		mRooms.push_back(glm::vec3(c.first * m_tile.x, c.second * m_tile.y, 0.f));
		m_Floor.insert(CellKey(c.first, c.second));
	}

	for (auto& v : mInst) v.clear();
	mSolids.clear();
	mHazards.clear();

	BuildWalls();
	BuildDecor();
}

// ============================================================================
// Placement helpers
// ============================================================================

// offset that puts the model's bbox center on xy and its bottom on baseZ
glm::vec3 Dungeon::PlaceAt(int id, glm::vec2 xy, float baseZ) const
{
	const Extent& e = mExt[id];
	return { xy.x - e.center.x,
	         xy.y - e.center.y,
	         baseZ - (e.center.z - e.size.z * 0.5f) };
}

glm::vec2 Dungeon::CellCenter(Cell c) const
{
	const Extent& t = mExt[M_FLOOR];
	return { c.first  * m_tile.x + t.center.x,
	         c.second * m_tile.y + t.center.y };
}

Cell Dungeon::CellAt(glm::vec2 p) const
{
	const Extent& t = mExt[M_FLOOR];
	return { (int)std::floor((p.x - t.center.x) / m_tile.x + 0.5f),
	         (int)std::floor((p.y - t.center.y) / m_tile.y + 0.5f) };
}

bool Dungeon::IsFloorCell(int x, int y) const
{
	return m_Floor.count(CellKey(x, y)) > 0;
}

// ============================================================================
// Walls and windowed walls
// ============================================================================

void Dungeon::BuildWalls()
{
	const Extent& t = mExt[M_FLOOR];
	const Extent& v = mExt[M_VWALL];   // runs along Y
	const Extent& h = mExt[M_HWALL];   // runs along X

	const float topZ = t.center.z + t.size.z * 0.5f;
	const int   levels = 2;
	const float windowChance = 0.2f;

	// how many wall pieces are needed to cover one tile edge
	auto segCount = [](float edge, float seg) {
		return glm::max(1, (int)std::round(edge / glm::max(seg, 0.001f)));
		};
	const int   nV = segCount(t.size.y, v.size.y);   // pieces on a left/right edge
	const int   nH = segCount(t.size.x, h.size.x);   // pieces on a top/bottom edge
	const float stepV = t.size.y / nV;                  // spacing so pieces tile the edge exactly
	const float stepH = t.size.x / nH;

	std::cout << "tile " << t.size.x << "x" << t.size.y
		<< "  Vwall len " << v.size.y << " -> " << nV << " pieces"
		<< "  Hwall len " << h.size.x << " -> " << nH << " pieces\n";

	std::uniform_real_distribution<float> roll(0.f, 1.f);

	auto addWall = [&](bool vertical, glm::vec2 xy) {
		const Extent& plain = vertical ? v : h;
		bool window = roll(mRng) < windowChance;
		for (int k = 0; k < levels; ++k) {
			int id = vertical ? M_VWALL : M_HWALL;
			if (window && k == 1)
				id = vertical ? M_VWINDOW : M_HWINDOW;
			mInst[id].push_back(PlaceAt(id, xy, topZ + k * plain.size.z));
		}
		};

	for (const Cell& cell : mCells) {
		const int cx = cell.first, cy = cell.second;
		const glm::vec2 c = CellCenter(cell);

		const float xr = c.x + (t.size.x - v.size.x) * 0.5f;   // right edge, inset by thickness
		const float xl = c.x - (t.size.x - v.size.x) * 0.5f;
		const float yt = c.y + (t.size.y - h.size.y) * 0.5f;   // top edge
		const float yb = c.y - (t.size.y - h.size.y) * 0.5f;

		const bool right = !IsFloorCell(cx + 1, cy);
		const bool left = !IsFloorCell(cx - 1, cy);
		const bool up = !IsFloorCell(cx, cy + 1);
		const bool down = !IsFloorCell(cx, cy - 1);

		// left/right edges: repeat along Y
		for (int i = 0; i < nV; ++i) {
			float y = c.y - t.size.y * 0.5f + stepV * (i + 0.5f);
			if (right) addWall(true, { xr, y });
			if (left)  addWall(true, { xl, y });
		}
		// top/bottom edges: repeat along X
		for (int i = 0; i < nH; ++i) {
			float x = c.x - t.size.x * 0.5f + stepH * (i + 0.5f);
			if (up)   addWall(false, { x, yt });
			if (down) addWall(false, { x, yb });
		}
	}
}

// ============================================================================
// Decorations (barrels, chests, spikes)
// ============================================================================

void Dungeon::BuildDecor()
{
	const Extent& t = mExt[M_FLOOR];
	const float topZ = t.center.z + t.size.z * 0.5f;

	constexpr float propsPerRoom = 1.5f;

	struct Rule { ModelId id; float weight; bool wallHug, solid, hazard; };
	static const Rule rules[] = {
		{ M_BARREL, 0.5f, true,  true,  false },
		{ M_CHEST,  0.2f, true,  true,  false },
		{ M_SPIKES, 0.3f, false, false, true  },
	};
	constexpr int nRules = (int)(sizeof(rules) / sizeof(rules[0]));
	static const int D[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

	// ---- cell inspection -------------------------------------------------
	struct Info { bool nearHall; int n8; int wallCount; int wallDir; };

	auto inspect = [&](int cx, int cy) {
		Info i{ false, 0, 0, -1 };
		for (int dx = -1; dx <= 1; ++dx)
			for (int dy = -1; dy <= 1; ++dy) {
				if (mHallCells.count({ cx + dx, cy + dy })) i.nearHall = true;
				if ((dx || dy) && IsFloorCell(cx + dx, cy + dy)) ++i.n8;
			}
		for (int d = 0; d < 4; ++d)
			if (!IsFloorCell(cx + D[d][0], cy + D[d][1])) { ++i.wallCount; i.wallDir = d; }
		return i;
		};

	auto valid = [](const Rule& r, const Info& i) {
		if (i.nearHall) return false;                                   // keep doorways clear
		return r.wallHug ? (i.wallCount == 1 && i.n8 >= 5)              // straight wall, not a corner
			: (i.n8 == 8);                                 // fully interior
		};

	auto place = [&](const Rule& r, Cell cell, const Info& info) {
		const Extent& e = mExt[r.id];
		glm::vec2 xy = CellCenter(cell);

		if (r.wallHug) {
			bool  alongX = D[info.wallDir][0] != 0;
			float thick = alongX ? mExt[M_VWALL].size.x : mExt[M_HWALL].size.y;
			float push = alongX ? (t.size.x * 0.5f - thick - e.size.x * 0.5f)
				: (t.size.y * 0.5f - thick - e.size.y * 0.5f);
			xy += glm::vec2((float)D[info.wallDir][0], (float)D[info.wallDir][1]) * glm::max(push, 0.f);
		}

		mInst[r.id].push_back(PlaceAt(r.id, xy, topZ));

		glm::vec2 half(e.size.x * 0.5f, e.size.y * 0.5f);
		if (r.solid)  mSolids.push_back({ xy - half, xy + half });
		if (r.hazard) mHazards.push_back({ xy - half, xy + half });
		};

	// ---- weighted type pick + random valid cell --------------------------
	std::vector<float> weights;
	for (const Rule& r : rules) weights.push_back(r.weight);
	std::discrete_distribution<int> pickType(weights.begin(), weights.end());

	std::set<Cell> used;                                                // one prop per cell

	auto tryPlace = [&](const std::vector<Cell>& cells) -> bool {
		int first = pickType(mRng);
		for (int k = 0; k < nRules; ++k) {                              // fall back to other types
			const Rule& r = rules[(first + k) % nRules];

			std::vector<std::pair<Cell, Info>> cand;
			for (const Cell& c : cells) {
				if (used.count(c)) continue;
				Info i = inspect(c.first, c.second);
				if (valid(r, i)) cand.push_back({ c, i });
			}
			if (cand.empty()) continue;

			std::uniform_int_distribution<size_t> d(0, cand.size() - 1);
			const auto& pick = cand[d(mRng)];
			place(r, pick.first, pick.second);
			used.insert(pick.first);
			return true;
		}
		return false;
		};

	// ---- floor cells that belong to each room ----------------------------
	std::vector<std::vector<Cell>> roomCells;
	for (Room* r : rooms) {
		std::vector<Cell> cs;
		for (int x = (int)r->qv.x; x < (int)r->qv.w; ++x)
			for (int y = (int)r->qv.y; y < (int)r->qv.h; ++y)
				if (IsFloorCell(x, y)) cs.push_back({ x, y });
		roomCells.push_back(std::move(cs));
	}

	// ---- distribute: 1,2,1,2... gives floor(1.5 * rooms) in total --------
	int pending = 0;                                                    // props a room couldn't take
	for (size_t i = 0; i < roomCells.size(); ++i) {
		int quota = (int)(propsPerRoom * (i + 1)) - (int)(propsPerRoom * i) + pending;
		pending = 0;
		for (int q = 0; q < quota; ++q)
			if (!tryPlace(roomCells[i])) ++pending;
	}

	// leftovers: spread over any room that still has space
	bool progress = true;
	while (pending > 0 && progress) {
		progress = false;
		for (size_t i = 0; i < roomCells.size() && pending > 0; ++i)
			if (tryPlace(roomCells[i])) { --pending; progress = true; }
	}

	std::cout << "props placed: " << used.size() << " / target "
		<< (int)(propsPerRoom * rooms.size()) << " (" << rooms.size() << " rooms)\n";
}

// ============================================================================
// Collision
// ============================================================================

glm::vec3 Dungeon::SpawnPoint() const
{
	Cell c = mCells.empty() ? Cell{ 0, 0 } : mCells[0];
	if (!rooms.empty()) {
		Room* r = rooms[0];
		Cell mid{ (int)((r->qv.x + r->qv.w) * 0.5f), (int)((r->qv.y + r->qv.h) * 0.5f) };
		if (IsFloorCell(mid.first, mid.second)) c = mid;
	}
	glm::vec2 p = CellCenter(c);
	return { p.x, p.y, GroundZ() };
}

// the entity never gets closer to an edge than a wall is thick
bool Dungeon::Blocked(glm::vec2 p, float radius) const
{
	const float r = glm::max(radius, m_wallThick);
	const glm::vec2 corners[4] = {
		{ p.x - r, p.y - r }, { p.x + r, p.y - r },
		{ p.x - r, p.y + r }, { p.x + r, p.y + r }
	};

	// every corner of the player's box must be over a floor cell (walls + void)
	for (const auto& q : corners) {
		Cell c = CellAt(q);
		if (!IsFloorCell(c.first, c.second)) return true;
	}

	// solid props
	for (const AABB& s : mSolids)
		if (p.x > s.min.x - r && p.x < s.max.x + r &&
		    p.y > s.min.y - r && p.y < s.max.y + r)
			return true;

	return false;
}

glm::vec2 Dungeon::MoveEntity(glm::vec2 pos, glm::vec2 delta, float radius) const
{
	if (!Blocked({ pos.x + delta.x, pos.y }, radius)) pos.x += delta.x;
	if (!Blocked({ pos.x, pos.y + delta.y }, radius)) pos.y += delta.y;
	return pos;
}

// Move(): use the shared function
void Dungeon::Move(glm::vec3 delta)
{
	glm::vec3& pos = m_manager.mCamera.mPos;
	glm::vec2 np = MoveEntity({ pos.x, pos.y }, { delta.x, delta.y }, m_radius);
	pos.x = np.x;
	pos.y = np.y;
}

bool Dungeon::OnHazard(const glm::vec3& pos) const
{
	for (const AABB& s : mHazards)
		if (pos.x > s.min.x && pos.x < s.max.x &&
		    pos.y > s.min.y && pos.y < s.max.y)
			return true;
	return false;
}

// how far back the camera can go before a wall gets in the way
float Dungeon::CameraClearance(const glm::vec3& target, const glm::vec3& dir, float maxDist) const
{
	const float step = 0.1f;
	float d = 0.f;
	for (float t = step; t <= maxDist; t += step) {
		glm::vec3 p = target + dir * t;
		if (Blocked({ p.x, p.y }, 0.15f)) break;
		d = t;
	}
	return d;
}

void Dungeon::UpdateCamera()
{
	auto& cam = m_manager.mCamera;

	// look at the head, not the feet
	glm::vec3 target = mHero->Position() + glm::vec3(0.f, 0.f, mHero->Height() * 0.85f);
	glm::vec3 back = -glm::normalize(cam.mFront);

	float dist = glm::max(CameraClearance(target, back, m_camDist), 0.3f);
	glm::vec3 p = target + back * dist;
	p.z = glm::max(p.z, GroundZ() + 0.2f);          // never go under the floor
	cam.mPos = p;
}

// ============================================================================
// Render / input
// ============================================================================

void Dungeon::Render(const float& dt)
{
	m_manager.useShader(ShaderType::OBJECT);
	m_manager.mShaders.setUniformVec3("light.diffusion", glm::vec3(0.4f), ShaderType::OBJECT);
	m_manager.mShaders.setUniformVec3("light.specular",  glm::vec3(1.0f), ShaderType::OBJECT);
	m_manager.mShaders.setUniformVec3("light.ambient",   glm::vec3(0.3f), ShaderType::OBJECT);
	m_manager.mShaders.setUniformVec3("light.position",  lightPos,        ShaderType::OBJECT);
	m_manager.mShaders.setUniform1Float("material.shininess", 16.f, ShaderType::OBJECT);
	m_manager.mShaders.setUniformVec3("uViewPos", m_manager.getCameraPos(), ShaderType::OBJECT);

	ImGui::Begin("Hey!!");
	ImGui::SliderFloat3("Enemy pos 1:",   &mEnemy.mPosition.x,  -11.f, 11.f);
	ImGui::SliderFloat3("Enemy pos 2:",   &mEnemy1.mPosition.x, -11.f, 11.f);
	ImGui::SliderFloat3("Enemy color 1:", &mEnemy.color.x,  0.f, 1.f);
	ImGui::SliderFloat3("Enemy color 2:", &mEnemy1.color.x, 0.f, 1.f);
	ImGui::Checkbox("PLAY MODE: ", &PlayMode);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
	            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	mHero->Update(*this, mHeroDir, dt);
	mHeroDir = glm::vec2(0.f);
	if(PlayMode)
		UpdateCamera();

	m_manager.useShader(ShaderType::OBJECT);

	m_manager.updateMVP(glm::vec3(0.0), ShaderType::OBJECT);
	mModelManager.DrawInstance(m_manager.mShaders, ShaderType::OBJECT);
	mHero->Render(m_manager);

	if (Collision::Detect(mEnemy.GetBoundingBox(),  mEnemy.mPosition,
	                      mEnemy1.GetBoundingBox(), mEnemy1.mPosition)) {
		mEnemy.color  = { 1.f, 0.f, 0.f };
		mEnemy1.color = { 1.f, 0.f, 0.f };
	} else {
		mEnemy.color  = { 0.f, 1.f, 0.f };
		mEnemy1.color = { 0.f, 1.f, 0.f };
	}
	mEnemy.Render();
	mEnemy1.Render();

	if (OnHazard(m_manager.mCamera.mPos)) {
		// TODO: damage the player
	}
}



void Dungeon::keyHandle(const float& dt)
{
	const float Speed = 3.0f * dt;

	auto fwd = [this]() {
		glm::vec2 f(m_manager.mCamera.mFront.x, m_manager.mCamera.mFront.y);
		return glm::length(f) > 1e-4f ? glm::normalize(f) : glm::vec2(0.f, 1.f);
		};
	KeyBind::BindKeyOnHold(GLFW_KEY_W, [this, fwd]() { mHeroDir += fwd(); });
	KeyBind::BindKeyOnHold(GLFW_KEY_S, [this, fwd]() { mHeroDir -= fwd(); });
	KeyBind::BindKeyOnHold(GLFW_KEY_A, [this, fwd]() { auto f = fwd(); mHeroDir += glm::vec2(-f.y, f.x); });
	KeyBind::BindKeyOnHold(GLFW_KEY_D, [this, fwd]() { auto f = fwd(); mHeroDir += glm::vec2(f.y, -f.x); });
	KeyBind::BindKeyPress(GLFW_KEY_SPACE, [this]() {mHero->Jump(); });
	KeyBind::BindKeyPress(GLFW_KEY_ENTER, [this]() {
		const glm::vec3& e = mEnemy.mPosition;
		const glm::vec3& c = m_manager.mCamera.mPos;
		std::cout << "\nEnemy Pos x: " << e.x << ", y:" << e.y << ", z:" << e.z;
		std::cout << "\nCam Pos   x: " << c.x << ", y:" << c.y << ", z:" << c.z;
	});

	KeyBind::BindKeyPress(GLFW_KEY_ESCAPE, [this]() { 
		if (m_manager.isCursorEnable()) {
			m_manager.CursorOn(false);
		} else
			m_manager.CursorOn(true);

		});
}
