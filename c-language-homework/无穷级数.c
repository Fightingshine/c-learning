#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    double i,j, sum=0;
    for (i = 1; i <= 500;i++){
        sum += 1 / i;
    }
    printf("%lf\n", sum);
    sum = 0;
    for (j = 0; j <= 500; j++)
    {
        sum += 1 / pow(2,j);
    }
    printf("%lf\n", sum);
}