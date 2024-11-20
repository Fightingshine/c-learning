#include <stdio.h>

int main(){
    float arr[100]={0.00};
    for (int i = 0;; i++)
    {
        scanf("%f", &arr[i]);
        if(arr[i] == 0.00)break;
    }
    float sum = 0.00;
    for (int i = 0;; i++)
    {
        if(arr[i] != 0.00 ){
            for (int j = 2;; j++)
            {
                if (arr[i] == 0.00)
                    break;
                sum += 1.00 / j;
                if (sum >= arr[i]){
                    printf("%d card(s)\n", j - 2);
                    break;
                }
                    
            }
            
        }
    }
    
}