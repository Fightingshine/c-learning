#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS
#include<math.h>
#define M_PI 3.1415926


int main()
{
    int N;
    scanf("%d", &N);
    float X[100], Y[100];
    for (int i = 0; i < N; i++)
    {
        scanf("%f %f", &X[i], &Y[i]);
    }
    for (int i = 0; i < N; i++)
    {
        float acreage = M_PI*(X[i]*X[i]+Y[i]*Y[i]);
        for (int j = 0;; j++){
            float missing_acre = 100.00*j;
            if(missing_acre>acreage){
                printf("Property %d: This property will begin eroding in year %d.\n",i+1, j);
                missing_acre = 0.00;
                break;
            }
       }
       
    }
    printf("END OF OUTPUT.");
}