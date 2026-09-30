#include "Dungeon.h"

unsigned int RoomSize = 5; // in -ve and +ve

Room::Room(int X, int Y, int W, int H)
	: x(X), y(Y), w(W), h(H), z(0)
{
}

Room::~Room()
{
}

Dungeon::Dungeon(InputHandler* manager) 
	: m_roomNum(10), m_minRoomSize(2 * RoomSize), m_manager(manager)
{
	m_Texture.CreateTexture("Game/BPS/wall.jpg");

	VB.BindObj();
	VB.BindLight();
	m_manager->mShaders[ShaderType::OBJECT].use();
	m_manager->mShaders[ShaderType::OBJECT].setUniform1Int("uTexture", 0);

	m_Texture.bind();
	IB.bind();

	root = Room(-m_minRoomSize, -m_minRoomSize, m_minRoomSize, m_minRoomSize );
	FillRoom();
}

bool collisionDetect(const glm::vec3& CamPos, const glm::vec3& objPos, const float& objSize) {

	glm::vec3 pos = CamPos - objPos;

	if ((pos.x <= objSize && pos.x >= -objSize)
		&& (pos.y <= objSize && pos.y >= -objSize)
		&& (pos.z <= objSize && pos.z >= -objSize)
		)
	{
		return true;
	}
	return false;
}

void Dungeon::Render()
{
	m_manager->useShader(ShaderType::OBJECT);
	m_manager->mShaders[ShaderType::OBJECT].setUniformVec3("lightColor", glm::vec3(1.0, 0.4, 0.0));
	m_manager->mShaders[ShaderType::OBJECT].setUniformVec3("uLightPos", m_manager->getCameraPos());
	m_manager->mShaders[ShaderType::OBJECT].setUniformVec3("uViewPos", m_manager->getCameraPos());

	for (auto r : m_map) {
		m_manager->useShader(ShaderType::OBJECT);
		if (collisionDetect(m_manager->getCameraPos(), r, 1.0f)) {
			m_manager->mCamera.mPos.z = 1.0f;
		}
		else
			m_manager->mShaders[ShaderType::OBJECT].setUniformVec3("objectColor", glm::vec3(1.0, 1.0, 1.0));
		m_manager->updateMVP(r, ShaderType::OBJECT);
		Draw(ShaderType::OBJECT);
	}

	m_manager->useShader(ShaderType::LIGHT);
	m_manager->updateMVP(m_manager->mCamera.mPos, ShaderType::LIGHT);
	//Draw(ShaderType::LIGHT);
}

void Dungeon::Draw(const ShaderType& type)
{
	if (type == ShaderType::OBJECT) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Texture.GetTextureID(0));
		glBindVertexArray(VB.getVAO());
	} else
		glBindVertexArray(VB.getLightVAO());

	glDrawArrays(GL_TRIANGLES, 0, 36);
}


void Dungeon::FillRoom()
{
	for (int x = root.x; x < root.w; x++) {

		for (int y = root.y; y < root.h; y++) {
			if (x == root.w / 2 && y == root.y) {
				m_map.push_back(glm::vec3(x, y, 0));
				continue; 
			}
			if ((x == root.x) || (y == root.y) || (x == root.w - 1) || (y == root.h - 1)) {
				for(int z = 1; z < 4; z++)
					m_map.push_back(glm::vec3(x, y, z));
				continue;
			}
			m_map.push_back(glm::vec3(x, y, 0));
		}
	}
}
