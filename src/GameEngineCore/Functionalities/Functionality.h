// Functionality.h
#ifndef FUNCTIONALITY_H
#define FUNCTIONALITY_H

#include "../Components/Component.h"

/// <summary>
/// Base class for all user-defined behaviors.
/// Extend this to add custom logic to a game object.
/// </summary>
class Functionality : public Component {
public:
    virtual void start() override {}
    virtual void update() override {}
    virtual void fixedUpdate() override {}
    virtual void end() override {}
};

#endif