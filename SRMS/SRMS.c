#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* 宏定义 */
#define MAX_NAME_LEN 50         // 学生姓名最大长度
#define MAX_STUDENTS 1000       // 最大学生数量
#define FILENAME "students.txt" // 数据文件名
#define INIT_CAPACITY 2         // 初始内存容量
#define NUM_SUBJECTS 5          // 科目数量

/* 科目名称定义 */
const char *SUBJECT_NAMES[] = {"科目1", "科目2", "科目3", "科目4", "科目5"};

/* 学生结构体定义 */
typedef struct
{
    int id;                     // 学号
    char name[MAX_NAME_LEN];    // 姓名
    float scores[NUM_SUBJECTS]; // 各科成绩
} Student;

/* 学生数据库结构体定义 */
typedef struct
{
    Student *data; // 学生数据数组
    int size;      // 已存储学生数量
    int capacity;  // 数据库容量
    int sort_mode; // 排序模式(0:无排序,1:学号升序,2:学号降序,3:姓名升序,4:姓名降序)
} StudentDB;

/* 函数声明 */
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
void applySort(StudentDB *db);
int compareById(const void *a, const void *b);
int compareByName(const void *a, const void *b);
void setSortMode(StudentDB *db);

/* 主函数 */
int main()
{
    StudentDB db;
    initDB(&db);
    loadFromFile(&db);
    applySort(&db);

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
        clearInputBuffer();

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

    saveToFile(&db);
    free(db.data);
    return 0;
}

/* 设置排序模式 */
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

/* 应用排序规则 */
void applySort(StudentDB *db)
{
    if (db->size == 0 || db->sort_mode == 0)
    {
        return;
    }

    switch (db->sort_mode)
    {
    case 1: // 学号升序
        qsort(db->data, db->size, sizeof(Student), compareById);
        break;
    case 2: // 学号降序
        qsort(db->data, db->size, sizeof(Student), compareById);
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
        for (int i = 0; i < db->size / 2; i++)
        {
            Student temp = db->data[i];
            db->data[i] = db->data[db->size - 1 - i];
            db->data[db->size - 1 - i] = temp;
        }
        break;
    }
}

/* 按学号比较函数 */
int compareById(const void *a, const void *b)
{
    Student *studentA = (Student *)a;
    Student *studentB = (Student *)b;
    return (studentA->id - studentB->id);
}

/* 按姓名比较函数 */
int compareByName(const void *a, const void *b)
{
    Student *studentA = (Student *)a;
    Student *studentB = (Student *)b;
    return strcmp(studentA->name, studentB->name);
}

/* 初始化数据库 */
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
    db->sort_mode = 3; // 默认按姓名升序
}

/* 从文件加载数据 */
void loadFromFile(StudentDB *db)
{
    FILE *file = fopen(FILENAME, "r");
    if (!file)
    {
        printf("数据库文件不存在，将创建新文件。\n");
        return;
    }

    // 读取排序模式
    if (fscanf(file, "SortMode: %d\n", &db->sort_mode) != 1)
    {
        rewind(file);
        db->sort_mode = 3; // 使用默认排序
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

/* 保存数据到文件 */
void saveToFile(StudentDB *db)
{
    FILE *file = fopen(FILENAME, "w");
    if (!file)
    {
        fprintf(stderr, "无法打开文件进行写入\n");
        return;
    }

    // 保存排序模式
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

/* 调整数据库容量 */
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

/* 确认操作 */
int confirmAction(const char *prompt)
{
    char choice;
    printf("%s", prompt);
    scanf(" %c", &choice);
    clearInputBuffer();
    return (choice == 'y' || choice == 'Y');
}

/* 显示成绩（0显示为空格，非0显示数值） */
void displayScore(float score)
{
    if (score == 0.0f)
    {
        printf("  "); // 两个空格表示空值
    }
    else
    {
        printf("%.2f", score);
    }
}

/* 添加学生 */
void addStudent(StudentDB *db)
{
    int mode;
    do
    {
        printf("\n添加学生模式:\n");
        printf("1. 单个添加 (输入完整信息)\n");
        printf("2. 批量添加 (仅输入多个姓名，学号自增，成绩默认空)\n");
        printf("请选择模式 (1-2): ");

        if (scanf("%d", &mode) != 1)
        {
            clearInputBuffer();
            printf("错误：请输入数字1或2\n");
            mode = -1;
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
        names_buffer[strcspn(names_buffer, "\n")] = 0;

        if (strlen(names_buffer) == 0)
        {
            printf("错误：未输入任何姓名！\n");
            return;
        }

        char *token = strtok(names_buffer, ",");
        int added_count = 0;
        int error_count = 0;
        char added_names[MAX_STUDENTS][MAX_NAME_LEN] = {0};

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
            char *name = token;
            while (isspace((unsigned char)*name))
                name++;

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
            end[1] = 0;

            // 检查姓名重复
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
            new_student.id = start_id + added_count;
            strncpy(new_student.name, name, MAX_NAME_LEN - 1);
            new_student.name[MAX_NAME_LEN - 1] = 0;

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
        printf("学号从 %d 开始连续分配，成绩默认为空\n", start_id);
        return;
    }

    // 单个添加模式
    if (db->size >= db->capacity)
    {
        resizeDB(db, db->capacity * 2);
    }

    Student new_student;

    // 输入学号
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

    // 输入姓名
    while (1)
    {
        printf("请输入姓名 (输入0取消): ");
        fgets(new_student.name, MAX_NAME_LEN, stdin);
        new_student.name[strcspn(new_student.name, "\n")] = '\0';

        if (strcmp(new_student.name, "0") == 0)
        {
            printf("添加操作已取消。\n");
            return;
        }

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

            if (new_student.scores[i] < 0)
            {
                if (confirmAction("检测到负数，确定要取消添加吗？(y/n): "))
                {
                    printf("添加操作已取消。\n");
                    return;
                }
                else
                {
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
        printf("%s:", SUBJECT_NAMES[i]);
        displayScore(new_student.scores[i]);
        printf(" ");
    }
    printf("\n");

    if (!confirmAction("确认添加吗？(y/n): "))
    {
        printf("添加操作已取消。\n");
        return;
    }

    db->data[db->size++] = new_student;
    applySort(db);
    saveToFile(db);
    printf("学生添加成功！\n");
}

/* 删除学生 */
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
    name[strcspn(name, "\n")] = '\0';

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
        if (db->data[found_index].scores[i] == 0.0f)
        {
            printf("%s:  ", SUBJECT_NAMES[i]);
        }
        else
        {
            printf("%s:%.2f ", SUBJECT_NAMES[i], db->data[found_index].scores[i]);
        }
    }
    printf("\n平均分: ");
    float avg = calculateAverage(&db->data[found_index]);
    if (avg == 0.0f)
    {
        printf("  \n");
    }
    else
    {
        printf("%.2f\n", avg);
    }

    if (!confirmAction("确定要删除这名学生吗？(y/n): "))
    {
        printf("删除操作已取消。\n");
        return;
    }

    // 删除学生记录
    db->data[found_index] = db->data[db->size - 1];
    db->size--;

    // 调整内存容量
    if (db->size > INIT_CAPACITY && db->size < db->capacity / 4)
    {
        resizeDB(db, db->capacity / 2);
    }

    applySort(db);
    saveToFile(db);
    printf("学生 '%s' (学号: %d) 已删除\n", name, db->data[found_index].id);
}

/* 查找学生 */
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
                    printf("%s: ", SUBJECT_NAMES[j]);
                    if (db->data[i].scores[j] == 0.0f)
                    {
                        printf("  \n");
                    }
                    else
                    {
                        printf("%.2f\n", db->data[i].scores[j]);
                    }
                }

                float avg = calculateAverage(&db->data[i]);
                printf("平均分: ");
                if (avg == 0.0f)
                {
                    printf("  \n");
                }
                else
                {
                    printf("%.2f\n", avg);
                }
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
                    printf("%s: ", SUBJECT_NAMES[j]);
                    if (db->data[i].scores[j] == 0.0f)
                    {
                        printf("  \n");
                    }
                    else
                    {
                        printf("%.2f\n", db->data[i].scores[j]);
                    }
                }

                float avg = calculateAverage(&db->data[i]);
                printf("平均分: ");
                if (avg == 0.0f)
                {
                    printf("  \n");
                }
                else
                {
                    printf("%.2f\n", avg);
                }
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

/* 修改学生信息 */
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

    // 查找学生
    if (search_mode == 1)
    {
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

    // 显示当前信息
    printf("\n当前学生信息: ");
    printf("\n学号: %d\n姓名: %s\n", db->data[found_index].id, db->data[found_index].name);
    printf("成绩: ");
    for (int j = 0; j < NUM_SUBJECTS; j++)
    {
        if (db->data[found_index].scores[j] == 0.0f)
        {
            printf("%s:  ", SUBJECT_NAMES[j]);
        }
        else
        {
            printf("%s:%.2f ", SUBJECT_NAMES[j], db->data[found_index].scores[j]);
        }
    }
    printf("\n平均分: ");
    float avg = calculateAverage(&db->data[found_index]);
    if (avg == 0.0f)
    {
        printf("  \n");
    }
    else
    {
        printf("%.2f\n", avg);
    }

    Student original = db->data[found_index];

    // 修改学号
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
            int duplicate = 0;
            for (int i = 0; i < db->size; i++)
            {
                if (i != found_index && db->data[i].id == new_id)
                {
                    printf("错误：学号 %d 已存在！\n", new_id);
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

    // 修改姓名
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
        int duplicate = 0;
        for (int i = 0; i < db->size; i++)
        {
            if (i != found_index && strcmp(db->data[i].name, new_name) == 0)
            {
                printf("错误：姓名 '%s' 已存在！\n", new_name);
                duplicate = 1;
                break;
            }
        }
        if (!duplicate)
        {
            strncpy(db->data[found_index].name, new_name, MAX_NAME_LEN);
            printf("姓名已更新为: %s\n", new_name);
        }
    }

    // 修改成绩
    printf("\n开始修改成绩（回车保留原值，-1取消全部）：\n");
    int cancel_all = 0;

    for (int j = 0; j < NUM_SUBJECTS; j++)
    {
        char score_buf[20];
        float new_score;
        int valid_input = 0;

        while (!valid_input)
        {
            printf("输入%s新成绩 (当前值: ", SUBJECT_NAMES[j]);
            if (db->data[found_index].scores[j] == 0.0f)
            {
                printf("  ");
            }
            else
            {
                printf("%.2f", db->data[found_index].scores[j]);
            }
            printf("): ");

            fgets(score_buf, sizeof(score_buf), stdin);
            score_buf[strcspn(score_buf, "\n")] = '\0';

            // 回车保留原值
            if (score_buf[0] == '\0')
            {
                valid_input = 1;
                continue;
            }

            // 取消全部操作
            if (strcmp(score_buf, "-1") == 0)
            {
                cancel_all = 1;
                valid_input = 1;
                break;
            }

            // 转换成绩
            char *endptr;
            new_score = strtof(score_buf, &endptr);
            if (*endptr != '\0')
            {
                printf("错误：请输入有效的数字！\n");
                continue;
            }

            // 验证成绩范围
            if (new_score < 0 || new_score > 100)
            {
                printf("错误：成绩应在0-100之间！\n");
                continue;
            }

            valid_input = 1;
            db->data[found_index].scores[j] = new_score;
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

    // 确认修改
    printf("\n修改后信息: ");
    printf("\n学号: %d\n姓名: %s\n", db->data[found_index].id, db->data[found_index].name);
    printf("成绩: ");
    for (int j = 0; j < NUM_SUBJECTS; j++)
    {
        if (db->data[found_index].scores[j] == 0.0f)
        {
            printf("%s:  ", SUBJECT_NAMES[j]);
        }
        else
        {
            printf("%s:%.2f ", SUBJECT_NAMES[j], db->data[found_index].scores[j]);
        }
    }
    printf("\n平均分: ");
    float new_avg = calculateAverage(&db->data[found_index]);
    if (new_avg == 0.0f)
    {
        printf("  \n");
    }
    else
    {
        printf("%.2f\n", new_avg);
    }

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

/* 显示所有学生 */
void printAll(StudentDB *db)
{
    if (db->size == 0)
    {
        printf("数据库为空\n");
        return;
    }

    // 显示排序方式
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

    // 打印表头
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

    // 打印学生数据
    for (int i = 0; i < db->size; i++)
    {
        printf("%-8d %-20s", db->data[i].id, db->data[i].name);
        for (int j = 0; j < NUM_SUBJECTS; j++)
        {
            if (db->data[i].scores[j] == 0.0f)
            {
                printf(" %-8s", "  ");
            }
            else
            {
                printf(" %-8.2f", db->data[i].scores[j]);
            }
        }
        float avg = calculateAverage(&db->data[i]);
        if (avg == 0.0f)
        {
            printf(" %-8s\n", "  ");
        }
        else
        {
            printf(" %-8.2f\n", avg);
        }
    }

    printf("\n共 %d 条记录\n", db->size);
}

/* 清理输入缓冲区 */
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/* 计算平均分 */
float calculateAverage(Student *s)
{
    float sum = 0;
    for (int i = 0; i < NUM_SUBJECTS; i++)
    {
        sum += s->scores[i];
    }
    return sum / NUM_SUBJECTS;
}