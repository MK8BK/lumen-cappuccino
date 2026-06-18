#include "Controller.hpp"
#include <SDL3/SDL_init.h>

#ifdef LUMEN_LOG
    #include <spdlog/spdlog.h>
#endif

using namespace LumenCappuccino;


Controller::Controller():valid(true), view(), model(){
    // for gnome and wayland when somehow libdecor is not used
    SDL_SetHint(
        SDL_HINT_VIDEO_WAYLAND_PREFER_LIBDECOR,
        "1"
    );
    SDL_SetAppMetadata("Lumen Cappuccino", "1.0", "io.github.mk8bk");
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS )) {
        #ifdef LUMEN_LOG
            spdlog::error("Couldn't initialize SDL: %s", SDL_GetError());
        #endif
        valid = false;
    }
    if (!view.setup() || !model.setup())
      valid = false;
}

// TODO: for now may be unused, change later
SDL_AppResult Controller::handleEvent([[maybe_unused]] SDL_Event* event){
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    return SDL_APP_CONTINUE;
}

bool Controller::isValid() const{
    return valid;
}

SDL_AppResult Controller::render() const{
    //#ifdef LUMEN_STRICT_CHECKS
    //if(!view.isValid())
    //    spdlog::error("View invalid state.");
    //#endif
    return view.render(model);
}
