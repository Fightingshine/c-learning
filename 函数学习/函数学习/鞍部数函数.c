#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<math.h> 
#include<string.h>



int main()
{
	int arr[3][3] = { {1,2,3},{1,2,3},{1,2,3} };
	int i, j, k, row, col;
	row = sizeof(arr) / sizeof(arr[0]);
	col = sizeof(arr[0]) / sizeof(arr[0][0]);
	printf("%d,%d", row, col);
}