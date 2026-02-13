#include "kit.h"

struct window w1;
struct mouse Mouse;

extern unsigned int selected_block;

static void keybind(ptr ptr) {
    SDL_Event event = *(SDL_Event*)ptr;

    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            if (Menu.id==1) {
                Menu.id=0;
                Menu.mode=0;
                Menu.stuff1=0;
                Menu.stuff2=0;
                return;
            }
            exitbool = 1;
        } else if (event.key.keysym.sym == SDLK_q) {
            selected_block = -1;
        }

    } else if (event.type == SDL_MOUSEWHEEL) {
        if (Menu.id!=1||Menu.mode==1) return;
        double old_zoom = Camera.zoom;
        double new_zoom = old_zoom;

        #define MAX_ZOOM 8.0
        #define MIN_ZOOM 0.4

        if (event.wheel.y > 0 && old_zoom < MAX_ZOOM) new_zoom = old_zoom * 1.1;
        else if (event.wheel.y < 0 && old_zoom > MIN_ZOOM) new_zoom = old_zoom * 0.9;
        else return;

        #undef MAX_ZOOM
        #undef MIN_ZOOM

        double world_x = (Mouse.x - Camera.x) / old_zoom;
        double world_y = (Mouse.y - Camera.y) / old_zoom;

        Camera.zoom = new_zoom;
        Camera.x = Mouse.x - world_x * new_zoom;
        Camera.y = Mouse.y - world_y * new_zoom;
    }
}

static void mouse(ptr p) {
    SDL_Event *ev = (SDL_Event *)p;

    switch (ev->type) {
        case SDL_MOUSEBUTTONDOWN: {
            SDL_MouseButtonEvent *b = &ev->button;
            if (b->button == SDL_BUTTON_LEFT || b->button == SDL_BUTTON_RIGHT) {
                Mouse.drag_button = b->button;
            }
            break;
        }

        case SDL_MOUSEBUTTONUP: {
            SDL_MouseButtonEvent *b = &ev->button;
            if (b->button == Mouse.drag_button) {
                Mouse.drag_button = 0;
            }
            break;
        }

        case SDL_MOUSEMOTION: {
            SDL_MouseMotionEvent *m = &ev->motion;

            int dx = m->x - Mouse.x;
            int dy = m->y - Mouse.y;

            Mouse.x = m->x;
            Mouse.y = m->y;

            Menu.mode = 0;
            if (Mouse.x>w1.width-250&&Mouse.y>w1.height-300&&Menu.id==1) Menu.mode=1;

            if (Mouse.drag_button == SDL_BUTTON_LEFT&&Menu.mode==0&&Menu.id==1) {
                #define multiplier *1
                Camera.x += dx multiplier;
                Camera.y += dy multiplier;
                #undef multiplier
            }
            break;
        }

        default:
            break;
    }
}

static void resize(SDL_Event *ev) {
    if (ev->type == SDL_WINDOWEVENT) {
        if (ev->window.event == SDL_WINDOWEVENT_SIZE_CHANGED || ev->window.event == SDL_WINDOWEVENT_RESIZED) {
            w1.height = ev->window.data2;
            w1.width = ev->window.data1;
            if (renderer) SDL_RenderSetViewport(renderer, NULL);
            if (renderer) {
                int rw = 0, rh = 0;
                if (SDL_GetRendererOutputSize(renderer, &rw, &rh) == 0) {
                    if (rw > 0) w1.width = rw;
                    if (rh > 0) w1.height = rh;
                }
            }
            call_event(EVENT_FPS, NULL);
        }
    }
}

static int click(SDL_Event *ev) {
    static char pressed, ticks;
    if (!ev) return ticks<UINT8_MAX?ticks++:0;

    switch (ev->type) {
        case SDL_MOUSEBUTTONDOWN: {
            SDL_MouseButtonEvent *b = &ev->button;
            if (pressed) break;
            if (b->button == SDL_BUTTON_LEFT || b->button == SDL_BUTTON_RIGHT) {
                pressed = b->button;
                ticks = 0;
            }
            break;
        }

        case SDL_MOUSEBUTTONUP: {
            SDL_MouseButtonEvent *b = &ev->button;
            if (b->button == pressed) {
                // DEBUG((int)ticks);
                if (ticks<=4) call_event(EVENT_CLK, &pressed);
                // if (ticks<=2) DEBUG((int)pressed);
                pressed = 0;
                ticks = 0;
            }
            break;
        }
    }
    return 0;
}

__attribute__((constructor))
static void constructor() {
    LOG("Module %s loaded",FILENAME);
    on_event(EVENT_INP, mouse);
    on_event(EVENT_INP, keybind);
    on_event(EVENT_INP, resize);
    on_event(EVENT_INP, click);
    on_event(EVENT_TPS, click);
}
