#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main () {

#define one 0
#ifdef one
    printf("one is defined ");
#endif
#ifndef one
    printf("one is not defined ");
#endif

   return(0);
}
