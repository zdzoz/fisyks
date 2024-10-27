#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Renderer.h"
#include "MessageBox.h"
#include "Particle.h"
#include "ParticleGrid.h"

// reference: https://www.youtube.com/watch?v=8Tf18MMZ-5U

#define WIDTH 800
#define HEIGHT 800
#define APPLICATION_NAME "Fisyks"

int main()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        MessageBox::get().error("SDL_Init failed", SDL_GetError());
        return 1;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    if (!SDL_CreateWindowAndRenderer(APPLICATION_NAME, WIDTH, HEIGHT, 0, &window, &renderer)) {
        MessageBox::get().error("SDL_CreateWindowAndRenderer failed", SDL_GetError());
        return 1;
    }

    Renderer::init(renderer);

    int n = 200;
    auto world = std::make_unique<ParticleGrid>(n, n);
    auto next = std::make_unique<ParticleGrid>(n, n);

    int w = world->width() / 4;
    for (int i = w; i < w * 3; i++) {
        for (int j = w; j < w * 3; j++) {
            if (std::rand() % 2 == 0)
                world->set(EParticleType::SAND, i, j);
        }
    }

    const float tile_width = (float)WIDTH / world->width();
    const float tile_height = (float)HEIGHT / world->height();

    std::queue<std::unique_ptr<IDrawable>> drawables;

    SDL_Event event;
    bool shouldQuit = false;
    bool mouseButtonIsHeld = false;
    while (!shouldQuit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_EVENT_QUIT:
                shouldQuit = true;
                break;
            case SDL_EVENT_KEY_DOWN: {
                if (event.key.key == SDLK_Q) {
                    shouldQuit = true;
                }
            } break;
            case SDL_EVENT_MOUSE_MOTION: {
            } break;
            case SDL_EVENT_MOUSE_BUTTON_UP: {
                mouseButtonIsHeld = false;
            } break;
            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                mouseButtonIsHeld = true;
            } break;
            }
        }

        if (mouseButtonIsHeld) {
            float x, y;
            auto button = SDL_GetMouseState(&x, &y);
            auto xpos = x / tile_width;
            auto ypos = y / tile_height;
            auto cur = world->get(xpos, ypos);
            if (cur.has_value()) {
                if (button & SDL_BUTTON_LMASK) {
                    if (cur->get() == EParticleType::AIR) {
                        world->set(EParticleType::SAND, xpos, ypos);
                    }
                } else if (button & SDL_BUTTON_RMASK) {
                    if (cur->get() != EParticleType::AIR) {
                        world->set(EParticleType::AIR, xpos, ypos);
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 22, 22, 22, 255);
        SDL_RenderClear(renderer);

        world->enact();

        for (int x = world->width() - 1; x >= 0; x--) {
            for (int y = world->height() - 1; y >= 0; y--) {
                auto particle_or_none = world->get(x, y);
                if (!particle_or_none.has_value())
                    continue;

                auto& particle = particle_or_none->get();
                if (particle == EParticleType::AIR)
                    continue;

                if (auto p = ParticleFactory::makeDrawable(x, y, tile_width, tile_height, particle))
                    drawables.emplace(std::move(p));

                auto below = world->get(x, y + 1);
                if (below.has_value() && below->get() == EParticleType::AIR) {
                    next->set(particle, x, y + 1);
                    world->set(EParticleType::AIR, x, y); // HACK: loop is going from bottom up and each row is dependent on lower row. This wouldn't work for gas as gas rises
                } else {
                    std::optional<std::reference_wrapper<const EParticleType>> side_or_none;

                    bool checkLeft = rand() % 2 == 0;
                    // check left or right
                    side_or_none = checkLeft ? world->get(x - 1, y + 1) : world->get(x + 1, y + 1);
                    EParticleType side = EParticleType::AIR;
                    int newx = x;

                    bool success = false;
                    if (side_or_none.has_value()) {
                        success = true;
                        side = side_or_none->get();
                        newx = checkLeft ? x - 1 : x + 1;
                    }

                    // if failed check other side
                    if (!success) {
                        side_or_none = !checkLeft ? world->get(x - 1, y + 1) : world->get(x + 1, y + 1);
                        if (side_or_none.has_value()) {
                            side = side_or_none->get();
                            newx = !checkLeft ? x - 1 : x + 1;
                        }
                    }

                    if (y + 1 < world->width() && side == EParticleType::AIR) {
                        next->set(particle, newx, y + 1);
                        world->set(EParticleType::AIR, x, y); // HACK: loop is going from bottom up and each row is dependent on lower row. This wouldn't work for gas as gas rises
                    } else {
                        // if both sides failed leave in place
                        next->set(particle, x, y);
                    }
                }
            }
        }
        std::swap(world, next);
        next->setAll(EParticleType::AIR);

        // Draw
        while (!drawables.empty()) {
            auto top = std::move(drawables.front());
            drawables.pop();
            top->draw();
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
