// #include "kit.h"

// #include <X11/Xlib.h>
// #include <stdio.h>

// static int dragging = 0;
// static int drag_button = 0;
// static int last_x = 0, last_y = 0;

// static void mouse(ptr p) {
//     XEvent *ev = (XEvent *)p;

//     switch (ev->type) {

//     case ButtonPress: {
//         XButtonEvent *b = &ev->xbutton;

//         // last_x = b->x;
//         // last_y = b->y;

//         // Modifier keys
//         // int shift = b->state & ShiftMask;
//         // int ctrl  = b->state & ControlMask;
//         // int alt   = b->state & Mod1Mask;

//         // printf("Button %u DOWN at (%d,%d)  [shift=%d ctrl=%d alt=%d]\n",
//         //        b->button, b->x, b->y, shift, ctrl, alt);

//         // Scroll wheel
//         // if (b->button == Button4)
//         //     puts("Scroll UP");
//         // else if (b->button == Button5)
//         //     puts("Scroll DOWN");

//         // Start drag
//         if (b->button == Button1 || b->button == Button3) {
//             dragging = 1;
//             drag_button = b->button;
//         }
//         break;
//     }

//     case ButtonRelease: {
//         XButtonEvent *b = &ev->xbutton;

//         // printf("Button %u UP at (%d,%d)\n", b->button, b->x, b->y);

//         // End drag
//         if (dragging && b->button == drag_button) {
//             dragging = 0;
//             drag_button = 0;
//             puts("Drag END");
//         }
//         break;
//     }

//     case MotionNotify: {
//         XMotionEvent *m = &ev->xmotion;

//         int dx = m->x - last_x;
//         int dy = m->y - last_y;

//         last_x = m->x;
//         last_y = m->y;

//         // int shift = m->state & ShiftMask;
//         // int ctrl  = m->state & ControlMask;
//         // int alt   = m->state & Mod1Mask;

//         if (dragging) {
//             #define multiplier *1
//             Camera.x += dx multiplier;
//             Camera.y += dy multiplier;
//             #undef multiplier
//             LOG("");
//             // printf("Dragging (btn %d) at (%d,%d) Δ(%d,%d) "
//             //        "[shift=%d ctrl=%d alt=%d]\n",
//             //        drag_button, m->x, m->y, dx, dy,
//             //        shift, ctrl, alt);
//         } else {
//             // printf("Mouse move at (%d,%d) Δ(%d,%d)\n",
//             //        m->x, m->y, dx, dy);
//         }
//         break;
//     }

//     default:
//         break;
//     }
// }


// __attribute__((constructor))
// static void constructor() {
//     LOG("Module %s loaded",FILENAME);
//     on_event(EVENT_INP, mouse);
// }