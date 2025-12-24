#pragma once

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

struct Camera {
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Euler Angles
	float Yaw;
	float Pitch;

	// Camera Options
	float MovementSpeed;
	float MouseSensitivity;
	float FOV;

	// Constructor
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f));

	// methods
	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(float deltaTime, bool forward, bool backward, bool left, bool right);
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

private:
	void updateCameraVectors();
};