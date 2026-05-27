#ifndef SYSTEM_H
#define SYSTEM_H

#include "../Utilities/Interfaces/ISerializable.h"
class GameScene;

class GameSystem : public ISerializable
{
public:
    virtual ~GameSystem() = default;
    virtual void start() {}
    virtual void update() {}
    virtual void fixedUpdate() {}
    virtual void end() {}
    virtual void setCurrentScene(GameScene* scene) {};
    virtual void clearCurrentScene() {};

    virtual std::string serialize() { return ""; };
    virtual void deserialize(const std::string& jsonData) { return; };
protected:
    GameScene* currentScene;
};
#endif