#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i, j;
    int arr[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (arr[j] * arr[i] < 10)
                printf("%d×%d=%d   ", arr[j], arr[i], arr[j] * arr[i]);
            else
                printf("%d×%d=%d  ", arr[j], arr[i], arr[j] * arr[i]);
        }
        printf("\n");
    }
}