#include "MessageBox.h"

#include <SDL3/SDL.h>

MessageBox& MessageBox::get()
{
    static MessageBox* instance = new MessageBox;
    return *instance;
}

void MessageBox::error(const std::string& title, const std::string& msg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), msg.c_str(), nullptr);
}

void MessageBox::warning(const std::string& title, const std::string& msg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, title.c_str(), msg.c_str(), nullptr);
}

void MessageBox::info(const std::string& title, const std::string& msg)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, title.c_str(), msg.c_str(), nullptr);
}
