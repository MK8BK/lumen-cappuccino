#include <SDL3/SDL_init.h>

#ifdef LUMEN_LOG
    #include <spdlog/spdlog.h>
#endif

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Controller.hpp"
using namespace LumenCappuccino;

/* We will use this renderer to draw into this window every frame. */


/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, [[maybe_unused]] char *argv[])
{
    #ifdef LUMEN_LOG
    spdlog::info("Welcome");
    #endif
    if(argc>1){
        #ifdef LUMEN_LOG
            spdlog::warn("Game takes no flags: %s ...", argv[1]);
        #endif
        // ignore for now
        // return SDL_APP_FAILURE;
    }
    Controller* controller{new Controller()};
    if(!controller || !controller->isValid()){
        #ifdef LUMEN_LOG
            spdlog::error("Could not setup SDL3 correctly.");
        #endif
        return SDL_APP_FAILURE;
    }
    *appstate = controller;
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    Controller* controller{(Controller*)appstate};
    return controller->handleEvent(event);
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    Controller* controller{(Controller*)appstate};
    #ifdef LUMEN_STRICT_CHECKS
        if(!controller || !controller->isValid()){
            spdlog::error("Could not setup SDL3 correctly.");
            return SDL_APP_FAILURE;
        }
    #endif

    return controller->render();
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, [[maybe_unused]] SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
    Controller* controller{(Controller*)appstate};
    if(controller)
        delete controller;

}
