#include "kit.h"

struct camera Camera;
struct menu Menu;
unsigned int hovered_rect_x;
unsigned int hovered_rect_y;
unsigned int hovered_block;
unsigned int selected_block;
unsigned int selected_rect_x;
unsigned int selected_rect_y;

#define block_size 10

static void set_color(uint32_t hex) {
    Uint8 r = (hex >> 16) & 0xFF;
    Uint8 g = (hex >> 8) & 0xFF;
    Uint8 b = hex & 0xFF;
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
}

static void render(ptr ptr) {
    if (!renderer) return;
    if (w1.width == 0 || w1.height == 0) return;

    switch (Menu.id) {

    case 0 : {
        set_color(0x3A3085);
        SDL_RenderClear(renderer);

        // SDL_RenderFillRect(renderer, &rect);
        // double sy = w1.height/100;
        // double sx = w1.width/100;

        int shift = w1.height/5;
        SDL_Rect rect = { .x=(w1.width-((w1.width/100.0)*70.0))/2.0, .y=shift, .w=(w1.width/100.0)*70.0, .h=(w1.height/100.0)*13.0 };
        set_color(0xFFFFFF);
        SDL_RenderFillRect(renderer, &rect);
        rect.y += shift;
        SDL_RenderFillRect(renderer, &rect);
        rect.y += shift;
        SDL_RenderFillRect(renderer, &rect);

        break;
    }

    case 1 : {
        int bs = (int)(block_size * Camera.zoom);
        if (bs < 1) bs = 1;

        // clock_t start = clock();

        // set_color(0xAAAAAA);
        set_color(0x3A3085);
        SDL_RenderClear(renderer);

        double inv = 1.0 / (block_size * Camera.zoom);

        int x0 = (int)floor((0 - Camera.x) * inv);
        int y0 = (int)floor((0 - Camera.y) * inv);
        int x1 = (int)ceil((w1.width  - Camera.x) * inv);
        int y1 = (int)ceil((w1.height - Camera.y) * inv);

        // if (x0 < 0) x0 = 0;
        // if (y0 < 0) y0 = 0;
        // if (x1 > world_size) x1 = world_size;
        // if (y1 > world_size) y1 = world_size;

        // double scale = block_size * Camera.zoom;

        // double rx = Camera.x + x0 * scale;
        // double ry = Camera.y + y0 * scale;
        // double rx_end = Camera.x + x1 * scale;
        // double ry_end = Camera.y + y1 * scale;

        // int rect_x = (int)floor(rx);
        // int rect_y = (int)floor(ry);
        // int rect_w = (int)ceil(rx_end) - rect_x;
        // int rect_h = (int)ceil(ry_end) - rect_y;

        // if (rect_w < 0) rect_w = 0;
        // if (rect_h < 0) rect_h = 0;

        // XSetForeground(d, gc, 0x878787);
        // XFillRectangle(d, buffer, gc, rect_x, rect_y, rect_w, rect_h);

        int block_x = WBlock_X();
        int block_y = WBlock_Y();

        int selected = 1;
        if (Menu.mode==1) selected = 0;

        for (int x = x0; x < x1; x++) {
            for (int y = y0; y < y1; y++) {

                int px = (int)(Camera.x + x * block_size * Camera.zoom);
                int py = (int)(Camera.y + y * block_size * Camera.zoom);

                int px1 = (int)(Camera.x + (x+1) * block_size * Camera.zoom);
                int py1 = (int)(Camera.y + (y+1) * block_size * Camera.zoom);

                if ((x + y) % 2 == 0) set_color(0x5E5E5E);
                else set_color(0x878787);
                SDL_Rect rect = { px, py, px1 - px, py1 - py };
                if (rect.w > 0 && rect.h > 0) SDL_RenderFillRect(renderer, &rect);
                else continue;
                // if (world[x][y].type<block_types&&world[x][y].type!=-1) {
                static long previous_color;
                long color = get_block(x, y).color;
                if (previous_color != color) {
                    previous_color = color;
                    SET_COLOR(get_block(x, y).color);
                }
                if (255-((color>>24)&0xFF)) SDL_RenderFillRect(renderer, &rect);
                // }
            }
        }

        if (selected) {
            int px = (int)(Camera.x + block_x * block_size * Camera.zoom);
            int py = (int)(Camera.y + block_y * block_size * Camera.zoom);

            int px1 = (int)(Camera.x + (block_x+1) * block_size * Camera.zoom);
            int py1 = (int)(Camera.y + (block_y+1) * block_size * Camera.zoom);

            SDL_Rect rect = {
                (int)(Camera.x + block_x * block_size * Camera.zoom),
                (int)(Camera.y + block_y * block_size * Camera.zoom),
                px1-px, py1-py
            };
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 128);
            SDL_RenderFillRect(renderer, &rect);
            set_color(0xFFFFFF);
            SDL_RenderDrawRect(renderer, &rect);
        }

        SDL_SetRenderDrawColor(renderer, 00, 00, 00, 128);
        SDL_Rect rect;
        rect.h = 300;
        rect.w = 250;
        rect.x = w1.width-rect.w;
        rect.y = w1.height-rect.h;
        SDL_RenderFillRect(renderer, &rect);
        set_color(0xFFFFFF);
        rect.h+=1;
        rect.w+=1;
        SDL_RenderDrawRect(renderer, &rect);

        int rx = rect.x+10;
        hovered_block = -1;
        SDL_Rect hovered_rect = {0};

        rect.h = 20;
        rect.w = 20;
        rect.x += 10;
        rect.y += 10;

        for (int i = 0; i < block_types; i++) {
            if (rect.x>w1.width-30) {
                rect.x = rx;
                rect.y += 30;
            }
            if (Mouse.x >= rect.x-5 && Mouse.x-5 < rect.x + rect.w &&
                Mouse.y >= rect.y-5 && Mouse.y-5 < rect.y + rect.h) {
                hovered_block = i;
                hovered_rect = rect;
            }
            SET_COLOR(block_type[i].color);
            SDL_RenderFillRect(renderer, &rect);
            rect.x += 30;
        }

        if (hovered_block != -1) {
            SDL_SetRenderDrawColor(renderer, 0x99, 0x99, 0x99, 255);
            hovered_rect_x = hovered_rect.x;
            hovered_rect_y = hovered_rect.y;

            hovered_rect.w += 10;
            hovered_rect.h += 10;
            hovered_rect.x -= 5;
            hovered_rect.y -= 5;

            SDL_RenderDrawRect(renderer, &hovered_rect);
        }

        if (selected_block != -1) {
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 255);
            hovered_rect.x = selected_rect_x;
            hovered_rect.y = selected_rect_y;

            hovered_rect.w = 30;
            hovered_rect.h = 30;

            hovered_rect.x -= 5;
            hovered_rect.y -= 5;
            SDL_RenderDrawRect(renderer, &hovered_rect);
        }
    }
    }
    SDL_RenderPresent(renderer);

    // clock_t end = clock();
    // double elapsed_ms = (end - start) * 1000.0 / CLOCKS_PER_SEC;
    // printf("Render took %.2f ms\n", elapsed_ms);
}

__attribute__((constructor))
static void start() {
    LOG("Module %s loaded",FILENAME);
    Menu.id = 1;
    Mouse.x = 0xFFFFFFFF;
    Mouse.y = 0xFFFFFFFF;
    on_event(EVENT_FPS, render);
}
