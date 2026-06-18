#include "View.hpp"
#include "Model.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <variant>
#include <iostream>

#ifdef LUMEN_LOG
#  include <spdlog/spdlog.h>
#endif

namespace LumenCappuccino {
View::View()
    : valid(), height(), width(), square_side(), square_gap(), window(),
      renderer() {}

bool View::isValid() const { return valid; }

bool View::setup() {
  if (!SDL_CreateWindowAndRenderer("Lumen Cappuccino", 500, 500,
                                   SDL_WINDOW_EXTERNAL, &window, &renderer)) {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't create window/renderer: %s", SDL_GetError());
#endif
    return false;
  }
  // critical for performance
  if (!SDL_SetRenderVSync(renderer, 1)) {
    SDL_Log("Couldn't vsync renderer: %s", SDL_GetError());
    return false;
  }
  //  if (!SDL_SetRenderLogicalPresentation(renderer, 640, 480,
  // SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
  // #ifdef LUMEN_LOG
  // spdlog::error("Couldn't set render logical presentation: %s",
  // SDL_GetError());
  // #endif
  // return false;
  // }
  if (!SDL_SetWindowBordered(window, true)) {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't set window borders: %s", SDL_GetError());
#endif
    return false;
  }

  SDL_DisplayID display = SDL_GetDisplayForWindow(window);
  if (!display) {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't get display: %s", SDL_GetError());
#endif
    return false;
  }
  SDL_Rect screenDimensions;
  if (!SDL_GetDisplayBounds(display, &screenDimensions)) {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't get display size: %s", SDL_GetError());
#endif
    return false;
  }
  height = screenDimensions.h * 3 / 4;
  width = screenDimensions.w * 3 / 4;
  square_side = std::min(height, width) / 4;
  square_gap = std::max(square_side / 5, 10);
  if (!SDL_SetWindowSize(window, width, height)) {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't set window size: %s", SDL_GetError());
#endif
    return false;
  }
  return true;
}

static constexpr SDL_Color BLUE_BACKGROUND{65, 105, 225, 255};
static constexpr SDL_Color HIDDEN_FOREGROUND{0, 20, 64, 255};

// TODO: for now may be unused, change later
SDL_AppResult View::render([[maybe_unused]] const Model& model) const {
  SDL_SetRenderDrawColor(renderer, BLUE_BACKGROUND.r, BLUE_BACKGROUND.g,
                         BLUE_BACKGROUND.b, BLUE_BACKGROUND.a);
  /* clear the window to the draw color. */
  SDL_RenderClear(renderer);
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
  SDL_RenderPresent(renderer);
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
    spdlog::warn("Could not set color %s", SDL_GetError());
    return;
  }
  if (!SDL_RenderFillRect(renderer, &rect)) {
    spdlog::warn("Could not fill rectangle %s", SDL_GetError());
    return;
  }
#else
  SDL_SetRenderDrawColor(renderer, HIDDEN_FOREGROUND.r, HIDDEN_FOREGROUND.g,
                         HIDDEN_FOREGROUND.b, HIDDEN_FOREGROUND.a);
  SDL_RenderFillRect(renderer, &rect);
#endif
}

} // namespace LumenCappuccino
