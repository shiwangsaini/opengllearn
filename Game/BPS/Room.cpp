#include "Room.h"

#include <algorithm>

Hall::Hall(float X, float Y, float W, float H) : qv{ X, Y, W, H }
{
}

void Hall::FillHall(std::vector<glm::vec3>& Map, const glm::vec3& floorSize)
{
	for (float i = qv.x; i < qv.w; i++) {
		for (float j = qv.y; j < qv.h; j++) {
			Map.push_back(glm::vec3(i * floorSize.x, j * floorSize.y, 0.0f));
		}
	}
}

Room::Room(float X, float Y, float W, float H)
	: qv{ X, Y, W, H }, z(0.0f), left(nullptr), right(nullptr)
{
	HNeighbour.clear();
	VNeighbour.clear();
	std::uniform_int_distribution<> randomI(0, 3);
	roomType = static_cast<RoomType>(randomI(m_gen));
	mBound.bounds.minBounds = { X,Y,z };
	mBound.bounds.maxBounds = { W,H,z };
	mBound.center = mBound.bounds.GetCenter();
}

Room::~Room()
{
	delete left;
	delete right;
}

bool Room::Divide(const float& MinRoomWidth, const float& MinRoomHeight)
{
	std::uniform_int_distribution<int> random(1, 3);
	int margin = random(m_gen);
	float width = (qv.w - margin) - (qv.x + margin);
	float height = (qv.h - margin) - (qv.y + margin);

	if (left != nullptr)
		return false;
	if (width < MinRoomWidth || height < MinRoomHeight)
		return false;

	std::uniform_int_distribution<> randomI(0, 1);

	if (width > height) {
		if (width < 2.0f * MinRoomWidth)
			return false;

		float minSplit = MinRoomWidth / width;
		float maxSplit = 1.0f - MinRoomWidth / width;

		std::uniform_real_distribution<float> randomF(minSplit, maxSplit);
		float newMid = std::round(qv.x + randomF(m_gen) * width);
		left = new Room(qv.x, qv.y, newMid, qv.h);
		right = new Room(newMid, qv.y, qv.w, qv.h);
	}
	else {
		if (height < 2.0f * MinRoomHeight)
			return false;

		float minSplit = MinRoomHeight / height;
		float maxSplit = 1.0f - MinRoomHeight / height;

		std::uniform_real_distribution<float> randomF(minSplit, maxSplit);
		float newMid = std::round(qv.y + randomF(m_gen) * height);
		left = new Room(qv.x, qv.y, qv.w, newMid);
		right = new Room(qv.x, newMid, qv.w, qv.h);
	}

	return true;
}

void Room::Shrink(const float& MinWidth, const float& MinHeight, std::vector<BoundsCenter>& bounds)
{
	if (left == nullptr) {
		float width = qv.w - qv.x;
		float height = qv.h - qv.y;

		std::uniform_real_distribution<float> randomF(0.5, 0.8);
		float newW = std::max(width * randomF(m_gen), float(MinWidth));
		float newH = std::max(height * randomF(m_gen), float(MinHeight));
		qv.x = std::floor(qv.x + ((width - newW) / 2));
		qv.w = std::floor(qv.w - ((width - newW) / 2));
		qv.y = std::floor(qv.y + ((height - newH) / 2));
		qv.h = std::floor(qv.h - ((height - newH) / 2));

		mBound.bounds.minBounds = { qv.x,qv.y,z };
		mBound.bounds.maxBounds = { qv.w,qv.h,z };
		mBound.center = mBound.bounds.GetCenter();
		bounds.push_back(mBound);
	}
	else {
		left->Shrink(MinWidth, MinHeight, bounds);
		right->Shrink(MinWidth, MinHeight, bounds);
	}
}

void Room::GetLastRoom(std::vector<Room*>& Rooms)
{
	if (left == nullptr) {
		Rooms.push_back(this);
	}
	else {
		left->GetLastRoom(Rooms);
		right->GetLastRoom(Rooms);
	}
}
