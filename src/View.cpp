#include "View.hpp"
#include "Model.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <variant>

#ifdef LUMEN_LOG
#  include <spdlog/spdlog.h>
#endif

namespace LumenCappuccino {
View::View()
    : valid(true), height(), width(), square_side(), square_gap(),
      window(nullptr), renderer(nullptr), lumenTexture(nullptr),
      coffeeBeanTexture(nullptr), cocoaBeanTexture(nullptr),
      espressoTexture(nullptr), machiattoTexture(nullptr),
      greenTeaTexture(nullptr), cappuccinoTexture(nullptr), font(nullptr),
      wonTextTexture(nullptr), lostTextTexture(nullptr),
      introTextTexture(nullptr), wonTextPosition(), lostTextPosition(),
      introTextPosition() {}

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

  if (!TTF_Init()) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't initialize text rendering system: {}",
                  SDL_GetError());
#endif
    valid = false;
    return valid;
  }

  font = TTF_OpenFont(FONT_FILENAME, 32);
  if (!font) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't load font: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }

  SDL_Surface* wonSurface =
      TTF_RenderText_Blended(font, WINNING_TEXT, 0, GREEN_FOREGROUND);
  if (!wonSurface) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't render text to surface: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  wonTextTexture = SDL_CreateTextureFromSurface(renderer, wonSurface);
  if (!wonTextTexture) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't render text to texture: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  if (!SDL_GetTextureSize(wonTextTexture, &wonTextPosition.w,
                          &wonTextPosition.h)) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't get texture size: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  wonTextPosition.x = square_side;
  wonTextPosition.y = square_side;
  SDL_DestroySurface(wonSurface);

  SDL_Surface* lostSurface =
      TTF_RenderText_Blended(font, LOSING_TEXT, 0, RED_FOREGROUND);
  if (!lostSurface) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't render text to surface: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  lostTextTexture = SDL_CreateTextureFromSurface(renderer, lostSurface);
  if (!lostTextTexture) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't render text to texture: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  if (!SDL_GetTextureSize(lostTextTexture, &lostTextPosition.w,
                          &lostTextPosition.h)) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't get texture size: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  lostTextPosition.x = square_side;
  lostTextPosition.y = square_side;
  SDL_DestroySurface(lostSurface);

  SDL_Surface* introSurface =
      TTF_RenderText_Blended(font, INTRO_TEXT, 0, BLACK_FOREGROUND);
  if (!introSurface) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't render text to surface: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  introTextTexture = SDL_CreateTextureFromSurface(renderer, introSurface);
  if (!introTextTexture) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't render text to texture: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  if (!SDL_GetTextureSize(introTextTexture, &introTextPosition.w,
                          &introTextPosition.h)) [[unlikely]] {
#ifdef LUMEN_LOG
    spdlog::error("Couldn't get texture size: {}", SDL_GetError());
#endif
    valid = false;
    return valid;
  }
  introTextPosition.x = square_side;
  introTextPosition.y = square_side;
  SDL_DestroySurface(introSurface);

  return valid;
}

View::~View() {
  SDL_DestroyTexture(cappuccinoTexture);
  SDL_DestroyTexture(coffeeBeanTexture);
  SDL_DestroyTexture(machiattoTexture);
  SDL_DestroyTexture(espressoTexture);
  SDL_DestroyTexture(greenTeaTexture);
  SDL_DestroyTexture(cocoaBeanTexture);
  SDL_DestroyTexture(lumenTexture);
  SDL_DestroyTexture(wonTextTexture);
  SDL_DestroyTexture(lostTextTexture);
  SDL_DestroyTexture(introTextTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(font);
  TTF_Quit();
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

SDL_AppResult View::render(const Model& model) {
  if (!model.isGameStarted()) [[likely]]
    return drawLoadingScreen();
  if (model.isGameStarted() && !model.isGameOver()) [[likely]] {
    return drawCurrentGameState(model);
  } else if (model.isGameOver() && model.isGameLost()) {
    return drawGameLost();
  } else if (model.isGameOver() && model.isGameWon()) {
    return drawGameWon(model);
  }
  // should be unreachable
  return SDL_APP_FAILURE;
}

SDL_AppResult View::drawGameLost() {

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

#ifdef LUMEN_STRICT_CHECKS
  if (!drawTexture(lostTextTexture, &lostTextPosition)) [[unlikely]] {
    // already prints on its own
    return SDL_APP_FAILURE;
  }
#else
  drawTexture(lostTextTexture, &lostTextPosition);
#endif

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

SDL_AppResult View::drawGameWon(const Model& model) {

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

#ifdef LUMEN_STRICT_CHECKS
  if (!drawTimer(model)) [[unlikely]] {
    // already prints on its own
    return SDL_APP_FAILURE;
  }
#else
  drawTimer(model);
#endif

#ifdef LUMEN_STRICT_CHECKS
  if (!drawTexture(wonTextTexture, &wonTextPosition)) [[unlikely]] {
    // already prints on its own
    return SDL_APP_FAILURE;
  }
#else
  drawTexture(wonTextTexture, &wonTextPosition);
#endif

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

SDL_AppResult View::drawLoadingScreen() {
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
  if (!drawTexture(introTextTexture, &introTextPosition)) [[unlikely]] {
    // already prints on its own
    return SDL_APP_FAILURE;
  }
#else
  drawTexture(introTextTexture, &introTextPosition);
#endif

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
  float maxBarHeight{static_cast<float>(9.0 / 10.0 * height)};
  // prints if errors
  float timerPercent{model.getTimerPercent()};
#ifdef LUMEN_STRICT_CHECKS
  if (timerPercent < 0 || timerPercent > 100)
    return false;
#endif
  float barHeight{timerPercent * maxBarHeight / 100.0f};
  float x{static_cast<float>(4 * square_side + 5 * square_gap)},
      y{static_cast<float>(height / 20.0 + maxBarHeight - barHeight)},
      w{static_cast<float>(square_side)}, h{barHeight};
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

std::optional<std::pair<int, int>> View::getTile(int x,
                                                 [[maybe_unused]] int y) const {
  int row{-1}, column{-1};
  int removedOffset{square_gap}, xSteps{}, ySteps{};
  while (x > 0) {
    ++xSteps;
    if (xSteps > 9)
      return {};
    x -= removedOffset;
    removedOffset =
        (removedOffset == square_gap) ? (square_side) : (square_gap);
  }
  if (xSteps % 2 == 0)
    column = (xSteps - 1) / 2;
  else
    return {};
  removedOffset = square_gap;
  while (y > 0) {
    ++ySteps;
    if (ySteps > 7)
      return {};
    y -= removedOffset;
    removedOffset =
        (removedOffset == square_gap) ? (square_side) : (square_gap);
  }
  if (ySteps % 2 == 0)
    row = (ySteps - 1) / 2;
  else
    return {};
  return {{row, column}};
}

} // namespace LumenCappuccino
