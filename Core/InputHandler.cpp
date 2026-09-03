#include "InputHandler.h"
#include <iostream>

InputHandler::InputHandler(GLFWwindow* window) : m_window(window)
{
	Input::initInput(m_window);
}

void InputHandler::keyHandle(float& dt, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp)
{
	const float cameraSpeed = 3.0f * dt; // adjust accordingly

	if (Input::isKeyHeld(GLFW_KEY_W)) {
		cameraPos += cameraSpeed * cameraFront;
	}
	if (Input::isKeyHeld(GLFW_KEY_S)) {
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (Input::isKeyHeld(GLFW_KEY_A)) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (Input::isKeyHeld(GLFW_KEY_D)) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (Input::isKeyPressed(GLFW_KEY_ESCAPE)) {
		if (cursorEnabled)
		{
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			cursorEnabled = false;
		}
		else
		{
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			cursorEnabled = true;
		}

		escWasPressed = false;
		return;
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

void InputHandler::cameraHandle(glm::vec3& cameraFront)
{
	MousePos pos = Input::getMousePos();
	if (firstMouse)
    {
        lastX = pos.x;
        lastY = pos.y;
        firstMouse = false;
    }
  
    float xoffset = pos.x- lastX;
    float yoffset = lastY - pos.y;
    lastX = pos.x;
    lastY = pos.y;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);

}
