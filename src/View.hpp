#ifndef VIEW__HPP
#define VIEW__HPP
#include "Model.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#define LUMEN_PICTURE_FILE "lumen.png"
#define COFFEEBEAN_PICTURE_FILE "coffee_bean.png"
#define COCOABEAN_PICTURE_FILE "cocoa_beans.png"
#define ESPRESSO_PICTURE_FILE "espresso.png"
#define GREENTEA_PICTURE_FILE "green_tea.png"
#define MACHIATTO_PICTURE_FILE "machiatto.png"
#define CAPPUCCINO_PICTURE_FILE "cappuccino.png"

namespace LumenCappuccino {

class View {
private:
  bool valid;
  int height, width, square_side, square_gap;
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* lumenTexture;
  SDL_Texture* coffeeBeanTexture;
  SDL_Texture* cocoaBeanTexture;
  SDL_Texture* espressoTexture;
  SDL_Texture* machiattoTexture;
  SDL_Texture* greenTeaTexture;
  SDL_Texture* cappuccinoTexture;
  SDL_AppResult drawLoadingScreen() const;
  SDL_AppResult drawCurrentGameState(const Model& model) const;
  bool loadTexture(const char* picturePath, SDL_Texture** destination);
  bool drawHidden(int row, int column) const;
  bool drawCoffeeBean(int row, int column) const;
  bool drawCocoaBean(int row, int column) const;
  bool drawCappuccino(int row, int column) const;
  bool drawEspresso(int row, int column) const;
  bool drawMachiatto(int row, int column) const;
  bool drawGreenTea(int row, int column) const;
  SDL_FRect getSquareDrawingBox(int row, int column) const;
  bool drawTexture(SDL_Texture* texture, SDL_FRect* position) const;
  bool drawTiles(const Model& model) const;
  bool drawTimer(const Model& model) const;

public:
  View();
  bool isValid() const;
  bool setup();
  SDL_AppResult render(const Model& model) const;
};

} // namespace LumenCappuccino
#endif
