#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include<math.h>


int main()
{
    int n, i ,X, Y, R,count=0;
    int Xi[100], Yi[100];
    double distc;
    printf("画了n个点:");
    scanf("%d", &n);
    printf("输入圆心坐标X Y和圆半径R(用空格分隔):");
    scanf("%d %d %d", &X,&Y,&R);
    for (i = 0; i <= n - 1;i++){
        printf("输入第%d点的坐标X%d Y%d:",i+1,i+1,i+1);
        scanf("%d %d", &Xi[i], &Yi[i]);
    }
    for (i = 0; i <= n - 1; i++)
    {
        distc = (Xi[i] - X) * (Xi[i] - X) + (Yi[i] - Y)*(Yi[i] - Y);
        if(distc<=R*R){
            count++;
            
        }
    }
    printf("%d", count);
}