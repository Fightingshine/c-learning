#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int N;
    printf("请输入一个正整数N(N≤2,000,000):");
    scanf("%d", &N);
    for (;;)
    {
        if (N % 2 == 0)
        {
            printf("%d/2=%d\n", N, N / 2);
            N /= 2;
        }
        else if (N % 2 != 0 && N != 1)
        {
            printf("%d*3+1=%d\n", N, N * 3 + 1);
            N = N * 3 + 1;
        }
        else
        {
            printf("End");
            break;
        }
    }
}