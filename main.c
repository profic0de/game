#include "kit.h"

char exitbool;
unsigned long lps;

// char timer;

int main(int argc, char **argv)
{
    unsigned long loops = 0;
    clock_t last_time = clock();
    double elapsed = 0;

    while (!exitbool) {
        call_event(EVENT_LPS, NULL);

        // This part calculates lps (loops per second)
        loops++;
        clock_t now = clock();
        elapsed = (double)(now - last_time) / CLOCKS_PER_SEC;
        if (elapsed >= 1.0) {
            lps = loops;
            loops = 0;
            last_time = now;
            // if (++timer>=5) exitbool = 1;
            call_event(EVENT_SEC, NULL);
            // LOG("%d", lps);
        }
    }
    return 0;
}
