#ifndef COMPONENT_H
#define COMPONENT_H

// Base class for all components. Make it polymorphic so dynamic_cast works.
class Component
{
public:
    virtual ~Component() = default;   // polymorphic type
    virtual void start() {}          // default no-op implementations
    virtual void update() {}
    virtual void end() {}
};

#endif // COMPONENT_H

