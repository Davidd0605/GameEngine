#include "SceneSerializer.h"

// --- This is the ONE translation unit allowed to include everything. ---
// Do NOT add these includes anywhere else to avoid circular dependencies.
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <json/json.h>
#include <glm/glm.hpp>

#include "src/GameEngineCore/GameObjects/GameObject.h"
#include "src/GameEngineCore/Components/Transform.h"
#include "src/GameEngineCore//Components/Camera.h"
#include "src/GameEngineCore//Components/Light.h"
#include "src/GameEngineCore//Components/Mesh.h"
#include "src/GameEngineCore//Rendering/Material.h"
#include "src/GameEngineCore//Rendering/ShaderPass.h"
#include "src/GameEngineCore//Rendering/ModelLoader.h"
#include "src/GameEngineCore//Scenes/GameScene.h"
#include "src/GameEngineCore//Systems/RenderSystem.h"
#include "src/GameEngineCore//Utilities/enums/LightTypes.h"

using json = nlohmann::json;

// -----------------------------------------------------------------------
// Internal helpers (static, translation-unit local)
// -----------------------------------------------------------------------

static Transform* deserializeTransform(const json& j) {
    Transform* t = new Transform();
    if (j.contains("position")) {
        t->setPosition(glm::vec3(
            j["position"][0].get<float>(),
            j["position"][1].get<float>(),
            j["position"][2].get<float>()
        ));
    }
    if (j.contains("rotation")) {
        // rotation is stored as radians on X/Y/Z axes
        t->setRotationX(j["rotation"][0].get<float>());
        t->setRotationY(j["rotation"][1].get<float>());
        t->setRotationZ(j["rotation"][2].get<float>());
    }
    if (j.contains("scale")) {
        t->setScale(glm::vec3(
            j["scale"][0].get<float>(),
            j["scale"][1].get<float>(),
            j["scale"][2].get<float>()
        ));
    }
    return t;
}

static Camera* deserializeCamera(const json& j) {
    float fov = j.value("fov", 45.0f);
    float aspectRatio = j.value("aspectRatio", 16.0f / 9.0f);
    float nearPlane = j.value("nearPlane", 0.1f);
    float farPlane = j.value("farPlane", 100.0f);
    int   priority = j.value("priority", 0);
    bool  postProc = j.value("postProcessing", false);
    return new Camera(fov, aspectRatio, nearPlane, farPlane, priority, postProc);
}

static Light* deserializeLight(const json& j) {
    LightType type = static_cast<LightType>(j.value("lightType", 0));
    float intensity = j.value("intensity", 1.0f);
    float range = j.value("range", 10.0f);
    float radius = j.value("radius", 1.0f);

    glm::vec3 color(1.0f);
    if (j.contains("color")) {
        color = glm::vec3(j["color"][0], j["color"][1], j["color"][2]);
    }

    glm::vec3 direction(0.0f, -1.0f, 0.0f);
    if (j.contains("direction")) {
        direction = glm::vec3(j["direction"][0], j["direction"][1], j["direction"][2]);
    }

    return new Light(type, intensity, color, range, direction, radius);
}

static Material* deserializeMaterial(const json& j) {
    std::string frag = j.value("fragShader", "");
    std::string vert = j.value("vertShader", "");
    ShaderPass* sp = new ShaderPass(frag.c_str(), vert.c_str());
    Material* mat = new Material(sp);

    if (j.contains("textures")) {
        int slot = 0;
        for (const auto& texPath : j["textures"]) {
            std::string path = texPath.get<std::string>();
            if (!path.empty()) {
                mat->setTexture(slot, path);
            }
            slot++;
        }
    }

    // Note: material properties (uniforms) are not restored here because they
    // are typically set by the shader introspection on bind(). Extend this
    // if you need to persist runtime-overridden uniforms.

    return mat;
}

static Mesh* deserializeMesh(const json& j) {
    // Model-loaded meshes are reconstructed via ModelLoader (from modelPath),
    // so we only handle inline meshes here.
    if (!j.value("hasIndices", false) && !j.contains("vertices")) {
        return nullptr; // nothing to restore for non-inline meshes
    }

    int stride = j.value("stride", 8);
    int noAttribs = j.value("attributeSizes", json::array()).size();

    std::vector<int> attribSizes;
    for (const auto& a : j["attributeSizes"]) {
        attribSizes.push_back(a.get<int>());
    }

    // Reconstruct vertex array
    std::vector<float> vertsVec;
    if (j.contains("vertices")) {
        for (const auto& v : j["vertices"]) vertsVec.push_back(v.get<float>());
    }
    int verticesSize = j.value("verticesSize", 0);
    int verticesBytes = (int)(vertsVec.size() * sizeof(float));

    Material* mat = nullptr;
    if (j.contains("material")) {
        mat = deserializeMaterial(j["material"]);
    }

    if (j.value("hasIndices", false) && j.contains("indices")) {
        std::vector<int> indsVec;
        for (const auto& i : j["indices"]) indsVec.push_back(i.get<int>());
        int indicesSize = (int)indsVec.size();

        Mesh* mesh = new Mesh(
            vertsVec.data(), verticesSize, verticesBytes,
            indsVec.data(), indicesSize,
            mat,
            stride, noAttribs, attribSizes.data()
        );
        mesh->isInlineMesh = true;
        return mesh;
    }
    else {
        Mesh* mesh = new Mesh(
            vertsVec.data(), verticesSize, verticesBytes,
            mat,
            stride, noAttribs, attribSizes.data()
        );
        mesh->isInlineMesh = true;
        return mesh;
    }
}

// Finds the value in a components JSON object whose key contains a given substring.
// This is needed because MSVC mangles typeid names like "class Transform".
static const json* findComponent(const json& components, const std::string& keyword) {
    for (auto it = components.begin(); it != components.end(); ++it) {
        if (it.key().find(keyword) != std::string::npos) {
            return &it.value();
        }
    }
    return nullptr;
}

static gameObject* deserializeGameObject(const json& j) {
    std::string name = j.value("name", "unnamed");
    std::string modelPath = j.value("modelPath", "");

    gameObject* go = new gameObject(name, modelPath);

    if (!j.contains("components")) return go;
    const json& components = j["components"];

    // Order matters: Transform first (other components may rely on it at start())
    const json* transformJ = findComponent(components, "Transform");
    if (transformJ) go->addComponent(deserializeTransform(*transformJ));

    const json* cameraJ = findComponent(components, "Camera");
    if (cameraJ) go->addComponent(deserializeCamera(*cameraJ));

    const json* lightJ = findComponent(components, "Light");
    if (lightJ) go->addComponent(deserializeLight(*lightJ));

    // Inline mesh only - model meshes come from ModelLoader below
    if (modelPath.empty()) {
        const json* meshJ = findComponent(components, "Mesh");
        if (meshJ) {
            Mesh* mesh = deserializeMesh(*meshJ);
            if (mesh) go->addComponent(mesh);
        }
    }

    return go;
}

static RenderSystem* deserializeRenderSystem(const json& j) {
    RenderSystem* rs = new RenderSystem();
    rs->postProcessingEnabled = j.value("postProcessingEnabled", false);

    if (j.contains("postProcessingPasses")) {
        for (const auto& pass : j["postProcessingPasses"]) {
            std::string frag = pass.value("fragShader", "");
            std::string vert = pass.value("vertShader", "");
            rs->addPostProcessingShaderPass(new ShaderPass(frag.c_str(), vert.c_str()));
        }
    }

    return rs;
}

// -----------------------------------------------------------------------
// Public API
// -----------------------------------------------------------------------

GameScene* SceneSerializer::deserialize(const std::string& jsonData, GLFWwindow* window) {
    json j;
    try {
        j = json::parse(jsonData);
    }
    catch (const std::exception& e) {
        std::cerr << "[SceneSerializer] ERROR :: Failed to parse JSON: " << e.what() << "\n";
        return nullptr;
    }

    std::string sceneName = j.value("name", "UnnamedScene");
    GameScene* scene = new GameScene(sceneName, window);

    // --- Systems first (RenderSystem must exist before objects call start()) ---
    if (j.contains("systems")) {
        for (const auto& sysJson : j["systems"]) {
            std::string type = sysJson.value("type", "");
            if (type == "RenderSystem") {
                scene->addSystem(deserializeRenderSystem(sysJson));
            }
            // Add new system types here as you add them to the engine.
        }
    }

    // --- Game objects ---
    if (j.contains("gameObjects")) {
        for (const auto& goJson : j["gameObjects"]) {
            std::string modelPath = goJson.value("modelPath", "");

            if (!modelPath.empty()) {
                // Reconstruct the model hierarchy via ModelLoader.
                // We need a material - reuse the RenderSystem's model shader if available,
                // otherwise fall back to a sensible default.
                ShaderPass* sp = new ShaderPass(
                    "src/Shaders/object/model.frag",
                    "src/Shaders/object/model.vert"
                );
                Material* mat = new Material(sp);
                gameObject* loadedModel = ModelLoader::load(modelPath.c_str(), mat);

                if (loadedModel) {
                    // Restore the saved transform onto the loaded root.
                    if (goJson.contains("components")) {
                        const json* transformJ = findComponent(goJson["components"], "Transform");
                        if (transformJ) {
                            Transform* saved = deserializeTransform(*transformJ);
                            Transform* modelT = loadedModel->getComponent<Transform>();
                            if (modelT) {
                                modelT->setPosition(saved->getPosition());
                                // scale/rotation getters would need exposing - extend Transform if needed
                            }
                            delete saved;
                        }
                    }

                    loadedModel->name = goJson.value("name", loadedModel->name);
                    scene->addObject(loadedModel);

                    Transform* t = loadedModel->getComponent<Transform>();
                    if (t) {
                        for (gameObject* child : t->getChildrenGameObjects()) {
                            scene->addObject(child);
                        }
                    }
                }
            }
            else {
                scene->addObject(deserializeGameObject(goJson));
            }
        }
    }

    std::cout << "[SceneSerializer] Loaded scene: " << sceneName << "\n";
    return scene;
}

GameScene* SceneSerializer::deserializeFromFile(const std::string& filePath, GLFWwindow* window) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "[SceneSerializer] ERROR :: Could not open file: " << filePath << "\n";
        return nullptr;
    }

    std::string jsonData((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    return deserialize(jsonData, window);
}