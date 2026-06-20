#include "Controller.hpp"
#include "Model.hpp"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <variant>

#ifdef LUMEN_LOG
#  include <spdlog/spdlog.h>
#endif

using namespace LumenCappuccino;

Controller::Controller() : valid(true), view(), model(), musicPlayer() {
  // for gnome and wayland when somehow libdecor is not used
  SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_PREFER_LIBDECOR, "1");
  // this better not fail haha, who cares
  SDL_SetAppMetadata("Lumen Cappuccino", "1.0", "io.github.mk8bk");
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS))
      [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't initialize SDL: {}", SDL_GetError());
#endif
    valid = false;
  }
  if (!model.setup() || !view.setup() || !musicPlayer.setup()) [[unlikely]]
    valid = false;
  musicPlayer.playLoadingScreenAudio();
}

SDL_AppResult Controller::handleKey(SDL_Keycode key) {
  if (key == SDLK_RETURN && !model.isGameStarted()) {
    musicPlayer.playCoffeeMachineAudio();
    model.startGame();
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult Controller::handleEvent(SDL_Event* event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
  } else if (event->type == SDL_EVENT_KEY_DOWN) {
    SDL_AppResult r{handleKey(event->key.key)};
    SDL_FlushEvent(SDL_EVENT_KEY_DOWN);
    SDL_FlushEvent(SDL_EVENT_KEY_UP);
    return r;
  } else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
             model.isGameStarted() && !model.isGameOver()) {
    handleMouseDown(&event->button);
    if (model.isGameWon())
      musicPlayer.playSuccessSound();
    SDL_FlushEvent(SDL_EVENT_MOUSE_BUTTON_DOWN);
    SDL_FlushEvent(SDL_EVENT_MOUSE_BUTTON_UP);
    return SDL_APP_CONTINUE;
  }
  return SDL_APP_CONTINUE;
}

void Controller::handleMouseDown(SDL_MouseButtonEvent* buttonEvent) {
  float x{buttonEvent->x}, y{buttonEvent->y};
#ifdef LUMEN_LOG
  // spdlog::info("{} {} {}", x, y, buttonEvent->timestamp);
#endif
  std::optional<std::pair<int, int>> clickedTilePosition{view.getTile(x, y)};
  if (!clickedTilePosition) {
    return;
  }
  int row{clickedTilePosition.value().first},
      column{clickedTilePosition.value().second};
  std::optional<Tile> tileOptional = model.getTile(row, column);

#ifdef LUMEN_STRICT_CHECKS
  // should be false since the clickedPosition is a valid position
  if (!tileOptional) {
#  ifdef LUMEN_LOG
    spdlog::error("Invalid clickedPosition -- aborting");
#  endif
    return;
  }
#endif
  Tile tile{tileOptional.value()};
  if (std::holds_alternative<DELETED>(tile)) {
    // do nothing
    return;
  }
  model.uncoverTile(row, column);
}

bool Controller::isValid() const { return valid; }

SDL_AppResult Controller::render() {
  static bool startedLostMusic = false;
  if (!startedLostMusic && model.isGameStarted() && model.isGameLost())
      [[unlikely]]
    musicPlayer.playFailureSound(), startedLostMusic = true;
  return view.render(model);
}
