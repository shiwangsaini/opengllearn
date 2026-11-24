#pragma once

#include <random>

#include "InputHandler.h"
#include "Model/struct.h"

// Types of Room
enum RoomType {
	BOSS = 0,
	PROFFESSION = 1,
	CLASS = 2,
	UPGRADE = 3
};

struct QuardVerticies{
	float x = 0.0f;
	float y = 0.0f;
	float w = 0.0f;
	float h = 0.0f;
};

struct BoundsCenter {
	glm::vec3 center;
	Model::BoundingBox bounds;
};

struct Hall
{
public:
	Hall() = default;
	Hall(float X, float Y, float W, float H);

	void FillHall(std::vector<glm::vec3>& Map, const glm::vec3& floorSize);
	QuardVerticies qv;
};

struct Room
{
	Room() = default;
	Room(float X, float Y, float W, float H);
	~Room();

	RoomType roomType;
	Room* left = nullptr;
	Room* right = nullptr;
	std::vector<Room*> HNeighbour;
	std::vector<Room*> VNeighbour;
	std::vector<Hall>  HHall;
	std::vector<Hall>  VHall;
	BoundsCenter GetBoundsCenter() const {return mBound;}
	bool Divide(const float& MinRoomWidth, const float& MinRoomHeight);
	void Shrink(const float& MinWidth, const float& MinHeight, std::vector<BoundsCenter>& bounds);
	void GetLastRoom(std::vector<Room*>& Rooms);

	QuardVerticies qv;
	float z = 0.0f;
	BoundsCenter mBound;

	std::mt19937 m_gen{ std::random_device{}() };

	// Operators;
	bool operator==(const Room& other) const {
		return  (other.qv.x == this->qv.x && other.qv.y == this->qv.y &&
			other.qv.w == this->qv.w && other.qv.h == this->qv.h &&
			other.z == this->z);
	}

};
