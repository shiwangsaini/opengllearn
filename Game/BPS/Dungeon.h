#pragma once

#include <vector>
#include <set>
#include <unordered_set>
#include <random>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <glm.hpp>

// ---- keep the includes from your ORIGINAL Dungeon.h for these ----------
// InputHandler, KeyBind, Room, Hall, Enemy, Model::Manager, ShaderType, imgui
#include "Room.h"
#include "Enemy.h"
// -------------------------------------------------------------------------
#include "Model/Character.h"
#include <memory>

// order must match the load order in Dungeon::Dungeon
enum ModelId : uint16_t {
	M_FLOOR, M_VWALL, M_HWALL, M_VWINDOW, M_HWINDOW,
	M_BARREL, M_CHEST, M_SPIKES, M_COUNT
};

using Cell = std::pair<int, int>;

struct Extent { glm::vec3 size{ 1.f }, center{ 0.f }; };
struct AABB   { glm::vec2 min{ 0.f }, max{ 0.f }; };

inline uint64_t CellKey(int x, int y)
{
	return (uint64_t(uint32_t(x)) << 32) | uint64_t(uint32_t(y));
}

class Dungeon
{
public:
	explicit Dungeon(InputHandler& manager);

	//
	glm::vec3 SpawnPoint() const;
	float     GroundZ() const { return mExt[M_FLOOR].center.z + mExt[M_FLOOR].size.z * 0.5f; }
	glm::vec2 MoveEntity(glm::vec2 pos, glm::vec2 delta, float radius) const;

	void Render(const float& dt);
	void keyHandle(const float& dt);

	bool OnHazard(const glm::vec3& pos) const;
	const bool GetPlayMode() const { return PlayMode; }
	void  UpdateCamera();

private:
	// ---- generation (BSP) ----
	bool  Blocked(glm::vec2 p, float radius) const;   // replaces Blocked(glm::vec2)
	float m_wallThick = 0.f;
	std::unique_ptr<Character> mHero;
	glm::vec2 mHeroDir{ 0.f };

	void DivideRooms(Room* room);
	void FindNeighbour();
	void Shrink();
	void AddHalls();
	void FillMap(Room* room, const glm::vec3& tile);
	void FillRoom(const float& x0, const float& x1, const float& y0, const float& y1, const glm::vec3& tile);

	void Generate();
	void BuildWalls();
	void BuildDecor();

	// ---- placement helpers ----
	glm::vec3 PlaceAt(int id, glm::vec2 xy, float baseZ) const;
	glm::vec2 CellCenter(Cell c) const;
	Cell      CellAt(glm::vec2 p) const;
	bool      IsFloorCell(int x, int y) const;

	// ---- collision ----
	void Move(glm::vec3 delta);

private:
	// ---- your original members (adjust types if yours differ) ----
	int                  m_minRoomSize;
	InputHandler&        m_manager;
	Enemy                mEnemy;
	Enemy                mEnemy1;
	Room                 root;
	std::vector<Room*>   rooms;
	std::vector<BoundsCenter> roombounds;   // <-- must match what Room::Shrink expects
	glm::vec3            lightPos{ 0.f, 0.f, 5.f };

	Model::Manager       mModelManager;

	// ---- generated data ----
	std::vector<glm::vec3>        mRooms;          // floor tile offsets (unique)
	std::vector<glm::vec3>        mInst[M_COUNT];  // instance offsets per model (M_FLOOR unused)
	std::vector<Cell>             mCells;          // unique floor cells
	std::set<Cell>                mHallCells;
	std::unordered_set<uint64_t>  m_Floor;
	std::vector<AABB>             mSolids;         // barrels, chests
	std::vector<AABB>             mHazards;        // spikes

	// ---- sizes ----
	Extent    mExt[M_COUNT];
	glm::vec3 m_tile{ 1.f };
	float     m_radius = 0.3f;
	bool PlayMode = false;

	std::mt19937 mRng{ std::random_device{}() };

	float CameraClearance(const glm::vec3& target, const glm::vec3& dir, float maxDist) const;
	float m_camDist = 4.0f;
};
