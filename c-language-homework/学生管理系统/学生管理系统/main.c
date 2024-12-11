#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

//�˺ź�����ṹ��
typedef struct account
{
    char Acnt_name[100];
    char Acnt_PIN[100];
}account;

//�������˻�������
typedef struct accountNode{
    account acnt;
    struct accountNode* next;
}accountNode;

//����˺ź����뵽����
accountNode* addaccount(accountNode* head, const char* account_name,const char* account_PIN) {
    accountNode* newAccount = (accountNode*)malloc(sizeof(accountNode));
    if (newAccount == NULL) {
        printf("�ڴ����ʧ�ܣ�\n");
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
//��֤�˺������Ƿ���ȷ
int loginAccount(accountNode* head, const char* account_name, const char* account_PIN) {
    accountNode* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->acnt.Acnt_name, account_name) == 0 && strcmp(temp->acnt.Acnt_PIN, account_PIN) == 0) {
            return 1; // ��¼�ɹ�
        }
        temp = temp->next;
    }
    return 0; // ��¼ʧ��
}
//�����˺����뵽�ļ���
void saveAccountsToFile(accountNode* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("�޷����ļ�����д�롣\n");
        return;
    }
    accountNode* current = head;
    while (current != NULL) {
        fprintf(file, "%s %s\n", current->acnt.Acnt_name, current->acnt.Acnt_PIN);
        current = current->next;
    }
    fclose(file);
}

// ���ļ���ȡ�˺�����
accountNode* loadAccountsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("�޷����ļ����ж�ȡ��\n");
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

// ����ѧ���ṹ��
typedef struct Student
{
    int id;
    char name[50];
    int age;
    struct Student* next; // ָ����һ��ѧ���ڵ��ָ��
} Student;

// ���ѧ����Ϣ������
Student* addStudent(Student* head, int id, const char* name, int age)
{
    Student* temp = head;
    while (temp != NULL)
    {
        if (temp->id == id)
        {
            printf("ID�Ѵ��ڣ��޷���ӣ�\n");
            return head;
        }
        temp = temp->next;
    }

    Student* newStudent = (Student*)malloc(sizeof(Student));
    if (newStudent == NULL)
    {
        printf("�ڴ����ʧ�ܣ�\n");
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

// ����ѧ����Ϣ���ļ�
void saveStudentsToFile(Student* head, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("�޷����ļ�����д�롣\n");
        return;
    }
    Student* current = head;
    while (current != NULL) {
        fprintf(file, "%d %s %d\n", current->id, current->name, current->age);
        current = current->next;
    }
    fclose(file);
}

// ���ļ���ȡѧ����Ϣ
Student* loadStudentsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("�޷����ļ����ж�ȡ��\n");
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


// ��ʾ����ѧ����Ϣ
void displayStudents(Student* head)
{
    Student* temp = head;
    while (temp != NULL)
    {
        printf("ID: %d, Name: %s, Age: %d\n", temp->id, temp->name, temp->age);
        temp = temp->next;
    }
}

// ����id����ѧ����Ϣ
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

// ������������ѧ����
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

// ����ѧ����Ϣ
void updateStudent(Student* head, int id, const char* newName, int newAge)
{
    Student* student = findStudent(head, id);
    if (student != NULL)
    {
        strcpy(student->name, newName);
        student->age = newAge;
        printf("���³ɹ���\n");
    }
    else
    {
        printf("δ�ҵ�IDΪ%d��ѧ����\n", id);
    }
}

// ɾ��ѧ����Ϣ
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
        printf("δ�ҵ�IDΪ%d��ѧ����\n", id);
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
    printf("ɾ���ɹ���\n");
    return head;
}
// ͳ��ѧ������ƽ��ֵ
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
// ����ID��С��ѧ��
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
// ��������ѧ���ڵ������
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

// �������ѧ������ʹ��ð�������㷨��
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

// ��ID��ѧ������ʹ��ð�������㷨��
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


// �ͷ�������ڴ�
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
//����
void clearwindow() {
    printf("��Enter������...\n");
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
        printf("��ӭ����ѡ��\n");
        printf("1.�����˺�\n");
        printf("2.�����˺ŵ�¼\n");
        scanf("%d", &choice1);
        switch (choice1)
        {
        case 1: {
            printf("�˺�����");
            scanf("%49s", account_name);
            printf("���룺");
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
            printf("�˺���ӳɹ���\n");
            clearwindow();
            break;
        }
        case 2: {
            printf("�˺�����");
            scanf("%49s", account_name);
            printf("���룺"); for (int i = 0;; i++)
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
                printf("��¼�ɹ�����ӭ����ѧ������ϵͳ��\n");
                clearwindow();
                while (1) {
                    printf("\nѧ������ϵͳ\n");
                    printf("1. ���ѧ��\n");
                    printf("2. ��ʾ����ѧ��\n");
                    printf("3. ����ID����ѧ��\n");
                    printf("4. ������������ѧ��\n");
                    printf("5. ����ѧ����Ϣ\n");
                    printf("6. ɾ��ѧ��\n");
                    printf("7. ͳ��ѧ������ƽ��ֵ\n");
                    printf("8. ����ID��С��ѧ��\n");
                    printf("9. �������ѧ������\n");
                    printf("10. ��ID��ѧ������\n");
                    printf("11. �˳�\n");
                    printf("������ѡ��: ");
                    scanf("%d", &choice2);

                    switch (choice2)
                    {
                    case 1:
                        printf("����ID: ");
                        scanf("%d", &id);
                        printf("��������: ");
                        scanf("%s", name);
                        printf("��������: ");
                        scanf("%d", &age);
                        head = addStudent(head, id, name, age);
                        clearwindow();
                        break;
                    case 2:
                        displayStudents(head);
                        clearwindow();
                        break;
                    case 3:
                        printf("����Ҫ���ҵ�ѧ��ID: ");
                        scanf("%d", &id);
                        Student* found = findStudent(head, id);
                        if (found != NULL)
                        {
                            printf("ID: %d, Name: %s, Age: %d\n", found->id, found->name, found->age);
                        }
                        else
                        {
                            printf("δ�ҵ���ѧ����\n");
                        }
                        clearwindow();
                        break;
                    case 4: {
                        printf("����Ҫ���ҵ�ѧ��������֧��ģ��������: ");
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
                            printf("δ�ҵ��õ�ѧ����\n");
                        }
                        clearwindow();
                        break;
                    }
                    case 5:
                        printf("����Ҫ���µ�ѧ��ID: ");
                        scanf("%d", &id);
                        printf("�����µ�����: ");
                        scanf("%s", name);
                        printf("�����µ�����: ");
                        scanf("%d", &age);
                        updateStudent(head, id, name, age);
                        clearwindow();
                        break;
                    case 6:
                        printf("����Ҫɾ����ѧ��ID: ");
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
                            printf("ѧ���б�Ϊ��\n");
                        }
                        clearwindow();
                        break;
                    }
                    case 9:
                        sortStudentsByAge(head);
                        printf("������������ɡ�\n");
                        clearwindow();
                        break;
                    case 10:
                        sortStudentsById(head);
                        printf("��ID������ɡ�\n");
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
                        printf("�˳�ϵͳ��\n");
                        clearwindow();
                        return 0; }
                    default:
                        printf("��Чѡ�����������룡\n");
                    }
                }
            }
            else {
                printf("�˺Ż�������󣬵�¼ʧ�ܣ�\n");
                clearwindow();
            }
            break; }
        default:
            printf("��Чѡ�����������룡\n");
            clearwindow();
            break;
        }
    }
    saveAccountsToFile(accounthead, accountFilename);
    saveStudentsToFile(head, studentFilename);
}
