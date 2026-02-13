#include "kit.h"

int hash(char *s){
    // unsigned int n = (int)((long)"ddd"%UINT32_MAX);
    // unsigned int n = (int)((long)'s'*'e'*'e'*'d'%UINT32_MAX);
    unsigned int n = 0;
    for (;*s!=0;s++) n = (*s+31*n)%UINT32_MAX;
    return (int)n; //Wait why do i need a hash function? -_-
}

// __attribute__((constructor))
// static void test() {
//     LOG("Module %s loaded",FILENAME);
//     DEBUG(hash("I mean this makes sense"));
// }