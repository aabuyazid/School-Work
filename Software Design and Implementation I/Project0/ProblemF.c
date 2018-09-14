//
// Created by Arkan Abuyazid on 1/29/2018.
//
//GDC between two numbers which have at most 100 factors each

#include <stdio.h>


//Assume x is greater than or equal to y
int gcd(int x, int y){
    int divisorMatch = 0;
    for(int currentDivisor = 1; currentDivisor <= y; currentDivisor++){
        if((x % currentDivisor == 0) && (y % currentDivisor == 0)){
            divisorMatch = currentDivisor;
        }
    }
    return divisorMatch;
}

void printGDC(int x, int y){
    printf("GDC of %d and %d is: %d\n", x, y, gcd(x,y));
}

int sumOfDigits(int x){

    int sum = 0;
    while(x > 0){
        sum += (x%10);
        x /= 10;
    }
    return sum;
}

int main(){

    printGDC(48,18);
    printGDC(144, 60);
    printGDC(65,12);
    printGDC(179, 7);

    return 0;

}

