#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_NAME_LEN 50 // ���������
#define FILENAME "students.txt"
#define INIT_CAPACITY 2
#define NUM_SUBJECTS 5

// ��Ŀ���ƶ���
const char *SUBJECT_NAMES[] = {"��Ŀ1", "��Ŀ2", "��Ŀ3", "��Ŀ4", "��Ŀ5"};

typedef struct
{
    int id;
    char name[MAX_NAME_LEN];
    float scores[NUM_SUBJECTS]; // ��Ƴɼ�
} Student;

typedef struct
{
    Student *data;
    int size;
    int capacity;
    int sort_mode; // 0: ������, 1: ��ѧ������, 2: ��ѧ�Ž���, 3: ����������, 4: ����������
} StudentDB;

// ��������
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
void applySort(StudentDB *db);                   // Ӧ�õ�ǰ�������
int compareById(const void *a, const void *b);   // ѧ�űȽϺ���
int compareByName(const void *a, const void *b); // �����ȽϺ���
void setSortMode(StudentDB *db);                 // ��������ģʽ

int main()
{
    StudentDB db;
    initDB(&db);
    loadFromFile(&db); // ��������ʱ��ָ�����ģʽ
    applySort(&db);    // ��ʼ���غ���������

    int choice;
    do
    {
        printf("\nѧ���ɼ�����ϵͳ\n");
        printf("1. ���ѧ��\n");
        printf("2. ɾ��ѧ��\n");
        printf("3. ����ѧ��\n");
        printf("4. �޸�ѧ��\n");
        printf("5. ��ʾ����ѧ��\n");
        printf("6. ��������ʽ\n");
        printf("0. �˳�ϵͳ\n");
        printf("��ѡ�����: ");
        scanf("%d", &choice);
        clearInputBuffer(); // ������뻺����

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
        case 6:
            setSortMode(&db);
            break;
        case 0:
            if (confirmAction("ȷ��Ҫ�˳�ϵͳ��(y/n): "))
            {
                printf("ϵͳ�˳��������ѱ��档\n");
                choice = 0;
            }
            break;
        default:
            printf("��Чѡ����������롣\n");
        }
    } while (choice != 0);

    // �������ݲ��ͷ��ڴ�
    saveToFile(&db);
    free(db.data);
    return 0;
}

// ��������ģʽ
void setSortMode(StudentDB *db)
{
    int choice;
    printf("\n��ѡ������ʽ:\n");
    printf("1. ��ѧ����������\n");
    printf("2. ��ѧ�Ž�������\n");
    printf("3. ��������������\n");
    printf("4. ��������������\n");
    printf("0. ȡ�����򣨱������˳��\n");
    printf("��ǰ����ʽ: ");
    switch (db->sort_mode)
    {
    case 0:
        printf("������\n");
        break;
    case 1:
        printf("ѧ������\n");
        break;
    case 2:
        printf("ѧ�Ž���\n");
        break;
    case 3:
        printf("��������\n");
        break;
    case 4:
        printf("��������\n");
        break;
    }
    printf("��ѡ��: ");
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice >= 0 && choice <= 4)
    {
        db->sort_mode = choice;
        applySort(db);
        printf("����ʽ������\n");
    }
    else
    {
        printf("��Чѡ��\n");
    }
}

// Ӧ�õ�ǰ�������
void applySort(StudentDB *db)
{
    if (db->size == 0 || db->sort_mode == 0)
    {
        return; // �����ݻ���Ҫ����
    }

    switch (db->sort_mode)
    {
    case 1: // ѧ������
        qsort(db->data, db->size, sizeof(Student), compareById);
        break;
    case 2: // ѧ�Ž���
        qsort(db->data, db->size, sizeof(Student), compareById);
        // ��ת����ʵ�ֽ���
        for (int i = 0; i < db->size / 2; i++)
        {
            Student temp = db->data[i];
            db->data[i] = db->data[db->size - 1 - i];
            db->data[db->size - 1 - i] = temp;
        }
        break;
    case 3: // ��������
        qsort(db->data, db->size, sizeof(Student), compareByName);
        break;
    case 4: // ��������
        qsort(db->data, db->size, sizeof(Student), compareByName);
        // ��ת����ʵ�ֽ���
        for (int i = 0; i < db->size / 2; i++)
        {
            Student temp = db->data[i];
            db->data[i] = db->data[db->size - 1 - i];
            db->data[db->size - 1 - i] = temp;
        }
        break;
    }
}

// ѧ�űȽϺ���
int compareById(const void *a, const void *b)
{
    Student *studentA = (Student *)a;
    Student *studentB = (Student *)b;
    return (studentA->id - studentB->id);
}

// �����ȽϺ���
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
        fprintf(stderr, "�ڴ����ʧ��\n");
        exit(EXIT_FAILURE);
    }
    db->size = 0;
    db->capacity = INIT_CAPACITY;
    db->sort_mode = 1; // Ĭ�ϰ�ѧ����������
}

void loadFromFile(StudentDB *db)
{
    FILE *file = fopen(FILENAME, "r");
    if (!file)
    {
        printf("���ݿ��ļ������ڣ����������ļ���\n");
        return;
    }

    // ���Զ�ȡ����ģʽ���ļ���һ�У�
    if (fscanf(file, "SortMode: %d\n", &db->sort_mode) != 1)
    {
        // ����ļ�û������ģʽ��ǣ����õ��ļ���ͷ
        rewind(file);
        db->sort_mode = 1; // ʹ��Ĭ������ģʽ
    }

    Student s;
    while (fscanf(file, "%d \"%49[^\"]\"", &s.id, s.name) == 2)
    {
        for (int i = 0; i < NUM_SUBJECTS; i++)
        {
            if (fscanf(file, "%f", &s.scores[i]) != 1)
            {
                printf("�ļ���ʽ���󣬼���ʧ��\n");
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
    printf("�ɹ����� %d ����¼������ģʽ: %d\n", db->size, db->sort_mode);
}

void saveToFile(StudentDB *db)
{
    FILE *file = fopen(FILENAME, "w");
    if (!file)
    {
        fprintf(stderr, "�޷����ļ�����д��\n");
        return;
    }

    // ��������ģʽ���ļ���һ�У�
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
    printf("�����ѱ��浽�ļ�������ģʽ: %d��\n", db->sort_mode);
}

void resizeDB(StudentDB *db, int new_capacity)
{
    Student *new_data = (Student *)realloc(db->data, new_capacity * sizeof(Student));
    if (!new_data)
    {
        fprintf(stderr, "�ڴ����ʧ��\n");
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

    // ����ѧ�ţ���ȡ����
    while (1)
    {
        printf("\n������ѧ�� (����0ȡ��): ");
        if (scanf("%d", &new_student.id) != 1)
        {
            clearInputBuffer();
            printf("������Ч�����������롣\n");
            continue;
        }
        clearInputBuffer();

        if (new_student.id == 0)
        {
            printf("��Ӳ�����ȡ����\n");
            return;
        }

        // ���ѧ���Ƿ��ظ�
        int duplicate = 0;
        for (int i = 0; i < db->size; i++)
        {
            if (db->data[i].id == new_student.id)
            {
                printf("����ѧ�� %d �Ѵ���\n", new_student.id);
                duplicate = 1;
                break;
            }
        }

        if (!duplicate)
            break;
    }

    // ������������ȡ����
    printf("���������� (����0ȡ��): ");
    fgets(new_student.name, MAX_NAME_LEN, stdin);
    new_student.name[strcspn(new_student.name, "\n")] = '\0'; // �Ƴ����з�

    if (strcmp(new_student.name, "0") == 0)
    {
        printf("��Ӳ�����ȡ����\n");
        return;
    }

    // ����ɼ�����ȡ����
    printf("��ʼ������Ƴɼ������븺��ȡ����\n");
    for (int i = 0; i < NUM_SUBJECTS; i++)
    {
        while (1)
        {
            printf("������%s�ĳɼ�: ", SUBJECT_NAMES[i]);
            if (scanf("%f", &new_student.scores[i]) != 1)
            {
                clearInputBuffer();
                printf("�����ʽ��������������!\n");
                continue;
            }
            clearInputBuffer();

            // ��⸺����ȡ���߼���
            if (new_student.scores[i] < 0)
            {
                if (confirmAction("��⵽������ȷ��Ҫȡ�������(y/n): "))
                {
                    printf("��Ӳ�����ȡ����\n");
                    return;
                }
                else
                {
                    // �������뵱ǰ��Ŀ
                    continue;
                }
            }

            // ������Ч���˳���ǰ��Ŀ����ѭ��
            break;
        }
    }
    clearInputBuffer();

    // ȷ�����
    printf("\n�������ѧ����\n");
    printf("ѧ��: %d\n����: %s\n", new_student.id, new_student.name);
    printf("�ɼ�: ");
    for (int i = 0; i < NUM_SUBJECTS; i++)
    {
        printf("%s:%.2f ", SUBJECT_NAMES[i], new_student.scores[i]);
    }
    printf("\n");

    if (!confirmAction("ȷ�������(y/n): "))
    {
        printf("��Ӳ�����ȡ����\n");
        return;
    }

    db->data[db->size++] = new_student;

    // ��Ӻ��Զ�����
    applySort(db);

    saveToFile(db); // �������浽�ļ�
    printf("ѧ����ӳɹ���\n");
}

void deleteStudent(StudentDB *db)
{
    if (db->size == 0)
    {
        printf("���ݿ�Ϊ��\n");
        return;
    }

    int id;
    printf("\n������Ҫɾ����ѧ�� (����0ȡ��): ");
    scanf("%d", &id);
    clearInputBuffer();

    if (id == 0)
    {
        printf("ɾ��������ȡ����\n");
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
        printf("δ�ҵ�ѧ�� %d\n", id);
        return;
    }

    // ��ʾҪɾ����ѧ����Ϣ
    printf("\n�ҵ�ѧ����\n");
    printf("ѧ��: %d\n����: %s\n", db->data[found_index].id, db->data[found_index].name);
    printf("�ɼ�: ");
    for (int i = 0; i < NUM_SUBJECTS; i++)
    {
        printf("%s:%.2f ", SUBJECT_NAMES[i], db->data[found_index].scores[i]);
    }
    printf("\nƽ����: %.2f\n", calculateAverage(&db->data[found_index]));

    if (!confirmAction("ȷ��Ҫɾ������ѧ����(y/n): "))
    {
        printf("ɾ��������ȡ����\n");
        return;
    }

    // �����һ��Ԫ���Ƶ���ǰλ��
    db->data[found_index] = db->data[db->size - 1];
    db->size--;

    // ����ռ������ʵ���25%����С����
    if (db->size > INIT_CAPACITY && db->size < db->capacity / 4)
    {
        resizeDB(db, db->capacity / 2);
    }

    // ɾ�����Զ�����
    applySort(db);

    saveToFile(db);
    printf("ѧ�� %d ��ɾ��\n", id);
}

void findStudent(StudentDB *db)
{
    if (db->size == 0)
    {
        printf("���ݿ�Ϊ��\n");
        return;
    }

    int id;
    printf("\n������Ҫ���ҵ�ѧ�� (����0ȡ��): ");
    scanf("%d", &id);
    clearInputBuffer();

    if (id == 0)
    {
        printf("���Ҳ�����ȡ����\n");
        return;
    }

    for (int i = 0; i < db->size; i++)
    {
        if (db->data[i].id == id)
        {
            printf("\nѧ��: %d\n����: %s\n", db->data[i].id, db->data[i].name);
            for (int j = 0; j < NUM_SUBJECTS; j++)
            {
                printf("%s: %.2f\n", SUBJECT_NAMES[j], db->data[i].scores[j]);
            }
            printf("ƽ����: %.2f\n", calculateAverage(&db->data[i]));
            return;
        }
    }

    printf("δ�ҵ�ѧ�� %d\n", id);
}

void modifyStudent(StudentDB *db)
{
    if (db->size == 0)
    {
        printf("���ݿ�Ϊ��\n");
        return;
    }

    int id;
    printf("\n������Ҫ�޸ĵ�ѧ�� (����0ȡ��): ");
    scanf("%d", &id);
    clearInputBuffer();

    if (id == 0)
    {
        printf("�޸Ĳ�����ȡ����\n");
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
        printf("δ�ҵ�ѧ�� %d\n", id);
        return;
    }

    // ��ʾ��ǰ��Ϣ
    printf("\n��ǰѧ����Ϣ: \n");
    printf("ѧ��: %d\n����: %s\n", db->data[found_index].id, db->data[found_index].name);
    for (int j = 0; j < NUM_SUBJECTS; j++)
    {
        printf("%s: %.2f\n", SUBJECT_NAMES[j], db->data[found_index].scores[j]);
    }
    printf("ƽ����: %.2f\n", calculateAverage(&db->data[found_index]));

    // ����ԭʼ�����Ա�ȡ���޸�
    Student original = db->data[found_index];

    // �޸�����
    printf("\n���������� (ֱ�ӻس�����ԭֵ������0ȡ���޸�): ");
    char new_name[MAX_NAME_LEN];
    fgets(new_name, MAX_NAME_LEN, stdin);
    new_name[strcspn(new_name, "\n")] = '\0';

    if (strcmp(new_name, "0") == 0)
    {
        printf("�޸Ĳ�����ȡ����\n");
        return;
    }
    if (new_name[0] != '\0')
    {
        strncpy(db->data[found_index].name, new_name, MAX_NAME_LEN);
    }

    // �޸ĳɼ�
    printf("�����³ɼ�������Ŀ������룩��\n");
    printf("  - ����-1��ȡ��ȫ���޸�\n");
    printf("  - ���븺������-1�⣩��������ǰ��Ŀ\n");
    printf("  - ����0����ʾ0�֣��Ϸ��ɼ���\n");

    int cancel_all = 0; // �Ƿ�ȡ��ȫ���޸�
    for (int j = 0; j < NUM_SUBJECTS; j++)
    {
        float new_score;
        while (1)
        {
            printf("������%s���³ɼ�: ", SUBJECT_NAMES[j]);
            if (scanf("%f", &new_score) != 1)
            {
                clearInputBuffer();
                printf("�����ʽ�������������룡\n");
                continue;
            }
            clearInputBuffer();

            // ����ȡ��ȫ��
            if (new_score == -1)
            {
                cancel_all = 1;
                break;
            }

            // ������������
            if (new_score < 0 && new_score != -1)
            {
                printf("������%s���޸�\n", SUBJECT_NAMES[j]);
                break;
            }

            // ��֤�ɼ���Χ
            if (new_score < 0 || new_score > 100)
            {
                printf("���棺�ɼ�Ӧ��0-100֮�䣡�Ƿ�ȷ�����룿(y/n): ");
                char confirm;
                scanf(" %c", &confirm);
                clearInputBuffer();
                if (confirm != 'y' && confirm != 'Y')
                {
                    continue; // �������뵱ǰ��Ŀ
                }
            }

            // ��Ч���룬���³ɼ�
            db->data[found_index].scores[j] = new_score;
            break;
        }

        if (cancel_all)
            break; // ȡ��ȫ�����˳�ѭ��
    }

    // ����ȡ��ȫ�������
    if (cancel_all)
    {
        db->data[found_index] = original; // �ָ�ԭʼ����
        printf("�޸Ĳ�����ȡ����\n");
        return;
    }

    // ��ʾ�޸ĺ����Ϣ
    printf("\n�޸ĺ��ѧ����Ϣ: \n");
    printf("ѧ��: %d\n����: %s\n", db->data[found_index].id, db->data[found_index].name);
    for (int j = 0; j < NUM_SUBJECTS; j++)
    {
        printf("%s: %.2f\n", SUBJECT_NAMES[j], db->data[found_index].scores[j]);
    }
    printf("ƽ����: %.2f\n", calculateAverage(&db->data[found_index]));

    // ����ȷ��
    if (!confirmAction("ȷ���޸���(y/n): "))
    {
        db->data[found_index] = original; // �ָ�ԭʼ����
        printf("�޸Ĳ�����ȡ����\n");
        return;
    }

    // �޸ĺ��Զ�����
    applySort(db);

    saveToFile(db);
    printf("��Ϣ���³ɹ���\n");
}

void printAll(StudentDB *db)
{
    if (db->size == 0)
    {
        printf("���ݿ�Ϊ��\n");
        return;
    }

    // ��ʾ��ǰ����ʽ
    printf("\n��ǰ����ʽ: ");
    switch (db->sort_mode)
    {
    case 0:
        printf("�����򣨰����˳��\n");
        break;
    case 1:
        printf("ѧ������\n");
        break;
    case 2:
        printf("ѧ�Ž���\n");
        break;
    case 3:
        printf("��������\n");
        break;
    case 4:
        printf("��������\n");
        break;
    }

    printf("\n%-8s %-20s", "ѧ��", "����");
    for (int i = 0; i < NUM_SUBJECTS; i++)
    {
        printf(" %-8s", SUBJECT_NAMES[i]);
    }
    printf(" %-8s\n", "ƽ����");

    // ��ӡ�ָ���
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
    printf("\n�� %d ����¼\n", db->size);
}

void clearInputBuffer() // �������뻺����
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