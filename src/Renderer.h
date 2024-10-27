#pragma once

#include <SDL3/SDL.h>

class Renderer {
    Renderer() { }

public:
    static bool init(SDL_Renderer* renderer);
    static SDL_Renderer* get();

private:
    static SDL_Renderer* _renderer;
};
