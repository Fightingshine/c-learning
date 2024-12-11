#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

//账号和密码结构体
typedef struct account
{
    char Acnt_name[100];
    char Acnt_PIN[100];
}account;

//链表存放账户和密码
typedef struct accountNode{
    account acnt;
    struct accountNode* next;
}accountNode;

//添加账号和密码到链表
accountNode* addaccount(accountNode* head, const char* account_name,const char* account_PIN) {
    accountNode* newAccount = (accountNode*)malloc(sizeof(accountNode));
    if (newAccount == NULL) {
        printf("内存分配失败！\n");
        return head;
    }
    strcpy(newAccount->acnt.Acnt_name, account_name);
    strcpy(newAccount->acnt.Acnt_PIN, account_PIN);
    newAccount->next = NULL;
    if (head == NULL) {
        return newAccount;
    }
    else
    {
        accountNode* temp = head;
        while (temp->next!=NULL)
        {
            temp = temp->next;
        }
        temp->next = newAccount;
    }if (head != NULL && head->next == NULL) {
        free(head);
    }
    return head;
}
//验证账号密码是否正确
int loginAccount(accountNode* head, const char* account_name, const char* account_PIN) {
    accountNode* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->acnt.Acnt_name, account_name) == 0 && strcmp(temp->acnt.Acnt_PIN, account_PIN) == 0) {
            return 1; // 登录成功
        }
        temp = temp->next;
    }
    return 0; // 登录失败
}
//保存账号密码到文件中
void saveAccountsToFile(accountNode* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法打开文件进行写入。\n");
        return;
    }
    accountNode* current = head;
    while (current != NULL) {
        fprintf(file, "%s %s\n", current->acnt.Acnt_name, current->acnt.Acnt_PIN);
        current = current->next;
    }
    fclose(file);
}

// 从文件读取账号密码
accountNode* loadAccountsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件进行读取。\n");
        return NULL;
    }
    accountNode* head = NULL;
    char account_name[100];
    char account_PIN[100];
    while (fscanf(file, "%99s %99s", account_name, account_PIN) == 2) {
        head = addaccount(head, account_name, account_PIN);
    }
    fclose(file);
    return head;
}

// 定义学生结构体
typedef struct Student
{
    int id;
    char name[50];
    int age;
    struct Student* next; // 指向下一个学生节点的指针
} Student;

// 添加学生信息到链表
Student* addStudent(Student* head, int id, const char* name, int age)
{
    Student* temp = head;
    while (temp != NULL)
    {
        if (temp->id == id)
        {
            printf("ID已存在，无法添加！\n");
            return head;
        }
        temp = temp->next;
    }

    Student* newStudent = (Student*)malloc(sizeof(Student));
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
        Student* temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newStudent;
    }
    return head;
}

// 保存学生信息到文件
void saveStudentsToFile(Student* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("无法打开文件进行写入。\n");
        return;
    }
    Student* current = head;
    while (current != NULL) {
        fprintf(file, "%d %s %d\n", current->id, current->name, current->age);
        current = current->next;
    }
    fclose(file);
}

// 从文件读取学生信息
Student* loadStudentsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("无法打开文件进行读取。\n");
        return NULL;
    }
    Student* head = NULL;
    int id;
    char name[50];
    int age;
    while (fscanf(file, "%d %49s %d", &id, name, &age) == 3) {
        head = addStudent(head, id, name, age);
    }
    fclose(file);
    return head;
}


// 显示所有学生信息
void displayStudents(Student* head)
{
    Student* temp = head;
    while (temp != NULL)
    {
        printf("ID: %d, Name: %s, Age: %d\n", temp->id, temp->name, temp->age);
        temp = temp->next;
    }
}

// 根据id查找学生信息
Student* findStudent(Student* head, int id)
{
    Student* temp = head;
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
Student* findStudentByName(Student* head, const char* name)
{
    Student* temp = head;
    while (temp != NULL)
    {
        if (strstr(temp->name, name) != NULL)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// 更新学生信息
void updateStudent(Student* head, int id, const char* newName, int newAge)
{
    Student* student = findStudent(head, id);
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
Student* deleteStudent(Student* head, int id)
{
    Student* temp = head;
    Student* prev = NULL;

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
double averageAge(Student* head)
{
    if (head == NULL)
    {
        return 0;
    }
    int totalAge = 0;
    int count = 0;
    Student* temp = head;
    while (temp != NULL)
    {
        totalAge += temp->age;
        count++;
        temp = temp->next;
    }
    return (double)totalAge / count;
}
// 查找ID最小的学生
Student* findMinIdStudent(Student* head)
{
    if (head == NULL)
    {
        return NULL;
    }
    Student* minStudent = head;
    Student* temp = head->next;
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
// 交换两个学生节点的数据
void swapStudents(Student* a, Student* b) {
    int tempId = a->id;
    a->id = b->id;
    b->id = tempId;

    char tempName[50];
    strcpy(tempName, a->name);
    strcpy(a->name, b->name);
    strcpy(b->name, tempName);

    int tempAge = a->age;
    a->age = b->age;
    b->age = tempAge;
}

// 按年龄给学生排序（使用冒泡排序算法）
void sortStudentsByAge(Student* head) {
    if (head == NULL) return;

    int swapped;
    Student* current;
    Student* last = NULL;

    do {
        swapped = 0;
        current = head;

        while (current->next != last) {
            if (current->age > current->next->age) {
                swapStudents(current, current->next);
                swapped = 1;
            }
            current = current->next;
        }
        last = current;
    } while (swapped);
}

// 按ID给学生排序（使用冒泡排序算法）
void sortStudentsById(Student* head) {
    if (head == NULL) return;

    int swapped;
    Student* current;
    Student* last = NULL;

    do {
        swapped = 0;
        current = head;

        while (current->next != last) {
            if (current->id > current->next->id) {
                swapStudents(current, current->next);
                swapped = 1;
            }
            current = current->next;
        }
        last = current;
    } while (swapped);
}


// 释放链表的内存
void freeList(Student* head)
{
    Student* temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}
//清屏
void clearwindow() {
    printf("按Enter键继续...\n");
    getchar();
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    system("cls");
}

int main()
{
    const char* accountFilename = "accounts.txt";
    const char* studentFilename = "students.txt";
    accountNode* accounthead = loadAccountsFromFile(accountFilename);
    Student* head = loadStudentsFromFile(studentFilename);    
    int choice1,choice2,id, age,secret;
    char name[50];
    char account_name[50];
    char account_PIN[50];
    while (1) {
        printf("欢迎！请选择：\n");
        printf("1.新增账号\n");
        printf("2.已有账号登录\n");
        scanf("%d", &choice1);
        switch (choice1)
        {
        case 1: {
            printf("账号名：");
            scanf("%49s", account_name);
            printf("密码：");
            for (int i = 0;; i++)
            {
                secret = getch();
                if (secret == '\r') { account_PIN[i] = '\0'; printf("\n"); break; }
                if (secret == '\b') { 
                    if (i > 0) { 
                        i--; printf("\b \b");
                    }
                    continue; 
                }
                if (secret == VK_LEFT) {
					continue;
                  
                }
                account_PIN[i] = secret;
                printf("*");
            }
            accounthead = addaccount(accounthead, account_name, account_PIN);
            printf("账号添加成功！\n");
            clearwindow();
            break;
        }
        case 2: {
            printf("账号名：");
            scanf("%49s", account_name);
            printf("密码："); for (int i = 0;; i++)
            {
                secret = getch();
                if (secret == '\r') { account_PIN[i] = '\0'; printf("\n"); break; }
                if (secret == '\b') {
                    if (i > 0) {
                        i--; printf("\b \b");
                    }
                    continue;
                }
                account_PIN[i] = secret;
                printf("*");
            }
            if (loginAccount(accounthead, account_name, account_PIN)) {
                printf("登录成功，欢迎进入学生管理系统！\n");
                clearwindow();
                while (1) {
                    printf("\n学生管理系统\n");
                    printf("1. 添加学生\n");
                    printf("2. 显示所有学生\n");
                    printf("3. 根据ID查找学生\n");
                    printf("4. 根据姓名查找学生\n");
                    printf("5. 更新学生信息\n");
                    printf("6. 删除学生\n");
                    printf("7. 统计学生年龄平均值\n");
                    printf("8. 查找ID最小的学生\n");
                    printf("9. 按年龄给学生排序\n");
                    printf("10. 按ID给学生排序\n");
                    printf("11. 退出\n");
                    printf("请输入选择: ");
                    scanf("%d", &choice2);

                    switch (choice2)
                    {
                    case 1:
                        printf("输入ID: ");
                        scanf("%d", &id);
                        printf("输入姓名: ");
                        scanf("%s", name);
                        printf("输入年龄: ");
                        scanf("%d", &age);
                        head = addStudent(head, id, name, age);
                        clearwindow();
                        break;
                    case 2:
                        displayStudents(head);
                        clearwindow();
                        break;
                    case 3:
                        printf("输入要查找的学生ID: ");
                        scanf("%d", &id);
                        Student* found = findStudent(head, id);
                        if (found != NULL)
                        {
                            printf("ID: %d, Name: %s, Age: %d\n", found->id, found->name, found->age);
                        }
                        else
                        {
                            printf("未找到该学生！\n");
                        }
                        clearwindow();
                        break;
                    case 4: {
                        printf("输入要查找的学生姓名（支持模糊搜索）: ");
                        scanf("%49s", name);
                        Student* current = head;
                        int found = 0;
                        while (current != NULL) {
                            if (strstr(current->name, name) != NULL) {
                                printf("ID: %d, Name: %s, Age: %d\n", current->id, current->name, current->age);
                                found = 1;
                            }
                            current = current->next;
                        }
                        if (!found) {
                            printf("未找到该的学生！\n");
                        }
                        clearwindow();
                        break;
                    }
                    case 5:
                        printf("输入要更新的学生ID: ");
                        scanf("%d", &id);
                        printf("输入新的姓名: ");
                        scanf("%s", name);
                        printf("输入新的年龄: ");
                        scanf("%d", &age);
                        updateStudent(head, id, name, age);
                        clearwindow();
                        break;
                    case 6:
                        printf("输入要删除的学生ID: ");
                        scanf("%d", &id);
                        head = deleteStudent(head, id);
                        clearwindow();
                        break;
                    case 7:
                        printf("%f", averageAge(head));
                        clearwindow();
                        break;
                    case 8: {
                        Student* minIDstudent = findMinIdStudent(head);
                        if (minIDstudent != NULL)
                        {
                            printf("ID: %d, Name: %s, Age: %d\n", minIDstudent->id, minIDstudent->name, minIDstudent->age);
                        }
                        else
                        {
                            printf("学生列表为空\n");
                        }
                        clearwindow();
                        break;
                    }
                    case 9:
                        sortStudentsByAge(head);
                        printf("按年龄排序完成。\n");
                        clearwindow();
                        break;
                    case 10:
                        sortStudentsById(head);
                        printf("按ID排序完成。\n");
                        clearwindow();
                        break;
                    case 11: {
                        saveAccountsToFile(accounthead, accountFilename);
                        saveStudentsToFile(head, studentFilename);
                        freeList(head);
                        accountNode* tempAccount;
                        while (accounthead != NULL) {
                            tempAccount = accounthead;
                            accounthead = accounthead->next;
                            free(tempAccount);
                        }
                        printf("退出系统。\n");
                        clearwindow();
                        return 0; }
                    default:
                        printf("无效选择，请重新输入！\n");
                    }
                }
            }
            else {
                printf("账号或密码错误，登录失败！\n");
                clearwindow();
            }
            break; }
        default:
            printf("无效选择，请重新输入！\n");
            clearwindow();
            break;
        }
    }
    saveAccountsToFile(accounthead, accountFilename);
    saveStudentsToFile(head, studentFilename);
}
