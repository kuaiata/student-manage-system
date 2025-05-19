#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#define STU_NUM 50      // ��ѧ��ѧ����������
#define COURSE_NUM 10   // һѧ�ڵĿγ�����
#define NAME_LEN 10     // ѧ��������󳤶�

// ��ɫö��
typedef enum { TEACHER, STUDENT } Role;

// �û��ṹ��
typedef struct user {
    char username[20];
    char password[20];
    Role role; // �û���ɫ
} User;

User currentUser;  // ��ǰ�û�

// ѧ���ṹ�����Ͷ���
typedef struct student {
    long num;                     // ѧ��
    char name[NAME_LEN];          // ����
    float score[COURSE_NUM];      // ÿ�ſγ̳ɼ�
    float sum;                    // �ܷ�
    float aver;                   // ƽ����
    char message[200];            // ��ʦ����
} STU;

// ����ڵ㶨��
typedef struct StudentNode {
    STU student;                  // ѧ����Ϣ
    struct StudentNode* next;     // ָ����һ��ѧ����ָ��
} StudentNode;

// �Զ��庯������
int Login();                                                                // ��¼
int Menu(void);                                                              // �����˵�
void SetPosition(int x, int y);                                               // ������������ڿ���̨�����е���ʼλ��
void ReturnToLogin();														//���ص�½ҳ�� 
void InputRecord(StudentNode** head, int* n, int* m);                         // ����ѧ����Ϣ
void AppendRecord(StudentNode** head, int m);                                 // ����ѧ����Ϣ
void DeleteRecord(StudentNode** head, int m);                                 // ɾ��ѧ����Ϣ
void SearchByNumber(StudentNode* head, int m);                                // ��ѧ�Ų�ѯѧ����Ϣ
void SearchByName(StudentNode* head, int m);                                  // ��������ѯѧ����Ϣ
void ModifyRecord(StudentNode* head, int m);                                  // �޸�ѧ����Ϣ
void CalculateScoreOfStudent(int n, int m, StudentNode* head);               // ����ѧ�����ֺܷ�ƽ����
void CalculateScoreOfCourse(int n, int m, StudentNode* head);                // ����γ̵��ֺܷ�ƽ����
void SortbyNum(StudentNode* head);                                           // ��ѧ������
void SortbyName(StudentNode* head);                                          // ����������
void SortbyScore(StudentNode* head, int (*compare)(float a, float b));      // ��ÿ��ѧ��ƽ���ֽ�������
int Ascending(float a, float b);                                             // ��������
int Descending(float a, float b);                                            // ��������
void StatisticAnalysis(StudentNode* head, int m);                            // ͳ�Ʋ��������������ѧ��������ռ��
void PrintRecord(StudentNode* head, int m);                                   // ��ӡѧ���ɼ�
void WritetoFile(StudentNode* head, int n, int m);                           // ��ѧ����Ϣ�������ļ�student.txt��
int ReadfromFile(StudentNode** head, int* n, int* m, int* first);           // ��ѧ����Ϣ���ļ�student.txt�ж�������������
void FreeList(StudentNode* head);                                            // �ͷ������ڴ�
void ViewOwnRecord(StudentNode* head, int m);                                // ѧ���鿴�Լ��ĳɼ�
void ReadDataFromDisk(StudentNode** head, int* n, int* m); 					//ѧ��ѡ���ȡ��������
void SetColor(int color);													//��ɫ 
void TeacherLeaveMessage(StudentNode* head);								//��ʦ���� 
void StudentViewMessage(StudentNode* head);									//ѧ���鿴��ʦ���� 


// ѧ���鿴���Թ���
void StudentViewMessage(StudentNode* head) {
    long student_id;
    printf("������ѧ�Ų鿴���ԣ�");
    scanf("%ld", &student_id);

    // ���Ҹ�ѧ��
    StudentNode* temp = head;
    while (temp != NULL) {
        if (temp->student.num == student_id) {
            if (strlen(temp->student.message) > 0) {
                printf("��ʦ���ԣ�\n%s\n", temp->student.message);
            } else {
                printf("��û���յ���ʦ�����ԡ�\n");
            }
            return;
        }
        temp = temp->next;
    }

    printf("δ�ҵ���ѧ���ļ�¼��\n");
}

// ��ʦ���Թ���
void TeacherLeaveMessage(StudentNode* head) {
    long student_id;
    printf("������ѧ��ѧ�ţ�");
    scanf("%ld", &student_id);

    // ���Ҹ�ѧ��
    StudentNode* temp = head;
    while (temp != NULL) {
        if (temp->student.num == student_id) {
            printf("�������������ݣ�\n");
            getchar();  // �����з�
            fgets(temp->student.message, 200, stdin);  // ������������
            // ȥ������ĩβ�Ļ��з�
            size_t len = strlen(temp->student.message);
            if (len > 0 && temp->student.message[len - 1] == '\n') {
                temp->student.message[len - 1] = '\0';
            }

            printf("�����ѳɹ����棡\n");
            return;
        }
        temp = temp->next;
    }

    printf("δ�ҵ���ѧ���ļ�¼��\n");
}

int main() {
    int n = 0;
    int m = 0;
    int first = 1;
    int ch;
    StudentNode* head = NULL; // ��ʼ������ͷ�ڵ�Ϊ��

    system("mode con cols=130 lines=60");
    system("color 0E");
	SetColor(14);
    // ��¼
    if (!Login()) {
        return 0;  // ��¼ʧ�ܣ��˳�����
    }

    while (1) {
        system("cls");
        SetPosition(0, 0);  // ������λ�õ���һ��
        fflush(stdout);  // ˢ�������������ȷ�����������ʾ
        ch = Menu();
        switch (ch) {
        case 1:
            if (currentUser.role == TEACHER) {
                system("cls");
                InputRecord(&head, &n, &m);
                WritetoFile(head, n, m);  // �������ݵ��ļ�
                first = 0;
            } else if (currentUser.role == STUDENT) {
            	system("cls");
				SetPosition(0, 0);  // ȷ��ѧ����¼���һ��������ȷ��ʾ
                ViewOwnRecord(head, m);
            }
            system("pause");
            break;
        case 2:
            if (currentUser.role == TEACHER) {
                system("cls");
                AppendRecord(&head, m);
            }else if (currentUser.role == STUDENT) {
                system("cls");
                ReadDataFromDisk(&head, &n, &m);  // ѧ��ѡ���ȡ��������
            }
            system("pause");
            break;
        case 3:
            if (currentUser.role == TEACHER) {
                system("cls");
                DeleteRecord(&head, m);
            }else if (currentUser.role == STUDENT) {
                system("cls");
                StudentViewMessage(head);  // ѧ���鿴���Թ���
            } 
            system("pause");
            break;
        case 4:
            if (currentUser.role == TEACHER) {
                system("cls");
                SearchByNumber(head, m);
            }
            system("pause");
            break;
        case 5:
            if (currentUser.role == TEACHER) {
                system("cls");
                SearchByName(head, m);
            }
            system("pause");
            break;
        case 6:
            if (currentUser.role == TEACHER) {
                system("cls");
                ModifyRecord(head, m);
            }
            system("pause");
            break;
        case 7:
            if (currentUser.role == TEACHER) {
                system("cls");
                CalculateScoreOfStudent(n, m, head);
            }
            system("pause");
            break;
        case 8:
            if (currentUser.role == TEACHER) {
                system("cls");
                CalculateScoreOfCourse(n, m, head);
            }
            system("pause");
            break;
        case 9:
            if (currentUser.role == TEACHER) {
                system("cls");
                SortbyNum(head);
            }
            system("pause");
            break;
        case 10:
            if (currentUser.role == TEACHER) {
                system("cls");
                SortbyName(head);
            }
            system("pause");
            break;
        case 11:
            if (currentUser.role == TEACHER) {
                system("cls");
                SortbyScore(head, Descending);
            }
            system("pause");
            break;
        case 12:
            if (currentUser.role == TEACHER) {
                system("cls");
                SortbyScore(head, Ascending);
            }
            system("pause");
            break;
        case 13:
            if (currentUser.role == TEACHER) {
                system("cls");
                StatisticAnalysis(head, m);
            }
            system("pause");
            break;
        case 14:
            if (currentUser.role == TEACHER) {
                system("cls");
                PrintRecord(head, m);
            }
            system("pause");
            break;
        case 15:
            if (currentUser.role == TEACHER) {
                system("cls");
                WritetoFile(head, n, m);
            }
            system("pause");
            break;
        case 16:
            if (currentUser.role == TEACHER) {
                system("cls");
                if (ReadfromFile(&head, &n, &m, &first) || first) {
                	SetColor(12);
                    printf("��δ����ѧ����Ϣ���ļ���ʧ��,���ȼ�飡\n");
                    SetColor(14);
                    system("pause");
                    break;
                }
            }
            system("pause");
            break;
        case 17:  // ��ʦ����ѡ��
            if (currentUser.role == TEACHER) {
                system("cls");
                TeacherLeaveMessage(head);  // ��ʦ���Թ���
            }
            system("pause");
            break;
        case 0:
            system("cls");
            printf("�˳������ص�¼���棡\n");
            FreeList(head);  // �ͷ������ڴ�
            SetPosition(0, 0);  // ������λ�õ���һ��
    		fflush(stdout);  // ˢ�������������ȷ�����������ʾ
            ReturnToLogin(); // ���ص�¼����
            exit(0);
        default:
            system("cls");
            SetColor(12);
            printf("�������������ѡ�������\n");
            SetColor(14);
            system("pause");
        }
    }

    return 0;
}

// �û���¼
int Login() {
	system("cls");  // ����
    SetPosition(0, 0);  // ������λ�õ���һ��
    fflush(stdout);  // ˢ�������������ȷ�����������ʾ
    char username[20], password[20];
    printf("�������û���: ");
    scanf("%s", username);
    printf("����������: ");
    scanf("%s", password);
    if (strcmp(username, "teacher") == 0 && strcmp(password, "teacher") == 0) {
        currentUser.role = TEACHER;
        strcpy(currentUser.username, username);
        return 1;
    } else if (strcmp(username, "student") == 0 && strcmp(password, "student") == 0) {
        currentUser.role = STUDENT;
        strcpy(currentUser.username, username);
        return 1;
    } else {
    	SetColor(12);
        printf("�û������������\n");
        SetColor(14);
        return 0;
    }
}

// ��ʾ�˵�
int Menu(void) {
    int posy = 5;
    int option;
    SetPosition(35, posy);
    printf("ѧ���ɼ�����ϵͳ\n");

    if (currentUser.role == TEACHER) {
    	SetColor(11);
	    for (int i = 0; i < 2; i++) {
	        SetPosition(35, ++posy);
	        for (int j = 0; j < 55; j++) {
	            printf("-");
	        }
	    }
	    SetColor(14);
	    SetPosition(35, ++posy);
	    printf("1. ����ѧ����Ϣ");
	    SetPosition(70, posy);
	    printf("2. ����ѧ���ɼ�");
	    SetPosition(35, posy += 2);
	    printf("3. ɾ��ѧ����Ϣ");
	    SetPosition(70, posy);
	    printf("4. ��ѧ�Ų��Ҽ�¼");
	    SetPosition(35, posy += 2);
	    printf("5. ���������Ҽ�¼");
	    SetPosition(70, posy);
	    printf("6. �޸�ѧ����Ϣ");
	    SetPosition(35, posy += 2);
	    printf("7. ����ѧ���ɼ�");
	    SetPosition(70, posy);
	    printf("8. ����γ̳ɼ�");
	    SetPosition(35, posy += 2);
	    printf("9. ��ѧ������");
	    SetPosition(70, posy);
	    printf("10. ����������");
	    SetPosition(35, posy += 2);
	    printf("11. ���ܳɼ���������");
	    SetPosition(70, posy);
	    printf("12. ���ܳɼ���������");
	    SetPosition(35, posy += 2);
	    printf("13. ѧ���ɼ�ͳ��");
	    SetPosition(70, posy);
	    printf("14. ��ӡѧ����¼");
	    SetPosition(35, posy += 2);
	    printf("15. ѧ����¼����");
	    SetPosition(70, posy);
	    printf("16. �Ӵ��̶�ȡѧ����¼");
	    SetPosition(35, posy += 2);
	    printf("17. ��ѧ������");
	    SetPosition(70, posy);
	    printf("0. �˳������ص�¼����");
	    SetColor(11);
	    for (int i = 0; i < 2; i++) {
	        SetPosition(35, ++posy);
	        for (int j = 0; j < 55; j++) {
	            printf("-");
	        }
	    }
	    SetColor(14);
	    SetPosition(35, ++posy);
	    printf("��ѡ������Ҫ���еĲ���[0~17]: [  ]\b\b\b");
    } else if (currentUser.role == STUDENT) {
    	SetColor(11);
        for (int i = 0; i < 2; i++) {
	        SetPosition(35, ++posy);
	        for (int j = 0; j < 55; j++) {
	            printf("-");
	        }
	    }
	    SetColor(14);
	    SetPosition(35, ++posy);
	    printf("1. �鿴���˳ɼ�");
	    SetPosition(70, posy);
	    printf("2. �Ӵ��̶�ȡ�ɼ�����");
	    SetPosition(35, posy += 2);
	    printf("3. �鿴��ʦ����");
		SetPosition(70, posy);
	    printf("0. �˳������ص�¼����");
	    SetColor(11);
	    for (int i = 0; i < 2; i++) {
	        SetPosition(35, ++posy);
	        for (int j = 0; j < 55; j++) {
	            printf("-");
	        }
	    }
	    SetColor(14);
	    SetPosition(35, ++posy);
	    printf("��ѡ������Ҫ���еĲ���[0~3]: [  ]\b\b\b");
    }
    scanf("%d", &option);
    return option;
}
void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
// ���ÿ���̨���λ��
void SetPosition(int x, int y) {
    HANDLE hOut;
    COORD pos;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hOut, pos);
}

// ѧ���鿴�Լ��ĳɼ�
void ViewOwnRecord(StudentNode* head, int m) {
    if (currentUser.role != STUDENT) {
    	SetColor(12);
        printf("��û��Ȩ�޲鿴�ɼ���\n");
        SetColor(14);
        return;
    }

    long id;
    printf("����������ѧ�Ų鿴�ɼ�: ");
    scanf("%ld", &id);

    StudentNode* temp = head;
    int found = 0;
     while (temp != NULL) {
        // ֻ�ڵ�һ���ҵ�ʱ��ӡ��Ϣ
        if (temp->student.num == id) {
            if (!found) {
                printf("�ҵ��ˣ���ѧ�Ŷ�Ӧ��ѧ����ϢΪ��\n");
                printf("%-10ld%-15s", temp->student.num, temp->student.name);

                // ֻ��ӡ��Ч�ĳɼ�
                for (int j = 0; j < m; j++) {
                    if (temp->student.score[j] >= 0 && temp->student.score[j] <= 100) {
                        printf("%10.2f", temp->student.score[j]);
                    }
                }

                // ��ӡ�ֺܷ�ƽ����
                printf("%10.2f%10.2f\n", temp->student.sum, temp->student.aver);
                found = 1;  // ���Ϊ���ҵ�
            }
        }
        temp = temp->next;
    }

    if (!found) {
        printf("û���ҵ����ѧ�Ŷ�Ӧ��ѧ����¼\n");
    }
}

// �ͷ������ڴ�
void FreeList(StudentNode* head) {
    StudentNode* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
// �˳������ص�¼����
void ReturnToLogin() {
    char choice;
    printf("\n������ɣ����Ƿ�Ҫ���ص�¼����? (Y/N): ");
    scanf(" %c", &choice);  // ע��ǰ��Ŀո���������������еĻ��з�

    if (choice == 'Y' || choice == 'y') {
        printf("\n���ڷ��ص�¼����...\n");
        system("cls");  // ����
        main();         // ���µ���main����ʵ�ַ��ص�¼����
    } else {
        printf("\n����������...\n");
    }
}
// ����ѧ����Ϣ
void InputRecord(StudentNode** head, int* n, int* m) {
    int i, j;
    StudentNode* new_node;
    StudentNode* temp;

    SetPosition(35, 6);              // ���ù��λ��
    printf("����ѧ������(n<%d):", STU_NUM);
    scanf("%d", n);
    SetPosition(35, 8);
    printf("����γ���(m<%d):", COURSE_NUM);
    scanf("%d", m);

    for (i = 0; i < *n; i++) {
        new_node = (StudentNode*)malloc(sizeof(StudentNode));
        SetPosition(35, 10 + i);
        printf("�����%d��ѧ����Ϣ:\t", i + 1);
        scanf("%ld%s", &new_node->student.num, new_node->student.name);
        for (j = 0; j < *m; j++) {
            scanf("%f", &new_node->student.score[j]);
        }

        new_node->next = NULL;

        // ���½ڵ���뵽�����ĩβ
        if (*head == NULL) {
            *head = new_node;
        } else {
            temp = *head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = new_node;
        }
    }
    // ���¼��󷵻ص�¼����
    ReturnToLogin();  // ���÷��ص�¼����ĺ���
}

// ����ѧ����¼
void AppendRecord(StudentNode** head, int m) {
    int i, j;
    StudentNode* new_node;
    StudentNode* temp;

    printf("��������Ҫ���ӵ�ѧ����¼������");
    int num_record;
    scanf("%d", &num_record);

    for (i = 0; i < num_record; i++) {
        new_node = (StudentNode*)malloc(sizeof(StudentNode));
        printf("�����%d��ѧ����Ϣ:\t", i + 1);
        scanf("%ld%s", &new_node->student.num, new_node->student.name);
        for (j = 0; j < m; j++) {
            scanf("%f", &new_node->student.score[j]);
        }

        new_node->next = NULL;

        // ���½ڵ���뵽�����ĩβ
        if (*head == NULL) {
            *head = new_node;
        } else {
            temp = *head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = new_node;
        }
    }

    printf("�����ϣ�\n");
}

// ɾ��ѧ����¼
void DeleteRecord(StudentNode** head, int m) {
    long id;
    char ch;
    StudentNode *temp = *head, *prev = NULL;

    printf("��������Ҫɾ��ѧ����Ϣ��Ӧ��ѧ�ţ�");
    scanf("%ld", &id);

    while (temp != NULL) {
        if (temp->student.num == id) {
            printf("�ҵ��˸�ѧ���ļ�¼����ϢΪ��\n");
            printf("%10ld%15s", temp->student.num, temp->student.name);
            for (int j = 0; j < m; j++) {
                printf("%10.2f", temp->student.score[j]);
            }
            printf("%10.2f%10.2f\n", temp->student.sum, temp->student.aver);

            printf("��ȷ���Ƿ�Ҫɾ��������¼��(Y/y, N/n):");
            getchar();
            scanf("%c", &ch);
            if (ch == 'Y' || ch == 'y') {
                if (prev == NULL) {
                    *head = temp->next; // ɾ��ͷ�ڵ�
                } else {
                    prev->next = temp->next;
                }
                free(temp); // �ͷ��ڴ�
                printf("ɾ�����\n");
                return;
            } else if (ch == 'N' || ch == 'n') {
                printf("û��ɾ����¼\n");
                return;
            } else {
            	SetColor(12);
                printf("�������\n");
                SetColor(14);
                return;
            }
        }
        prev = temp;
        temp = temp->next;
    }

    printf("û���ҵ�������¼!\n");
}

// ��ѧ�Ų���ѧ����¼
void SearchByNumber(StudentNode* head,int m) {
    long id;
    StudentNode* temp = head;
    int found = 0;  // ��������Ƿ��ҵ�ѧ����¼

    printf("��������Ҫ���ҵ�ѧ����ѧ�ţ�");
    scanf("%ld", &id);

    while (temp != NULL) {
        // ֻ�ڵ�һ���ҵ�ʱ��ӡ��Ϣ
        if (temp->student.num == id) {
            if (!found) {
                printf("�ҵ��ˣ���ѧ�Ŷ�Ӧ��ѧ����ϢΪ��\n");
                printf("%-10ld%-15s", temp->student.num, temp->student.name);

                // ֻ��ӡ��Ч�ĳɼ�
                for (int j = 0; j < m; j++) {
                    if (temp->student.score[j] >= 0 && temp->student.score[j] <= 100) {
                        printf("%10.2f", temp->student.score[j]);
                    }
                }

                // ��ӡ�ֺܷ�ƽ����
                printf("%10.2f%10.2f\n", temp->student.sum, temp->student.aver);
                found = 1;  // ���Ϊ���ҵ�
            }
        }
        temp = temp->next;
    }

    if (!found) {
        printf("û���ҵ����ѧ�Ŷ�Ӧ��ѧ����¼\n");
    }
}

// ����������ѧ����¼
void SearchByName(StudentNode* head,int m) {
    char name[NAME_LEN];
    StudentNode* temp = head;
    int flag = 1;

    printf("��������Ҫ���ҵ�ѧ����������");
    scanf("%s", name);

    while (temp != NULL) {
        if (strcmp(temp->student.name, name) == 0) {
            printf("�ҵ��ˣ�ѧ����ϢΪ��\n");
            printf("%10ld%15s", temp->student.num, temp->student.name);
            for (int j = 0; j < m; j++) {
                printf("%10.2f", temp->student.score[j]);
            }
            printf("%10.2f%10.2f\n", temp->student.sum, temp->student.aver);
            flag = 0;
        }
        temp = temp->next;
    }

    if (flag) {
        printf("û���ҵ����������Ӧ��ѧ����¼\n");
    }
}

// �޸�ѧ����¼
void ModifyRecord(StudentNode* head, int m) {
    long id;
    char ch;
    StudentNode* temp = head;

    printf("��������Ҫ�޸���Ϣ��Ӧ��ѧ��:");
    scanf("%ld", &id);

    while (temp != NULL) {
        if (temp->student.num == id) {
            printf("�ҵ��˸�ѧ���ļ�¼����ϢΪ:\n");
            printf("%10ld%15s", temp->student.num, temp->student.name);
            for (int j = 0; j < m; j++) {
                printf("%10.2f", temp->student.score[j]);
            }
            printf("%10.2f%10.2f\n", temp->student.sum, temp->student.aver);

            printf("��ȷ���Ƿ���Ҫ�޸ģ�(Y/N��y/n):");
            getchar();
            scanf("%c", &ch);

            if (ch == 'Y' || ch == 'y') {
                printf("������Ҫ�޸ĵ�ѧ����Ϣ:");
                scanf("%ld%s", &temp->student.num, temp->student.name);
                temp->student.sum = 0;
                for (int j = 0; j < m; j++) {
                    scanf("%f", &temp->student.score[j]);
                    temp->student.sum += temp->student.score[j];
                }
                temp->student.aver = temp->student.sum / m;
                printf("�޸����\n");
                return;
            } else if (ch == 'N' || ch == 'n') {
                printf("û���޸ĸ�ѧ����¼\n");
                return;
            } else {
            	SetColor(12);
                printf("�������\n");
                SetColor(14);
                return;
            }
        }
        temp = temp->next;
    }
    printf("û���ҵ����ѧ���ļ�¼!\n");
}

// ����ѧ���ֺܷ�ƽ����
void CalculateScoreOfStudent(int n,int m, StudentNode* head) {
    StudentNode* temp = head;

    printf("ÿ��ѧ�����ֺܷ�ƽ����Ϊ��\n");
    while (temp != NULL) {
        temp->student.sum = 0;
        for (int j = 0; j < m; j++) {
            temp->student.sum += temp->student.score[j];
        }
        temp->student.aver = temp->student.sum / m;
        printf("ѧ�ţ�%d��������%s���ܷ֣�%.2f��ƽ���֣�%.2f\n", temp->student.num, temp->student.name, temp->student.sum, temp->student.aver);
        temp = temp->next;
    }
}

// ����γ̵��ֺܷ�ƽ����
void CalculateScoreOfCourse(int n, int m, StudentNode* head) {
    float sum[COURSE_NUM] = {0};
    float aver[COURSE_NUM] = {0};
    StudentNode* temp = head;

    printf("ÿ�ſγ̵��ֺܷ�ƽ����Ϊ��\n");
    for (int j = 0; j < m; j++) {
        sum[j] = 0;
        temp = head;
        while (temp != NULL) {
            sum[j] += temp->student.score[j];
            temp = temp->next;
        }
        aver[j] = sum[j] / n;
        printf("�γ�%d���ܷ�=%.2f, ƽ����=%.2f\n", j + 1, sum[j], aver[j]);
    }
}

// ��ѧ������
void SortbyNum(StudentNode* head) {
    StudentNode* i, * j;
    STU temp;
    for (i = head; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (i->student.num > j->student.num) {
                temp = i->student;
                i->student = j->student;
                j->student = temp;
            }
        }
    }
    printf("��ѧ���������\n");
}

// ����������
void SortbyName(StudentNode* head) {
    StudentNode* i, * j;
    STU temp;
    for (i = head; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (strcmp(i->student.name, j->student.name) > 0) {
                temp = i->student;
                i->student = j->student;
                j->student = temp;
            }
        }
    }
    printf("�������������\n");
}

// ���ܷ�����
void SortbyScore(StudentNode* head, int (*compare)(float a, float b)) {
    StudentNode* i, * j;
    STU temp;
    for (i = head; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if ((*compare)(i->student.sum, j->student.sum)) {
                temp = i->student;
                i->student = j->student;
                j->student = temp;
            }
        }
    }
    printf("���ܷ��������\n");
}

int Ascending(float a, float b) {
    return a > b;  // ����
}

int Descending(float a, float b) {
    return a < b;  // ����
}


void StatisticAnalysis(StudentNode* head, int m) {
    StudentNode* temp = head;
    int count[6] = {0};  // ����ͳ�Ʒ�����������0 - <60, 1 - 60-69, 2 - 70-79, 3 - 80-89, 4 - 90-99, 5 - 100

    // ��������ѧ������
    while (temp != NULL) {
        // ����ÿ��ѧ����ÿ�ſγ̳ɼ�
        for (int i = 0; i < m; i++) {
            if (temp->student.score[i] < 60) {
                count[0]++;
            } else if (temp->student.score[i] < 70) {
                count[1]++;
            } else if (temp->student.score[i] < 80) {
                count[2]++;
            } else if (temp->student.score[i] < 90) {
                count[3]++;
            } else if (temp->student.score[i] < 100) {
                count[4]++;
            } else if (temp->student.score[i] == 100) {
                count[5]++;
            }
        }
        temp = temp->next;
    }

    // ���ÿ�������ε�����ͳ��
    printf("\n�ɼ�ͳ�ƣ�\n");
    printf("������\t����\n");
    printf("<60\t%d\n", count[0]);
    printf("60-69\t%d\n", count[1]);
    printf("70-79\t%d\n", count[2]);
    printf("80-89\t%d\n", count[3]);
    printf("90-99\t%d\n", count[4]);
    printf("100\t%d\n", count[5]);

    // ���ÿ��ѧ����ƽ����ͳ��
    printf("\nѧ��ƽ���ɼ�ͳ�ƣ�\n");
    printf("ѧ��\t����\tƽ����\n");
    temp = head;
    while (temp != NULL) {
        printf("%ld\t%s\t%.2f\n", temp->student.num, temp->student.name, temp->student.aver);
        temp = temp->next;
    }
}

// ���ѧ����¼
void PrintRecord(StudentNode* head, int m) {
    StudentNode* temp = head;
    printf("ѧ��\t����\t�ɼ�...\n");

    while (temp != NULL) {
        printf("%-16ld\t%-16s\t", temp->student.num, temp->student.name);
        for (int j = 0; j < m; j++) {
            printf("%-16.1lf\t", temp->student.score[j]);
        }
        printf("�ܷ֣�%-16.1lf\tƽ���֣�%-16.1lf\n", temp->student.sum, temp->student.aver);
        temp = temp->next;
    }
}

// ��ѧ����Ϣд���ļ�
// ��ѧ����Ϣд���ļ� 
void WritetoFile(StudentNode* head, int n, int m) {
    FILE* fp = fopen("student1.txt", "w");
    //FILE *fp;
    //fp=fopen("C:\\Users\\asus\\Desktop\\StudentScoreManagementSystem\\data\\student.txt","w");
    StudentNode* temp = head;

    if (!fp) {
    	SetColor(12);
        printf("�ļ���ʧ��\n");
        SetColor(14);
        return;
    }
    
    // ���ѧ�������Ϳγ���
    fprintf(fp, "%d %d\n", n, m);

    // ����������ÿ��ѧ������ϸ��Ϣд���ļ�
    while (temp != NULL) {
        fprintf(fp,"%10ld%10s\n", temp->student.num, temp->student.name);
        for (int j = 0; j < m; j++) {
            fprintf(fp,"%10.1f", temp->student.score[j]);
        }
        fprintf(fp, "%10.1f%10.1f\n", temp->student.sum, temp->student.aver); // �ֺܷ�ƽ����
        // ��������
        if (strlen(temp->student.message) > 0) {
            fprintf(fp, "%s\n", temp->student.message);  // ��������
        } else {
            fprintf(fp, "������\n");  // ���û�����ԣ�д�롰�����ԡ�
        }
		temp = temp->next;
    }

    fclose(fp);
    printf("ѧ����Ϣ�ѱ���\n");
}
// ѧ��ѡ���ȡ��������
void ReadDataFromDisk(StudentNode** head, int* n, int* m) {
    int first = 1;  // ���ڼ���Ƿ��Ѿ���ȡ����
    if (ReadfromFile(head, n, m, &first) == 0) {
        printf("���ݳɹ��Ӵ��̶�ȡ��\n");
        //ViewOwnRecord(*head, *m);  // ��ʾѧ���ɼ�
    } else {
    	SetColor(12);
        printf("�޷��Ӵ��̶�ȡ���ݣ�\n");
        SetColor(14);
    }
}
// ���ļ���ȡѧ����Ϣ
int ReadfromFile(StudentNode** head, int* n, int* m, int *first) {
    FILE* fp = fopen("student1.txt", "r");
   // FILE *fp;
   // fp=fopen("C:\\Users\\asus\\Desktop\\StudentScoreManagementSystem\\data\\student.txt","r");
    StudentNode* new_node;
    StudentNode* temp;

    if (!fp) {
    	SetColor(12);
        printf("�ļ���ʧ��\n");
        SetColor(14);
        return 1; // �ļ���ʧ��
    }
	// ����������������ݣ������ͷ����������ظ���ȡ
	if (*head != NULL) {
		FreeList(*head);
		*head = NULL;
	}
    // ��ȡѧ�������Ϳγ���
    fscanf(fp,"%10d%10d",n,m);

    // ��ȡѧ����Ϣ
    while (1) {
        // �ڶ�ȡ֮ǰ�ȷ����ڴ�
        new_node = (StudentNode*)malloc(sizeof(StudentNode));
        if (new_node == NULL) {
        	SetColor(12);
            printf("�ڴ����ʧ�ܣ�\n");
            SetColor(14);
            fclose(fp);
            return 1;
        }

        // ��ȡѧ�ź�����
        if (fscanf(fp, "%10ld%10s", &new_node->student.num, new_node->student.name) != 2) {
            free(new_node); // �����ڴ�
            break; // ��������
        }

        // ��ȡ�ɼ�
        new_node->student.sum = 0;  // ��ʼ���ܷ�
        for (int j = 0; j < *m; j++) {
            if (fscanf(fp, "%10f", &new_node->student.score[j]) != 1) {
            	SetColor(12);
                printf("�ļ���ʽ����ȷ���޷���ȡ�ɼ���\n");
                SetColor(14);
                free(new_node);
                fclose(fp);
                return 1;
            }
            new_node->student.sum += new_node->student.score[j];
        }

        new_node->student.aver = new_node->student.sum / *m;

        // ��ȡ�ֺܷ�ƽ����
        if (fscanf(fp, "%10f%10f", &new_node->student.sum, &new_node->student.aver) != 2) {
        	SetColor(12);
            printf("�ļ���ʽ����ȷ���޷���ȡ�ֺܷ�ƽ���֣�\n");
            SetColor(14);
            free(new_node);
            fclose(fp);
            return 1;
        }
        fflush(stdin);
		// ��ȡ��������
        getchar();  // �����з�
        fgets(new_node->student.message, 200, fp);  // ��ȡ����
        // ȥ������ĩβ�Ļ��з�
        size_t len = strlen(new_node->student.message);
        if (len > 0 && new_node->student.message[len - 1] == '\n') {
            new_node->student.message[len - 1] = '\0';
        }
        //printf("%s\n", new_node->student.message);  // ��������
        // ���½ڵ���뵽�����ĩβ
        new_node->next = NULL;
        if (*head == NULL) {
            *head = new_node;
        } else {
            temp = *head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = new_node;
        }
    }
	*first = 0;  //�޸ı�־����
    fclose(fp);
    printf("���ݴӴ��̶�ȡ��ϣ�");
    return 0; // �ļ���ȡ�ɹ�
}
