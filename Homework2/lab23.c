#include <stdio.h>


int main() {
    int n = 0;
    printf("Enter N: "); 
    scanf("%d", &n); 
    if(n<=0){
        printf("Error");
        return 0;
    }
    int matrix[n];
    for(int i=0; i < n; i++){
        matrix[i]=i+1;
    }
    printf("Base: ");
    for(int i=0; i < n; i++){
        printf("%d ",matrix[i]);
    }
    printf("\nReverse: ");
    for(int i=n-1; i >= 0; i--){
        printf("%d ",matrix[i]);
    }
    return 0;

}