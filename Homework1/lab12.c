#include <stdio.h>
int countSetBitsRecursive(int n) {
    if (n == 0) {
        return 0;
    }
    return 1 + countSetBitsRecursive(n >> 1);
}
int main() {
    int n,setBitsCount;
    printf("Enter a negative integer: ");
    if (scanf("%d", &n) != 1) {
        printf("Error enter.\n");
        return 1;
    }
    int numBits = sizeof(int) * 8; 
    if(n < 0){
        setBitsCount = countSetBitsRecursive(n*(-1));
    }
    else{
        setBitsCount = countSetBitsRecursive(n);
    }
    printf("Binary representation %d:\n", n);
    for (int i = numBits - 1; i >= 0; i--) {
        int bit = (n >> i) & 1;
        if(i <=setBitsCount)
        printf("%d", bit);
        
    }
    printf("\n");
    return 0;
}