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
#include "../Rendering/Material.h"
#include "../Rendering/ShaderPass.h"
#include "../Rendering/Texture.h"

using json = nlohmann::json;

class ModelLoader {
public:
    static std::vector<gameObject*> load(const char* filePath, Material* material);

private:
    struct Context {
        json JSON;
        std::vector<unsigned char> binaryData;
        std::string fileDirectory;
        Material* material;
        std::vector<gameObject*> result;

        std::vector<Texture*> loadedTextures;
        std::vector<std::string> loadedTextureNames;
    };

    static void traverseNode(Context& ctx, unsigned int nodeIndex, glm::mat4 parentMatrix);
    static void loadMesh(Context& ctx, unsigned int meshIndex, glm::mat4 matrix);

    static void getTexturesForMaterial(Context& ctx, unsigned int matIndex, Material* material);
    static Texture* loadTexture(Context& ctx, unsigned int texIndex);

    static std::vector<unsigned char> getBinaryData(Context& ctx);
    static std::vector<float> getFloats(Context& ctx, json accessor);
    static std::vector<GLuint> getIndices(Context& ctx, json accessor);

    static std::vector<glm::vec2> groupFloatsVec2(std::vector<float>& floatVec);
    static std::vector<glm::vec3> groupFloatsVec3(std::vector<float>& floatVec);

    static std::string readFile(const char* path);
};