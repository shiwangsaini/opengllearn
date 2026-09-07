#include "Camera.h"

Camera::Camera(GLFWwindow* window, Shader* shader)
	: mWindow(window), mShader(shader), mPos(glm::vec3(0.0f, 0.0f, 3.0f)), mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mUp(glm::vec3(0.0f, 1.0f, 0.0f))
{
    int w, h;
    glfwGetWindowSize(mWindow, &w, &h);

    mProjection = glm::perspective(glm::radians(mFov), (float)w / (float)h, 0.1f, 100.0f);
    mView = glm::lookAt(mPos, mPos + mFront, mUp);
    mShader->setUniformMat4f("uProj", mProjection);
    mShader->setUniformMat4f("uView", mView);
}

void Camera::update()
{
    int w, h;
    glfwGetWindowSize(mWindow, &w, &h);

    mProjection = glm::perspective(glm::radians(mFov), (float)w / (float)h, 0.1f, 100.0f);
    mView = glm::lookAt(mPos, mPos + mFront, mUp);
    mShader->setUniformMat4f("uProj", mProjection);
    mShader->setUniformMat4f("uView", mView);
}

void Camera::setFov(float& fov)
{
    mFov = fov;
}
