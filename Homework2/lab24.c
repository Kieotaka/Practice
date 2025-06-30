#include <stdio.h>
#include <stdlib.h>

void spirall(int n, int matrix[n][n]) {
    int top = 0, bottom = n - 1, left = 0, right = n - 1;
    int move = 0, num = 1;
    while (top <= bottom && left <= right) {
        if (move == 0) {
            for (int i = left; i <= right; i++) {
                matrix[top][i] = num++;
            }
            top++;
            move=1;
        } else if (move == 1) {
            for (int i = top; i <= bottom; i++) {
                matrix[i][right] = num++;
            }
            right--;
            move=2;
        } else if (move == 2) {
            for (int i = right; i >= left; i--) {
                matrix[bottom][i] = num++;
            }
            bottom--;
            move=3;
        } else if (move == 3) {
            for (int i = bottom; i >= top; i--) {
                matrix[i][left] = num++;
            }
            left++;
            move=0;
        }
    }
}
void printMatrix(int n, int matrix[n][n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]); 
        }
        printf("\n");
    }
}

int main() {
    int n;
    printf("Enter N: ");
    scanf("%d", &n);
    if (n <= 0) {
        printf("Error.\n");
        return 1;
    }
    int matrix[n][n];
    spirall(n, matrix);
    printMatrix(n, matrix);
    return 0;
}