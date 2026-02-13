#include "kit.h"

Block world[world_size][world_size];

unsigned int block_types;
type *block_type;
extern unsigned int selected_block;

static void add_block(int type, char r, char g, char b, char a) {
    if (type < 0) return;
    unsigned int idx = (unsigned int)block_types;
    unsigned int needed = idx + 1;

    if (!block_type) {
        block_type = (struct type *)calloc(needed, sizeof(*block_type));
        if (!block_type) return;
        block_types = needed;
    } else if (needed > block_types) {
        struct type *next = (struct type *)realloc(block_type, needed * sizeof(*block_type));
        if (!next) return;
        memset(next + block_types, 0, (needed - block_types) * sizeof(*block_type));
        block_type = next;
        block_types = needed;
    }

    uint32_t color =
        ((uint32_t)(unsigned char)(255-a) << 24) |
        ((uint32_t)(unsigned char)r << 16) |
        ((uint32_t)(unsigned char)g << 8)  |
        (uint32_t)(unsigned char)b;

    block_type[idx].color = (unsigned long)color;
    block_type[idx].type = (char)type;
}

inline type get_block(int x, int y) {
    type out;
    out.type = 0;
    out.color = 0xFFFFFFFF;
    if (out.type < block_types) {
        out.color = block_type[(int)out.type].color;
    }
    return out;
}

__attribute__((constructor))
static void start() {
    LOG("Module %s loaded",FILENAME);
    for (int x = 0; x<world_size;x++) {
        for (int y = 0; y<world_size;y++) {
            world[x][y].type=-1;
        }
    }

    add_block(0, 0x70, 0x70, 0xF0, 0);
    add_block(0, 0, 128, 128, 0);
    add_block(0, 0, 255, 255, 0);
    add_block(0, 255, 0, 0, 0);
    add_block(0, 0, 0, 0, 0);

    DEBUG(block_types);

    selected_block = -1;
    world[5][5].type = 1;
    world[5][4].type = 2;
    world[5][3].type = 3;
    Camera.zoom = 4.0;
    Camera.x = 0.0;
    Camera.y = 0.0;
}
