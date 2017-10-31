#include "for.h"
#define N 10

active proctype P() {
    int sum = 0;
    for (i, 1, N)
        sum = sum + i
    rof(i);
    printf("The sum of the first %d numbers is %d\n", N, sum);
}
