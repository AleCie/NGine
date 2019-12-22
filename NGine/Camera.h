#pragma once

#include <glm/glm.hpp>

#include <memory>

#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera(float fov, float ratio, float close, float far);

	glm::mat4 GetProjectionMatrix();
	void SetProjectionMatrix(float fov, float ratio, float close, float far);

	glm::mat4 GetViewMatrix();

	void Update(float dt);
	void HandleInput(GLFWwindow *window, float dt, int windowWidth, int windowHeight);
	void MouseLook(GLFWwindow* window, float dt, int windowWidth, int windowHeight);
	void SetSpeed(float val);
	void SetPosition(glm::vec3 val);
	void SetDirection(glm::vec3 val);
	void SetAngles(float horizontal, float vertical);

private:
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	glm::vec3 Position;
	glm::vec3 Direction;
	glm::vec3 Up;
	glm::vec3 Right;

	float VerticalAngle;
	float HorizontalAngle;

	float Speed;
	float MouseSpeed;

	float FOV;
	float Ratio;
	float Close;
	float Far;

	void CalculateDirections();
	
};