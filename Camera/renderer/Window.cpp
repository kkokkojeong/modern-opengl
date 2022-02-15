#include "Window.hpp"

Window::Window()
{
    width = 800;
    height = 600;
    
    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
    
    lastX = 0.0f;
    lastY = 0.0f;
    xChange = 0.0f;
    yChange = 0.0f;
    mouseFirstMoved = true;
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;
    
    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
    
    lastX = 0.0f;
    lastY = 0.0f;
    xChange = 0.0f;
    yChange = 0.0f;
    mouseFirstMoved = true;
}

int Window::Initialise()
{
    if (!glfwInit())
    {
        printf("Error Initialising GLFW");
        glfwTerminate();
        return 1;
    }

    // Setup GLFW Windows Properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core Profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatiblity
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create the window
    mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
    if (!mainWindow)
    {
        printf("Error creating GLFW window!");
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    
    // Callback
    CreateCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Set the current context
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension access
    glewExperimental = GL_TRUE;

    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        printf("Error: %s", glewGetErrorString(error));
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Create Viewport
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    glfwSetWindowUserPointer(mainWindow, this);
    
    return 0;
}

void Window::HandleKeys(GLFWwindow *window, int key, int code, int action, int mode)
{
    Window *theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
            printf("Pressed: %d\n", key);
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
            printf("Released: %d\n", key);
        }
    }
}

void Window::HandleMouse(GLFWwindow *window, double xPos, double yPos)
{
    Window *theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    
    if (theWindow->mouseFirstMoved)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        
        theWindow->mouseFirstMoved = false;
    }
     
    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos; // y-coordinate inversed
    
    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
    
//    printf("x: %.2f, y: %.2f\n", xPos, yPos);
}

void Window::CreateCallbacks()
{
//    printf("CreateCallbacks\n");
    glfwSetKeyCallback(mainWindow, HandleKeys);
    glfwSetCursorPosCallback(mainWindow, HandleMouse);
}

GLfloat Window::GetXChange()
{
    GLfloat change = xChange;
    xChange = 0.0f;
    return change;
}

GLfloat Window::GetYChange()
{
    GLfloat change = yChange;
    yChange = 0.0f;
    return change;
}


Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
