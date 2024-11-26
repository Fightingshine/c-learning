#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char str[260];
    int len, i, sum;
    while (gets(str) != NULL && str[0] != '#')
    {
        sum = 0;
        len = strlen(str);
        for (i = 0; i < len; i++)
        {
            if (str[i] == ' ')
                continue;
            sum += (str[i] - 'A' + 1) * (i + 1);
        }
        printf("%d\n", sum);
    }
    return 0;

    
}