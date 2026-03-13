#ifndef COMPONENT_H
#define COMPONENT_H

/// <summary>
/// Base class for all components in the game engine.
/// </summary>
class Component
{
public:
    virtual ~Component() = default;  
    virtual void start() {}          
    virtual void update() {}
    virtual void end() {}
};

#endif // COMPONENT_H

