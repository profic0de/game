#include "kit.h"

SDL_Window *window_handle;
SDL_Renderer *renderer;

int screen_w;
int screen_h;

static void loop(ptr ptr) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) exitbool = 1;
        call_event(EVENT_INP, &e);
    }
}

__attribute__((constructor))
static int window(void) {
    LOG("Module %s loaded",FILENAME);
#ifdef __linux__
    // if (!getenv("SDL_VIDEODRIVER")) {
    //     setenv("SDL_VIDEODRIVER", "x11", 0);
    // }
    // SDL_SetHint(SDL_HINT_VIDEO_X11_FORCE_EGL, "0");
#endif
    if (SDL_Init(SDL_INIT_VIDEO) != 0) return fprintf(stderr, "%cSDL init failed: %s\n", exitbool++, SDL_GetError());

    SDL_DisplayMode mode;
    if (SDL_GetCurrentDisplayMode(0, &mode) == 0) {
        screen_w = mode.w;
        screen_h = mode.h;
    } else {
        screen_w = 854;
        screen_h = 480;
    }

    window_handle = SDL_CreateWindow(
        "mcords",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        854, 480,
        SDL_WINDOW_RESIZABLE
    );
    if (!window_handle) return fprintf(stderr, "%cSDL window create failed: %s\n", exitbool++, SDL_GetError());

    renderer = SDL_CreateRenderer(window_handle, -1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) renderer = SDL_CreateRenderer(window_handle, -1, SDL_RENDERER_SOFTWARE);
    if (!renderer) return fprintf(stderr, "%cSDL renderer create failed: %s\n", exitbool++, SDL_GetError());

    w1.width = 854;
    w1.height = 480;
    on_event(EVENT_LPS, loop);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    return 0;
}

__attribute__((destructor))
static void end(void) {
    if (!window_handle) return;
    if (renderer) SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window_handle);
    SDL_GL_UnloadLibrary();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}
