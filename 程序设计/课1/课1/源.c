#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
int main()
{
    int n, m, o, i, j, k;
    srand((unsigned int)time(NULL));
    n = 3;
    m = 2;
    o = 4;
    printf("%d %d %d\n", n, m, o);
    int arr1[n][m], arr2[m][o], arr3[n][o];
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            arr1[i][j] = rand() % 10 + 1;
        }
    }
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            printf("%d  ", arr1[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < o; j++)
        {
            arr2[i][j] = rand() % 10 + 1;
        }
    }
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < o; j++)
        {
            printf("%d  ", arr2[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < o; j++)
        {
            for (k = 0; k < m; k++)
            {
                arr3[i][j] += arr1[i][k] * arr2[k][j];
            }
        }
    }
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < o; j++)
        {
            printf("%d ", arr3[i][j]);
        }
        printf("\n");
    }
}