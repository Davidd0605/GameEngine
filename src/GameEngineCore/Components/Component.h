#ifndef COMPONENT_H
#define COMPONENT_H

#include <../../src/GameEngineCore/Utilities/Interfaces/ISerializable.h>
class gameObject;

class Component : public ISerializable{
public:
    virtual ~Component() = default;
    virtual void start() {}
    virtual void update() {}
    virtual void fixedUpdate() {}
    virtual void end() {}

    std::string serialize() { return ""; };
    void deserialize(const std::string& jsonData) { return; };

    gameObject* owner = nullptr;
};

#endif // COMPONENT_H