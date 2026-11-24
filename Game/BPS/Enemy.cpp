#include "Enemy.h"

std::vector<float> cube{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0, 0,   //0
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1, 0,   //1
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1, 1,   //2
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0, 1,   //3

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0, 0,   //4
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1, 0,   //5
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    1, 1,   //6
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,    0, 1,   //7

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0, 0,   //8
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1, 0,   //9
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1, 1,   //10
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0, 1,   //11

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   0, 0,   //12
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1, 0,   //13
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1, 1,   //14
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   0, 1,   //15 

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0, 0,   //16
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   1, 0,   //17
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1, 1,   //18
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   0, 1,   //19

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0, 0,   //20
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   1, 0,   //21
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1, 1,   //22
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   0, 1,   //23
};

std::vector<unsigned> indicies{
	//// bottom
	0, 1, 2,
	2, 3, 0,

	// top
	4, 5, 6,
	6, 7, 4,

	// left
	8, 9, 10,
	10, 11, 8,

	// right
	12, 13, 14,
	14, 15, 12,

	// back
	16, 17, 18,
	18, 19, 16,

	//// front
	20, 21, 22,
	22, 23, 20
};

Enemy::Enemy(InputHandler& manager, const glm::vec3& position) : mManager(&manager), mPosition(position)
{
	std::cout <<  "complete enemy first befor calling \n";

	VB.AddDrawData(cube, GL_STATIC_DRAW);
	Buffers::Layout layout;
	layout.Push<float>(3);
	layout.Push<float>(3);
	layout.Push<float>(2);
	VB.AddVertex(layout);
	VB.Bind();
	IB.AddIndex(indicies);
	IB.bind();

	mBB.minBounds = glm::vec3(-0.5, -0.5, -0.5);
	mBB.maxBounds = glm::vec3( 0.5,  0.5,  0.5);
}

void Enemy::Render()
{
	mManager->useShader(ShaderType::ENEMY);
	mManager->updateMVP(mPosition, ShaderType::ENEMY);
	mManager->mShaders.setUniformVec3("color", color, ShaderType::ENEMY);
	Draw();
}

void Enemy::Draw()
{
	glBindVertexArray(VB.getVAO());
	glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}
