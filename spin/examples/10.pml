#include "for.h"
byte n = 0;

proctype P() {
    byte temp;
    for (i, 1, 10)
        temp = n + 1;
    n = temp;
    rof(i);
}

init {
    atomic {
        run P();
        run Q();
    }
    (_nr_pr == 1) -> printf("The value is %d\n", n);
}
