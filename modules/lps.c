#include "kit.h"

unsigned long max;

static void testing(ptr ptr) {
    // DEBUG((void *)"lps");
    // LOG("Working");
    if (lps>max) max = lps;
    if (exitbool) DEBUG(max);
    // DEBUG(lps);
}

__attribute__((constructor))
static void test() {
    LOG("Module %s loaded",FILENAME);
    // DEBUG((ptr)"lps");
    on_event(EVENT_SEC, testing);
}