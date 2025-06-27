#include <stdio.h>

int countSetBitsRecursive(int n) {
    if (n == 0) {
        return 0;
    }
    return (n & 1) + countSetBitsRecursive(n >> 1);
}
int main() {
    int n;
    printf("Enter a positive integer: ");
    if (scanf("%d", &n) != 1) {
        printf("Error enter.\n");
        return 1;
    }
    int setBitsCount = countSetBitsRecursive(n);
    printf("The number of single bits in %d: %d\n", n, setBitsCount);
    return 0;
}