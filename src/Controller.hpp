#ifndef CONTROLLER__HPP
#define CONTROLLER__HPP
#include "Model.hpp"
#include "MusicPlayer.hpp"
#include "View.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>

namespace LumenCappuccino {

class Controller {
private:
  bool valid;
  View view;
  Model model;
  MusicPlayer musicPlayer;
  SDL_AppResult handleKey(SDL_Keycode key);

public:
  Controller();
  SDL_AppResult handleEvent(SDL_Event* event);
  bool isValid() const;
  SDL_AppResult render() const;
};

} // namespace LumenCappuccino
#endif
