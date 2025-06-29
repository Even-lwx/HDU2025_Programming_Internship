#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ����ڵ�ṹ�嶨��
struct node
{
    int data;          // �ڵ�洢������ֵ
    struct node *next; // ָ����һ���ڵ��ָ��
};

/**
 * �����½ڵ㲢��ʼ��
 * @param val �ڵ�洢������ֵ
 * @return �´����Ľڵ�ָ�룬ʧ��ʱ����NULL
 */
struct node *createNode(int val)
{
    // ����ڵ��ڴ�
    struct node *pnode = (struct node *)malloc(sizeof(struct node));
    if (pnode != NULL)
    {
        pnode->data = val;  // ��ʼ���ڵ�����
        pnode->next = NULL; // �½ڵ�Ĭ�ϲ�ָ���κνڵ�
    }
    return pnode;
}

/**
 * ������β������½ڵ�
 * @param pheadptr ָ��ͷָ���ָ�루�����޸�ͷָ�룩
 * @param val Ҫ��ӵ�����ֵ
 * @return �����ɹ�����1��ʧ�ܷ���0
 */
int addNode(struct node **pheadptr, int val)
{
    struct node *p = createNode(val); // �����½ڵ�
    if (p == NULL)
        return 0; // �ڵ㴴��ʧ��

    // ������������
    if (*pheadptr == NULL)
    {
        *pheadptr = p; // �½ڵ���Ϊͷ�ڵ�
    }
    else
    {
        // �ҵ�����β���ڵ�
        struct node *ptmp = *pheadptr;
        while (ptmp->next)
        {
            ptmp = ptmp->next;
        }
        ptmp->next = p; // β���ڵ�ָ���½ڵ�
    }
    return 1;
}

/**
 * ���������еĽڵ�����
 * @param headptr ����ͷָ��
 * @return �ڵ��������޷���������
 */
unsigned countOfNodes(struct node *headptr)
{
    unsigned c = 0;
    while (headptr)
    {
        ++c;                     // ������1
        headptr = headptr->next; // �ƶ�����һ���ڵ�
    }
    return c;
}

/**
 * ��ӡ���������нڵ������
 * @param headptr ����ͷָ��
 */
void printAllNode(struct node *headptr)
{
    while (headptr)
    {
        printf("%d ", headptr->data); // ��ӡ��ǰ�ڵ�����
        headptr = headptr->next;      // �ƶ�����һ���ڵ�
    }
    printf("\n"); // ��ӡ���з�
}

/**
 * ɾ��������ָ��λ�õĽڵ�
 * @param pheadptr ָ��ͷָ���ָ�루�����޸�ͷָ�룩
 * @param loc Ҫɾ���Ľڵ�λ�ã���1��ʼ������
 * @return �����ɹ�����1��ʧ�ܷ���0
 */
int deleteNode(struct node **pheadptr, unsigned loc)
{
    unsigned c = countOfNodes(*pheadptr); // ��ȡ�ڵ�����
    if (c < loc)
        return 0; // λ�ó�����Χ��ɾ��ʧ��

    struct node *p = *pheadptr;

    // ɾ��ͷ�ڵ�������λ��1��
    if (loc == 1)
    {
        *pheadptr = (*pheadptr)->next; // ͷָ��ָ����һ���ڵ�
        free(p);                       // �ͷ�ԭͷ�ڵ��ڴ�
    }
    else
    {
        // �ҵ���ɾ���ڵ��ǰһ���ڵ�
        for (int i = 2; i < loc; ++i)
        {
            p = p->next;
        }
        // ִ��ɾ������
        struct node *pdel = p->next;
        p->next = pdel->next;
        free(pdel); // �ͷű�ɾ���ڵ���ڴ�
    }
    return 1;
}

/**
 * �ͷ����������нڵ���ڴ�
 * @param pheadptr ָ��ͷָ���ָ�루�����޸�ͷָ�룩
 */
void deleteAllNodes(struct node **pheadptr)
{
    // ѭ��ɾ��ͷ�ڵ㣬ֱ������Ϊ��
    while (countOfNodes(*pheadptr))
    {
        deleteNode(pheadptr, 1);
    }
}

/**
 * ��ָ��λ��ǰ�����½ڵ㣨������Χ����
 * @param pheadptr ָ��ͷָ���ָ��
 * @param loc ����λ�ã���0��ʼ������
 * @param val Ҫ���������ֵ
 * @return �����ɹ�����1��ʧ�ܷ���0
 */
int insertNodeBefore(struct node **pheadptr, unsigned loc, int val)
{
    // ����ͷ�ڵ�ǰ��������
    if (loc == 0)
    {
        struct node *newNode = createNode(val);
        if (newNode == NULL)
            return 0;
        newNode->next = *pheadptr;
        *pheadptr = newNode;
        return 1;
    }
    // �����������ȵ����
    unsigned c = countOfNodes(*pheadptr);
    if (loc > c)
    {
        printf("���󣺲���λ�� %u ���������� %u\n", loc, c);
        return 0;
    }

    struct node *current = *pheadptr;
    struct node *prev = NULL;
    unsigned pos = 0;

    // �ҵ�����λ�õ�ǰһ���ڵ㣨posʼ��ָ��current��ǰһ���ڵ㣩
    while (current != NULL && pos < loc)
    {
        prev = current;
        current = current->next;
        pos++;
    }

    struct node *newNode = createNode(val);
    if (newNode == NULL)
        return 0;

    // ִ�в���������޸�ָ����
    prev->next = newNode;
    newNode->next = current;
    return 1;
}

// /**
//  * ��ָ��λ��ǰ�����½ڵ㣨������Χֱ�Ӽ���ĩβ��
//  * @param pheadptr ָ��ͷָ���ָ��
//  * @param loc ����λ�ã���0��ʼ������
//  * @param val Ҫ���������ֵ
//  * @return �����ɹ�����1��ʧ�ܷ���0
//  */
// int insertNodeBefore(struct node **pheadptr, unsigned loc, int val)
// {
//     // ����ͷ�ڵ�ǰ��������
//     if (loc == 0)
//     {
//         struct node *newNode = createNode(val);
//         if (newNode == NULL)
//             return 0;
//         newNode->next = *pheadptr;
//         *pheadptr = newNode;
//         return 1;
//     }

//     // ������������
//     if (*pheadptr == NULL)
//     {
//         // �������Ϊ�գ�ֱ������½ڵ�
//         return addNode(pheadptr, val);
//     }

//     struct node *current = *pheadptr;
//     struct node *prev = NULL;
//     unsigned pos = 0;

//     // ��������ֱ���ҵ�λ�û򵽴�β��
//     while (current != NULL && pos < loc)
//     {
//         prev = current;
//         current = current->next;
//         pos++;
//     }

//     // �����½ڵ�
//     struct node *newNode = createNode(val);
//     if (newNode == NULL)
//         return 0;

//     // ���λ�ó��������ȣ����뵽β��
//     if (pos < loc)
//     {
//         // ��������β�������½ڵ㸽�ӵ�β��
//         prev->next = newNode;
//         return 1;
//     }

//     // ��ָ��λ��ǰ����ڵ�
//     prev->next = newNode;
//     newNode->next = current;
//     return 1;
// }

/**
 * ���ļ���ȡ���ݹ�������
 * @param pheadptr ָ��ͷָ���ָ�루�����޸�ͷָ�룩
 * @param filename �ļ���
 * @return �����ɹ�����1��ʧ�ܷ���0
 */
int buildListFromFile(struct node **pheadptr, const char *filename)
{
    FILE *file = fopen(filename, "r"); // ��ֻ����ʽ���ļ�
    if (file == NULL)
    {
        printf("�޷����ļ� %s\n", filename);
        return 0;
    }

    char buffer[1024]; //
    // ��ȡ�ļ��е�һ������
    if (fgets(buffer, sizeof(buffer), file) == NULL)
    {
        fclose(file);
        return 0;
    }
    fclose(file); // �ر��ļ�

    // �����ŷָ��ַ�����ת��Ϊ����
    char *token = strtok(buffer, ",");
    while (token != NULL)
    {
        int value = atoi(token);  // �ַ���ת����
        addNode(pheadptr, value); // ��ӵ�����β��
        token = strtok(NULL, ",");
    }

    return 1;
}

int main()
{
    struct node *headPtr = NULL;

    // ���ļ���ȡ���ݹ�������
    if (!buildListFromFile(&headPtr, "3.txt"))
    {
        return 1;
    }

    printf("��ʼ����ڵ�����: ");
    printAllNode(headPtr);

    // �����ڵ�0���ڵ�ǰ����
    {
        struct node *testHead = NULL;
        struct node *current = headPtr;
        while (current != NULL)
        {
            addNode(&testHead, current->data);
            current = current->next;
        }

        printf("\n����3.2: �ڵ�0���ڵ�ǰ����100��: ");
        if (insertNodeBefore(&testHead, 0, 100))
        {
            printAllNode(testHead);
        }
        else
        {
            printf("����ʧ��\n");
        }
        deleteAllNodes(&testHead);
    }

    // �����ڵ�1���ڵ�ǰ����
    {
        struct node *testHead = NULL;
        struct node *current = headPtr;
        while (current != NULL)
        {
            addNode(&testHead, current->data);
            current = current->next;
        }

        printf("����3.3: �ڵ�1���ڵ�ǰ����100��: ");
        if (insertNodeBefore(&testHead, 1, 100))
        {
            printAllNode(testHead);
        }
        else
        {
            printf("����ʧ��\n");
        }
        deleteAllNodes(&testHead);
    }

    // �����ڵ�10���ڵ�ǰ���루���������ȣ�
    {
        struct node *testHead = NULL;
        struct node *current = headPtr;
        while (current != NULL)
        {
            addNode(&testHead, current->data);
            current = current->next;
        }

        printf("����3.4: �ڵ�10���ڵ�ǰ����100��: ");
        if (insertNodeBefore(&testHead, 10, 100))
        {
            printAllNode(testHead);
        }
        else
        {
            // ��ʹ����ʧ��Ҳ��ӡԭ����������ĿҪ��
            printAllNode(testHead);
        }
        deleteAllNodes(&testHead);
    }

    deleteAllNodes(&headPtr);
    return 0;
}