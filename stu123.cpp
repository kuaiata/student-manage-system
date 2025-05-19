#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#define STU_NUM 50      // 教学班学生人数上限
#define COURSE_NUM 10   // 一学期的课程上限
#define NAME_LEN 10     // 学生姓名最大长度

// 角色枚举
typedef enum { TEACHER, STUDENT } Role;

// 用户结构体
typedef struct user {
    char username[20];
    char password[20];
    Role role; // 用户角色
} User;

User currentUser;  // 当前用户

// 学生结构体类型定义
typedef struct student {
    long num;                     // 学号
    char name[NAME_LEN];          // 姓名
    float score[COURSE_NUM];      // 每门课程成绩
    float sum;                    // 总分
    float aver;                   // 平均分
    char message[200];            // 教师留言
} STU;

// 链表节点定义
typedef struct StudentNode {
    STU student;                  // 学生信息
    struct StudentNode* next;     // 指向下一个学生的指针
} StudentNode;

// 自定义函数声明
int Login();                                                                // 登录
int Menu(void);                                                              // 操作菜单
void SetPosition(int x, int y);                                               // 设置输出内容在控制台窗口中的起始位置
void ReturnToLogin();														//返回登陆页面 
void InputRecord(StudentNode** head, int* n, int* m);                         // 输入学生信息
void AppendRecord(StudentNode** head, int m);                                 // 增加学生信息
void DeleteRecord(StudentNode** head, int m);                                 // 删除学生信息
void SearchByNumber(StudentNode* head, int m);                                // 按学号查询学生信息
void SearchByName(StudentNode* head, int m);                                  // 按姓名查询学生信息
void ModifyRecord(StudentNode* head, int m);                                  // 修改学生信息
void CalculateScoreOfStudent(int n, int m, StudentNode* head);               // 计算学生的总分和平均分
void CalculateScoreOfCourse(int n, int m, StudentNode* head);                // 计算课程的总分和平均分
void SortbyNum(StudentNode* head);                                           // 按学号排序
void SortbyName(StudentNode* head);                                          // 按姓名排序
void SortbyScore(StudentNode* head, int (*compare)(float a, float b));      // 按每个学生平均分进行排序
int Ascending(float a, float b);                                             // 升序排序
int Descending(float a, float b);                                            // 降序排序
void StatisticAnalysis(StudentNode* head, int m);                            // 统计并输出各个分数段学生人数及占比
void PrintRecord(StudentNode* head, int m);                                   // 打印学生成绩
void WritetoFile(StudentNode* head, int n, int m);                           // 将学生信息保存至文件student.txt中
int ReadfromFile(StudentNode** head, int* n, int* m, int* first);           // 将学生信息从文件student.txt中读出，存入链表
void FreeList(StudentNode* head);                                            // 释放链表内存
void ViewOwnRecord(StudentNode* head, int m);                                // 学生查看自己的成绩
void ReadDataFromDisk(StudentNode** head, int* n, int* m); 					//学生选择读取磁盘数据
void SetColor(int color);													//颜色 
void TeacherLeaveMessage(StudentNode* head);								//教师留言 
void StudentViewMessage(StudentNode* head);									//学生查看教师留言 


// 学生查看留言功能
void StudentViewMessage(StudentNode* head) {
    long student_id;
    printf("请输入学号查看留言：");
    scanf("%ld", &student_id);

    // 查找该学生
    StudentNode* temp = head;
    while (temp != NULL) {
        if (temp->student.num == student_id) {
            if (strlen(temp->student.message) > 0) {
                printf("教师留言：\n%s\n", temp->student.message);
            } else {
                printf("您没有收到教师的留言。\n");
            }
            return;
        }
        temp = temp->next;
    }

    printf("未找到该学生的记录！\n");
}

// 教师留言功能
void TeacherLeaveMessage(StudentNode* head) {
    long student_id;
    printf("请输入学生学号：");
    scanf("%ld", &student_id);

    // 查找该学生
    StudentNode* temp = head;
    while (temp != NULL) {
        if (temp->student.num == student_id) {
            printf("请输入留言内容：\n");
            getchar();  // 处理换行符
            fgets(temp->student.message, 200, stdin);  // 读入留言内容
            // 去除留言末尾的换行符
            size_t len = strlen(temp->student.message);
            if (len > 0 && temp->student.message[len - 1] == '\n') {
                temp->student.message[len - 1] = '\0';
            }

            printf("留言已成功保存！\n");
            return;
        }
        temp = temp->next;
    }

    printf("未找到该学生的记录！\n");
}

int main() {
    int n = 0;
    int m = 0;
    int first = 1;
    int ch;
    StudentNode* head = NULL; // 初始化链表头节点为空

    system("mode con cols=130 lines=60");
    system("color 0E");
	SetColor(14);
    // 登录
    if (!Login()) {
        return 0;  // 登录失败，退出程序
    }

    while (1) {
        system("cls");
        SetPosition(0, 0);  // 重设光标位置到第一行
        fflush(stdout);  // 刷新输出缓冲区，确保输出立即显示
        ch = Menu();
        switch (ch) {
        case 1:
            if (currentUser.role == TEACHER) {
                system("cls");
                InputRecord(&head, &n, &m);
                WritetoFile(head, n, m);  // 保存数据到文件
                first = 0;
            } else if (currentUser.role == STUDENT) {
            	system("cls");
				SetPosition(0, 0);  // 确保学生登录后第一行内容正确显示
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
                ReadDataFromDisk(&head, &n, &m);  // 学生选择读取磁盘数据
            }
            system("pause");
            break;
        case 3:
            if (currentUser.role == TEACHER) {
                system("cls");
                DeleteRecord(&head, m);
            }else if (currentUser.role == STUDENT) {
                system("cls");
                StudentViewMessage(head);  // 学生查看留言功能
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
                    printf("尚未输入学生信息或文件打开失败,请先检查！\n");
                    SetColor(14);
                    system("pause");
                    break;
                }
            }
            system("pause");
            break;
        case 17:  // 教师留言选项
            if (currentUser.role == TEACHER) {
                system("cls");
                TeacherLeaveMessage(head);  // 教师留言功能
            }
            system("pause");
            break;
        case 0:
            system("cls");
            printf("退出并返回登录界面！\n");
            FreeList(head);  // 释放链表内存
            SetPosition(0, 0);  // 重设光标位置到第一行
    		fflush(stdout);  // 刷新输出缓冲区，确保输出立即显示
            ReturnToLogin(); // 返回登录界面
            exit(0);
        default:
            system("cls");
            SetColor(12);
            printf("输入出错，请重新选择操作！\n");
            SetColor(14);
            system("pause");
        }
    }

    return 0;
}

// 用户登录
int Login() {
	system("cls");  // 清屏
    SetPosition(0, 0);  // 重设光标位置到第一行
    fflush(stdout);  // 刷新输出缓冲区，确保输出立即显示
    char username[20], password[20];
    printf("请输入用户名: ");
    scanf("%s", username);
    printf("请输入密码: ");
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
        printf("用户名或密码错误！\n");
        SetColor(14);
        return 0;
    }
}

// 显示菜单
int Menu(void) {
    int posy = 5;
    int option;
    SetPosition(35, posy);
    printf("学生成绩管理系统\n");

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
	    printf("1. 输入学生信息");
	    SetPosition(70, posy);
	    printf("2. 增加学生成绩");
	    SetPosition(35, posy += 2);
	    printf("3. 删除学生信息");
	    SetPosition(70, posy);
	    printf("4. 按学号查找记录");
	    SetPosition(35, posy += 2);
	    printf("5. 按姓名查找记录");
	    SetPosition(70, posy);
	    printf("6. 修改学生信息");
	    SetPosition(35, posy += 2);
	    printf("7. 计算学生成绩");
	    SetPosition(70, posy);
	    printf("8. 计算课程成绩");
	    SetPosition(35, posy += 2);
	    printf("9. 按学号排序");
	    SetPosition(70, posy);
	    printf("10. 按姓名排序");
	    SetPosition(35, posy += 2);
	    printf("11. 按总成绩降序排序");
	    SetPosition(70, posy);
	    printf("12. 按总成绩升序排序");
	    SetPosition(35, posy += 2);
	    printf("13. 学生成绩统计");
	    SetPosition(70, posy);
	    printf("14. 打印学生记录");
	    SetPosition(35, posy += 2);
	    printf("15. 学生记录存盘");
	    SetPosition(70, posy);
	    printf("16. 从磁盘读取学生记录");
	    SetPosition(35, posy += 2);
	    printf("17. 给学生留言");
	    SetPosition(70, posy);
	    printf("0. 退出并返回登录界面");
	    SetColor(11);
	    for (int i = 0; i < 2; i++) {
	        SetPosition(35, ++posy);
	        for (int j = 0; j < 55; j++) {
	            printf("-");
	        }
	    }
	    SetColor(14);
	    SetPosition(35, ++posy);
	    printf("请选择你想要进行的操作[0~17]: [  ]\b\b\b");
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
	    printf("1. 查看个人成绩");
	    SetPosition(70, posy);
	    printf("2. 从磁盘读取成绩数据");
	    SetPosition(35, posy += 2);
	    printf("3. 查看教师留言");
		SetPosition(70, posy);
	    printf("0. 退出并返回登录界面");
	    SetColor(11);
	    for (int i = 0; i < 2; i++) {
	        SetPosition(35, ++posy);
	        for (int j = 0; j < 55; j++) {
	            printf("-");
	        }
	    }
	    SetColor(14);
	    SetPosition(35, ++posy);
	    printf("请选择你想要进行的操作[0~3]: [  ]\b\b\b");
    }
    scanf("%d", &option);
    return option;
}
void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
// 设置控制台光标位置
void SetPosition(int x, int y) {
    HANDLE hOut;
    COORD pos;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hOut, pos);
}

// 学生查看自己的成绩
void ViewOwnRecord(StudentNode* head, int m) {
    if (currentUser.role != STUDENT) {
    	SetColor(12);
        printf("您没有权限查看成绩！\n");
        SetColor(14);
        return;
    }

    long id;
    printf("请输入您的学号查看成绩: ");
    scanf("%ld", &id);

    StudentNode* temp = head;
    int found = 0;
     while (temp != NULL) {
        // 只在第一次找到时打印信息
        if (temp->student.num == id) {
            if (!found) {
                printf("找到了，该学号对应的学生信息为：\n");
                printf("%-10ld%-15s", temp->student.num, temp->student.name);

                // 只打印有效的成绩
                for (int j = 0; j < m; j++) {
                    if (temp->student.score[j] >= 0 && temp->student.score[j] <= 100) {
                        printf("%10.2f", temp->student.score[j]);
                    }
                }

                // 打印总分和平均分
                printf("%10.2f%10.2f\n", temp->student.sum, temp->student.aver);
                found = 1;  // 标记为已找到
            }
        }
        temp = temp->next;
    }

    if (!found) {
        printf("没有找到这个学号对应的学生记录\n");
    }
}

// 释放链表内存
void FreeList(StudentNode* head) {
    StudentNode* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
// 退出并返回登录界面
void ReturnToLogin() {
    char choice;
    printf("\n操作完成！您是否要返回登录界面? (Y/N): ");
    scanf(" %c", &choice);  // 注意前面的空格，用于清除缓冲区中的换行符

    if (choice == 'Y' || choice == 'y') {
        printf("\n正在返回登录界面...\n");
        system("cls");  // 清屏
        main();         // 重新调用main函数实现返回登录界面
    } else {
        printf("\n继续操作中...\n");
    }
}
// 输入学生信息
void InputRecord(StudentNode** head, int* n, int* m) {
    int i, j;
    StudentNode* new_node;
    StudentNode* temp;

    SetPosition(35, 6);              // 设置光标位置
    printf("输入学生人数(n<%d):", STU_NUM);
    scanf("%d", n);
    SetPosition(35, 8);
    printf("输入课程数(m<%d):", COURSE_NUM);
    scanf("%d", m);

    for (i = 0; i < *n; i++) {
        new_node = (StudentNode*)malloc(sizeof(StudentNode));
        SetPosition(35, 10 + i);
        printf("输入第%d个学生信息:\t", i + 1);
        scanf("%ld%s", &new_node->student.num, new_node->student.name);
        for (j = 0; j < *m; j++) {
            scanf("%f", &new_node->student.score[j]);
        }

        new_node->next = NULL;

        // 将新节点插入到链表的末尾
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
    // 完成录入后返回登录界面
    ReturnToLogin();  // 调用返回登录界面的函数
}

// 增加学生记录
void AppendRecord(StudentNode** head, int m) {
    int i, j;
    StudentNode* new_node;
    StudentNode* temp;

    printf("请输入需要增加的学生记录条数：");
    int num_record;
    scanf("%d", &num_record);

    for (i = 0; i < num_record; i++) {
        new_node = (StudentNode*)malloc(sizeof(StudentNode));
        printf("输入第%d个学生信息:\t", i + 1);
        scanf("%ld%s", &new_node->student.num, new_node->student.name);
        for (j = 0; j < m; j++) {
            scanf("%f", &new_node->student.score[j]);
        }

        new_node->next = NULL;

        // 将新节点插入到链表的末尾
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

    printf("添加完毕！\n");
}

// 删除学生记录
void DeleteRecord(StudentNode** head, int m) {
    long id;
    char ch;
    StudentNode *temp = *head, *prev = NULL;

    printf("请输入你要删除学生信息对应的学号：");
    scanf("%ld", &id);

    while (temp != NULL) {
        if (temp->student.num == id) {
            printf("找到了该学生的记录，信息为：\n");
            printf("%10ld%15s", temp->student.num, temp->student.name);
            for (int j = 0; j < m; j++) {
                printf("%10.2f", temp->student.score[j]);
            }
            printf("%10.2f%10.2f\n", temp->student.sum, temp->student.aver);

            printf("请确认是否要删除这条记录？(Y/y, N/n):");
            getchar();
            scanf("%c", &ch);
            if (ch == 'Y' || ch == 'y') {
                if (prev == NULL) {
                    *head = temp->next; // 删除头节点
                } else {
                    prev->next = temp->next;
                }
                free(temp); // 释放内存
                printf("删除完毕\n");
                return;
            } else if (ch == 'N' || ch == 'n') {
                printf("没有删除记录\n");
                return;
            } else {
            	SetColor(12);
                printf("输入错误\n");
                SetColor(14);
                return;
            }
        }
        prev = temp;
        temp = temp->next;
    }

    printf("没有找到该生记录!\n");
}

// 按学号查找学生记录
void SearchByNumber(StudentNode* head,int m) {
    long id;
    StudentNode* temp = head;
    int found = 0;  // 用来标记是否找到学生记录

    printf("请输入你要查找的学生的学号：");
    scanf("%ld", &id);

    while (temp != NULL) {
        // 只在第一次找到时打印信息
        if (temp->student.num == id) {
            if (!found) {
                printf("找到了，该学号对应的学生信息为：\n");
                printf("%-10ld%-15s", temp->student.num, temp->student.name);

                // 只打印有效的成绩
                for (int j = 0; j < m; j++) {
                    if (temp->student.score[j] >= 0 && temp->student.score[j] <= 100) {
                        printf("%10.2f", temp->student.score[j]);
                    }
                }

                // 打印总分和平均分
                printf("%10.2f%10.2f\n", temp->student.sum, temp->student.aver);
                found = 1;  // 标记为已找到
            }
        }
        temp = temp->next;
    }

    if (!found) {
        printf("没有找到这个学号对应的学生记录\n");
    }
}

// 按姓名查找学生记录
void SearchByName(StudentNode* head,int m) {
    char name[NAME_LEN];
    StudentNode* temp = head;
    int flag = 1;

    printf("请输入你要查找的学生的姓名：");
    scanf("%s", name);

    while (temp != NULL) {
        if (strcmp(temp->student.name, name) == 0) {
            printf("找到了，学生信息为：\n");
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
        printf("没有找到这个姓名对应的学生记录\n");
    }
}

// 修改学生记录
void ModifyRecord(StudentNode* head, int m) {
    long id;
    char ch;
    StudentNode* temp = head;

    printf("请输入需要修改信息对应的学号:");
    scanf("%ld", &id);

    while (temp != NULL) {
        if (temp->student.num == id) {
            printf("找到了该学生的记录，信息为:\n");
            printf("%10ld%15s", temp->student.num, temp->student.name);
            for (int j = 0; j < m; j++) {
                printf("%10.2f", temp->student.score[j]);
            }
            printf("%10.2f%10.2f\n", temp->student.sum, temp->student.aver);

            printf("请确认是否需要修改？(Y/N或y/n):");
            getchar();
            scanf("%c", &ch);

            if (ch == 'Y' || ch == 'y') {
                printf("请输入要修改的学生信息:");
                scanf("%ld%s", &temp->student.num, temp->student.name);
                temp->student.sum = 0;
                for (int j = 0; j < m; j++) {
                    scanf("%f", &temp->student.score[j]);
                    temp->student.sum += temp->student.score[j];
                }
                temp->student.aver = temp->student.sum / m;
                printf("修改完毕\n");
                return;
            } else if (ch == 'N' || ch == 'n') {
                printf("没有修改该学生记录\n");
                return;
            } else {
            	SetColor(12);
                printf("输入错误！\n");
                SetColor(14);
                return;
            }
        }
        temp = temp->next;
    }
    printf("没有找到这个学生的记录!\n");
}

// 计算学生总分和平均分
void CalculateScoreOfStudent(int n,int m, StudentNode* head) {
    StudentNode* temp = head;

    printf("每个学生的总分和平均分为：\n");
    while (temp != NULL) {
        temp->student.sum = 0;
        for (int j = 0; j < m; j++) {
            temp->student.sum += temp->student.score[j];
        }
        temp->student.aver = temp->student.sum / m;
        printf("学号：%d，姓名：%s，总分：%.2f，平均分：%.2f\n", temp->student.num, temp->student.name, temp->student.sum, temp->student.aver);
        temp = temp->next;
    }
}

// 计算课程的总分和平均分
void CalculateScoreOfCourse(int n, int m, StudentNode* head) {
    float sum[COURSE_NUM] = {0};
    float aver[COURSE_NUM] = {0};
    StudentNode* temp = head;

    printf("每门课程的总分和平均分为：\n");
    for (int j = 0; j < m; j++) {
        sum[j] = 0;
        temp = head;
        while (temp != NULL) {
            sum[j] += temp->student.score[j];
            temp = temp->next;
        }
        aver[j] = sum[j] / n;
        printf("课程%d：总分=%.2f, 平均分=%.2f\n", j + 1, sum[j], aver[j]);
    }
}

// 按学号排序
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
    printf("按学号排序完毕\n");
}

// 按姓名排序
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
    printf("按姓名排序完毕\n");
}

// 按总分排序
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
    printf("按总分排序完毕\n");
}

int Ascending(float a, float b) {
    return a > b;  // 升序
}

int Descending(float a, float b) {
    return a < b;  // 降序
}


void StatisticAnalysis(StudentNode* head, int m) {
    StudentNode* temp = head;
    int count[6] = {0};  // 用于统计分数段人数：0 - <60, 1 - 60-69, 2 - 70-79, 3 - 80-89, 4 - 90-99, 5 - 100

    // 遍历所有学生数据
    while (temp != NULL) {
        // 遍历每个学生的每门课程成绩
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

    // 输出每个分数段的人数统计
    printf("\n成绩统计：\n");
    printf("分数段\t人数\n");
    printf("<60\t%d\n", count[0]);
    printf("60-69\t%d\n", count[1]);
    printf("70-79\t%d\n", count[2]);
    printf("80-89\t%d\n", count[3]);
    printf("90-99\t%d\n", count[4]);
    printf("100\t%d\n", count[5]);

    // 输出每个学生的平均分统计
    printf("\n学生平均成绩统计：\n");
    printf("学号\t姓名\t平均分\n");
    temp = head;
    while (temp != NULL) {
        printf("%ld\t%s\t%.2f\n", temp->student.num, temp->student.name, temp->student.aver);
        temp = temp->next;
    }
}

// 输出学生记录
void PrintRecord(StudentNode* head, int m) {
    StudentNode* temp = head;
    printf("学号\t姓名\t成绩...\n");

    while (temp != NULL) {
        printf("%-16ld\t%-16s\t", temp->student.num, temp->student.name);
        for (int j = 0; j < m; j++) {
            printf("%-16.1lf\t", temp->student.score[j]);
        }
        printf("总分：%-16.1lf\t平均分：%-16.1lf\n", temp->student.sum, temp->student.aver);
        temp = temp->next;
    }
}

// 将学生信息写入文件
// 将学生信息写入文件 
void WritetoFile(StudentNode* head, int n, int m) {
    FILE* fp = fopen("student1.txt", "w");
    //FILE *fp;
    //fp=fopen("C:\\Users\\asus\\Desktop\\StudentScoreManagementSystem\\data\\student.txt","w");
    StudentNode* temp = head;

    if (!fp) {
    	SetColor(12);
        printf("文件打开失败\n");
        SetColor(14);
        return;
    }
    
    // 输出学生人数和课程数
    fprintf(fp, "%d %d\n", n, m);

    // 遍历链表，将每个学生的详细信息写入文件
    while (temp != NULL) {
        fprintf(fp,"%10ld%10s\n", temp->student.num, temp->student.name);
        for (int j = 0; j < m; j++) {
            fprintf(fp,"%10.1f", temp->student.score[j]);
        }
        fprintf(fp, "%10.1f%10.1f\n", temp->student.sum, temp->student.aver); // 总分和平均分
        // 保存留言
        if (strlen(temp->student.message) > 0) {
            fprintf(fp, "%s\n", temp->student.message);  // 留言内容
        } else {
            fprintf(fp, "无留言\n");  // 如果没有留言，写入“无留言”
        }
		temp = temp->next;
    }

    fclose(fp);
    printf("学生信息已保存\n");
}
// 学生选择读取磁盘数据
void ReadDataFromDisk(StudentNode** head, int* n, int* m) {
    int first = 1;  // 用于检查是否已经读取数据
    if (ReadfromFile(head, n, m, &first) == 0) {
        printf("数据成功从磁盘读取！\n");
        //ViewOwnRecord(*head, *m);  // 显示学生成绩
    } else {
    	SetColor(12);
        printf("无法从磁盘读取数据！\n");
        SetColor(14);
    }
}
// 从文件读取学生信息
int ReadfromFile(StudentNode** head, int* n, int* m, int *first) {
    FILE* fp = fopen("student1.txt", "r");
   // FILE *fp;
   // fp=fopen("C:\\Users\\asus\\Desktop\\StudentScoreManagementSystem\\data\\student.txt","r");
    StudentNode* new_node;
    StudentNode* temp;

    if (!fp) {
    	SetColor(12);
        printf("文件打开失败\n");
        SetColor(14);
        return 1; // 文件打开失败
    }
	// 如果链表中已有数据，则先释放链表，避免重复读取
	if (*head != NULL) {
		FreeList(*head);
		*head = NULL;
	}
    // 读取学生人数和课程数
    fscanf(fp,"%10d%10d",n,m);

    // 读取学生信息
    while (1) {
        // 在读取之前先分配内存
        new_node = (StudentNode*)malloc(sizeof(StudentNode));
        if (new_node == NULL) {
        	SetColor(12);
            printf("内存分配失败！\n");
            SetColor(14);
            fclose(fp);
            return 1;
        }

        // 读取学号和姓名
        if (fscanf(fp, "%10ld%10s", &new_node->student.num, new_node->student.name) != 2) {
            free(new_node); // 清理内存
            break; // 正常结束
        }

        // 读取成绩
        new_node->student.sum = 0;  // 初始化总分
        for (int j = 0; j < *m; j++) {
            if (fscanf(fp, "%10f", &new_node->student.score[j]) != 1) {
            	SetColor(12);
                printf("文件格式不正确，无法读取成绩！\n");
                SetColor(14);
                free(new_node);
                fclose(fp);
                return 1;
            }
            new_node->student.sum += new_node->student.score[j];
        }

        new_node->student.aver = new_node->student.sum / *m;

        // 读取总分和平均分
        if (fscanf(fp, "%10f%10f", &new_node->student.sum, &new_node->student.aver) != 2) {
        	SetColor(12);
            printf("文件格式不正确，无法读取总分和平均分！\n");
            SetColor(14);
            free(new_node);
            fclose(fp);
            return 1;
        }
        fflush(stdin);
		// 读取留言内容
        getchar();  // 处理换行符
        fgets(new_node->student.message, 200, fp);  // 读取留言
        // 去除留言末尾的换行符
        size_t len = strlen(new_node->student.message);
        if (len > 0 && new_node->student.message[len - 1] == '\n') {
            new_node->student.message[len - 1] = '\0';
        }
        //printf("%s\n", new_node->student.message);  // 留言内容
        // 将新节点插入到链表的末尾
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
	*first = 0;  //修改标志变量
    fclose(fp);
    printf("数据从磁盘读取完毕！");
    return 0; // 文件读取成功
}
