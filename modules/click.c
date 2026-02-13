#include "kit.h"

extern unsigned int hovered_rect_x;
extern unsigned int hovered_rect_y;
extern unsigned int hovered_block;
extern unsigned int selected_block;
extern unsigned int selected_rect_x;
extern unsigned int selected_rect_y;

static void click(char *ptr) {
    char button = *ptr;
    if (Menu.id!=1||Menu.mode!=1) return;
    if (Mouse.x >= hovered_rect_x-5 && Mouse.x-5 < hovered_rect_x + 30 &&
    Mouse.y >= hovered_rect_y-5 && Mouse.y-5 < hovered_rect_y + 30) {
        if (button!=1) return;
        selected_block = hovered_block;
        selected_rect_x = hovered_rect_x;
        selected_rect_y = hovered_rect_y;
        // LOG("Right click");
    }
}

static void place(char *ptr) {
    char button = *ptr;
    if (Menu.id!=1||Menu.mode!=0||button!=1) return;
    #define block_size 10
    int x = WBlock_X();
    int y = WBlock_Y();

    if (x<world_size&&y<world_size&&x>=0&&y>=0) {
        world[x][y].type = selected_block;
    }
}

__attribute__((constructor))
static void start() {
    LOG("Module %s loaded",FILENAME);
    on_event(EVENT_CLK, click);
    on_event(EVENT_CLK, place);
}