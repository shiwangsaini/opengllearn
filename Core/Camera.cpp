#include "Camera.h"

Camera::Camera(GLFWwindow* window)
	: mWindow(window), mPos(glm::vec3(0.0f, 0.0f, 3.0f)), mFront(glm::vec3(1.0f, 1.0f, 0.0f)), mUp(glm::vec3(0.0f, 0.0f, 1.0f))
{
    glfwGetWindowSize(mWindow, &mWidth, &mHeight);

    mProjection = glm::perspective(glm::radians(mFov), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
    mView = glm::lookAt(mPos, mPos + mFront, mUp);
}

void Camera::update(Shader& shader)
{
    mProjection = glm::perspective(glm::radians(mFov), (float)mWidth / (float)mHeight, 0.1f, 100.0f);
    mView = glm::lookAt(mPos, mPos + mFront, mUp);
    shader.setUniformMat4f("uProj", mProjection);
    shader.setUniformMat4f("uView", mView);
}

void Camera::setFov(float& fov)
{
    mFov = fov;
}
