#include "Particle.h"

#include "Renderer.h"

///////////////
// PARTICLES //
///////////////

// TODO: move drawing logic
void SandParticle::enact() const
{
    // if (auto p = DrawableParticleFactory::createDrawable(x, y, tile_width, tile_height, particle))
    //     drawables.emplace(std::move(p));

    // auto below = _grid->get(x, y + 1);
    // if (below.has_value() && below->get() == EParticleType::AIR) {
    //     next->set(particle, x, y + 1);
    //     world->set(EParticleType::AIR, x, y); // HACK: loop is going from bottom up and each row is dependent on lower row. This wouldn't work for gas as gas rises
    // } else {
    // std::optional<std::reference_wrapper<const EParticleType>> side_or_none;

    // bool checkLeft = rand() % 2 == 0;
    // check left or right
    // side_or_none = checkLeft ? world->get(x - 1, y + 1) : world->get(x + 1, y + 1);
    // EParticleType side = EParticleType::AIR;
    // int newx = x;

    // bool success = false;
    // if (side_or_none.has_value()) {
    //     success = true;
    //     side = side_or_none->get();
    //     newx = checkLeft ? x - 1 : x + 1;
    // }

    // if failed check other side
    // if (!success) {
    //     side_or_none = !checkLeft ? world->get(x - 1, y + 1) : world->get(x + 1, y + 1);
    //     if (side_or_none.has_value()) {
    //         side = side_or_none->get();
    //         newx = !checkLeft ? x - 1 : x + 1;
    //     }
    // }

    // if (y + 1 < world->width() && side == EParticleType::AIR) {
    //     next->set(particle, newx, y + 1);
    //     world->set(EParticleType::AIR, x, y); // HACK: loop is going from bottom up and each row is dependent on lower row. This wouldn't work for gas as gas rises
    // } else {
    //     // if both sides failed leave in place
    //     next->set(particle, x, y);
    // }
    // }
}

void AirParticle::enact() const
{
}

void WaterParticle::enact() const
{
}

// END PARTICLES //

void DrawableParticle::draw()
{
    SDL_SetRenderDrawColor(Renderer::get(), _color.r, _color.g, _color.b, _color.a);
    if (!SDL_RenderFillRect(Renderer::get(), &_rect)) {
        MessageBox::get().error("SDL_RenderFillRect", SDL_GetError());
    }
}

///////////////
// FACTORIES //
///////////////

std::unique_ptr<Particle> ParticleFactory::make(int x, int y, EParticleType type, ParticleGrid& grid)
{
    std::unique_ptr<Particle> particle = nullptr;
    switch (type) {
    case EParticleType::AIR:
        particle = std::make_unique<AirParticle>(x, y, grid);
        break;
    case EParticleType::SAND:
        particle = std::make_unique<SandParticle>(x, y, grid);
        break;
    case EParticleType::WATER:
        particle = std::make_unique<WaterParticle>(x, y, grid);
        break;
    }
    return particle;
}

std::unique_ptr<DrawableParticle> ParticleFactory::makeDrawable(int x, int y, int tile_width, int tile_height, EParticleType particle)
{
    SDL_FRect rect = {
        .x = (float)x * tile_width,
        .y = (float)y * tile_height,
        .w = (float)tile_width,
        .h = (float)tile_height,
    };

    SDL_Color color = { 0, 0, 0, 0 };

    switch (particle) {
    case EParticleType::SAND:
        color = { 238, 232, 170, 255 };
        break;
    case EParticleType::WATER:
        color = { 0, 20, 242, 255 };
        break;
    default:
        // MessageBox::get().error("DrawableParticle Error", "Unknown Particle");
        return nullptr;
        break;
    }
    return std::make_unique<DrawableParticle>(std::move(rect), std::move(color));
}
