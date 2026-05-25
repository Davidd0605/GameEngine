#ifndef MATERIAL_H
#define MATERIAL_H
#include <string>
#include <unordered_map>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "../Rendering/ShaderPass.h"
#include "../Rendering/Texture.h"

/// <summary>
/// Material encapsulates a ShaderPass and all its material uniforms (prefixed with _).
/// System uniforms (model, VP, time, lights etc.) are set by RenderSystem, not Material.
/// Call bind() before drawing — this binds the shader and uploads all material uniforms and textures.
/// </summary>
struct MaterialProperty {
    GLenum type;
    union {
        float f;
        int i;
        struct { float x, y; } v2;
        struct { float x, y, z; } v3;
        struct { float x, y, z, w; } v4;
    } value;
};

class Material {
public:
    Material(ShaderPass* shaderPass);

    void setFloat(const std::string& name, float value);
    void setInt(const std::string& name, int value);
    void setVec3(const std::string& name, glm::vec3 value);
    void setVec4(const std::string& name, glm::vec4 value);
    void setTexture(int slot, const std::string& path);
    void setTexture(int slot, Texture* texture);

    void bind() const;
    void unbind() const;

    ShaderPass* getShaderPass() const;

    // Returns all texture paths in slot order; empty string if slot has no path-based texture.
    std::vector<std::string> getTexturePaths() const;

    std::string serialize() const;

private:
    ShaderPass* shaderPass;
    std::unordered_map<std::string, MaterialProperty> properties;
    std::vector<Texture*> textures;
    void introspect();
};

#endif
