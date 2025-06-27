#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <locale.h>

int main() {
    unsigned int originalNumber;
    unsigned char newByte;
    char inputBuffer[256];
    char *endptr;

    setlocale(LC_ALL, "C");

    printf("Enter a positive integer: ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    originalNumber = (unsigned int)strtoul(inputBuffer, &endptr, 10);

    printf("Enter a new value for the third byte (0-255): ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    newByte = (unsigned char)strtoul(inputBuffer, &endptr, 10);

    printf("Enter (binary): ");
    for (int i = 31; i >= 0; i--) {
        int bit = (originalNumber >> i) & 1;
        printf("%d", bit);
        if (i % 8 == 0 && i != 0) {
            printf(" ");
        }
    }
    printf("\n");
    unsigned int mask = 0x00FF0000;
    originalNumber = originalNumber & (~mask);
    unsigned int shiftedByte = (unsigned int)newByte << 16;
    originalNumber = originalNumber | shiftedByte;
    printf("Result (decimal): %u\n", originalNumber);
    printf("Result (binary): ");
    for (int i = 31; i >= 0; i--) {
        int bit = (originalNumber >> i) & 1;
        printf("%d", bit);
        if (i % 8 == 0 && i != 0) {
            printf(" ");
        }
    }
    printf("\n");

    return 0;
}