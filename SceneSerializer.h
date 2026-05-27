#pragma once

#include <string>

// Forward declarations - SceneSerializer is the only one that knows everything,
// but callers only need these pointers.
class GameScene;
struct GLFWwindow;

/// <summary>
/// Owns all deserialization logic for the engine.
/// This is the ONLY file allowed to include all component/system headers together,
/// which is what keeps circular dependencies out of the rest of the codebase.
///
/// Usage:
///   GameScene* scene = SceneSerializer::deserializeFromFile("scenes/MyScene.json", window);
///   GameScene* scene = SceneSerializer::deserialize(jsonString, window);
/// </summary>
class SceneSerializer {
public:
    // Load a scene from a JSON file on disk.
    static GameScene* deserializeFromFile(const std::string& filePath, GLFWwindow* window);

    // Load a scene from a JSON string (e.g. already read from disk or network).
    static GameScene* deserialize(const std::string& jsonData, GLFWwindow* window);
};