#include "Controller.hpp"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3_mixer/SDL_mixer.h>

#ifdef LUMEN_LOG
    #include <spdlog/spdlog.h>
#endif

using namespace LumenCappuccino;


Controller::Controller():valid(true), view(), model(), musicPlayer(){
    // for gnome and wayland when somehow libdecor is not used
    SDL_SetHint(
        SDL_HINT_VIDEO_WAYLAND_PREFER_LIBDECOR,
        "1"
    );
    // this better not fail haha, who cares
    SDL_SetAppMetadata("Lumen Cappuccino", "1.0", "io.github.mk8bk");
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS )) {
        #ifdef LUMEN_LOG
            spdlog::error("Couldn't initialize SDL: {}", SDL_GetError());
        #endif
        valid = false;
    }
    if (!view.setup() || !model.setup() || !musicPlayer.setup())
      valid = false;
    musicPlayer.playLoadingScreenAudio();
}

SDL_AppResult Controller::handleKey(SDL_Keycode key){
    if(key==SDLK_SPACE){
        if(model.isGameStarted()) return SDL_APP_CONTINUE;
        static bool playCoffee = false;
        if(playCoffee)
            musicPlayer.playCoffeeMachineAudio();
        else
            musicPlayer.playLoadingScreenAudio();
        playCoffee = !playCoffee;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult Controller::handleEvent(SDL_Event* event){
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }else if(event->type==SDL_EVENT_KEY_DOWN){
        SDL_FlushEvents(SDL_EVENT_KEY_DOWN, SDL_EVENT_KEY_UP);
        return handleKey(event->key.key);

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
