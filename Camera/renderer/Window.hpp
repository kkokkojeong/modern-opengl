#ifndef Window_hpp
#define Window_hpp

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight);
    ~Window();

    int Initialise();

    GLint getBufferWidth() { return bufferWidth; }
    GLint getBufferHeight() { return bufferHeight; }

    bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

    void swapBuffers() { glfwSwapBuffers(mainWindow); }
    
    // input event
    bool keys[1024];
    void CreateCallbacks();
    static void HandleKeys(GLFWwindow *window, int key, int code, int action, int mode);
    static void HandleMouse(GLFWwindow *window, double xPos, double yPos);
    
    bool* GetKeys() { return keys; }
    GLfloat GetXChange();
    GLfloat GetYChange();
    
    GLfloat lastX, lastY, xChange, yChange; // positions
    bool mouseFirstMoved;

private:
    GLFWwindow* mainWindow;

    GLint width, height;
    GLint bufferWidth, bufferHeight;
};

#endif /* Window_hpp */
