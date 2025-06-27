#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100 // 静态内存大小
/**
 * 交换两个整数的值
 * @param a 指向第一个整数的指针
 * @param b 指向第二个整数的指针
 */
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * 快速排序的分区函数 - 将数组的一部分分为两部分，左侧小于基准，右侧大于等于基准
 * @param arr 待分区的数组
 * @param low 分区的起始索引
 * @param high 分区的结束索引（包含）
 * @return 基准元素的最终位置
 */
int partition(int arr[], int low, int high)
{
    int pivot = arr[high]; // 取最后一位为基准
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/**
 * 快速排序 - 对数组的指定区间进行原地排序
 * @param arr 待排序的数组
 * @param low 排序的起始索引
 * @param high 排序的结束索引（包含）
 */
void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main()
{
    FILE *fp1, *fp2;
    char buffer[MAX_SIZE];
    int a[MAX_SIZE];
    int count = 0; // 记录整数个数
    char *token;

    /*==========================================
                        读取文件
     ===========================================*/
    if ((fp1 = fopen("1.txt", "r")) == NULL)
    {
        printf("无法打开文件1.txt\n");
        return 1;
    }

    if (fgets(buffer, MAX_SIZE, fp1) != NULL)
    {
        token = strtok(buffer, ",");
        while (token != NULL && count < MAX_SIZE)
        {
            a[count++] = atoi(token); // 将字符串转化为整数
            token = strtok(NULL, ",");
        }
    }
    fclose(fp1);
    /*=====================================
                    排序
    =======================================*/
    // 打印排序前的数组
    printf("排序前: ");
    for (int i = 0; i < count; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");

    // 快速排序
    quickSort(a, 0, count - 1);

    // 打印排序后的数组
    printf("排序后: ");
    for (int i = 0; i < count; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    /*==========================================
                    写入2.txt
    ============================================*/
    // 写入文件
    if ((fp2 = fopen("2.txt", "w")) == NULL)
    {
        printf("无法打开文件2.txt\n");
        return 1;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(fp2, "%d", a[i]);
        if (i < count - 1)
        {
            fprintf(fp2, ",");
        }
    }
    fclose(fp2);

    return 0;
}