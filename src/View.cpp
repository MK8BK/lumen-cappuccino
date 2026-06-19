#include "View.hpp"
#include "Model.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <variant>

#ifdef LUMEN_LOG
#  include <spdlog/spdlog.h>
#endif

namespace LumenCappuccino {
View::View()
    : valid(true), height(), width(), square_side(), square_gap(), window(),
      renderer(nullptr), lumenTexture(nullptr) {}

bool View::isValid() const { return valid; }

bool View::setup() {
  if (!SDL_CreateWindowAndRenderer("Lumen Cappuccino", 500, 500,
                                   SDL_WINDOW_EXTERNAL, &window, &renderer)) {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't create window/renderer: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  // critical for performance
  if (!SDL_SetRenderVSync(renderer, 1)) {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't vsync renderer: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  if (!SDL_SetWindowBordered(window, true)) {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't set window borders: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }

  SDL_DisplayID display = SDL_GetDisplayForWindow(window);
  if (!display) {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't get display: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  SDL_Rect screenDimensions;
  if (!SDL_GetDisplayBounds(display, &screenDimensions)) {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't get display size: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  height = screenDimensions.h * 3 / 4;
  width = screenDimensions.w * 3 / 4;
  square_side = std::min(height, width) / 4;
  square_gap = std::max(square_side / 5, 10);
  if (!SDL_SetWindowSize(window, width, height)) {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't set window size: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  if (!loadTexture(LUMEN_PICTURE_FILE, &lumenTexture))
    return valid; // valid was already set to false
  return valid;
}

bool View::loadTexture(const char* picturePath, SDL_Texture** destination) {
#ifdef LUMEN_STRICT_CHECKS
  if (!valid || !renderer || !window) {
#  ifdef LUMEN_LOG
    spdlog::error("Invalid View state. {} {} {} {}", valid, renderer==nullptr, window==nullptr, picturePath);
#  endif
    valid = false;
    return valid;
  }
#endif
  SDL_Surface* surface = IMG_Load(picturePath);
  if (!surface) {
#ifdef LUMEN_LOG
    spdlog::error("Failed to load image to sruface: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  *destination = SDL_CreateTextureFromSurface(renderer, surface);
  if (!(*destination)) {
#ifdef LUMEN_LOG
    spdlog::error("Failed create texture from surface: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  return valid;
}

static constexpr SDL_Color BLUE_BACKGROUND{65, 105, 225, 255};
static constexpr SDL_Color HIDDEN_FOREGROUND{0, 20, 64, 255};

SDL_AppResult View::render(const Model& model) const {
  if (model.isGameStarted()) [[likely]] {
    return drawCurrentGameState(model);
  }
  return drawLoadingScreen();
}

SDL_AppResult View::drawLoadingScreen() const {
#ifdef LUMEN_STRICT_CHECKS
  if (!SDL_SetRenderDrawColor(renderer, BLUE_BACKGROUND.r, BLUE_BACKGROUND.g,
                              BLUE_BACKGROUND.b, BLUE_BACKGROUND.a)) {
#  ifdef LUMEN_LOG
    spdlog::error("Could not set draw color {}. ", SDL_GetError());
#  endif
    return SDL_APP_FAILURE;
  }
#else
  SDL_SetRenderDrawColor(renderer, BLUE_BACKGROUND.r, BLUE_BACKGROUND.g,
                         BLUE_BACKGROUND.b, BLUE_BACKGROUND.a);
#endif

#ifdef LUMEN_STRICT_CHECKS
  if (!SDL_RenderClear(renderer)) {

#  ifdef LUMEN_LOG
    spdlog::error("Could not set draw color {}. ", SDL_GetError());
#  endif
    return SDL_APP_FAILURE;
  }
#else
  SDL_RenderClear(renderer);
#endif

SDL_FRect dst = {
    width/10.0f,  // x
    height/10.0f,  // y
    width/10.0f*7.8f,   // width
    height/10.0f*7.8f,   // height
};

#ifdef LUMEN_STRICT_CHECKS
if(!SDL_RenderTexture(renderer, lumenTexture, nullptr, &dst)){
    #  ifdef LUMEN_LOG
        spdlog::error("Could not render texture {}. ", SDL_GetError());
    #  endif
    return SDL_APP_FAILURE;
}
#else
SDL_RenderTexture(renderer, lumenTexture, nullptr, &dst);
#endif

#ifdef LUMEN_STRICT_CHECKS
  if (!SDL_RenderPresent(renderer)) {
#  ifdef LUMEN_LOG
    spdlog::error("Could not render present the renderer {}. ", SDL_GetError());
#  endif
    return SDL_APP_FAILURE;
  }
#else
  SDL_RenderPresent(renderer);
#endif
  return SDL_APP_CONTINUE;
}

SDL_AppResult View::drawCurrentGameState(const Model& model) const {
#ifdef LUMEN_STRICT_CHECKS
  if (!SDL_SetRenderDrawColor(renderer, BLUE_BACKGROUND.r, BLUE_BACKGROUND.g,
                              BLUE_BACKGROUND.b, BLUE_BACKGROUND.a)) {

#  ifdef LUMEN_LOG
    spdlog::error("Could not set render draw color {}. ", SDL_GetError());
#  endif
    return SDL_APP_FAILURE;
  }
  if (!SDL_RenderClear(renderer)) {
#  ifdef LUMEN_LOG
    spdlog::error("Could not render clear {}. ", SDL_GetError());
#  endif
    return SDL_APP_FAILURE;
  }
#else
  SDL_SetRenderDrawColor(renderer, BLUE_BACKGROUND.r, BLUE_BACKGROUND.g,
                         BLUE_BACKGROUND.b, BLUE_BACKGROUND.a);
  SDL_RenderClear(renderer);
#endif
  for (int row{}; row < 3; ++row) {
    for (int column{}; column < 4; ++column) {
      std::optional<Tile> tileOption = model.getTile(row, column);
#ifdef LUMEN_STRICT_CHECKS
      if (!tileOption.has_value())
        return SDL_APP_FAILURE;
#endif
      Tile t = tileOption.value();
      if (std::holds_alternative<COFFEE_BEAN>(t)) {
      } else if (std::holds_alternative<COCOA_BEAN>(t)) {
      } else if (std::holds_alternative<CAPPUCCINO>(t)) {
      } else if (std::holds_alternative<ESPRESSO>(t)) {
      } else if (std::holds_alternative<MACHIATTO>(t)) {
      } else if (std::holds_alternative<GREEN_TEA>(t)) {
      } else if (std::holds_alternative<HIDDEN>(t)) {
        drawHidden(row, column);
      } else if (std::holds_alternative<DELETED>(t)) {
      }
    }
  }
/* put the newly-cleared rendering on the screen. */
#ifdef LUMEN_STRICT_CHECKS
  if (!SDL_RenderPresent(renderer)) {
    spdlog::error("Could not render present the renderer {}. ", SDL_GetError());
    return SDL_APP_FAILURE;
  }
#else
  SDL_RenderPresent(renderer);
#endif

  return SDL_APP_CONTINUE; /* carry on with the program! */
}

void View::drawHidden(int row, int column) const {
  float y{static_cast<float>(row * square_side + (row + 1) * square_gap)};
  float x{static_cast<float>(column * square_side + (column + 1) * square_gap)};
  const SDL_FRect rect{x, y, static_cast<float>(square_side),
                       static_cast<float>(square_side)};
#ifdef LUMEN_STRICT_CHECKS
  if (!SDL_SetRenderDrawColor(renderer, HIDDEN_FOREGROUND.r,
                              HIDDEN_FOREGROUND.g, HIDDEN_FOREGROUND.b,
                              HIDDEN_FOREGROUND.a)) {
#  ifdef LUMEN_LOG
    spdlog::error("Could not set color {}", SDL_GetError());
#  endif
    return;
  }
  if (!SDL_RenderFillRect(renderer, &rect)) {
#  ifdef LUMEN_LOG
    spdlog::error("Could not fill rectangle {}", SDL_GetError());
#  endif
    return;
  }
#else
  SDL_SetRenderDrawColor(renderer, HIDDEN_FOREGROUND.r, HIDDEN_FOREGROUND.g,
                         HIDDEN_FOREGROUND.b, HIDDEN_FOREGROUND.a);
  SDL_RenderFillRect(renderer, &rect);
#endif
}

} // namespace LumenCappuccino
