#include "Material.h"
#include <iostream>

Material::Material(ShaderPass* shaderPass) {
    this->shaderPass = shaderPass;
    introspect();
}

void Material::introspect() {
    std::vector<UniformInfo> uniforms = shaderPass->getMaterialUniforms();

    for (const UniformInfo& info : uniforms) {
        if (info.type == GL_SAMPLER_2D) {
            // reserve a slot for this texture, filled later via setTexture()
            textures.push_back(nullptr);
            continue;
        }

        MaterialProperty prop;
        prop.type = info.type;

        // default values
        switch (info.type) {
        case GL_FLOAT:
            prop.value.f = 0.0f;
            break;
        case GL_INT:
            prop.value.i = 0;
            break;
        case GL_FLOAT_VEC2:
            prop.value.v2 = { 0.0f, 0.0f };
            break;
        case GL_FLOAT_VEC3:
            prop.value.v3 = { 1.0f, 1.0f, 1.0f }; // default white
            break;
        case GL_FLOAT_VEC4:
            prop.value.v4 = { 1.0f, 1.0f, 1.0f, 1.0f };
            break;
        default:
            std::cerr << "[Material] WARNING :: unsupported uniform type for: " << info.name << "\n";
            continue;
        }

        properties[info.name] = prop;
    }
}

void Material::setFloat(const std::string& name, float value) {
    auto it = properties.find(name);
    if (it == properties.end()) {
        std::cerr << "[Material] WARNING :: uniform not found: " << name << "\n";
        return;
    }
    it->second.value.f = value;
}

void Material::setInt(const std::string& name, int value) {
    auto it = properties.find(name);
    if (it == properties.end()) {
        std::cerr << "[Material] WARNING :: uniform not found: " << name << "\n";
        return;
    }
    it->second.value.i = value;
}

void Material::setVec3(const std::string& name, glm::vec3 value) {
    auto it = properties.find(name);
    if (it == properties.end()) {
        std::cerr << "[Material] WARNING :: uniform not found: " << name << "\n";
        return;
    }
    it->second.value.v3 = { value.x, value.y, value.z };
}

void Material::setVec4(const std::string& name, glm::vec4 value) {
    auto it = properties.find(name);
    if (it == properties.end()) {
        std::cerr << "[Material] WARNING :: uniform not found: " << name << "\n";
        return;
    }
    it->second.value.v4 = { value.x, value.y, value.z, value.w };
}

void Material::setTexture(int slot, const std::string& path) {
    if (slot < 0 || slot >= (int)textures.size()) {
        std::cerr << "[Material] WARNING :: texture slot out of range: " << slot << "\n";
        return;
    }
    textures[slot] = new Texture(path.c_str());
}

void Material::setTexture(int slot, Texture* texture) {
    if (slot < 0 || slot >= (int)textures.size()) {
        std::cerr << "[Material] WARNING :: texture slot out of range: " << slot << "\n";
        return;
    }
    textures[slot] = texture;
}

void Material::bind() const {
    shaderPass->bind();

    for (const auto& pair : properties) {
        const std::string& name = pair.first;
        const MaterialProperty& prop = pair.second;

        switch (prop.type) {
        case GL_FLOAT:
            shaderPass->setFloat(name, prop.value.f);
            break;
        case GL_INT:
            shaderPass->setInt(name, prop.value.i);
            break;
        case GL_FLOAT_VEC2:
            break;
        case GL_FLOAT_VEC3:
            shaderPass->setVec3(name, glm::vec3(prop.value.v3.x, prop.value.v3.y, prop.value.v3.z));
            break;
        case GL_FLOAT_VEC4:
            shaderPass->setVec4(name, glm::vec4(prop.value.v4.x, prop.value.v4.y, prop.value.v4.z, prop.value.v4.w));
            break;
        }
    }

    for (int i = 0; i < (int)textures.size(); i++) {
        if (textures[i] != nullptr)
            textures[i]->bind(i);
    }
}

void Material::unbind() const {
    shaderPass->unbind();
}

ShaderPass* Material::getShaderPass() const {
    return shaderPass;
}