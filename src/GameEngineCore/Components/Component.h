#ifndef COMPONENT_H
#define COMPONENT_H

class gameObject;

class Component {
public:
    virtual ~Component() = default;
    virtual void start() {}
    virtual void update() {}
    virtual void end() {}

    gameObject* owner = nullptr;
};

#endif // COMPONENT_H