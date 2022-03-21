#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <GL/glew.h>

#include "stb_image.hpp"

class Texture
{
public:
    Texture();
    Texture(char *file);
    ~Texture();
    
    void LoadTexture();
    void ClearTexture();
    void UseTexture();
    

private:
    GLuint textureID;
    int width, height, bitDepth;
    
    char *fileLocation;
};

#endif /* Texture_hpp */
