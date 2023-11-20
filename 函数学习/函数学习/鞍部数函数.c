#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<math.h> 
#include<string.h>

int saddle_point(int a[100][100])
{
	int i, j, k,row,col;
	row = sizeof(a) / sizeof(a[0]);
	col = sizeof(a[0]) / sizeof(a[0][0]);
	printf("%d,%d", row, col);
}

int main()
{
	int arr[3][3] = { {1,2,3},{1,2,3},{1,2,3} };
	saddle_point(arr[3][3]);
	
}