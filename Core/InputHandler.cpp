#include "InputHandler.h"
#include <iostream>

InputHandler::InputHandler(GLFWwindow* window, Shader& shaders)
	: m_window(window), mShaders(shaders)
{
	Input::initInput(m_window);
	mCamera = Camera(m_window);
}


void InputHandler::keyHandle(float& dt)
{
	const float cameraSpeed = 3.0f * dt; // adjust accordingly

	if (Input::isKeyHeld(GLFW_KEY_W)) {
		mCamera.mPos += cameraSpeed * mCamera.mFront;
	}
	if (Input::isKeyHeld(GLFW_KEY_S)) {
		mCamera.mPos -= cameraSpeed * mCamera.mFront;
	}
	if (Input::isKeyHeld(GLFW_KEY_A)) {
		mCamera.mPos -= glm::normalize(glm::cross(mCamera.mFront, mCamera.mUp)) * cameraSpeed;
	}
	if (Input::isKeyHeld(GLFW_KEY_D)) {
		mCamera.mPos += glm::normalize(glm::cross(mCamera.mFront, mCamera.mUp)) * cameraSpeed;
	}
	if (Input::isKeyPressed(GLFW_KEY_ESCAPE)) {
		if (cursorEnabled)
			CursorOn(false);
		else 
			CursorOn(true);

	}
	
}

void InputHandler::mouseHandle(float& deltaTime, float& x, float& y, float& z, const unsigned int& width, const unsigned int& height)
{

	if (Input::isButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
		x = (float)((Input::getMousePos().x * 2.0f) / (float)width - 1.0f);
		y = (float)(1.0f - (Input::getMousePos().y * 2.0f) / (float)height);
	}

	if (Input::isButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) std::cout << "pressed RIGHT\t";
}

void InputHandler::cameraHandle(float& deltaTime)
{
	if (cursorEnabled) {
		MousePos pos = Input::getMousePos();
		if (firstMouse)
		{
			lastX = pos.x;
			lastY = pos.y;
			firstMouse = false;
		}

		float xoffset = pos.x - lastX;
		float yoffset = lastY - pos.y;
		lastX = pos.x;
		lastY = pos.y;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 direction;
		direction.x = -(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
		direction.y = (cos(glm::radians(pitch)) * sin(glm::radians(yaw)));
		direction.z = (sin(glm::radians(pitch)));
		mCamera.mFront = (glm::normalize(direction));
	}
	else firstMouse = true;
}

void InputHandler::scrollHandle()
{
	mCamera.setFov(Input::m_fov);
}

void InputHandler::updateMVP(const glm::vec3& Position ,const ShaderType& Type)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f), Position);
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// if Simple obj is to update for MVP
	mShaders.setUniformMat4f("uModel", model, Type);
	mCamera.update(mShaders, Type);
}

void InputHandler::updateMVP(const glm::vec3& Position, const glm::vec3& scale, const ShaderType& type)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Position) * glm::scale(model, scale);
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	mShaders.setUniformMat4f("uModel", model, type);
	mCamera.update(mShaders, type);
}

void InputHandler::updateMVP(const glm::vec3& Position, const glm::vec3& scale, const float& radian ,const glm::vec3& rotation, const ShaderType& type)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, Position) * glm::scale(model, scale);
	model = glm::rotate(model, glm::radians(radian), rotation);

	mShaders.setUniformMat4f("uModel", model, type);
	mCamera.update(mShaders, type);
}

void InputHandler::useShader(const ShaderType& Type)
{
	mShaders.use(Type);
}

void InputHandler::SetCameraPos(glm::vec3& camPos)
{
	mCamera.mPos = camPos;
}

void InputHandler::CursorOn(const bool& on)
{
	if(on)
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	cursorEnabled = on;
}

void KeyBind::BindKeyPress(int key, std::function<void()> func)
{
	if (Input::isKeyPressed(key)) {
		func();
	}
}

void KeyBind::BindKeyOnHold(int key, std::function<void()> func)
{
	if (Input::isKeyHeld(key)) {
		func();
	}
}

void KeyBind::BindKeyRelease(int key, std::function<void()> func)
{
	if (Input::isKeyReleased(key)) {
		func();
	}
}
