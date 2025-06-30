#include <stdio.h>


int main() {
    int n = 0;
    printf("Enter N: "); 
    scanf("%d", &n); 
    if(n<=0){
        printf("Error");
        return 0;
    }
    int matrix[n][n];
    for(int i=0; i < n; i++){
        for(int j=0; j < n; j++){
            if(j<n-i-1)
            matrix[i][j] = 0;
            else
            matrix[i][j] = 1;
        }
    }

    for(int i=0; i < n; i++){
        for(int j=0; j < n; j++){
           printf("%d ",matrix[i][j]);
        }
            printf("\n");
    }
    return 0;

}