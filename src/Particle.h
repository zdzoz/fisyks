#pragma once

#include <SDL3/SDL.h>

#include "IPhysicsObject.h"
#include "IDrawable.h"
#include "MessageBox.h"
#include "Renderer.h"
#include "ParticleGrid.h"

#define EPARTICLETYPE_LIST \
    X(AIR)                 \
    X(SAND)                \
    X(WATER)

#define X(name) name,

enum class EParticleType {
    EPARTICLETYPE_LIST
};

#undef X
#define X(name) #name,

constexpr const char* __EParticleTypeToString[] = {
    EPARTICLETYPE_LIST
};

#undef EPARTICLETYPE_LIST

inline std::string EParticleTypeGetString(EParticleType part)
{
    return __EParticleTypeToString[(size_t)part];
}

class Particle : public IPhysicsObject {
public:
    Particle(int x, int y, ParticleGrid& grid)
        : _x(x)
        , _y(y)
        , _grid(grid)
    {
    }

    virtual void enact() const override = 0;

protected:
    int _x, _y;
    ParticleGrid& _grid;
};

class SandParticle : public Particle {
public:
    SandParticle(int x, int y, ParticleGrid& grid)
        : Particle(x, y, grid)
    {
    }

    virtual void enact() const override;
};

class AirParticle : public Particle {
public:
    AirParticle(int x, int y, ParticleGrid& grid)
        : Particle(x, y, grid)
    {
    }

    virtual void enact() const override;
};

class WaterParticle : public Particle {
public:
    WaterParticle(int x, int y, ParticleGrid& grid)
        : Particle(x, y, grid)
    {
    }

    virtual void enact() const override;
};

class DrawableParticle : public IDrawable {
public:
    DrawableParticle(SDL_FRect rect, SDL_Color color)
        : _rect(rect)
        , _color(color)
    {
    }

    DrawableParticle(SDL_FRect& rect, SDL_Color& color)
        : _rect(std::move(rect))
        , _color(std::move(color))
    {
    }

    virtual void draw() override;

private:
    SDL_FRect _rect;
    SDL_Color _color;
};

class ParticleFactory {
    ParticleFactory() = delete;
public:
    static std::unique_ptr<Particle> make(int x, int y, EParticleType type, ParticleGrid& grid);
    static std::unique_ptr<DrawableParticle> makeDrawable(int x, int y, int tile_width, int tile_height, EParticleType particle);
};
