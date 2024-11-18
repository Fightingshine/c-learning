#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>

int main()
{
    int n, m, o, i, j, k;
    srand((unsigned int)time(NULL));
    n = rand() % 10 + 1;
    m = rand() % 10 + 1;
    o = rand() % 10 + 1;
    int arr1[n][m], arr2[m][o], arr3[n][o];
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            arr1[i][j] = rand() % 10 + 1;
        }
    }
    printf("arr1为：\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < m; j++)
        {
            printf("%d  ", arr1[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("arr2为：\n");
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
            arr3[i][j] = 0;
        }
    }

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
    printf("矩阵乘积为\n");
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < o; j++)
        {
            printf("%d ", arr3[i][j]);
        }
        printf("\n");
    }
}