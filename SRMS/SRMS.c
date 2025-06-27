#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LEN 50
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

int main()
{
    StudentDB db;
    initDB(&db);
    loadFromFile(&db);

    int choice;
    do
    {
        printf("\n学生成绩管理系统\n");
        printf("1. 添加学生\n");
        printf("2. 删除学生\n");
        printf("3. 查找学生\n");
        printf("4. 修改学生\n");
        printf("5. 显示所有学生\n");
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
            findStudent(&db);
            break;
        case 4:
            modifyStudent(&db);
            break;
        case 5:
            printAll(&db);
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
}

void loadFromFile(StudentDB *db)
{
    FILE *file = fopen(FILENAME, "r");
    if (!file)
    {
        printf("数据库文件不存在，将创建新文件。\n");
        return;
    }

    Student s;
    while (fscanf(file, "%d %49s", &s.id, s.name) == 2)
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
    printf("成功加载 %d 条记录\n", db->size);
}

void saveToFile(StudentDB *db)
{
    FILE *file = fopen(FILENAME, "w");
    if (!file)
    {
        fprintf(stderr, "无法打开文件进行写入\n");
        return;
    }

    for (int i = 0; i < db->size; i++)
    {
        fprintf(file, "%d %s", db->data[i].id, db->data[i].name);
        for (int j = 0; j < NUM_SUBJECTS; j++)
        {
            fprintf(file, " %.2f", db->data[i].scores[j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("数据已保存到文件\n");
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
    if (db->size >= db->capacity)
    {
        resizeDB(db, db->capacity * 2);
    }

    Student new_student;

    // 输入学号（可取消）
    while (1)
    {
        printf("\n请输入学号 (输入0取消): ");
        if (scanf("%d", &new_student.id) != 1)
        {
            clearInputBuffer();
            printf("输入无效，请重新输入。\n");
            continue;
        }
        clearInputBuffer();

        if (new_student.id == 0)
        {
            printf("添加操作已取消。\n");
            return;
        }

        // 检查学号是否重复
        int duplicate = 0;
        for (int i = 0; i < db->size; i++)
        {
            if (db->data[i].id == new_student.id)
            {
                printf("错误：学号 %d 已存在\n", new_student.id);
                duplicate = 1;
                break;
            }
        }

        if (!duplicate)
            break;
    }

    // 输入姓名（可取消）
    printf("请输入姓名 (输入0取消): ");
    fgets(new_student.name, MAX_NAME_LEN, stdin);
    new_student.name[strcspn(new_student.name, "\n")] = '\0'; // 移除换行符

    if (strcmp(new_student.name, "0") == 0)
    {
        printf("添加操作已取消。\n");
        return;
    }

    // 输入成绩（可取消）
    printf("请输入五科成绩（科目1 科目2 科目3 科目4 科目5），输入0取消: ");
    for (int i = 0; i < NUM_SUBJECTS; i++)
    {
        if (scanf("%f", &new_student.scores[i]) != 1)
        {
            clearInputBuffer();
            printf("输入格式错误，操作取消\n");
            return;
        }
        // 检查是否取消
        if (new_student.scores[i] == 0)
        {
            // 确认取消
            if (confirmAction("检测到0，确定要取消添加吗？(y/n): "))
            {
                printf("添加操作已取消。\n");
                return;
            }
            else
            {
                // 重新输入当前科目
                i--;
                continue;
            }
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

    int id;
    printf("\n请输入要删除的学号 (输入0取消): ");
    scanf("%d", &id);
    clearInputBuffer();

    if (id == 0)
    {
        printf("删除操作已取消。\n");
        return;
    }

    int found_index = -1;
    for (int i = 0; i < db->size; i++)
    {
        if (db->data[i].id == id)
        {
            found_index = i;
            break;
        }
    }

    if (found_index == -1)
    {
        printf("未找到学号 %d\n", id);
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
    if (db->size > 0 && db->size < db->capacity / 4)
    {
        resizeDB(db, db->capacity / 2);
    }

    saveToFile(db);
    printf("学号 %d 已删除\n", id);
}

void findStudent(StudentDB *db)
{
    if (db->size == 0)
    {
        printf("数据库为空\n");
        return;
    }

    int id;
    printf("\n请输入要查找的学号 (输入0取消): ");
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

void modifyStudent(StudentDB *db)
{
    if (db->size == 0)
    {
        printf("数据库为空\n");
        return;
    }

    int id;
    printf("\n请输入要修改的学号 (输入0取消): ");
    scanf("%d", &id);
    clearInputBuffer();

    if (id == 0)
    {
        printf("修改操作已取消。\n");
        return;
    }

    int found_index = -1;
    for (int i = 0; i < db->size; i++)
    {
        if (db->data[i].id == id)
        {
            found_index = i;
            break;
        }
    }

    if (found_index == -1)
    {
        printf("未找到学号 %d\n", id);
        return;
    }

    // 显示当前信息
    printf("\n当前学生信息: \n");
    printf("学号: %d\n姓名: %s\n", db->data[found_index].id, db->data[found_index].name);
    for (int j = 0; j < NUM_SUBJECTS; j++)
    {
        printf("%s: %.2f\n", SUBJECT_NAMES[j], db->data[found_index].scores[j]);
    }
    printf("平均分: %.2f\n", calculateAverage(&db->data[found_index]));

    // 备份原始数据以便取消修改
    Student original = db->data[found_index];

    // 修改姓名
    printf("\n输入新姓名 (直接回车保留原值，输入0取消修改): ");
    char new_name[MAX_NAME_LEN];
    fgets(new_name, MAX_NAME_LEN, stdin);
    new_name[strcspn(new_name, "\n")] = '\0';

    if (strcmp(new_name, "0") == 0)
    {
        printf("修改操作已取消。\n");
        return;
    }
    if (new_name[0] != '\0')
    {
        strncpy(db->data[found_index].name, new_name, MAX_NAME_LEN);
    }

    // 修改成绩
    printf("输入新成绩（按顺序输入五科成绩，输入负数跳过修改，输入0取消修改）: ");
    float new_scores[NUM_SUBJECTS];
    int cancel = 0;
    for (int j = 0; j < NUM_SUBJECTS; j++)
    {
        if (scanf("%f", &new_scores[j]) != 1)
        {
            clearInputBuffer();
            printf("输入格式错误，修改取消\n");
            // 恢复原始数据
            db->data[found_index] = original;
            return;
        }
        if (new_scores[j] == 0)
        {
            cancel = 1;
            break;
        }
    }
    clearInputBuffer();

    if (cancel)
    {
        if (confirmAction("检测到0，确定要取消修改吗？(y/n): "))
        {
            // 恢复原始数据
            db->data[found_index] = original;
            printf("修改操作已取消。\n");
            return;
        }
        else
        {
            // 继续修改
            cancel = 0;
        }
    }

    // 应用成绩修改
    for (int j = 0; j < NUM_SUBJECTS; j++)
    {
        if (new_scores[j] >= 0)
        {
            db->data[found_index].scores[j] = new_scores[j];
        }
    }

    // 显示修改后的信息
    printf("\n修改后的学生信息: \n");
    printf("学号: %d\n姓名: %s\n", db->data[found_index].id, db->data[found_index].name);
    for (int j = 0; j < NUM_SUBJECTS; j++)
    {
        printf("%s: %.2f\n", SUBJECT_NAMES[j], db->data[found_index].scores[j]);
    }
    printf("平均分: %.2f\n", calculateAverage(&db->data[found_index]));

    if (!confirmAction("确认修改吗？(y/n): "))
    {
        // 恢复原始数据
        db->data[found_index] = original;
        printf("修改操作已取消。\n");
        return;
    }

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

void clearInputBuffer()
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