#include <iostream>
#include <string.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;

GLuint VAO, VBO, IBO, shader, uniformModel, uniformProj;

bool direction = true;
float triangleOffset = 0.0f;
float triangleMaxOffset = 0.7f;
float triangleIncrement = 0.005f;

float currentAngle = 0.0f;

bool sizeDirection = false;
float currentSize = 0.4f;
const float maxSize = 0.8f;
const float minSize = 0.1f;


// Vertex Shader
static const char *vShader = "          \n\
#version 330                            \n\
                                        \n\
layout (location = 0) in vec3 pos;      \n\
uniform mat4 uModel;                    \n\
uniform mat4 uProj;                     \n\
                                        \n\
out vec4 vColor;                         \n\
                                        \n\
void main()                             \n\
{                                       \n\
    gl_Position = uProj * uModel * vec4(pos, 1.0f);      \n\
    vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);  \n\
//    gl_Position = vec4(pos, 1.0f);      \n\
//    gl_Position = vec4(pos.x * 0.4f, pos.y * 0.4f, pos.z, 1.0f);      \n\
//    gl_Position = vec4(pos.x * 0.4f + xMove, pos.y * 0.4f, pos.z, 1.0f);      \n\
}                                       \n\
";

// Fragment Shader
static const char *fShader = "                       \n\
#version 330                                         \n\
                                                     \n\
                                                     \n\
in vec4 vColor;                                      \n\
out vec4 colour;                                     \n\
                                                     \n\
void main()                                          \n\
{                                                    \n\
//    colour = vec4(1.0f, 0.0f, 0.0f, 1.0f);           \n\
    colour = vColor;                                    \n\
}                                                    \n\
";

void CreateTriangle()
{
    GLuint indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };
    
    GLfloat vertices[] = {
      -1.0f, -1.0f, 0.0f,
       0.0f, -1.0f, 1.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f
    };
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    // Program Status `glValidateProgram` 상태에서 Vertex Array가 delocated 되어있으면 워닝발생
    // https://github.com/GlPortal/glPortal/issues/157
//    glBindVertexArray(0);
}

void AddShader(GLuint program, const char *shaderCode, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    
    const GLchar* code[1];
    code[0] = shaderCode;
    
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);
    
    glShaderSource(shader, 1, code, codeLength);
    glCompileShader(shader);
    
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cout << "Error Compiling shader: " << shaderType << ", " << eLog << std::endl;
        return;
    }
    
    glAttachShader(program, shader);
}

void CompileShaders()
{
    shader = glCreateProgram();
    if (!shader)
    {
        std::cout << "Error creating shader program" << std::endl;
        return;
    }
    
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);
    
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cout << "Error linking program: " << eLog << std::endl;
        return;
    }
    
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        std::cout << "Error validating program: " << eLog << std::endl;
        return;
    }
    
    uniformModel = glGetUniformLocation(shader, "uModel");
    uniformProj = glGetUniformLocation(shader, "uProj");
}

int main()
{
    std::cout << "Modern OpenGL - Geometry Example(width = " << WIDTH << ", height = " << HEIGHT << ")" << std::endl;
    
    // Initailize GLFW
    if (!glfwInit())
    {
        std::cout << "GLFW initialization failed." << std::endl;
        glfwTerminate();
        return 1;
    }
    
    // Setup GLFW window properties
    // OpenGL Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    // Core Profile = No Backwars Compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Allow Forward Compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // Create the window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Geometry Example", NULL, NULL);
    if (!window)
    {
        std::cout << "GLFW window creation failed." << std::endl;
        glfwTerminate();
        return 1;
    }
    
    // Get buffer size, information
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(window);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW initialization failed." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }
    
    // Depth
    glEnable(GL_DEPTH_TEST);
    
    // Setup viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    // Initialize object, shader
    CreateTriangle();
    CompileShaders();
    
    
    // Projection
    glm::mat4 proj = glm::perspective(45.0f, (GLfloat)bufferWidth/(GLfloat)bufferHeight, 0.1f, 100.0f);
    

    // Loop until window closed
    while (!glfwWindowShouldClose(window))
    {
        if (direction)
        {
            triangleOffset += triangleIncrement;
        }
        else
        {
            triangleOffset -= triangleIncrement;
        }
        if (abs(triangleOffset) >= triangleMaxOffset)
        {
            direction = !direction;
        }
        
        currentAngle += 1.0f;
        if (currentAngle >= 360.0f)
        {
            currentAngle = 0.0f;
        }
        
        if (sizeDirection)
        {
            currentSize += 0.001f;
        }
        else
        {
            currentSize -= 0.001f;
        }
        if (currentSize >= maxSize || currentSize <= minSize)
        {
            sizeDirection = !sizeDirection;
        }
        
//        std::cout <<currentAngle << std::endl;
        
        // Get + Handle user input events
        glfwPollEvents();
        
        // Clear buffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use shader
        glUseProgram(shader);
        
        // Transformation
        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(triangleOffset, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::rotate(model, currentAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
//        model = glm::scale(model, glm::vec3(currentSize, currentSize, 1.0f));
        
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(proj));
        
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        
//        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        
        // Dellocation
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
    }
    
    return 0;
}
