#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    int i, j;
    for (i = 100; i <= 200; i++)
    {
        for (j = 2; j < i; j++)
        {
            if ( i % j == 0)
            {
                break;
            }
            else
            {
                if(j == i-1){
                    printf("%d ", i);
                }
            }
        }
    }
}