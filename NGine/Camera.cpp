#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fov, float ratio, float close, float far)
	: FOV(fov),
	Ratio(ratio),
	Close(close),
	Far(far),
	ViewMatrix(glm::mat4(1)),
	ProjectionMatrix(glm::mat4(1)),
	VerticalAngle(0),
	HorizontalAngle(3.14f),
	Position(glm::vec3(0)),
	Direction(glm::vec3(0)),
	Up(glm::vec3(0, 0, 0)),
	Right(glm::vec3(0)),
	Speed(0.1f),
	MouseSpeed(0.005f)
{
	ProjectionMatrix = glm::perspective(glm::radians(FOV), Ratio, Close, Far);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return ProjectionMatrix;
}

void Camera::SetProjectionMatrix(float fov, float ratio, float close, float far)
{
	FOV = fov;
	Ratio = ratio;
	Close = close;
	Far = far;

	ProjectionMatrix = glm::perspective(glm::radians(FOV), Ratio, Close, Far);
}

glm::mat4 Camera::GetViewMatrix()
{
	return ViewMatrix;
}

void Camera::Update(float dt)
{
	CalculateDirections();

	ViewMatrix = glm::lookAt(
		Position,           // Camera is here
		Position + Direction, // and looks here : at the same position, plus "direction"
		Up                  // Head is up (set to 0,-1,0 to look upside-down)
	);
}

void Camera::CalculateDirections()
{
	// Direction : Spherical coordinates to Cartesian coordinates conversion
	Direction = glm::vec3(
		cos(VerticalAngle) * sin(HorizontalAngle),
		sin(VerticalAngle),
		cos(VerticalAngle) * cos(HorizontalAngle)
	);
	/*Direction = glm::vec3(
		sin(VerticalAngle) * cos(HorizontalAngle),
		cos(VerticalAngle),
		sin(VerticalAngle) * sin(HorizontalAngle)
	);*/

	// Right vector
	Right = glm::vec3(
		sin(HorizontalAngle - 3.14f / 2.0f),
		0,
		cos(HorizontalAngle - 3.14f / 2.0f)
	);

	/*Right = glm::vec3(
		cos(HorizontalAngle - 3.14f / 2.0f),
		0,
		sin(HorizontalAngle - 3.14f / 2.0f)
	);*/

	// Up vector
	Up = glm::cross(Right, Direction);
}

void Camera::HandleInput(GLFWwindow *window, float dt, int windowWidth, int windowHeight)
{
	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		Position += Direction * dt * Speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		Position -= Direction * dt * Speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		Position += Right * dt * Speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		Position -= Right * dt * Speed;
	}

	
}

void Camera::MouseLook(GLFWwindow* window, float dt, int windowWidth, int windowHeight)
{
	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);

	// Compute new orientation
	HorizontalAngle += MouseSpeed * float(windowWidth / 2 - xpos);
	VerticalAngle += MouseSpeed * float(windowHeight / 2 - ypos);

	if (VerticalAngle > 90)
	{
		VerticalAngle = 90;
	}
	if (VerticalAngle < -90)
	{
		VerticalAngle = -90;
	}
}

void Camera::SetSpeed(float val)
{
	Speed = val;
}

void Camera::SetPosition(glm::vec3 val)
{
	Position = val;
}

void Camera::SetDirection(glm::vec3 val)
{
	Direction = val;
}

void Camera::SetAngles(float horizontal, float vertical)
{
	HorizontalAngle = horizontal;
	VerticalAngle = vertical;
}
