#pragma once

#include "IPhysicsObject.h"
#include "Grid.h"

#include <vector>

enum class EParticleType;

class ParticleGrid : public IPhysicsObject, public Grid<EParticleType> {
public:
    ParticleGrid(int width, int height);

    virtual void enact() const override;

private:
    std::vector<EParticleType> next;
};
