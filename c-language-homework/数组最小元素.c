#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    int i, min;
    int n = rand();
    int num[n];
    printf("数组元素为：{");
    for (i = 0; i < n; i++)
    {
        num[i] = rand();
        printf("%d ", num[i]);
        if (i == n - 1)
            printf("}");
    }
    min = num[0];
    printf("\n");
    for (i = 0; i < n; i++)
    {
        if (num[i] <= min)
            min = num[i];
    }
    printf("数组中最小元素为：%d", min);
}