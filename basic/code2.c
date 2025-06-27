#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

// 折半查找函数（返回任意一个匹配索引）
int binarySearch(int arr[], int size, int target);
// 查找所有匹配索引并输出
void findAllIndices(int arr[], int size, int target);

int main()
{
    FILE *fp;
    char buffer[MAX_SIZE * 10]; // 假设每行最多1000个字符
    int a[MAX_SIZE];
    int count = 0;
    char *token;

    // 读取2.txt文件
    if ((fp = fopen("2.txt", "r")) == NULL)
    {
        printf("无法打开文件2.txt\n");
        return 1;
    }

    // 读取整行数据
    if (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // 移除换行符
        buffer[strcspn(buffer, "\n")] = 0;

        // 使用逗号分割字符串并转换为整数
        token = strtok(buffer, ",");
        while (token != NULL && count < MAX_SIZE)
        {
            a[count++] = atoi(token);
            token = strtok(NULL, ",");
        }
    }
    fclose(fp);

    // 输出数组内容
    printf("数组a中的数据: ");
    for (int i = 0; i < count; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n\n");

    // 要查找的目标值
    int targets[] = {27, 50, 76};
    int numTargets = sizeof(targets) / sizeof(targets[0]);

    // 查找每个目标值的所有出现位置
    for (int i = 0; i < numTargets; i++)
    {
        printf("查找数字%d的所有出现位置:\n", targets[i]);
        findAllIndices(a, count, targets[i]);
        printf("\n");
    }

    return 0;
}

/**
 * 折半查找（二分查找）函数 - 在有序数组中查找目标值
 * @param arr 有序数组
 * @param size 数组长度
 * @param target 要查找的目标值
 * @return 目标值在数组中的任意一个索引，若未找到返回-1
 */
int binarySearch(int arr[], int size, int target)
{
    int left = 0;
    int right = size - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2; // 计算中间索引，避免溢出

        if (arr[mid] == target)
        {
            return mid; // 找到目标值，返回索引
        }
        else if (arr[mid] < target)
        {
            left = mid + 1; // 目标在右半部分
        }
        else
        {
            right = mid - 1; // 目标在左半部分
        }
    }

    return -1; // 未找到目标值
}

/**
 * 查找目标值的所有出现位置
 * @param arr 有序数组
 * @param size 数组长度
 * @param target 要查找的目标值
 */
void findAllIndices(int arr[], int size, int target)
{
    int index = binarySearch(arr, size, target);

    if (index == -1)
    {
        printf("数字%d不在数组中\n", target);
        return;
    }

    // 向左扩展找到第一个出现位置
    int left = index;
    while (left > 0 && arr[left - 1] == target)
    {
        left--;
    }

    // 向右扩展找到最后一个出现位置
    int right = index;
    while (right < size - 1 && arr[right + 1] == target)
    {
        right++;
    }

    // 输出所有索引
    printf("数字%d出现在以下索引位置: ", target);
    for (int i = left; i <= right; i++)
    {
        printf("%d ", i);
    }
    printf("\n共出现 %d 次\n", right - left + 1);
}