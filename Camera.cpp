#include "Camera.h"

// Default values
static constexpr float SPEED = 5.0f;
static constexpr float SENSITIVITY = 0.1f;
static constexpr float FOV_DEFAULT = 70.0f;

Camera::Camera(glm::vec3 position) : Position(position),
Front(glm::vec3(0.0f, 0.0f, -1.0f)),
WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
Yaw(-90.0f),
Pitch(0.0f),
MovementSpeed(SPEED),
MouseSensitivity(SENSITIVITY),
FOV(FOV_DEFAULT) {
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(float deltaTime, bool forward, bool backward, bool left, bool right) {
    float velocity = MovementSpeed * deltaTime;

    if (forward)
        Position += Front * velocity;
    if (backward)
        Position -= Front * velocity;
    if (left)
        Position -= Right * velocity;
    if (right)
        Position += Right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
