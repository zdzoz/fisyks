#pragma once

class IPhysicsObject {
public:
    virtual void enact() const = 0;
    virtual ~IPhysicsObject() = default;
};
