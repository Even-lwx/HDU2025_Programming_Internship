#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

// �۰���Һ�������������һ��ƥ��������
int binarySearch(int arr[], int size, int target);
// ��������ƥ�����������
void findAllIndices(int arr[], int size, int target);

int main()
{
    FILE *fp;
    char buffer[MAX_SIZE * 10]; // ����ÿ�����1000���ַ�
    int a[MAX_SIZE];
    int count = 0;
    char *token;

    // ��ȡ2.txt�ļ�
    if ((fp = fopen("2.txt", "r")) == NULL)
    {
        printf("�޷����ļ�2.txt\n");
        return 1;
    }

    // ��ȡ��������
    if (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // �Ƴ����з�
        buffer[strcspn(buffer, "\n")] = 0;

        // ʹ�ö��ŷָ��ַ�����ת��Ϊ����
        token = strtok(buffer, ",");
        while (token != NULL && count < MAX_SIZE)
        {
            a[count++] = atoi(token);
            token = strtok(NULL, ",");
        }
    }
    fclose(fp);

    // �����������
    printf("����a�е�����: ");
    for (int i = 0; i < count; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n\n");

    // Ҫ���ҵ�Ŀ��ֵ
    int targets[] = {27, 50, 76};
    int numTargets = sizeof(targets) / sizeof(targets[0]);

    // ����ÿ��Ŀ��ֵ�����г���λ��
    for (int i = 0; i < numTargets; i++)
    {
        printf("��������%d�����г���λ��:\n", targets[i]);
        findAllIndices(a, count, targets[i]);
        printf("\n");
    }

    return 0;
}

/**
 * �۰���ң����ֲ��ң����� - �����������в���Ŀ��ֵ
 * @param arr ��������
 * @param size ���鳤��
 * @param target Ҫ���ҵ�Ŀ��ֵ
 * @return Ŀ��ֵ�������е�����һ����������δ�ҵ�����-1
 */
int binarySearch(int arr[], int size, int target)
{
    int left = 0;
    int right = size - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2; // �����м��������������

        if (arr[mid] == target)
        {
            return mid; // �ҵ�Ŀ��ֵ����������
        }
        else if (arr[mid] < target)
        {
            left = mid + 1; // Ŀ�����Ұ벿��
        }
        else
        {
            right = mid - 1; // Ŀ������벿��
        }
    }

    return -1; // δ�ҵ�Ŀ��ֵ
}

/**
 * ����Ŀ��ֵ�����г���λ��
 * @param arr ��������
 * @param size ���鳤��
 * @param target Ҫ���ҵ�Ŀ��ֵ
 */
void findAllIndices(int arr[], int size, int target)
{
    int index = binarySearch(arr, size, target);

    if (index == -1)
    {
        printf("����%d����������\n", target);
        return;
    }

    // ������չ�ҵ���һ������λ��
    int left = index;
    while (left > 0 && arr[left - 1] == target)
    {
        left--;
    }

    // ������չ�ҵ����һ������λ��
    int right = index;
    while (right < size - 1 && arr[right + 1] == target)
    {
        right++;
    }

    // �����������
    printf("����%d��������������λ��: ", target);
    for (int i = left; i <= right; i++)
    {
        printf("%d ", i);
    }
    printf("\n������ %d ��\n", right - left + 1);
}