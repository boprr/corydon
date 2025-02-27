#include "utils.h"

int math_divroundup(int a, int b) {
    if (a % b == 0) return a / b;
    return (a / b) + 1;
}
