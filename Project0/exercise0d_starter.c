#include <stdio.h>

void printMultTableInt(int n);
void printMultTableFrac(int n);

int main(void){

    // You can change this to test your code
    int size = 10;

    printMultTableInt(size);
    printMultTableFrac(size);

    return 0;

}

// Prints multiplication table of integers from 1 to n
void printMultTableInt(int n){

    int product = 0;

    for(int row = 1; row < (n + 1); row++){
        for(int column = 1; column < (n + 1); column++){
            product = row * column;
            printf("%d    ", product);
        }
        printf("\n");
    }

}

// Prints multiplication table for fractions from 1, 1/2, 1/3 to 1/n
void printMultTableFrac(int n){

    float product = 0; float factor1 = 0; float factor2 = 0;

    for(float row = 1; row < (n+1); row++){
        factor1 = 1/row;
        for(float column = 1; column < (n+1); column++){
            factor2 = 1/column;
            product = factor1 * factor2;
            printf("%f   ", product);
        }
        printf("\n");
    }

}
