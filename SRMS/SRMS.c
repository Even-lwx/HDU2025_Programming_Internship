#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_NAME_LEN 50   // 名字最长长度
#define MAX_STUDENTS 1000 // 最大学生数量
#define FILENAME "students.txt"
#define INIT_CAPACITY 2
#define NUM_SUBJECTS 5

// 科目名称定义
const char *SUBJECT_NAMES[] = {"科目1", "科目2", "科目3", "科目4", "科目5"};

typedef struct
{
    int id;
    char name[MAX_NAME_LEN];
    float scores[NUM_SUBJECTS]; // 五科成绩
} Student;

typedef struct
{
    Student *data;
    int size;
    int capacity;
    int sort_mode; // 0: 无排序, 1: 按学号升序, 2: 按学号降序, 3: 按姓名升序, 4: 按姓名降序
} StudentDB;

// 函数声明
void initDB(StudentDB *db);
void loadFromFile(StudentDB *db);
void saveToFile(StudentDB *db);
void addStudent(StudentDB *db);
void deleteStudent(StudentDB *db);
void findStudent(StudentDB *db);
void modifyStudent(StudentDB *db);
void printAll(StudentDB *db);
void resizeDB(StudentDB *db, int new_capacity);
void clearInputBuffer();
float calculateAverage(Student *s);
int confirmAction(const char *prompt);
void applySort(StudentDB *db);                   // 应用当前排序规则
int compareById(const void *a, const void *b);   // 学号比较函数
int compareByName(const void *a, const void *b); // 姓名比较函数
void setSortMode(StudentDB *db);                 // 设置排序模式

int main()
{
    StudentDB db;
    initDB(&db);
    loadFromFile(&db); // 加载数据时会恢复排序模式
    applySort(&db);    // 初始加载后立即排序

    int choice;
    do
    {
        printf("\n学生成绩管理系统\n");
        printf("1. 添加学生\n");
        printf("2. 删除学生\n");
        printf("3. 显示所有学生\n");
        printf("4. 修改学生\n");
        printf("5. 查找学生\n");
        printf("6. 设置排序方式\n");
        printf("0. 退出系统\n");
        printf("请选择操作: ");
        scanf("%d", &choice);
        clearInputBuffer(); // 清除输入缓冲区

        switch (choice)
        {
        case 1:
            addStudent(&db);
            break;
        case 2:
            deleteStudent(&db);
            break;
        case 3:
            printAll(&db);
            break;
        case 4:
            modifyStudent(&db);
            break;
        case 5:
            findStudent(&db);
            break;
        case 6:
            setSortMode(&db);
            break;
        case 0:
            if (confirmAction("确定要退出系统吗？(y/n): "))
            {
                printf("系统退出，数据已保存。\n");
                choice = 0;
            }
            break;
        default:
            printf("无效选项，请重新输入。\n");
        }
    } while (choice != 0);

    // 保存数据并释放内存
    saveToFile(&db);
    free(db.data);
    return 0;
}

// 设置排序模式
void setSortMode(StudentDB *db)
{
    int choice;
    printf("\n请选择排序方式:\n");
    printf("1. 按学号升序排列\n");
    printf("2. 按学号降序排列\n");
    printf("3. 按姓名升序排列\n");
    printf("4. 按姓名降序排列\n");
    printf("0. 取消排序（保持添加顺序）\n");
    printf("当前排序方式: ");
    switch (db->sort_mode)
    {
    case 0:
        printf("无排序\n");
        break;
    case 1:
        printf("学号升序\n");
        break;
    case 2:
        printf("学号降序\n");
        break;
    case 3:
        printf("姓名升序\n");
        break;
    case 4:
        printf("姓名降序\n");
        break;
    }
    printf("请选择: ");
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice >= 0 && choice <= 4)
    {
        db->sort_mode = choice;
        applySort(db);
        printf("排序方式已设置\n");
    }
    else
    {
        printf("无效选择\n");
    }
}

// 应用当前排序规则
void applySort(StudentDB *db)
{
    if (db->size == 0 || db->sort_mode == 0)
    {
        return; // 无数据或不需要排序
    }

    switch (db->sort_mode)
    {
    case 1: // 学号升序
        qsort(db->data, db->size, sizeof(Student), compareById);
        break;
    case 2: // 学号降序
        qsort(db->data, db->size, sizeof(Student), compareById);
        // 反转数组实现降序
        for (int i = 0; i < db->size / 2; i++)
        {
            Student temp = db->data[i];
            db->data[i] = db->data[db->size - 1 - i];
            db->data[db->size - 1 - i] = temp;
        }
        break;
    case 3: // 姓名升序
        qsort(db->data, db->size, sizeof(Student), compareByName);
        break;
    case 4: // 姓名降序
        qsort(db->data, db->size, sizeof(Student), compareByName);
        // 反转数组实现降序
        for (int i = 0; i < db->size / 2; i++)
        {
            Student temp = db->data[i];
            db->data[i] = db->data[db->size - 1 - i];
            db->data[db->size - 1 - i] = temp;
        }
        break;
    }
}

// 学号比较函数
int compareById(const void *a, const void *b)
{
    Student *studentA = (Student *)a;
    Student *studentB = (Student *)b;
    return (studentA->id - studentB->id);
}

// 姓名比较函数
int compareByName(const void *a, const void *b)
{
    Student *studentA = (Student *)a;
    Student *studentB = (Student *)b;
    return strcmp(studentA->name, studentB->name);
}

void initDB(StudentDB *db)
{
    db->data = (Student *)malloc(INIT_CAPACITY * sizeof(Student));
    if (!db->data)
    {
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }
    db->size = 0;
    db->capacity = INIT_CAPACITY;
    db->sort_mode = 3; // 默认按名字升序排序
}

void loadFromFile(StudentDB *db)
{
    FILE *file = fopen(FILENAME, "r");
    if (!file)
    {
        printf("数据库文件不存在，将创建新文件。\n");
        return;
    }

    // 尝试读取排序模式（文件第一行）
    if (fscanf(file, "SortMode: %d\n", &db->sort_mode) != 1)
    {
        // 如果文件没有排序模式标记，重置到文件开头
        rewind(file);
        db->sort_mode = 3; // 使用默认排序模式
    }

    Student s;
    while (fscanf(file, "%d \"%49[^\"]\"", &s.id, s.name) == 2)
    {
        for (int i = 0; i < NUM_SUBJECTS; i++)
        {
            if (fscanf(file, "%f", &s.scores[i]) != 1)
            {
                printf("文件格式错误，加载失败\n");
                fclose(file);
                return;
            }
        }

        if (db->size >= db->capacity)
        {
            resizeDB(db, db->capacity * 2);
        }
        db->data[db->size++] = s;
    }

    fclose(file);
    printf("成功加载 %d 条记录，排序模式: %d\n", db->size, db->sort_mode);
}

void saveToFile(StudentDB *db)
{
    FILE *file = fopen(FILENAME, "w");
    if (!file)
    {
        fprintf(stderr, "无法打开文件进行写入\n");
        return;
    }

    // 保存排序模式（文件第一行）
    fprintf(file, "SortMode: %d\n", db->sort_mode);

    for (int i = 0; i < db->size; i++)
    {
        fprintf(file, "%d \"%s\"", db->data[i].id, db->data[i].name);
        for (int j = 0; j < NUM_SUBJECTS; j++)
        {
            fprintf(file, " %.2f", db->data[i].scores[j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("数据已保存到文件（排序模式: %d）\n", db->sort_mode);
}

void resizeDB(StudentDB *db, int new_capacity)
{
    Student *new_data = (Student *)realloc(db->data, new_capacity * sizeof(Student));
    if (!new_data)
    {
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }
    db->data = new_data;
    db->capacity = new_capacity;
}

int confirmAction(const char *prompt)
{
    char choice;
    printf("%s", prompt);
    scanf(" %c", &choice);
    clearInputBuffer();
    return (choice == 'y' || choice == 'Y');
}

void addStudent(StudentDB *db)
{
    int mode;
    do
    {
        printf("\n添加学生模式:\n");
        printf("1. 单个添加 (输入完整信息)\n");
        printf("2. 批量添加 (仅输入多个姓名，学号自增，成绩默认0)\n");
        printf("请选择模式 (1-2): ");

        if (scanf("%d", &mode) != 1)
        {
            clearInputBuffer();
            printf("错误：请输入数字1或2\n");
            mode = -1; // 强制重新输入
            continue;
        }
        clearInputBuffer();

        if (mode < 1 || mode > 2)
        {
            printf("错误：请选择有效模式（1或2）\n");
        }
    } while (mode < 1 || mode > 2);

    if (mode == 2)
    {
        // 批量添加模式
        if (db->size >= db->capacity)
        {
            resizeDB(db, db->capacity * 2);
        }

        char names_buffer[1024];
        printf("请输入学生姓名（用逗号分隔，如：张三,李四,王五）:\n");
        fgets(names_buffer, sizeof(names_buffer), stdin);
        names_buffer[strcspn(names_buffer, "\n")] = 0; // 移除换行符

        if (strlen(names_buffer) == 0)
        {
            printf("错误：未输入任何姓名！\n");
            return;
        }

        // 解析姓名列表
        char *token = strtok(names_buffer, ",");
        int added_count = 0;
        int error_count = 0;
        char added_names[MAX_STUDENTS][MAX_NAME_LEN] = {0}; // 存储已添加的姓名

        // 获取当前最大学号（处理删除后不连续的情况）
        int max_id = 0;
        for (int i = 0; i < db->size; i++)
        {
            if (db->data[i].id > max_id)
            {
                max_id = db->data[i].id;
            }
        }
        int start_id = max_id > 0 ? max_id + 1 : 1;

        while (token != NULL)
        {
            // 去除首尾空格
            char *name = token;
            while (isspace((unsigned char)*name))
                name++;

            // 检查姓名是否为空
            if (*name == 0)
            {
                printf("警告：发现空姓名，已跳过\n");
                error_count++;
                token = strtok(NULL, ",");
                continue;
            }

            char *end = name + strlen(name) - 1;
            while (end > name && isspace((unsigned char)*end))
                end--;
            end[1] = 0; // 截断尾部空格

            // 检查数据库中是否有重复姓名
            int db_duplicate = 0;
            for (int i = 0; i < db->size; i++)
            {
                if (strcmp(db->data[i].name, name) == 0)
                {
                    printf("错误：姓名 '%s' 已存在于数据库中，跳过添加\n", name);
                    error_count++;
                    db_duplicate = 1;
                    break;
                }
            }

            // 检查本次添加中是否有重复姓名
            int batch_duplicate = 0;
            for (int i = 0; i < added_count; i++)
            {
                if (strcmp(added_names[i], name) == 0)
                {
                    printf("错误：姓名 '%s' 在本次添加中重复，跳过添加\n", name);
                    error_count++;
                    batch_duplicate = 1;
                    break;
                }
            }

            if (db_duplicate || batch_duplicate)
            {
                token = strtok(NULL, ",");
                continue;
            }

            if (db->size >= db->capacity)
            {
                resizeDB(db, db->capacity * 2);
            }

            // 创建新学生记录
            Student new_student;
            new_student.id = start_id + added_count; // 连续分配学号
            strncpy(new_student.name, name, MAX_NAME_LEN - 1);
            new_student.name[MAX_NAME_LEN - 1] = 0; // 确保字符串终止

            // 初始化成绩为0
            for (int i = 0; i < NUM_SUBJECTS; i++)
            {
                new_student.scores[i] = 0.0f;
            }

            db->data[db->size++] = new_student;
            strncpy(added_names[added_count], name, MAX_NAME_LEN - 1);
            added_count++;
            token = strtok(NULL, ",");
        }

        if (added_count == 0)
        {
            printf("错误：所有输入姓名均无效或重复！\n");
            return;
        }

        applySort(db);
        saveToFile(db);
        printf("成功批量添加 %d 名学生！\n", added_count);
        if (error_count > 0)
        {
            printf("警告：跳过 %d 个无效或重复姓名\n", error_count);
        }
        printf("学号从 %d 开始连续分配，成绩默认初始化为0\n", start_id);
        return;
    }

    // 单个添加模式
    if (db->size >= db->capacity)
    {
        resizeDB(db, db->capacity * 2);
    }

    Student new_student;

    // 输入学号（带严格验证）
    while (1)
    {
        printf("\n请输入学号 (输入0取消): ");
        if (scanf("%d", &new_student.id) != 1)
        {
            clearInputBuffer();
            printf("错误：请输入有效数字\n");
            continue;
        }
        clearInputBuffer();

        if (new_student.id == 0)
        {
            printf("添加操作已取消。\n");
            return;
        }

        // 检查学号重复
        int duplicate = 0;
        for (int i = 0; i < db->size; i++)
        {
            if (db->data[i].id == new_student.id)
            {
                printf("错误：学号 %d 已存在于数据库中\n", new_student.id);
                duplicate = 1;
                break;
            }
        }

        if (!duplicate)
            break;

        printf("是否尝试其他学号？(y/n): ");
        char retry;
        scanf(" %c", &retry);
        clearInputBuffer();
        if (retry != 'y' && retry != 'Y')
        {
            printf("添加操作已取消。\n");
            return;
        }
    }

    // 输入姓名（带重复检查）
    while (1)
    {
        printf("请输入姓名 (输入0取消): ");
        fgets(new_student.name, MAX_NAME_LEN, stdin);
        new_student.name[strcspn(new_student.name, "\n")] = '\0'; // 移除换行符

        if (strcmp(new_student.name, "0") == 0)
        {
            printf("添加操作已取消。\n");
            return;
        }

        // 检查姓名重复
        int duplicate = 0;
        for (int i = 0; i < db->size; i++)
        {
            if (strcmp(db->data[i].name, new_student.name) == 0)
            {
                printf("错误：姓名 '%s' 已存在于数据库中\n", new_student.name);
                duplicate = 1;
                break;
            }
        }

        if (!duplicate)
            break;

        printf("是否尝试其他姓名？(y/n): ");
        char retry;
        scanf(" %c", &retry);
        clearInputBuffer();
        if (retry != 'y' && retry != 'Y')
        {
            printf("添加操作已取消。\n");
            return;
        }
    }

    // 输入成绩
    printf("开始输入各科成绩（输入负数取消）\n");
    for (int i = 0; i < NUM_SUBJECTS; i++)
    {
        while (1)
        {
            printf("请输入%s的成绩: ", SUBJECT_NAMES[i]);
            if (scanf("%f", &new_student.scores[i]) != 1)
            {
                clearInputBuffer();
                printf("输入格式错误，请输入数字!\n");
                continue;
            }
            clearInputBuffer();

            // 检测负数（取消逻辑）
            if (new_student.scores[i] < 0)
            {
                if (confirmAction("检测到负数，确定要取消添加吗？(y/n): "))
                {
                    printf("添加操作已取消。\n");
                    return;
                }
                else
                {
                    // 重新输入当前科目
                    continue;
                }
            }

            break;
        }
    }
    clearInputBuffer();

    // 确认添加
    printf("\n即将添加学生：\n");
    printf("学号: %d\n姓名: %s\n", new_student.id, new_student.name);
    printf("成绩: ");
    for (int i = 0; i < NUM_SUBJECTS; i++)
    {
        printf("%s:%.2f ", SUBJECT_NAMES[i], new_student.scores[i]);
    }
    printf("\n");

    if (!confirmAction("确认添加吗？(y/n): "))
    {
        printf("添加操作已取消。\n");
        return;
    }

    db->data[db->size++] = new_student;

    // 添加后自动排序
    applySort(db);

    saveToFile(db); // 立即保存到文件
    printf("学生添加成功！\n");
}

void deleteStudent(StudentDB *db)
{
    if (db->size == 0)
    {
        printf("数据库为空\n");
        return;
    }

    char name[MAX_NAME_LEN];
    printf("\n请输入要删除的学生姓名 (输入0取消): ");
    fgets(name, MAX_NAME_LEN, stdin);
    name[strcspn(name, "\n")] = '\0'; // 移除换行符

    if (strcmp(name, "0") == 0)
    {
        printf("删除操作已取消。\n");
        return;
    }

    int found_index = -1;
    for (int i = 0; i < db->size; i++)
    {
        if (strcmp(db->data[i].name, name) == 0)
        {
            found_index = i;
            break;
        }
    }

    if (found_index == -1)
    {
        printf("未找到姓名为 '%s' 的学生\n", name);
        return;
    }

    // 显示要删除的学生信息
    printf("\n找到学生：\n");
    printf("学号: %d\n姓名: %s\n", db->data[found_index].id, db->data[found_index].name);
    printf("成绩: ");
    for (int i = 0; i < NUM_SUBJECTS; i++)
    {
        printf("%s:%.2f ", SUBJECT_NAMES[i], db->data[found_index].scores[i]);
    }
    printf("\n平均分: %.2f\n", calculateAverage(&db->data[found_index]));

    if (!confirmAction("确定要删除这名学生吗？(y/n): "))
    {
        printf("删除操作已取消。\n");
        return;
    }

    // 将最后一个元素移到当前位置
    db->data[found_index] = db->data[db->size - 1];
    db->size--;

    // 如果空间利用率低于25%，缩小容量
    if (db->size > INIT_CAPACITY && db->size < db->capacity / 4)
    {
        resizeDB(db, db->capacity / 2);
    }

    // 删除后自动排序
    applySort(db);

    saveToFile(db);
    printf("学生 '%s' (学号: %d) 已删除\n", name, db->data[found_index].id);
}

void findStudent(StudentDB *db)
{
    if (db->size == 0)
    {
        printf("数据库为空\n");
        return;
    }

    int search_mode;
    printf("\n选择查找方式：\n");
    printf("1. 按学号查找\n");
    printf("2. 按姓名查找\n");
    printf("请选择: ");
    scanf("%d", &search_mode);
    clearInputBuffer();

    if (search_mode == 1)
    {
        // 按学号查找
        int id;
        printf("\n请输入学号 (输入0取消): ");
        scanf("%d", &id);
        clearInputBuffer();

        if (id == 0)
        {
            printf("查找操作已取消。\n");
            return;
        }

        for (int i = 0; i < db->size; i++)
        {
            if (db->data[i].id == id)
            {
                printf("\n学号: %d\n姓名: %s\n", db->data[i].id, db->data[i].name);
                for (int j = 0; j < NUM_SUBJECTS; j++)
                {
                    printf("%s: %.2f\n", SUBJECT_NAMES[j], db->data[i].scores[j]);
                }
                printf("平均分: %.2f\n", calculateAverage(&db->data[i]));
                return;
            }
        }

        printf("未找到学号 %d\n", id);
    }
    else if (search_mode == 2)
    {
        // 按姓名查找
        char name[MAX_NAME_LEN];
        printf("\n请输入姓名 (输入0取消): ");
        fgets(name, MAX_NAME_LEN, stdin);
        name[strcspn(name, "\n")] = '\0';

        if (strcmp(name, "0") == 0)
        {
            printf("查找操作已取消。\n");
            return;
        }

        for (int i = 0; i < db->size; i++)
        {
            if (strcmp(db->data[i].name, name) == 0)
            {
                printf("\n学号: %d\n姓名: %s\n", db->data[i].id, db->data[i].name);
                for (int j = 0; j < NUM_SUBJECTS; j++)
                {
                    printf("%s: %.2f\n", SUBJECT_NAMES[j], db->data[i].scores[j]);
                }
                printf("平均分: %.2f\n", calculateAverage(&db->data[i]));
                return;
            }
        }

        printf("未找到姓名为 '%s' 的学生\n", name);
    }
    else
    {
        printf("无效选择！\n");
    }
}

void modifyStudent(StudentDB *db)
{
    if (db->size == 0)
    {
        printf("数据库为空\n");
        return;
    }

    int search_mode;
    printf("\n选择查找方式：\n");
    printf("1. 按学号查找\n");
    printf("2. 按姓名查找\n");
    printf("请选择: ");
    scanf("%d", &search_mode);
    clearInputBuffer();

    int found_index = -1;

    /* 查找学生 */
    if (search_mode == 1)
    {
        // 按学号查找
        int id;
        printf("\n请输入学号 (输入0取消): ");
        scanf("%d", &id);
        clearInputBuffer();

        if (id == 0)
        {
            printf("操作取消。\n");
            return;
        }

        for (int i = 0; i < db->size; i++)
        {
            if (db->data[i].id == id)
            {
                found_index = i;
                break;
            }
        }
    }
    else if (search_mode == 2)
    {
        // 按姓名查找
        char name[MAX_NAME_LEN];
        printf("\n请输入姓名 (输入0取消): ");
        fgets(name, MAX_NAME_LEN, stdin);
        name[strcspn(name, "\n")] = '\0';

        if (strcmp(name, "0") == 0)
        {
            printf("操作取消。\n");
            return;
        }

        for (int i = 0; i < db->size; i++)
        {
            if (strcmp(db->data[i].name, name) == 0)
            {
                found_index = i;
                break;
            }
        }
    }
    else
    {
        printf("无效选择！\n");
        return;
    }

    if (found_index == -1)
    {
        printf("未找到该学生\n");
        return;
    }

    /* 显示当前信息 */
    printf("\n当前学生信息: ");
    printf("\n学号: %d\n姓名: %s\n", db->data[found_index].id, db->data[found_index].name);
    printf("成绩: ");
    for (int j = 0; j < NUM_SUBJECTS; j++)
    {
        printf("%s:%.2f ", SUBJECT_NAMES[j], db->data[found_index].scores[j]);
    }
    printf("\n平均分: %.2f\n", calculateAverage(&db->data[found_index]));

    Student original = db->data[found_index]; // 备份原始数据

    /* 修改学号 */
    printf("\n输入新学号 (回车保留原值，0取消): ");
    char id_buf[20];
    fgets(id_buf, sizeof(id_buf), stdin);
    id_buf[strcspn(id_buf, "\n")] = '\0';

    if (strcmp(id_buf, "0") == 0)
    {
        printf("操作取消。\n");
        return;
    }

    if (id_buf[0] != '\0')
    {
        int new_id = atoi(id_buf);
        if (new_id > 0)
        {
            // 检查学号重复
            int duplicate = 0;
            for (int i = 0; i < db->size; i++)
            {
                if (i != found_index && db->data[i].id == new_id)
                {
                    printf("学号已存在，保留原学号！\n");
                    duplicate = 1;
                    break;
                }
            }
            if (!duplicate)
            {
                db->data[found_index].id = new_id;
                printf("学号已更新为: %d\n", new_id);
            }
        }
        else
        {
            printf("学号必须为正整数，保留原学号！\n");
        }
    }

    /* 修改姓名 */
    printf("\n输入新姓名 (回车保留原值，0取消): ");
    char new_name[MAX_NAME_LEN];
    fgets(new_name, MAX_NAME_LEN, stdin);
    new_name[strcspn(new_name, "\n")] = '\0';

    if (strcmp(new_name, "0") == 0)
    {
        printf("操作取消。\n");
        return;
    }

    if (new_name[0] != '\0')
    {
        strncpy(db->data[found_index].name, new_name, MAX_NAME_LEN);
        printf("姓名已更新为: %s\n", new_name);
    }

    /* 修改成绩 */
    printf("\n开始修改成绩（-1取消全部，负数跳过当前科目）：\n");
    int cancel_all = 0;

    for (int j = 0; j < NUM_SUBJECTS; j++)
    {
        float new_score;
        while (1)
        {
            printf("输入%s新成绩: ", SUBJECT_NAMES[j]);
            if (scanf("%f", &new_score) != 1)
            {
                clearInputBuffer();
                printf("请输入数字！\n");
                continue;
            }
            clearInputBuffer();

            if (new_score == -1)
            {
                cancel_all = 1;
                break;
            }

            if (new_score < 0)
            {
                printf("跳过%s修改\n", SUBJECT_NAMES[j]);
                break;
            }

            if (new_score < 0 || new_score > 100)
            {
                printf("成绩应在0-100之间，确认输入？(y/n): ");
                char confirm;
                scanf(" %c", &confirm);
                clearInputBuffer();
                if (confirm != 'y' && confirm != 'Y')
                    continue;
            }

            db->data[found_index].scores[j] = new_score;
            break;
        }
        if (cancel_all)
            break;
    }

    if (cancel_all)
    {
        db->data[found_index] = original;
        printf("操作取消。\n");
        return;
    }

    /* 确认修改 */
    printf("\n修改后信息: ");
    printf("\n学号: %d\n姓名: %s\n", db->data[found_index].id, db->data[found_index].name);
    printf("成绩: ");
    for (int j = 0; j < NUM_SUBJECTS; j++)
    {
        printf("%s:%.2f ", SUBJECT_NAMES[j], db->data[found_index].scores[j]);
    }
    printf("\n平均分: %.2f\n", calculateAverage(&db->data[found_index]));

    if (!confirmAction("确认修改？(y/n): "))
    {
        db->data[found_index] = original;
        printf("修改已取消。\n");
        return;
    }

    applySort(db);
    saveToFile(db);
    printf("信息更新成功！\n");
}

void printAll(StudentDB *db)
{
    if (db->size == 0)
    {
        printf("数据库为空\n");
        return;
    }

    // 显示当前排序方式
    printf("\n当前排序方式: ");
    switch (db->sort_mode)
    {
    case 0:
        printf("无排序（按添加顺序）\n");
        break;
    case 1:
        printf("学号升序\n");
        break;
    case 2:
        printf("学号降序\n");
        break;
    case 3:
        printf("姓名升序\n");
        break;
    case 4:
        printf("姓名降序\n");
        break;
    }

    printf("\n%-8s %-20s", "学号", "姓名");
    for (int i = 0; i < NUM_SUBJECTS; i++)
    {
        printf(" %-8s", SUBJECT_NAMES[i]);
    }
    printf(" %-8s\n", "平均分");

    // 打印分隔线
    int line_length = 28 + NUM_SUBJECTS * 10;
    for (int i = 0; i < line_length; i++)
    {
        printf("-");
    }
    printf("\n");

    for (int i = 0; i < db->size; i++)
    {
        printf("%-8d %-20s", db->data[i].id, db->data[i].name);
        for (int j = 0; j < NUM_SUBJECTS; j++)
        {
            printf(" %-8.2f", db->data[i].scores[j]);
        }
        printf(" %-8.2f\n", calculateAverage(&db->data[i]));
    }
    printf("\n共 %d 条记录\n", db->size);
}

void clearInputBuffer() // 清理输入缓冲区
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

float calculateAverage(Student *s)
{
    float sum = 0;
    for (int i = 0; i < NUM_SUBJECTS; i++)
    {
        sum += s->scores[i];
    }
    return sum / NUM_SUBJECTS;
}