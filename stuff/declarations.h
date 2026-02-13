#ifndef D_H
#define D_H

#include "kit.h"

void on_event(ptr event, ptr func);
void call_event(ptr event, ptr ptr);

extern SDL_Window *window_handle;
extern SDL_Renderer *renderer;

#define SET_COLOR(packed) do { \
    uint32_t _c = (uint32_t)(packed); \
    SDL_SetRenderDrawColor(renderer, (_c >> 16) & 0xFF, (_c >> 8) & 0xFF, _c & 0xFF, (_c >> 24) & 0xFF); \
} while (0)

struct window {
    int height;
    int width;
};

struct camera {
    double x, y;
    double zoom;
};

typedef struct type {
   unsigned long color;
   char type;
} type;
extern type *block_type;

struct mouse {
    int x, y;
    char drag_button;
};

struct menu {
    char id;
    char mode;
    char stuff1;
    char stuff2;
};

extern struct menu Menu;
extern struct window w1;
extern struct mouse Mouse;
extern struct camera Camera;

#define world_size 16
typedef struct Block {
    char type;
    char rotation;
} Block;
extern Block world[world_size][world_size];
extern unsigned int block_types;
type get_block(int x, int y);

#define WBlock_X() ((int)floor(((Mouse.x - Camera.x) / Camera.zoom) / block_size))
#define WBlock_Y() ((int)floor(((Mouse.y - Camera.y) / Camera.zoom) / block_size))

int delay(double seconds_interval, int *last_tick);

#define EVENT (void *)0
#define EVENT_LPS (void *)2
#define EVENT_FRE (void *)3
#define EVENT_SEC (void *)4
#define EVENT_INP (void *)5
#define EVENT_FPS (void *)6
#define EVENT_CLK (void *)7
#define EVENT_TPS (void *)8

#endif
