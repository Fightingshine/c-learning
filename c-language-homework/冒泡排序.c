#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    int i, j;
    int n = rand();
    int num[n];
    for (i = 0; i < n; i++)
    {
        num[i] = rand() % 100;
        printf("%d ", num[i]);
    }
    printf("\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            
        }
    }
}