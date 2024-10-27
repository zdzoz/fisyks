#include "ParticleGrid.h"

#include "Particle.h"

ParticleGrid::ParticleGrid(int width, int height)
    : Grid(width, height, EParticleType::AIR)
{
    next.resize(width * height);
}

void ParticleGrid::enact() const
{
    for (int x = 0; x < this->width(); x++) {
        for (int y = 0; y < this->height(); y++) {
            auto type = this->get(x, y);
            if (type.has_value() && type->get() != EParticleType::AIR) {
                auto p = ParticleFactory::make(x, y, type->get(), (ParticleGrid&)*this);
                p->enact();
            }
        }
    }
}
