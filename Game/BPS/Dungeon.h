#pragma once

#include "InputHandler.h"
#include "Texture.h"

struct Room
{
public:
	Room() = default;
	Room(int X, int Y, int W, int H);
	~Room();

	int x;
	int y;
	int w;
	int h;
	int z;
};

class Dungeon
{
public:
	Dungeon() = default;
	Dungeon(InputHandler* manager);

	Room root;
	unsigned int m_roomNum;
	int m_minRoomSize;

	void Render();
	void Draw(const ShaderType& type);

private:
	void FillRoom();

private:
	VertexBuffer VB;
	IndexBuffer IB;
	Texture m_Texture;

	glm::vec3 lightPos{ 3.0f, 3.0f, 4.0f };

	std::vector<glm::vec3> m_map;
	InputHandler* m_manager = nullptr;
};

