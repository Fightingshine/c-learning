#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<math.h>

int main()
{
	int a[100][100], m, n, i, j, max1, max2, max3, max4, max5;
	static int a1, a2, a3, a4, a5;
	static int b1, b2, b3, b4, b5;
	printf("the size of the row:");
	scanf("%d", &m);
	printf("the size of the collum:");
	scanf("%d", &n);
	printf("the input array is:\n");
	for (i = 0; i < m;i++)
	{
		for (j = 0; j < n; j++)
		{
			scanf("%d", &a[i][j]);
		}
	}
	for (i = 0,max1 = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (max1 <= a[i][j])
			{
				max1 = a[i][j];
				a1 = i;
				b1 = j;
			}
			
		}
	}
	for (i = 0,max2=0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (max2 <= a[i][j]&&max2<max1&&a[i][j]!=max1)
			{
				max2 = a[i][j];
				a2 = i;
				b2 = j;
			}

		}
	}
	for (i = 0, max3 = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (max3 <= a[i][j] && max3< max2 && a[i][j] != max1 && a[i][j] != max2)
			{
				max3 = a[i][j];
				a3= i;
				b3 = j;
			}

		}
	}
	for (i = 0, max4 = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (max4 <= a[i][j] && max4 < max3 && a[i][j] != max1 && a[i][j] != max2 && a[i][j] != max3)
			{
				max4 = a[i][j];
				a4 = i;
				b4 = j;
			}

		}
	}
	for (i = 0, max5 = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
		{
			if (max5 <= a[i][j] && max5 < max4 && a[i][j] != max1 && a[i][j] != max2 && a[i][j] != max3 && a[i][j] != max4)
			{
				max5 = a[i][j];
				a5 = i;
				b5 = j;
			}

		}
	}
	printf("max 1st number is array[%d][%d] = %d\n", a1, b1, max1);
	printf("max 2nd number is array[%d][%d] = %d\n", a2, b2, max2);
	printf("max 3rd number is array[%d][%d] = %d\n", a3, b3, max3);
	printf("max 4th number is array[%d][%d] = %d\n", a4, b4, max4);
	printf("max 5th number is array[%d][%d] = %d\n", a5, b5, max5);
}