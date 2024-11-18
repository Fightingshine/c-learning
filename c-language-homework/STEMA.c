#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int score;
    int n, i;
    int arr[1000];
    float rate;
    printf("请输入你的成绩score(0<=score<=100)：");
    scanf("%d", &score);
    printf("请输入参加测评的总人数n(10<=n<=1000)：");
    scanf("%d", &n);
    rate = n;
    printf("请输入参与测评的每个考生的成绩score(0<=score<=100)(中间用空格分隔)：");
    for (i = 0; i < n - 1; i++)
    {
        scanf("%d", &arr[i]);
    }
    for (i = 0; i < n - 1; i++)
    {
        if (score >= arr[i])
        {
            rate--;
        }
    }
    if (rate <= 0.1 * n||rate == 1)
    {
        printf("A");
    }
    else if (rate > 0.1 * n && rate <= 0.3 * n)
    {
        printf("B");
    }
    else if (rate > 0.3 * n && rate <= 0.6 * n)
    {
        printf("C");
    }
    else if (rate > 0.6 * n && rate <= 0.8 * n)
    {
        printf("D");
    }
    else
    {
        printf("E");
    }
}