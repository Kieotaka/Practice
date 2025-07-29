#include "libcalc.h"
#include <stdio.h> 
int divide(int* numbers, int count) {
    if (numbers == NULL || count <= 0) {
        return 0; 
    }

    int result = numbers[0];
    for (int i = 1; i < count; i++) {
        if (numbers[i] == 0) {
            printf("Error: Division by zero!\n");
            return 0; 
        }
        result /= numbers[i];
    }
    return result;
}