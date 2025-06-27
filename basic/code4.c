#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * 链表节点结构
 * data: 节点存储的整数值
 * next: 指向下一个节点的指针
 */
struct node
{
    int data;
    struct node *next;
};

/**
 * 创建新节点
 * @param val 节点存储的值
 * @return 成功返回新节点指针，失败返回NULL
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
 * 在链表尾部添加新节点
 * @param pheadptr 指向头指针的指针
 * @param val 新节点的值
 * @return 成功返回1，内存分配失败返回0
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
 * 计算链表节点数量
 * @param headptr 链表头指针
 * @return 节点数量
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
 * 打印链表所有节点数据
 * @param headptr 链表头指针
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
 * 删除指定位置的节点
 * @param pheadptr 指向头指针的指针
 * @param loc 删除位置(从1开始)
 * @return 成功返回1，位置无效返回0
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
 * 删除链表所有节点，释放内存
 * @param pheadptr 指向头指针的指针
 */
void deleteAllNodes(struct node **pheadptr)
{
    while (countOfNodes(*pheadptr))
        deleteNode(pheadptr, 1);
}

/**
 * 逆置链表
 * @param headptr 原链表头指针
 * @return 逆置后链表的头指针
 */
struct node *reverseList(struct node *headptr)
{
    struct node *prev = NULL;       // 前驱节点
    struct node *current = headptr; // 当前节点
    struct node *next = NULL;       // 后继节点

    // 遍历链表，改变指针方向
    while (current != NULL)
    {
        next = current->next; // 保存下一个节点
        current->next = prev; // 当前节点指向前驱
        prev = current;       // 前驱后移
        current = next;       // 当前节点后移
    }

    return prev; // 返回新的头指针
}

/**
 * 从文件读取数据构建链表
 * @param pheadptr 指向头指针的指针
 * @param filename 文件名
 * @return 成功返回1，失败返回0
 */
int buildListFromFile(struct node **pheadptr, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("无法打开文件 %s\n", filename);
        return 0;
    }

    char line[1024];
    // 读取文件第一行
    if (fgets(line, sizeof(line), file) == NULL)
    {
        printf("读取文件内容失败\n");
        fclose(file);
        return 0;
    }
    fclose(file);

    // 解析逗号分隔的数值
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
 * 将链表数据追加到文件
 * @param headptr 链表头指针
 * @param filename 文件名
 * @return 成功返回1，失败返回0
 */
int appendListToFile(struct node *headptr, const char *filename)
{
    FILE *file = fopen(filename, "a");
    if (file == NULL)
    {
        printf("无法打开文件 %s 进行追加\n", filename);
        return 0;
    }

    // 追加换行符，与原数据分隔
    fprintf(file, "\n");

    // 追加逆置后的链表数据
    struct node *current = headptr;
    if (current != NULL)
    {
        fprintf(file, "%d", current->data);
        current = current->next;
    }

    // 以逗号分隔输出剩余节点
    while (current != NULL)
    {
        fprintf(file, ",%d", current->data);
        current = current->next;
    }

    fclose(file);
    return 1;
}

/**
 * 主函数：程序入口点
 * 1. 从4.txt读取数据构建链表
 * 2. 逆置链表
 * 3. 将逆置后的链表数据追加到4.txt
 */
int main()
{
    struct node *headPtr = NULL;

    // 从文件读取数据构建链表
    if (buildListFromFile(&headPtr, "4.txt"))
    {
        printf("从文件读取数据构建链表成功\n");
        printf("原始链表数据: ");
        printAllNode(headPtr);

        // 逆置链表
        headPtr = reverseList(headPtr);
        printf("逆置后链表数据: ");
        printAllNode(headPtr);

        // 将逆置后的链表数据追加到文件
        if (appendListToFile(headPtr, "4.txt"))
        {
            printf("逆置后的链表数据已成功追加到文件\n");
        }

        // 清理资源
        deleteAllNodes(&headPtr);
    }

    return 0;
}