#pragma once

#include "InputHandler.h"
#include "CollisionSystem/Collision.h"


// Enemy!!!!!!
class Enemy
{
public:
	Enemy() = delete;
	Enemy(InputHandler& manager, const glm::vec3& position);
	~Enemy() {}

	void Render();
	const Model::BoundingBox& GetBoundingBox() const { return mBB; }
	// Mesh
	glm::vec3 mPosition;
	glm::vec3 color{0.3f, 0.45f, 0.5f};

private:
	Buffers::Index IB;
	Buffers::Vertex VB;
	InputHandler* mManager;
	Model::BoundingBox mBB;
	void Draw();
	//movement, tracking ,Actions
	// Bind it to a room
	// Room* room = nullptr
	//AI
};

