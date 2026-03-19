#ifndef SYSTEM_H
#define SYSTEM_H

class GameScene;

class GameSystem
{
public:
    virtual ~GameSystem() = default;
    virtual void start() {}
    virtual void update() {}
    virtual void fixedUpdate() {}
    virtual void end() {}
    virtual void setCurrentScene(GameScene* scene) {};
    virtual void clearCurrentScene() {};
protected:
    GameScene* currentScene;
};
#endif