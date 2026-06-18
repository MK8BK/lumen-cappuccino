#ifndef CONTROLLER__HPP
#define CONTROLLER__HPP
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include "Model.hpp"
#include "View.hpp"

namespace LumenCappuccino{

class Controller{
private:
    bool valid;
    View view;
    Model model;
public:
    Controller();
    SDL_AppResult handleEvent(SDL_Event* event);
    bool isValid() const;
    SDL_AppResult render() const;
};

} // namespace LumenCappuccino
#endif
