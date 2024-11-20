#include <stdio.h>

int main()
{
    float arr[12] = {0.00};
    for (int i = 0;i<12; i++)
    {
        scanf("%f", &arr[i]);
    }
    float sum = 0.00;
    for (int i = 0;i<12; i++)
    {
        sum += arr[i];
    }
    printf("$%.2f", sum / 12.00);
}