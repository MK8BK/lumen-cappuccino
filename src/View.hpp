#ifndef VIEW__HPP
#define VIEW__HPP
#include <SDL3/SDL.h>
#include "Model.hpp"

namespace LumenCappuccino {

class View{
private:
    bool valid;
    int height, width, square_side, square_gap;
    SDL_Window *window;
    SDL_Renderer *renderer;
    void drawHidden(int row, int column) const;
public:
    View();
    bool isValid() const;
    bool setup();
    SDL_AppResult render(const Model& model) const;
};


} // namespace LumenCappuccino
#endif
