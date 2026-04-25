#include "ModelLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<gameObject*> ModelLoader::load(const char* filePath, Material* material) {
    Context ctx;
    ctx.material = material;

    std::string fileStr = std::string(filePath);
    ctx.fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

    std::string text = readFile(filePath);
    if (text.empty()) return {};

    ctx.JSON = json::parse(text);
    ctx.binaryData = getBinaryData(ctx);

    json scene_json = ctx.JSON["scenes"][0];
    for (unsigned int i = 0; i < scene_json["nodes"].size(); i++) {
        traverseNode(ctx, scene_json["nodes"][i], glm::mat4(1.0f));
    }

    std::cout << "[ModelLoader] Loaded: " << filePath << " (" << ctx.result.size() << " objects)\n";
    return ctx.result;
}

void ModelLoader::traverseNode(Context& ctx, unsigned int nodeIndex, glm::mat4 parentMatrix) {
    json node = ctx.JSON["nodes"][nodeIndex];

    glm::vec3 translation(0.0f);
    if (node.find("translation") != node.end()) {
        translation = glm::vec3(
            (float)node["translation"][0],
            (float)node["translation"][1],
            (float)node["translation"][2]
        );
    }

    glm::quat rotation(1.0f, 0.0f, 0.0f, 0.0f);
    if (node.find("rotation") != node.end()) {
        rotation = glm::quat(
            (float)node["rotation"][3], // w
            (float)node["rotation"][0], // x
            (float)node["rotation"][1], // y
            (float)node["rotation"][2]  // z
        );
    }

    glm::vec3 scale(1.0f);
    if (node.find("scale") != node.end()) {
        scale = glm::vec3(
            (float)node["scale"][0],
            (float)node["scale"][1],
            (float)node["scale"][2]
        );
    }

    glm::mat4 matNode(1.0f);
    if (node.find("matrix") != node.end()) {
        float vals[16];
        for (int i = 0; i < 16; i++) vals[i] = (float)node["matrix"][i];
        matNode = glm::make_mat4(vals);
    }
    else {
        glm::mat4 transMat = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 rotMat = glm::mat4_cast(rotation);
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
        matNode = transMat * rotMat * scaleMat;
    }

    glm::mat4 localMatrix = parentMatrix * matNode;

    if (node.find("mesh") != node.end()) {
        loadMesh(ctx, node["mesh"], localMatrix);
    }

    if (node.find("children") != node.end()) {
        for (unsigned int i = 0; i < node["children"].size(); i++) {
            traverseNode(ctx, node["children"][i], localMatrix);
        }
    }
}

void ModelLoader::loadMesh(Context& ctx, unsigned int meshIndex, glm::mat4 matrix) {
    json meshJSON = ctx.JSON["meshes"][meshIndex];
    std::string meshName = meshJSON.value("name", "mesh_" + std::to_string(meshIndex));

    for (unsigned int p = 0; p < meshJSON["primitives"].size(); p++) {
        json primitive = meshJSON["primitives"][p];

        // --- Positions ---
        unsigned int posAccInd = primitive["attributes"]["POSITION"];
        std::vector<float> posVec = getFloats(ctx, ctx.JSON["accessors"][posAccInd]);
        std::vector<glm::vec3> positions = groupFloatsVec3(posVec);

        // --- Normals ---
        std::vector<glm::vec3> normals;
        if (primitive["attributes"].find("NORMAL") != primitive["attributes"].end()) {
            unsigned int normalAccInd = primitive["attributes"]["NORMAL"];
            std::vector<float> normalVec = getFloats(ctx, ctx.JSON["accessors"][normalAccInd]);
            normals = groupFloatsVec3(normalVec);
        }
        else {
            normals.resize(positions.size(), glm::vec3(0.0f, 1.0f, 0.0f));
        }

        // --- UVs ---
        std::vector<glm::vec2> texUVs;
        if (primitive["attributes"].find("TEXCOORD_0") != primitive["attributes"].end()) {
            unsigned int texAccInd = primitive["attributes"]["TEXCOORD_0"];
            std::vector<float> texVec = getFloats(ctx, ctx.JSON["accessors"][texAccInd]);
            for (unsigned int i = 0; i < texVec.size(); i += 2)
                texUVs.push_back(glm::vec2(texVec[i], texVec[i + 1]));
        }
        else {
            texUVs.resize(positions.size(), glm::vec2(0.0f));
        }

        // --- Indices ---
        unsigned int indAccInd = primitive["indices"];
        std::vector<GLuint> indices = getIndices(ctx, ctx.JSON["accessors"][indAccInd]);

        // --- Pack: x y z  nx ny nz  u v ---
        std::vector<float> vertices;
        for (unsigned int i = 0; i < positions.size(); i++) {
            vertices.push_back(positions[i].x);
            vertices.push_back(positions[i].y);
            vertices.push_back(positions[i].z);
            vertices.push_back(normals[i].x);
            vertices.push_back(normals[i].y);
            vertices.push_back(normals[i].z);
            vertices.push_back(texUVs[i].x);
            vertices.push_back(texUVs[i].y);
        }

        float* vertexData = new float[vertices.size()];
        std::copy(vertices.begin(), vertices.end(), vertexData);

        int* indexData = new int[indices.size()];
        for (unsigned int i = 0; i < indices.size(); i++) indexData[i] = (int)indices[i];

        int* attrSizes = new int[3] { 3, 3, 2 };

        // each primitive gets its own material copy so textures don't bleed
        Material* primMaterial = new Material(*ctx.material);

        if (primitive.find("material") != primitive.end()) {
            unsigned int matIndex = primitive["material"];
            getTexturesForMaterial(ctx, matIndex, primMaterial);
        }

        Mesh* mesh = new Mesh(
            vertexData,
            (int)vertices.size(),
            (int)(vertices.size() * sizeof(float)),
            indexData,
            (int)indices.size(),
            primMaterial,
            8, 3, attrSizes
        );

        // --- Create gameObject ---
        gameObject* go = new gameObject(meshName + "_" + std::to_string(p));
        Transform* transform = new Transform();
        go->addComponent(transform);
        go->addComponent(mesh);

        glm::vec3 pos = glm::vec3(matrix[3]);
        glm::vec3 scl = glm::vec3(
            glm::length(glm::vec3(matrix[0])),
            glm::length(glm::vec3(matrix[1])),
            glm::length(glm::vec3(matrix[2]))
        );
        glm::mat3 rotMatrix = glm::mat3(
            glm::vec3(matrix[0]) / scl.x,
            glm::vec3(matrix[1]) / scl.y,
            glm::vec3(matrix[2]) / scl.z
        );
        glm::quat rot = glm::quat_cast(rotMatrix);
        glm::vec3 euler = glm::eulerAngles(rot);

        transform->setPosition(pos);
        transform->setScale(scl);
        transform->setRotationX(euler.x);
        transform->setRotationY(euler.y);
        transform->setRotationZ(euler.z);

        ctx.result.push_back(go);
        std::cout << "[ModelLoader] Created: " << go->name << "\n";
    }
}

void ModelLoader::getTexturesForMaterial(Context& ctx, unsigned int matIndex, Material* material) {
    if (ctx.JSON.find("materials") == ctx.JSON.end()) return;

    json matJSON = ctx.JSON["materials"][matIndex];
    int slot = 0;

    if (matJSON.find("pbrMetallicRoughness") != matJSON.end()) {
        json pbr = matJSON["pbrMetallicRoughness"];

        if (pbr.find("baseColorTexture") != pbr.end()) {
            unsigned int texIndex = pbr["baseColorTexture"]["index"];
            Texture* tex = loadTexture(ctx, texIndex);
            if (tex) material->setTexture(slot++, tex);
        }

        if (pbr.find("metallicRoughnessTexture") != pbr.end()) {
            unsigned int texIndex = pbr["metallicRoughnessTexture"]["index"];
            Texture* tex = loadTexture(ctx, texIndex);
            if (tex) material->setTexture(slot++, tex);
        }
    }

    if (matJSON.find("normalTexture") != matJSON.end()) {
        unsigned int texIndex = matJSON["normalTexture"]["index"];
        Texture* tex = loadTexture(ctx, texIndex);
        if (tex) material->setTexture(slot++, tex);
    }
}

Texture* ModelLoader::loadTexture(Context& ctx, unsigned int texIndex) {
    if (ctx.JSON.find("textures") == ctx.JSON.end()) return nullptr;

    json textureNode = ctx.JSON["textures"][texIndex];
    if (textureNode.find("source") == textureNode.end()) return nullptr;

    unsigned int imageIndex = textureNode["source"];
    std::string texPath = ctx.JSON["images"][imageIndex]["uri"];

    for (unsigned int j = 0; j < ctx.loadedTextureNames.size(); j++) {
        if (ctx.loadedTextureNames[j] == texPath)
            return ctx.loadedTextures[j];
    }

    std::string fullPath = ctx.fileDirectory + texPath;
    Texture* tex = new Texture(fullPath.c_str());
    ctx.loadedTextures.push_back(tex);
    ctx.loadedTextureNames.push_back(texPath);
    return tex;
}

std::vector<unsigned char> ModelLoader::getBinaryData(Context& ctx) {
    std::string uri = ctx.JSON["buffers"][0]["uri"];
    std::string binPath = ctx.fileDirectory + uri;
    std::string raw = readFile(binPath.c_str());
    return std::vector<unsigned char>(raw.begin(), raw.end());
}

std::vector<float> ModelLoader::getFloats(Context& ctx, json accessor) {
    std::vector<float> floatVec;

    unsigned int buffViewInd = accessor.value("bufferView", 1);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    std::string type = accessor["type"];

    json bufferView = ctx.JSON["bufferViews"][buffViewInd];
    unsigned int byteOffset = bufferView["byteOffset"];

    unsigned int numPerVert;
    if (type == "SCALAR") numPerVert = 1;
    else if (type == "VEC2")   numPerVert = 2;
    else if (type == "VEC3")   numPerVert = 3;
    else if (type == "VEC4")   numPerVert = 4;
    else throw std::invalid_argument("Invalid accessor type: " + type);

    unsigned int start = byteOffset + accByteOffset;
    unsigned int length = count * 4 * numPerVert;
    for (unsigned int i = start; i < start + length; i += 4) {
        unsigned char bytes[] = { ctx.binaryData[i], ctx.binaryData[i + 1], ctx.binaryData[i + 2], ctx.binaryData[i + 3] };
        float value;
        std::memcpy(&value, bytes, sizeof(float));
        floatVec.push_back(value);
    }

    return floatVec;
}

std::vector<GLuint> ModelLoader::getIndices(Context& ctx, json accessor) {
    std::vector<GLuint> indices;

    unsigned int buffViewInd = accessor.value("bufferView", 0);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    unsigned int componentType = accessor["componentType"];

    json bufferView = ctx.JSON["bufferViews"][buffViewInd];
    unsigned int byteOffset = bufferView["byteOffset"];
    unsigned int start = byteOffset + accByteOffset;

    if (componentType == 5125) {
        for (unsigned int i = start; i < start + count * 4; i += 4) {
            unsigned char bytes[] = { ctx.binaryData[i], ctx.binaryData[i + 1], ctx.binaryData[i + 2], ctx.binaryData[i + 3] };
            unsigned int value;
            std::memcpy(&value, bytes, sizeof(unsigned int));
            indices.push_back((GLuint)value);
        }
    }
    else if (componentType == 5123) {
        for (unsigned int i = start; i < start + count * 2; i += 2) {
            unsigned char bytes[] = { ctx.binaryData[i], ctx.binaryData[i + 1] };
            unsigned short value;
            std::memcpy(&value, bytes, sizeof(unsigned short));
            indices.push_back((GLuint)value);
        }
    }
    else if (componentType == 5122) {
        for (unsigned int i = start; i < start + count * 2; i += 2) {
            unsigned char bytes[] = { ctx.binaryData[i], ctx.binaryData[i + 1] };
            short value;
            std::memcpy(&value, bytes, sizeof(short));
            indices.push_back((GLuint)value);
        }
    }

    return indices;
}

std::vector<glm::vec2> ModelLoader::groupFloatsVec2(std::vector<float>& floatVec) {
    std::vector<glm::vec2> result;
    for (unsigned int i = 0; i < floatVec.size(); i += 2)
        result.push_back(glm::vec2(floatVec[i], floatVec[i + 1]));
    return result;
}

std::vector<glm::vec3> ModelLoader::groupFloatsVec3(std::vector<float>& floatVec) {
    std::vector<glm::vec3> result;
    for (unsigned int i = 0; i < floatVec.size(); i += 3)
        result.push_back(glm::vec3(floatVec[i], floatVec[i + 1], floatVec[i + 2]));
    return result;
}

std::string ModelLoader::readFile(const char* path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "[ModelLoader] ERROR :: Could not open file: " << path << "\n";
        return "";
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}