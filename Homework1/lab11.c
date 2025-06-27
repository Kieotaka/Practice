#include <stdio.h>

void decimalToBinaryRecursive(int n) {
    if (n == 0) {
        return; 
    }
    decimalToBinaryRecursive(n >> 1);
    printf("%d", n & 1);
}

int main() {
    int n;
    printf("Enter a positive integer: ");
    if (scanf("%d", &n) != 1) {
        printf("Error enter.\n");
        return 1;
    }
    printf("Binary representation: ");
    if (n == 0) {
        printf("0"); 
    } else {
        decimalToBinaryRecursive(n); 
    }
    printf("\n");

    return 0;
}