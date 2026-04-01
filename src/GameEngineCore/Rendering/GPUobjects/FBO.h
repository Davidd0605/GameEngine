#ifndef FBO_H
#define FBO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "../Texture.h"

class FBO {
public:
    GLuint ID;
    GLuint textureID;

    FBO(int width = 1920, int height = 1080);
    void Bind();
    void Unbind();
    void Delete();
    Texture* getTexture();

private:
    GLuint RBO;
    Texture* texture;
};

#endif