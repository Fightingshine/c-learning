#include <stdio.h>

int main()
{
	int arr[10] = {0};
	char a[10] = { 1,1 };
	printf("%d", &arr[0] - &a[0]);
}