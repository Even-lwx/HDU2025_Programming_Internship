#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * ����ڵ�ṹ
 * data: �ڵ�洢������ֵ
 * next: ָ����һ���ڵ��ָ��
 */
struct node
{
    int data;
    struct node *next;
};

/**
 * �����½ڵ�
 * @param val �ڵ�洢��ֵ
 * @return �ɹ������½ڵ�ָ�룬ʧ�ܷ���NULL
 */
struct node *createNode(int val)
{
    struct node *pnode = (struct node *)malloc(sizeof(struct node));
    if (pnode != NULL)
    {
        pnode->data = val;
        pnode->next = NULL;
    }
    return pnode;
}

/**
 * ������β������½ڵ�
 * @param pheadptr ָ��ͷָ���ָ��
 * @param val �½ڵ��ֵ
 * @return �ɹ�����1���ڴ����ʧ�ܷ���0
 */
int addNode(struct node **pheadptr, int val)
{
    struct node *p = createNode(val);
    if (p == NULL)
        return 0;
    if (*pheadptr == NULL)
        *pheadptr = p;
    else
    {
        struct node *ptmp = *pheadptr;
        while (ptmp->next)
            ptmp = ptmp->next;
        ptmp->next = p;
    }
    return 1;
}

/**
 * ��������ڵ�����
 * @param headptr ����ͷָ��
 * @return �ڵ�����
 */
unsigned countOfNodes(struct node *headptr)
{
    unsigned c = 0;
    while (headptr)
    {
        ++c;
        headptr = headptr->next;
    }
    return c;
}

/**
 * ��ӡ�������нڵ�����
 * @param headptr ����ͷָ��
 */
void printAllNode(struct node *headptr)
{
    while (headptr)
    {
        printf("%d ", headptr->data);
        headptr = headptr->next;
    }
    printf("\n");
}

/**
 * ɾ��ָ��λ�õĽڵ�
 * @param pheadptr ָ��ͷָ���ָ��
 * @param loc ɾ��λ��(��1��ʼ)
 * @return �ɹ�����1��λ����Ч����0
 */
int deleteNode(struct node **pheadptr, unsigned loc)
{
    unsigned c = countOfNodes(*pheadptr);
    if (c < loc)
        return 0;
    struct node *p = *pheadptr;
    if (loc == 1)
    {
        *pheadptr = (*pheadptr)->next;
        free(p);
    }
    else
    {
        for (int i = 2; i < loc; ++i)
            p = p->next;
        struct node *pdel = p->next;
        p->next = pdel->next;
        free(pdel);
    }
    return 1;
}

/**
 * ɾ���������нڵ㣬�ͷ��ڴ�
 * @param pheadptr ָ��ͷָ���ָ��
 */
void deleteAllNodes(struct node **pheadptr)
{
    while (countOfNodes(*pheadptr))
        deleteNode(pheadptr, 1);
}

/**
 * ��������
 * @param headptr ԭ����ͷָ��
 * @return ���ú������ͷָ��
 */
struct node *reverseList(struct node *headptr)
{
    struct node *prev = NULL;       // ǰ���ڵ�
    struct node *current = headptr; // ��ǰ�ڵ�
    struct node *next = NULL;       // ��̽ڵ�

    // ���������ı�ָ�뷽��
    while (current != NULL)
    {
        next = current->next; // ������һ���ڵ�
        current->next = prev; // ��ǰ�ڵ�ָ��ǰ��
        prev = current;       // ǰ������
        current = next;       // ��ǰ�ڵ����
    }

    return prev; // �����µ�ͷָ��
}

/**
 * ���ļ���ȡ���ݹ�������
 * @param pheadptr ָ��ͷָ���ָ��
 * @param filename �ļ���
 * @return �ɹ�����1��ʧ�ܷ���0
 */
int buildListFromFile(struct node **pheadptr, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("�޷����ļ� %s\n", filename);
        return 0;
    }

    char line[1024];
    // ��ȡ�ļ���һ��
    if (fgets(line, sizeof(line), file) == NULL)
    {
        printf("��ȡ�ļ�����ʧ��\n");
        fclose(file);
        return 0;
    }
    fclose(file);

    // �������ŷָ�����ֵ
    char *token = strtok(line, ",");
    while (token != NULL)
    {
        int value = atoi(token);
        addNode(pheadptr, value);
        token = strtok(NULL, ",");
    }

    return 1;
}

/**
 * ����������׷�ӵ��ļ�
 * @param headptr ����ͷָ��
 * @param filename �ļ���
 * @return �ɹ�����1��ʧ�ܷ���0
 */
int appendListToFile(struct node *headptr, const char *filename)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("�޷����ļ� %s ����׷��\n", filename);
        return 0;
    }

    // ׷�ӻ��з�����ԭ���ݷָ�
    fprintf(file, "\n");

    // ׷�����ú����������
    struct node *current = headptr;
    if (current != NULL)
    {
        fprintf(file, "%d", current->data);
        current = current->next;
    }

    // �Զ��ŷָ����ʣ��ڵ�
    while (current != NULL)
    {
        fprintf(file, ",%d", current->data);
        current = current->next;
    }

    fclose(file);
    return 1;
}

/**
 * ��������������ڵ�
 * 1. ��4.txt��ȡ���ݹ�������
 * 2. ��������
 * 3. �����ú����������׷�ӵ�4.txt
 */
int main()
{
    struct node *headPtr = NULL;

    // ���ļ���ȡ���ݹ�������
    if (buildListFromFile(&headPtr, "4.txt"))
    {
        printf("���ļ���ȡ���ݹ�������ɹ�\n");
        printf("ԭʼ��������: ");
        printAllNode(headPtr);

        // ��������
        headPtr = reverseList(headPtr);
        printf("���ú���������: ");
        printAllNode(headPtr);

        // �����ú����������׷�ӵ��ļ�
        if (appendListToFile(headPtr, "4.txt"))
        {
            printf("���ú�����������ѳɹ�׷�ӵ��ļ�\n");
        }

        // ������Դ
        deleteAllNodes(&headPtr);
    }

    return 0;
}