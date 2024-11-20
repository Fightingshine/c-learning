#include <stdio.h>
#define _CRT_SECURE_NO_WARNINGS

int main()
{
    int i[100],j[100];
    for (int k = 0; k < 4; k++)
    {
        scanf("%d %d", &i[k], &j[k]);
    }
    for (int k = 0; k < 4; k++)
    {
        int max;
        for (int l = i[k]; l <= j[k]; l++)
        {
            int count = 1;
            int current = l;
            while (current != 1)
            {
                
                if (current % 2 == 0)
                {
                    current /= 2;
                    count++;
               }
               else{
                   current = 3 * current + 1;
                   count++;
               }
            }
            if (max < count)
            {
                max = count;
            }
        }
        printf("%d %d %d\n", i[k], j[k], max);
        max = 0;
    }
}