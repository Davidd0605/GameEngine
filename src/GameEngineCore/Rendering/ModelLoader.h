#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <json/json.h>

#include "../Scenes/GameScene.h"
#include "../GameObjects/GameObject.h"
#include "../Components/Transform.h"
#include "../Components/Mesh.h"
#include "../Rendering/ShaderPass.h"
#include "../Rendering/Texture.h"

using json = nlohmann::json;

class ModelLoader {
public:
    // Loads a GLTF model and returns all spawned gameObjects
    // Caller is responsible for adding them to a scene or managing their lifetime
    static std::vector<gameObject*> load(const char* filePath, ShaderPass* shader);

private:
    struct Context {
        json JSON;
        std::vector<unsigned char> binaryData;
        std::string fileDirectory;
        ShaderPass* shader;
        std::vector<gameObject*> result; // collected game objects

        std::vector<Texture*> loadedTextures;
        std::vector<std::string> loadedTextureNames;
    };

    static void traverseNode(Context& ctx, unsigned int nodeIndex, glm::mat4 parentMatrix);
    static void loadMesh(Context& ctx, unsigned int meshIndex, glm::mat4 matrix);

    static void getTexturesForMaterial(Context& ctx, unsigned int matIndex, Mesh* mesh);
    static Texture* loadTexture(Context& ctx, unsigned int texIndex);

    static std::vector<unsigned char> getBinaryData(Context& ctx);
    static std::vector<float> getFloats(Context& ctx, json accessor);
    static std::vector<GLuint> getIndices(Context& ctx, json accessor);

    static std::vector<glm::vec2> groupFloatsVec2(std::vector<float>& floatVec);
    static std::vector<glm::vec3> groupFloatsVec3(std::vector<float>& floatVec);

    static std::string readFile(const char* path);
};