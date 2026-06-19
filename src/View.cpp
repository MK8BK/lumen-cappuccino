#include "View.hpp"
#include "Model.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
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
      renderer(nullptr), lumenTexture(nullptr), coffeeBeanTexture(),
      cocoaBeanTexture(), espressoTexture(), machiattoTexture(),
      greenTeaTexture(), cappuccinoTexture() {}

bool View::isValid() const { return valid; }

bool View::setup() {
  if (!SDL_CreateWindowAndRenderer("Lumen Cappuccino", 500, 500,
                                   SDL_WINDOW_EXTERNAL, &window, &renderer))
      [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't create window/renderer: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  // critical for performance
  if (!SDL_SetRenderVSync(renderer, 1)) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't vsync renderer: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  if (!SDL_SetWindowBordered(window, true)) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't set window borders: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }

  SDL_DisplayID display = SDL_GetDisplayForWindow(window);
  if (!display) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't get display: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  SDL_Rect screenDimensions;
  if (!SDL_GetDisplayBounds(display, &screenDimensions)) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't get display size: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  height = screenDimensions.h * 3 / 4;
  width = screenDimensions.w * 3 / 4;
  square_side = std::min(height * 5 / 19, width / 6);
  square_gap = std::max(square_side / 5, 10);
  if (!SDL_SetWindowSize(window, width, height)) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't set window size: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  if (!loadTexture(LUMEN_PICTURE_FILE, &lumenTexture) ||
      !loadTexture(COFFEEBEAN_PICTURE_FILE, &coffeeBeanTexture) ||
      !loadTexture(COCOABEAN_PICTURE_FILE, &cocoaBeanTexture) ||
      !loadTexture(ESPRESSO_PICTURE_FILE, &espressoTexture) ||
      !loadTexture(GREENTEA_PICTURE_FILE, &greenTeaTexture) ||
      !loadTexture(MACHIATTO_PICTURE_FILE, &machiattoTexture) ||
      !loadTexture(CAPPUCCINO_PICTURE_FILE, &cappuccinoTexture)) [[unlikely]]
    return valid; // valid was already set to false
  return valid;
}

bool View::loadTexture(const char* picturePath, SDL_Texture** destination) {
#ifdef LUMEN_STRICT_CHECKS
  if (!valid || !renderer || !window) [[unlikely]] {
#  ifdef LUMEN_LOG
    spdlog::error("Invalid View state. {} {} {} {}", valid, renderer == nullptr,
                  window == nullptr, picturePath);
#  endif
    valid = false;
    return valid;
  }
#endif
  SDL_Surface* surface = IMG_Load(picturePath);
  if (!surface) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Failed to load image to sruface: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  *destination = SDL_CreateTextureFromSurface(renderer, surface);
  if (!(*destination)) [[unlikely]] {
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
static constexpr SDL_Color COFFEE_COLOR{44, 31, 22, 255};

SDL_AppResult View::render(const Model& model) const {
  if (model.isGameStarted()) [[likely]] {
    return drawCurrentGameState(model);
  }
  return drawLoadingScreen();
}

SDL_AppResult View::drawLoadingScreen() const {
#ifdef LUMEN_STRICT_CHECKS
  if (!SDL_SetRenderDrawColor(renderer, BLUE_BACKGROUND.r, BLUE_BACKGROUND.g,
                              BLUE_BACKGROUND.b, BLUE_BACKGROUND.a))
      [[unlikely]] {
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
  if (!SDL_RenderClear(renderer)) [[unlikely]] {

#  ifdef LUMEN_LOG
    spdlog::error("Could not set draw color {}. ", SDL_GetError());
#  endif
    return SDL_APP_FAILURE;
  }
#else
  SDL_RenderClear(renderer);
#endif

  SDL_FRect dst = {
      width / 10.0f,         // x
      height / 10.0f,        // y
      width / 10.0f * 7.8f,  // width
      height / 10.0f * 7.8f, // height
  };

  if (!drawTexture(lumenTexture, &dst)) [[unlikely]]
    return SDL_APP_FAILURE;

#ifdef LUMEN_STRICT_CHECKS
  if (!SDL_RenderPresent(renderer)) [[unlikely]] {
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

bool View::drawTiles(const Model& model) const {
#ifdef LUMEN_STRICT_CHECKS
  if (!SDL_SetRenderDrawColor(renderer, BLUE_BACKGROUND.r, BLUE_BACKGROUND.g,
                              BLUE_BACKGROUND.b, BLUE_BACKGROUND.a))
      [[unlikely]] {

#  ifdef LUMEN_LOG
    spdlog::error("Could not set render draw color {}. ", SDL_GetError());
#  endif
    return false;
  }
  if (!SDL_RenderClear(renderer)) [[unlikely]] {
#  ifdef LUMEN_LOG
    spdlog::error("Could not render clear {}. ", SDL_GetError());
#  endif
    return false;
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
      if (!tileOption.has_value()) [[unlikely]]
        return false;
#endif
      Tile t = tileOption.value();
      if (std::holds_alternative<HIDDEN>(t)) [[likely]] {
        if (!drawHidden(row, column)) [[unlikely]]
          return false;
      } else if (std::holds_alternative<DELETED>(t)) {
        // do nothing
      } else if (std::holds_alternative<COFFEE_BEAN>(t)) {
        if (!drawCoffeeBean(row, column)) [[unlikely]]
          return false;
      } else if (std::holds_alternative<COCOA_BEAN>(t)) {
        if (!drawCocoaBean(row, column)) [[unlikely]]
          return false;
      } else if (std::holds_alternative<CAPPUCCINO>(t)) {
        if (!drawCappuccino(row, column)) [[unlikely]]
          return false;
      } else if (std::holds_alternative<ESPRESSO>(t)) {
        if (!drawEspresso(row, column)) [[unlikely]]
          return false;
      } else if (std::holds_alternative<MACHIATTO>(t)) {
        if (!drawMachiatto(row, column)) [[unlikely]]
          return false;
      } else if (std::holds_alternative<GREEN_TEA>(t)) {
        if (!drawGreenTea(row, column)) [[unlikely]]
          return false;
      }
    }
  }
  return true;
}

bool View::drawTimer(const Model& model) const {
  std::optional<Model::TimePoint> t{model.getStartTime()};
#ifdef LUMEN_STRICT_CHECKS
  if (!t) [[unlikely]] {
#  ifdef LUMEN_LOG
    spdlog::error(
        "Invalid model state, game not started yet but requesting start time.");
#  endif
    return false;
  }
#endif
  Model::TimePoint startTime{t.value()};

  int ms =
      static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(
                           Model::Clock::now() - startTime)
                           .count());
  float maxBarHeight{static_cast<float>(9.0/10.0*height)};
  float barHeight{static_cast<float>(std::min(ms / MAX_TIME_MILLISECONDS, 1.0) *
                                     9.0 / 10.0 * height)};
  float x{static_cast<float>(4 * square_side + 5 * square_gap)},
      y{static_cast<float>(height / 20.0 + maxBarHeight - barHeight)}, w{static_cast<float>(square_side)},
      h{barHeight};
  SDL_FRect rect{x, y, w, h};
  if (!SDL_SetRenderDrawColor(renderer, COFFEE_COLOR.r, COFFEE_COLOR.g,
                              COFFEE_COLOR.b, COFFEE_COLOR.a)) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Could not render timer.");
#endif
    return false;
  }
  if (!SDL_RenderFillRect(renderer, &rect)) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Could not render timer.");
#endif
    return false;
  }
  return true;
}

SDL_AppResult View::drawCurrentGameState(const Model& model) const {
  if (!drawTiles(model)) [[unlikely]]
    return SDL_APP_FAILURE;
  if (!drawTimer(model)) [[unlikely]]
    return SDL_APP_FAILURE;
/* put the newly-cleared rendering on the screen. */
#ifdef LUMEN_STRICT_CHECKS
  if (!SDL_RenderPresent(renderer)) [[unlikely]] {
    spdlog::error("Could not render present the renderer {}. ", SDL_GetError());
    return SDL_APP_FAILURE;
  }
#else
  SDL_RenderPresent(renderer);
#endif

  return SDL_APP_CONTINUE; /* carry on with the program! */
}

bool View::drawTexture(SDL_Texture* texture, SDL_FRect* position) const {
#ifdef LUMEN_STRICT_CHECKS
  if (!SDL_RenderTexture(renderer, texture, nullptr, position)) [[unlikely]] {
#  ifdef LUMEN_LOG
    spdlog::error("Could not render texture {}. ", SDL_GetError());
#  endif
    return false;
  }
  return true;
#else
  return SDL_RenderTexture(renderer, texture, nullptr, position);
#endif
}

SDL_FRect View::getSquareDrawingBox(int row, int column) const {
  float y{static_cast<float>(row * square_side + (row + 1) * square_gap)};
  float x{static_cast<float>(column * square_side + (column + 1) * square_gap)};
  const SDL_FRect rect{x, y, static_cast<float>(square_side),
                       static_cast<float>(square_side)};
  return rect;
}

bool View::drawHidden(int row, int column) const {
  SDL_FRect rect{getSquareDrawingBox(row, column)};
#ifdef LUMEN_STRICT_CHECKS
  if (!SDL_SetRenderDrawColor(renderer, HIDDEN_FOREGROUND.r,
                              HIDDEN_FOREGROUND.g, HIDDEN_FOREGROUND.b,
                              HIDDEN_FOREGROUND.a)) [[unlikely]] {
#  ifdef LUMEN_LOG
    spdlog::error("Could not set color {}", SDL_GetError());
#  endif
    return false;
  }
  if (!SDL_RenderFillRect(renderer, &rect)) [[unlikely]] {
#  ifdef LUMEN_LOG
    spdlog::error("Could not fill rectangle {}", SDL_GetError());
#  endif
    return false;
  }
  return true;
#else
  return SDL_SetRenderDrawColor(renderer, HIDDEN_FOREGROUND.r,
                                HIDDEN_FOREGROUND.g, HIDDEN_FOREGROUND.b,
                                HIDDEN_FOREGROUND.a) &&
         SDL_RenderFillRect(renderer, &rect);
#endif
}

bool View::drawCoffeeBean(int row, int column) const {
  SDL_FRect rect{getSquareDrawingBox(row, column)};
  return drawTexture(coffeeBeanTexture, &rect);
}

bool View::drawCocoaBean(int row, int column) const {
  SDL_FRect rect{getSquareDrawingBox(row, column)};
  return drawTexture(cocoaBeanTexture, &rect);
}
bool View::drawCappuccino(int row, int column) const {
  SDL_FRect rect{getSquareDrawingBox(row, column)};
  return drawTexture(cappuccinoTexture, &rect);
}
bool View::drawEspresso(int row, int column) const {
  SDL_FRect rect{getSquareDrawingBox(row, column)};
  return drawTexture(espressoTexture, &rect);
}
bool View::drawMachiatto(int row, int column) const {
  SDL_FRect rect{getSquareDrawingBox(row, column)};
  return drawTexture(machiattoTexture, &rect);
}
bool View::drawGreenTea(int row, int column) const {
  SDL_FRect rect{getSquareDrawingBox(row, column)};
  return drawTexture(greenTeaTexture, &rect);
}

} // namespace LumenCappuccino
