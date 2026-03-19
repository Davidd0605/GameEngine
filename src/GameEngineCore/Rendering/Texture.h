#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>

/// <summary>
/// Texture class.
/// Loads an image from disk using stb_image and uploads it to the GPU.
/// Supports PNG, JPG, BMP and most common formats.
/// </summary>
class Texture {
public:
    Texture(const char* path, GLenum format = GL_RGB);
    void bind(int slot = 0) const;
    void unbind() const;
    void Delete();

    GLuint ID;
    int width, height;

private:
    GLenum format;
};