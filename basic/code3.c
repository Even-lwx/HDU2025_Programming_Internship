#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 链表节点结构体定义
struct node
{
    int data;          // 节点存储的整数值
    struct node *next; // 指向下一个节点的指针
};

/**
 * 创建新节点并初始化
 * @param val 节点存储的整数值
 * @return 新创建的节点指针，失败时返回NULL
 */
struct node *createNode(int val)
{
    // 分配节点内存
    struct node *pnode = (struct node *)malloc(sizeof(struct node));
    if (pnode != NULL)
    {
        pnode->data = val;  // 初始化节点数据
        pnode->next = NULL; // 新节点默认不指向任何节点
    }
    return pnode;
}

/**
 * 在链表尾部添加新节点
 * @param pheadptr 指向头指针的指针（用于修改头指针）
 * @param val 要添加的整数值
 * @return 操作成功返回1，失败返回0
 */
int addNode(struct node **pheadptr, int val)
{
    struct node *p = createNode(val); // 创建新节点
    if (p == NULL)
        return 0; // 节点创建失败

    // 处理空链表情况
    if (*pheadptr == NULL)
    {
        *pheadptr = p; // 新节点作为头节点
    }
    else
    {
        // 找到链表尾部节点
        struct node *ptmp = *pheadptr;
        while (ptmp->next)
        {
            ptmp = ptmp->next;
        }
        ptmp->next = p; // 尾部节点指向新节点
    }
    return 1;
}

/**
 * 计算链表中的节点数量
 * @param headptr 链表头指针
 * @return 节点数量（无符号整数）
 */
unsigned countOfNodes(struct node *headptr)
{
    unsigned c = 0;
    while (headptr)
    {
        ++c;                     // 计数加1
        headptr = headptr->next; // 移动到下一个节点
    }
    return c;
}

/**
 * 打印链表中所有节点的数据
 * @param headptr 链表头指针
 */
void printAllNode(struct node *headptr)
{
    while (headptr)
    {
        printf("%d ", headptr->data); // 打印当前节点数据
        headptr = headptr->next;      // 移动到下一个节点
    }
    printf("\n"); // 打印换行符
}

/**
 * 删除链表中指定位置的节点
 * @param pheadptr 指向头指针的指针（用于修改头指针）
 * @param loc 要删除的节点位置（从1开始计数）
 * @return 操作成功返回1，失败返回0
 */
int deleteNode(struct node **pheadptr, unsigned loc)
{
    unsigned c = countOfNodes(*pheadptr); // 获取节点总数
    if (c < loc)
        return 0; // 位置超出范围，删除失败

    struct node *p = *pheadptr;

    // 删除头节点的情况（位置1）
    if (loc == 1)
    {
        *pheadptr = (*pheadptr)->next; // 头指针指向下一个节点
        free(p);                       // 释放原头节点内存
    }
    else
    {
        // 找到待删除节点的前一个节点
        for (int i = 2; i < loc; ++i)
        {
            p = p->next;
        }
        // 执行删除操作
        struct node *pdel = p->next;
        p->next = pdel->next;
        free(pdel); // 释放被删除节点的内存
    }
    return 1;
}

/**
 * 释放链表中所有节点的内存
 * @param pheadptr 指向头指针的指针（用于修改头指针）
 */
void deleteAllNodes(struct node **pheadptr)
{
    // 循环删除头节点，直到链表为空
    while (countOfNodes(*pheadptr))
    {
        deleteNode(pheadptr, 1);
    }
}

/**
 * 在指定位置前插入新节点
 * @param pheadptr 指向头指针的指针（用于修改头指针）
 * @param loc 插入位置（从0开始计数，0表示在头节点前插入）
 * @param val 要插入的整数值
 * @return 操作成功返回1，失败返回0
 */
int insertNodeBefore(struct node **pheadptr, unsigned loc, int val)
{
    // 在头节点前插入的情况（位置0）
    if (loc == 0)
    {
        struct node *newNode = createNode(val);
        if (newNode == NULL)
            return 0;
        newNode->next = *pheadptr; // 新节点指向原头节点
        *pheadptr = newNode;       // 头指针指向新节点
        return 1;
    }

    unsigned c = countOfNodes(*pheadptr);
    if (loc > c)
        return 0; // 位置超出范围，插入失败

    struct node *current = *pheadptr;
    struct node *prev = NULL;
    unsigned pos = 0;

    // 找到插入位置的前一个节点
    while (current != NULL && pos < loc)
    {
        prev = current;
        current = current->next;
        pos++;
    }

    struct node *newNode = createNode(val);
    if (newNode == NULL)
        return 0;

    // 执行插入操作
    if (prev == NULL)
    {
        // 特殊情况：插入到空链表或头节点前
        newNode->next = *pheadptr;
        *pheadptr = newNode;
    }
    else
    {
        // 普通情况：在prev和current之间插入
        prev->next = newNode;
        newNode->next = current;
    }

    return 1;
}

/**
 * 从文件读取数据构建链表
 * @param pheadptr 指向头指针的指针（用于修改头指针）
 * @param filename 文件名
 * @return 操作成功返回1，失败返回0
 */
int buildListFromFile(struct node **pheadptr, const char *filename)
{
    FILE *file = fopen(filename, "r"); // 以只读方式打开文件
    if (file == NULL)
    {
        printf("无法打开文件 %s\n", filename);
        return 0;
    }

    char buffer[1024];
    // 读取文件中的一行数据
    if (fgets(buffer, sizeof(buffer), file) == NULL)
    {
        fclose(file);
        return 0;
    }
    fclose(file); // 关闭文件

    // 按逗号分割字符串并转换为整数
    char *token = strtok(buffer, ",");
    while (token != NULL)
    {
        int value = atoi(token);  // 字符串转整数
        addNode(pheadptr, value); // 添加到链表尾部
        token = strtok(NULL, ",");
    }

    return 1;
}

int main()
{
    struct node *headPtr = NULL;

    // 从文件读取数据构建链表
    if (!buildListFromFile(&headPtr, "3.txt"))
    {
        return 1;
    }

    printf("初始链表节点数据: ");
    printAllNode(headPtr);

    // 测试在第0个节点前插入（头节点前插入）
    {
        struct node *testHead = NULL;
        // 复制原链表用于测试
        struct node *current = headPtr;
        while (current != NULL)
        {
            addNode(&testHead, current->data);
            current = current->next;
        }

        printf("\n在第0个节点前插入100后: ");
        insertNodeBefore(&testHead, 0, 100);
        printAllNode(testHead);
        deleteAllNodes(&testHead); // 释放测试链表内存
    }

    // 测试在第1个节点前插入
    {
        struct node *testHead = NULL;
        struct node *current = headPtr;
        while (current != NULL)
        {
            addNode(&testHead, current->data);
            current = current->next;
        }

        printf("在第1个节点前插入100后: ");
        insertNodeBefore(&testHead, 1, 100);
        printAllNode(testHead);
        deleteAllNodes(&testHead);
    }

    // 测试在第10个节点前插入（可能超出链表长度）
    {
        struct node *testHead = NULL;
        struct node *current = headPtr;
        while (current != NULL)
        {
            addNode(&testHead, current->data);
            current = current->next;
        }

        printf("在第10个节点前插入100后: ");
        insertNodeBefore(&testHead, 10, 100);
        printAllNode(testHead);
        deleteAllNodes(&testHead);
    }

    deleteAllNodes(&headPtr); // 释放主链表内存
    return 0;
}