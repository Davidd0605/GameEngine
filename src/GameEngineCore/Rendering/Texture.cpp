#include "Texture.h"

#include <stb/stb_image.h>

Texture::Texture(const char* path, GLenum format, bool flipVertically) {
    this->format = format;

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image
    int channels;
    stbi_set_flip_vertically_on_load(flipVertically);
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

    if (data) {
        // Auto detect format based on channels if not specified
        GLenum imageFormat = format;
        if (channels == 4) imageFormat = GL_RGBA;
        else if (channels == 3) imageFormat = GL_RGB;
        else if (channels == 1) imageFormat = GL_RED;

        glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "[Texture] ERROR :: FAILED TO LOAD: " << path << "\n";
        std::cerr << "[Texture] ERROR :: STB_IMAGE: " << stbi_failure_reason() << "\n";
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
    glDeleteTextures(1, &ID);
}