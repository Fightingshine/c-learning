#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    int i,j,product,maxprodct=0;
    int n = rand();
    int num[n];
    printf("数组元素为：");
    for (i = 0; i < n;i++){
        num[i] = rand()%100;
        printf("%d ", num[i]);
    }
    printf("\n");
    for (i = 0; i < n;i++){
        for (j = 0; j < n; j++)
        {
            product = num[i] * num[j];
            if(product>=maxprodct){
                maxprodct = product;
            }
        }
    }
    printf("数组中最大乘积为：%d", maxprodct);
}