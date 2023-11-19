#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<math.h> 
#include<string.h>

void reversestring(char str[])
{
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) 
    {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int main()
{
    char str[100];
    printf("Enter a string: ");
    scanf("%[^\n]%*c", str);
    reverseString(str);
    printf("Reversed string: %s\n", str);
    return 0;
}