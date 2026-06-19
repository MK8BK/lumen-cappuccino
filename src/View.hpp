#ifndef VIEW__HPP
#define VIEW__HPP
#include <SDL3/SDL.h>
#include "Model.hpp"

#define LUMEN_PICTURE_FILE "lumen.png"

namespace LumenCappuccino {

class View{
private:
    bool valid;
    int height, width, square_side, square_gap;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *lumenTexture;
    void drawHidden(int row, int column) const;
    SDL_AppResult drawLoadingScreen() const;
    SDL_AppResult drawCurrentGameState(const Model& model) const;
    bool loadTexture(const char* picturePath, SDL_Texture** destination);
public:
    View();
    bool isValid() const;
    bool setup();
    SDL_AppResult render(const Model& model) const;
};


} // namespace LumenCappuccino
#endif
