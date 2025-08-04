#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    const char *filename = "output.txt";
    const char str[] = "String from file";
    char buffer[256];
    FILE *fp = fopen(filename, "w");

    size_t count = fwrite(str, sizeof(char), strlen(str), fp);
    if (count != strlen(str)) {
        fprintf(stderr, "Error writing to file\n");
        fclose(fp);  
        return 1;
    }
    fclose(fp);
    fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);

    if (file_size == -1) {
        perror("Error getting file size");
        fclose(fp);
        return 1;
    }

    if (file_size == 0) {
        printf("File is empty.\n");
        fclose(fp);
        return 0;
    }

    printf("Content of the file (reversed):\n");
    for (long i = file_size - 1; i >= 0; i--) {
        fseek(fp, i, SEEK_SET);
        int c = fgetc(fp);
        if (c == EOF) {
            fprintf(stderr, "Error reading character from file\n");
            break;
        }

        printf("%c", (char)c);
    }

    printf("\n");
    fclose(fp);
    return 0;
}