#include "Renderer.h"

SDL_Renderer* Renderer::_renderer = nullptr;

bool Renderer::init(SDL_Renderer* renderer)
{
    if (_renderer)
        return false;
    _renderer = renderer;
    return true;
}

SDL_Renderer* Renderer::get()
{
    return _renderer;
}
