#ifndef Camera_hpp
#define Camera_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera();
    Camera(glm::vec3 pos, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat s1, GLfloat s2);
    ~Camera();
    
    void KeyControls(bool *keys, GLfloat deltaTime);
    void MouseControls(GLfloat x, GLfloat y);
    
    glm::mat4 CalculateViewMatrix();
    
private:
    void Update();
    
    glm::vec3 position;
    glm::vec3 front, up, right, worldUp;
    
    GLfloat yaw, pitch;
    
    GLfloat movementSpeed, turnSpeed;
};

#endif /* Camera_hpp */
