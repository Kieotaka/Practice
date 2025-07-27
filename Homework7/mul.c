#include "libcalc.h"
#include <stdio.h> 
int multiply(int* numbers, int count) {
    if (numbers == NULL || count <= 0) {
        return 0; 
    }

    int result = numbers[0];
    for (int i = 1; i < count; i++) {
        result *= numbers[i];
    }
    return result;
}