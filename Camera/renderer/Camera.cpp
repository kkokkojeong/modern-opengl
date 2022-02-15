#include "Camera.hpp"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 pos, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat s1, GLfloat s2)
{
    position = pos;
    worldUp = up;
    yaw = yaw;
    pitch = pitch;
    
    front = glm::vec3(0.0f, 0.0f, -1.0f);

    movementSpeed = s1;
    turnSpeed = s2;
    
    Update();
}

Camera::~Camera()
{
    
}

void Camera::Update()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

void Camera::KeyControls(bool *keys, GLfloat deltaTime)
{
    GLfloat velocity = deltaTime * movementSpeed;
    
    if (keys[GLFW_KEY_W])
    {
        position += front * velocity;
    }
    else if (keys[GLFW_KEY_S])
    {
        position -= front * velocity;
    }
    else if (keys[GLFW_KEY_A])
    {
        position -= right * velocity;
    }
    else if (keys[GLFW_KEY_D])
    {
        position += right * velocity;
    }
}

void Camera::MouseControls(GLfloat x, GLfloat y)
{
    x *= turnSpeed;
    y *= turnSpeed;
    
    yaw += x;
    pitch += y;
    
    if (pitch >= 89.9f)
    {
        pitch = 89.9f;
    }
    if (pitch < -89.9f)
    {
        pitch = -89.9f;
    }
    
    Update();
}

glm::mat4 Camera::CalculateViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

