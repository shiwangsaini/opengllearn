#include "flap.h"

#include <iostream>
#include <random>


FlapBird::FlapBird(InputHandler* inHandle)
    : m_inputs(inHandle), m_obstecleMaxDistance(8), m_obstecleMinDistance(2), m_obstecleMaxHeight(10),
    m_obstecleMinHeight(6), m_camPos(glm::vec3(-10,10,10))
{
    m_inputs->SetCameraPos(m_camPos);
    m_bird = glm::vec3(m_camPos.x, m_camPos.y, 0.0f);
    FillObstacles(m_map);
}

Obstacles FlapBird::MakeObstacle()
{
    Obstacles obs;

    std::uniform_int_distribution<> bottom(m_obstecleMinHeight, m_obstecleMaxHeight);
    obs.bottom = bottom(m_gen);

    int gap = 5;
    obs.top = obs.bottom + gap;

    std::uniform_int_distribution<> nextDis(6, m_obstecleMaxDistance);
    obs.nextDistance = nextDis(m_gen);

    return obs;
}

void fillBottomToTop(float x, int bottomMax, int topMin, std::vector<glm::vec3>& data) {
    for (int y = 0; y < bottomMax; y++)
    {
        data.push_back(glm::vec3(x, y, 0.0f));
    }

    for (int y = topMin; y < 20; y++)
    {
        data.push_back(glm::vec3(x, y, 0.0f));
    }
}
void FlapBird::FillObstacles(std::vector<glm::vec3>& data)
{
    data.clear();

    int maxHeight = 20;
    int worldWidth = 100;

    for (int x = 0; x < worldWidth;)
    {
        Obstacles obs = MakeObstacle();
        fillBottomToTop( static_cast<float>(x), obs.bottom, obs.top, data);
        x += obs.nextDistance;
    }
}

bool isInJump = false;
bool JumpAgain = false;
bool isInFall = false;
float jumpHeight = 2.0f;
float jumpDis = 2.0f;
void FlapBird::jump(bool& isJumping, bool& againJump, float& dt)
{
    if (againJump) {
        jumpHeight = 2.0f;
        againJump = false;
    }

    float vel = 2.0f * dt;
    if (isJumping && jumpHeight >= 0.0) {
        m_bird.y += vel * jumpHeight;
        jumpHeight -= vel * jumpDis;
        return;
    }
    else {
        jumpHeight = 2.0f;
        isJumping = false;
        isInFall = true;
    }
}

void FlapBird::fall(bool& isFalling, float& dt)
{
    if (isFalling) {
        m_bird.y -= 2.0f * dt;
    }
}

bool collisionD(glm::vec3& BirdPos, const glm::vec3& objPos, const float& objSize) {

    glm::vec3 pos = BirdPos - objPos;

    if ((pos.x <= objSize && pos.x >= -objSize)
        && (pos.y <= objSize && pos.y >= -objSize)
        && (pos.z <= objSize && pos.z >= -objSize)
        )
    {
        return true;
    }
    return false;
}

void FlapBird::Render(float& dt)
{
    //Moving
    m_moveSpeed += dt;
    m_inputs->mCamera.mPos.x += m_moveSpeed*dt;
    m_bird.x = m_inputs->mCamera.mPos.x;
    
    // jump
    if (!isInJump && Input::isKeyPressed(GLFW_KEY_SPACE)) {
        isInJump = true;
        isInFall = false;
    } if (isInJump && Input::isKeyPressed(GLFW_KEY_SPACE)) {
        JumpAgain = true;
    }
    //else   //falling
    //    //m_bird.y -= 1.0f * dt;

    jump(isInJump, JumpAgain,dt);
    fall(isInFall, dt);

    m_inputs->mShader->setUniformVec3("objectColor", glm::vec3(0.2, 0.2, 0.8));
    m_inputs->updateMVP(m_bird, *m_inputs->mShader);
    m_inputs->mShader->drawObj();


    for (auto i : m_map) {
        if ((m_inputs->mCamera.mPos.x + 20) >= i.x && (m_inputs->mCamera.mPos.x - 20) <= i.x) {
            if (collisionD(m_bird, i, 1.0f)) {
                throw "lol losser!!!";
            }
            m_inputs->mShader->setUniformVec3("objectColor", glm::vec3(0.8, 0.2, 0.3));
            m_inputs->updateMVP(i, *m_inputs->mShader);
            m_inputs->mShader->drawObj();
        }
    }
}
