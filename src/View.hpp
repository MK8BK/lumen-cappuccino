#ifndef VIEW__HPP
#define VIEW__HPP
#include "Model.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace LumenCappuccino {

class View {
#define LUMEN_PICTURE_FILE "lumen.png"
#define COFFEEBEAN_PICTURE_FILE "coffee_bean.png"
#define COCOABEAN_PICTURE_FILE "cocoa_beans.png"
#define ESPRESSO_PICTURE_FILE "espresso.png"
#define GREENTEA_PICTURE_FILE "green_tea.png"
#define MACHIATTO_PICTURE_FILE "machiatto.png"
#define CAPPUCCINO_PICTURE_FILE "cappuccino.png"
#define FONT_FILENAME "Gokart-Bubble.ttf"
#define WINNING_TEXT "You won!   You deserve a coffee. Coming right up Chief!"
#define LOSING_TEXT "You lost!  Hope this coffee cheers you up mate!"
#define INTRO_TEXT "Habibi, welcome to Lumen!\n Click Enter to order a coffee."
  static constexpr SDL_Color BLUE_BACKGROUND{65, 105, 225, 255};
  static constexpr SDL_Color HIDDEN_FOREGROUND{0, 20, 64, 255};
  static constexpr SDL_Color COFFEE_COLOR{44, 31, 22, 255};
  static constexpr SDL_Color RED_FOREGROUND{255, 105, 97, 255};
  static constexpr SDL_Color GREEN_FOREGROUND{50, 205, 50, 255};
  static constexpr SDL_Color BLACK_FOREGROUND{0, 0, 0, 255};

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
  TTF_Font* font;
  SDL_Texture* wonTextTexture;
  SDL_Texture* lostTextTexture;
  SDL_Texture* introTextTexture;
  SDL_FRect wonTextPosition;
  SDL_FRect lostTextPosition;
  SDL_FRect introTextPosition;
  SDL_AppResult drawLoadingScreen();
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
  SDL_AppResult drawGameLost();
  SDL_AppResult drawGameWon(const Model& model);

public:
  View();
  ~View();
  bool isValid() const;
  bool setup();
  SDL_AppResult render(const Model& model);
  std::optional<std::pair<int, int>> getTile(int x, int y) const;
};

} // namespace LumenCappuccino
#endif
