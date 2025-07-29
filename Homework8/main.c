#include <stdio.h>
#include <stdlib.h>
#include "libcalc.h"

int main() {
    int choice;
    int *numbers = NULL;
    int num_count = 0;
    int result = 0;

    while (1) {

        printf("\nCalculator Menu:\n");
        printf("1) Addition\n");
        printf("2) Subtraction\n");
        printf("3) Multiplication\n");
        printf("4) Division\n");
        printf("5) Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number between 1 and 5.\n");
            while (getchar() != '\n');
            continue;
        }

        if (choice == 5) {
            printf("Exiting calculator.\n");
            return 0;
        }

        printf("Enter the number of integers you want to enter: ");
        if (scanf("%d", &num_count) != 1 || num_count <= 0) {
            printf("Invalid number of integers.\n");
            while (getchar() != '\n');
            continue;
        }

        numbers = (int *)malloc(num_count * sizeof(int));
        if (numbers == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        printf("Enter the %d integers separated by spaces: ", num_count);
        for (int i = 0; i < num_count; i++) {
            if (scanf("%d", &numbers[i]) != 1) {
                printf("Invalid integer input.\n");
                free(numbers);
                numbers = NULL;
                while (getchar() != '\n');
                continue; 
            }
        }

        switch (choice) {
            case 1:
                result = add(numbers, num_count);
                break;
            case 2:
                result = subtract(numbers, num_count);
                break;
            case 3:
                result = multiply(numbers, num_count);
                break;
            case 4:
                result = divide(numbers, num_count);
                break;
            default:
                printf("Invalid choice.\n");
                result = 0;
        }

        printf("Result: %d\n", result);

        free(numbers);
        numbers = NULL;

        continue;
    }

    return 0;
}