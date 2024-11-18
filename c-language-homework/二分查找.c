#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int binary_search(int arr[],int n, int y)
{
    int bottom = 0, top =n, mid;
    while (bottom <= top)
    {
        mid = (bottom + top) / 2;
        if (arr[mid] == y)
        {
            return mid;
        }
        else if (arr[mid] < y)
        {
            bottom = mid + 1;
        }
        else
        {
            top = mid - 1;
        }
    }
    return -1;
}
int findClosestElement(int arr[],int n,int target)
{

    int closest = arr[0];
    int minDiff = abs(target - arr[0]);
    for (int i = 1; i < n; i++)
    {
        int diff = abs(target - arr[i]);
        if (diff < minDiff)
        {
            minDiff = diff;
            closest = arr[i];
        }
    }
    return closest;
}

int main()
{
    int i, j, n, m, a;
    printf("请输入非降序序列长度n(1<=n<=1000000):");
    scanf("%d", &n);
    int arr1[n];
    printf("请输入非降序列各元素，所有元素的大小均在0∼1,000,000,000之间(用空格分隔)：");
    for (i = 0; i < n; i++)
    {
        scanf("%d", &arr1[i]);
    }
    printf("请输入要询问的给定值个数m(1≤m≤10000):");
    scanf("%d", &m);
    int arr2[m];
    printf("请输入要询问的最接近元素的给定值，所有给定值的大小均在0∼1,000,000,000之间（用回车分隔）\n");
    for (j = 0; j < m; j++)
    {
        scanf("%d", &arr2[j]);
    }
    for (j = 0; j < m; j++)
    {
        a = binary_search(arr1,n,arr2[j]);
        if (a != -1)
        {
            printf("%d\n", arr1[a]);
        }
        else
        {
            printf("%d\n", findClosestElement(arr1,n,arr2[j]));
        }
    }
}