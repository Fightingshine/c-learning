#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<math.h>

int min()
{
	int n, arr[1000] = { 0 }, i, j, k, x, l;
	printf("�������ж��ٸ�����Ҫ����");
	scanf("%d", &n);
	printf("��������Ҫ��������֣��м��ÿո�ָ�)��");
	for (i = 0; i <= n - 1; i++)
	{
		scanf("%d", &arr[i]);

	}
	for (i = 0; i <= n - 2; i++)
	{
		k = i;												//k��iΪ��������Ƚϣ�����i�����������ҵ���С�����
		for (j = i; j <= n - 1; j++)
		{
			if (arr[j] < arr[k])
				k = j;										//�ҳ�������С��
		}
		if (k != i)
		{
			x = arr[i], arr[i] = arr[k], arr[k] = x;
		}
		printf("��%d������Ľ���ǣ�", i + 1);
		for (l = 0; l <= n - 1; l++)
		{
			printf("%d ", arr[l]);
			if (l == n - 1)
				printf("\n");
		}
	}
	printf("\n���յ���������:");
	for (l = 0; l <= n - 1; l++)
	{
		printf("%d ", arr[l]);
		if (l == n - 1)
			printf("\n");
	}
}