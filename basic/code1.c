#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100 // ��̬�ڴ��С
/**
 * ��������������ֵ
 * @param a ָ���һ��������ָ��
 * @param b ָ��ڶ���������ָ��
 */
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * ��������ķ������� - �������һ���ַ�Ϊ�����֣����С�ڻ�׼���Ҳ���ڵ��ڻ�׼
 * @param arr ������������
 * @param low ��������ʼ����
 * @param high �����Ľ���������������
 * @return ��׼Ԫ�ص�����λ��
 */
int partition(int arr[], int low, int high)
{
    int pivot = arr[high]; // ȡ���һλΪ��׼
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
 * �������� - �������ָ���������ԭ������
 * @param arr �����������
 * @param low �������ʼ����
 * @param high ����Ľ���������������
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
    int count = 0; // ��¼��������
    char *token;

    /*==========================================
                        ��ȡ�ļ�
     ===========================================*/
    if ((fp1 = fopen("1.txt", "r")) == NULL)
    {
        printf("�޷����ļ�1.txt\n");
        return 1;
    }

    if (fgets(buffer, MAX_SIZE, fp1) != NULL)
    {
        token = strtok(buffer, ",");
        while (token != NULL && count < MAX_SIZE)
        {
            a[count++] = atoi(token); // ���ַ���ת��Ϊ����
            token = strtok(NULL, ",");
        }
    }
    fclose(fp1);
    /*=====================================
                    ����
    =======================================*/
    // ��ӡ����ǰ������
    printf("����ǰ: ");
    for (int i = 0; i < count; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");

    // ��������
    quickSort(a, 0, count - 1);

    // ��ӡ����������
    printf("�����: ");
    for (int i = 0; i < count; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
    /*==========================================
                    д��2.txt
    ============================================*/
    // д���ļ�
    if ((fp2 = fopen("2.txt", "w")) == NULL)
    {
        printf("�޷����ļ�2.txt\n");
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