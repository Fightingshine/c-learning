#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 定义学生结构体
typedef struct Student
{
    int id;
    char name[50];
    int age;
    struct Student *next; // 指向下一个学生节点的指针
} Student;

// 添加学生信息到链表
Student *addStudent(Student *head, int id, const char *name, int age)
{
    Student *temp = head;
    while (temp != NULL)
    {
        if (temp->id == id)
        {
            printf("ID已存在，无法添加！\n");
            return head;
        }
        temp = temp->next;
    }

    Student *newStudent = (Student *)malloc(sizeof(Student));
    if (newStudent == NULL)
    {
        printf("内存分配失败！\n");
        return head;
    }
    newStudent->id = id;
    strcpy(newStudent->name, name);
    newStudent->age = age;
    newStudent->next = NULL;

    if (head == NULL)
    {
        return newStudent;
    }
    else
    {
        Student *temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newStudent;
    }
    return head;
}



// 显示所有学生信息
void displayStudents(Student *head)
{
    Student *temp = head;
    while (temp != NULL)
    {
        printf("ID: %d, Name: %s, Age: %d\n", temp->id, temp->name, temp->age);
        temp = temp->next;
    }
}

// 根据id查找学生信息
Student *findStudent(Student *head, int id)
{
    Student *temp = head;
    while (temp != NULL)
    {
        if (temp->id == id)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// 根据姓名查找学生。
Student *findStudentByName(Student *head, const char *name)
{
    Student *temp = head;
    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// 更新学生信息
void updateStudent(Student *head, int id, const char *newName, int newAge)
{
    Student *student = findStudent(head, id);
    if (student != NULL)
    {
        strcpy(student->name, newName);
        student->age = newAge;
        printf("更新成功！\n");
    }
    else
    {
        printf("未找到ID为%d的学生！\n", id);
    }
}

// 删除学生信息
Student *deleteStudent(Student *head, int id)
{
    Student *temp = head;
    Student *prev = NULL;

    while (temp != NULL && temp->id != id)
    {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("未找到ID为%d的学生！\n", id);
        return head;
    }

    if (prev == NULL)
    {
        head = temp->next;
    }
    else
    {
        prev->next = temp->next;
    }
    free(temp);
    printf("删除成功！\n");
    return head;
}
// 统计学生年龄平均值
double averageAge(Student *head)
{
    if (head == NULL)
    {
        return 0;
    }
    int totalAge = 0;
    int count = 0;
    Student *temp = head;
    while (temp != NULL)
    {
        totalAge += temp->age;
        count++;
        temp = temp->next;
    }
    return (double)totalAge / count;
}
// 查找ID最小的学生
Student *findMinIdStudent(Student *head)
{
    if (head == NULL)
    {
        return NULL;
    }
    Student *minStudent = head;
    Student *temp = head->next;
    while (temp != NULL)
    {
        if (temp->id < minStudent->id)
        {
            minStudent = temp;
        }
        temp = temp->next;
    }
    return minStudent;
}

// 释放链表的内存
void freeList(Student *head)
{
    Student *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main()
{
    Student *head = NULL;
    int choice, id, age;
    char name[50];

    while (1)
    {
        printf("\n学生管理系统\n");
        printf("1. 添加学生\n");
        printf("2. 显示所有学生\n");
        printf("3. 根据ID查找学生\n");
        printf("4. 根据姓名查找学生\n");
        printf("5. 更新学生信息\n");
        printf("6. 删除学生\n");
        printf("7. 统计学生年龄平均值\n");
        printf("8. 查找ID最小的学生\n");
        printf("9. 退出\n");
        printf("请输入选择: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("输入ID: ");
            scanf("%d", &id);
            printf("输入姓名: ");
            scanf("%s", name);
            printf("输入年龄: ");
            scanf("%d", &age);
            head = addStudent(head, id, name, age);
            break;
        case 2:
            displayStudents(head);
            break;
        case 3:
            printf("输入要查找的学生ID: ");
            scanf("%d", &id);
            Student *found = findStudent(head, id);
            if (found != NULL)
            {
                printf("ID: %d, Name: %s, Age: %d\n", found->id, found->name, found->age);
            }
            else
            {
                printf("未找到该学生！\n");
            }
            break;
        case 4:
            printf("输入要查找的学生姓名: ");
            scanf("%s", name);
            getchar();
            Student *foundbyname = findStudentByName(head, name);
            if (foundbyname != NULL)
            {
                printf("ID: %d, Name: %s, Age: %d\n", foundbyname->id, foundbyname->name, foundbyname->age);
            }
            else
            {
                printf("未找到该学生！\n");
            }
            break;
        case 5:
            printf("输入要更新的学生ID: ");
            scanf("%d", &id);
            printf("输入新的姓名: ");
            scanf("%s", name);
            printf("输入新的年龄: ");
            scanf("%d", &age);
            updateStudent(head, id, name, age);
            break;
        case 6:
            printf("输入要删除的学生ID: ");
            scanf("%d", &id);
            head = deleteStudent(head, id);
            break;
        case 7:
            printf("%f",averageAge(head));
            break;
        case 8:{
            Student *minIDstudent = findMinIdStudent(head);
            if (minIDstudent != NULL)
            {
                printf("ID: %d, Name: %s, Age: %d\n", minIDstudent->id, minIDstudent->name, minIDstudent->age);
            }
            else
            {
                printf("学生列表为空\n");
            }
            break;
            }
        case 9:
            freeList(head);
            printf("退出系统。\n");
            return 0;
        default:
            printf("无效选择，请重新输入！\n");
        }
    }
}
