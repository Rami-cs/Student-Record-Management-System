#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_STUDENT 100
#define NAME_LENGTH 50

typedef struct {
    int studentID;    /*  4-digit unique ID (1000-9999)     */      
    char name[50];   /*  non-empty, up to 49 characters    */       
    int age;        /*  6 – 50                            */        
    double gpa;    /*  0.0 – 4.0                         */         
} Student;

Student students[MAX_STUDENT];           /* array of structs, max 100 */
int studentCount = 0;                   /* number of students stored */

void displayMainMenu();
void initializeStudentRecords();
void addStudent(Student *);
void removeStudent(Student *);
void searchStudent(Student *);
void displayAllStudents(Student *);
void saveStudentRecords();

/*
    Student ID – a four-digit integer (must be unique).
    Name – a string (up to 49 characters, non-empty).
    Age – a positive integer (reasonable range 6–50).
    GPA – a double between 0.0 and 4.0 inclusive.
*/

int main()
{
    int choice;
    char excess;
    
    initializeStudentRecords();
    
    do
    {
        displayMainMenu();

        if(scanf("%d%c", &choice, &excess) != 2 || excess != '\n')
        {
            printf("\nEnter number only!\n");
            while(getchar() != '\n');
            continue;
        }
        
        if(choice > 5 || choice < 1)
        {
            printf("\nInvalid Choice\n");
            continue;
        }
        else if(choice == 1)
            addStudent(students);
        else if(choice == 2)
            removeStudent(students);
        else if(choice == 3)
            searchStudent(students);
        else if(choice == 4)
            displayAllStudents(students);
        else if(choice == 5)
        {
            saveStudentRecords();
            printf("Thank you for using Student Record Management System!\n");
            break;
        }
        
    }
    while(1);

    return 0;
}

void displayMainMenu()
{
    printf("\n=== STUDENT RECORD MANAGEMENT SYSTEM ===\n");
    printf("1- Add New Student\n");
    printf("2- Remove Student\n");
    printf("3- Search for Student\n");
    printf("4- Display All Students\n");
    printf("5- Exit System\n");
    printf("Enter your choice (1-5): ");
}

void initializeStudentRecords()
{
    FILE *in = fopen("students.bin", "rb");
    if (in == NULL) 
    {
        printf("No existing records found. Starting with empty system.\n");
        return;
    }

    studentCount = fread(students, sizeof(Student), 100, in);
    
    fclose(in);

    printf("\n%d student record(s) loaded from students.bin.\n", studentCount);
}

void addStudent(Student *student)
{
    int ID;
    int duplicates;
    char excess;

    if(studentCount >= MAX_STUDENT)
    {
        printf("Error! Max limit is 100 students\n");
        return; 
    }

    printf("\n=== ADD NEW STUDENT ===\n");

    // get ID

    while(1)
    {

        printf("Enter 4-digit Student ID (1000-9999): ");

        if(scanf("%d%c", &ID, &excess) != 2 || excess != '\n')
        {
            printf("Enter numbers only!\n");
            while(getchar() != '\n');
            continue;
        }

        if(ID > 9999 || ID < 1000)
        {
            printf("\nInvalid ID. It should be 4 digits only!\n");
            continue;
        }

        duplicates = 0;
        
        for(int i = 0; i < studentCount; i++)
        {
            if(student[i].studentID == ID)            
            {
                duplicates = 1;
                break;
            }
        }
        
        if(duplicates)
        {
            printf("%d is already present, pick another one\n", ID);
            continue;
        }

        student[studentCount].studentID = ID;
        break;
    }
    
    // get name 
    
    while(1)
    {
        printf("Enter Student name: ");
        fgets(student[studentCount].name, NAME_LENGTH, stdin);
        int length = strlen(student[studentCount].name);
        
        if (student[studentCount].name[length - 1] != '\n')
        {
            while(getchar() != '\n');
        }
        else
        {
            student[studentCount].name[length - 1] = '\0';
        }
        
        length = strlen(student[studentCount].name); // new length
        if(length == 0)
        {
            printf("Name cannot be empty!!\n");
            continue;
        }

        int flag = 0; // if the input is a symbol 

        if(student[studentCount].name[0] == ' ' || student[studentCount].name[length - 1] == ' ')
        {
            printf("name cannot start or end with space\n");
            continue;
        }

        for(int i = 0; i < length; i++)
        {
            if(!(isalpha(student[studentCount].name[i])) && student[studentCount].name[i] != ' ')
            {
                flag = 1;
                break;
            }
        }
        if(flag == 1)
        {
            printf("Invalid name, use letters only\n");
            continue;
        }        
        break;
    }
    
    // get age

    while(1)
    {
        printf("Enter Student Age: ");

        
        if(!(scanf("%d", &student[studentCount].age)))
        {
            printf("Invalid input!\n");
            while(getchar() != '\n');
        }
        else if(student[studentCount].age < 6 || student[studentCount].age > 50)
        {
            printf("Age range 6 - 50\n");
        }
        else
        {
            while(getchar() != '\n');
            break;
        }

    }

    // get GPA

    while(1)
    {
        printf("Enter GPA: ");
        if((!scanf("%lf", &student[studentCount].gpa)))
        {
            printf("Invalid Input!\n");
            while(getchar() != '\n');
            continue;
        }
        while(getchar() != '\n');
        
        if(student[studentCount].gpa > 4.0 || student[studentCount].gpa < 0.0)
        {
            printf("Student's GPA must be between 0 and 4\n");
            continue;
        }

        break;
    }

    printf("\nStudent added successfully!\n");
    
    studentCount++;
}

void removeStudent(Student *student)
{
    int stdIndex = -1;
    int id;
    char excess;

    if(studentCount == 0)
    {
        printf("No stored students to remove.\n");
        return;
    }

    while(1)
    {
        printf("\n=== REMOVE STUDENT ===\n");
        printf("Enter Student ID to remove: ");
        if(scanf("%d%c", &id, &excess) != 2 || excess != '\n')
        {
            printf("Invalid input!\n");
            while(getchar() != '\n');
            continue;
        }

        for(int i = 0; i < studentCount; i++)
        {
            if(id == student[i].studentID)
            {
                stdIndex = i;
                break;
            }
        }

        if(stdIndex != -1)
        {
            printf("Removing Student: %s (ID: %d)\n", student[stdIndex].name, student[stdIndex].studentID);

            for(int i = stdIndex; i < studentCount - 1; i++)
            {
                student[i] = student[i + 1];
            }
            studentCount--;
            printf("Student removed successfully.\n");
        }
        else printf("Error: Student ID not found.\n");

        break;
    }
}

void searchStudent(Student *student)
{
    if(studentCount == 0)
    {
        printf("No students stored\n");
        return;
    }

    printf("\n=== SEARCH STUDENT ===\n");
    int targetID;
    int Index = -1;
    char excess;

    while(1)
    {
        printf("Enter student's ID: ");

        if(scanf("%d%c", &targetID, &excess) != 2 || excess != '\n')
        {
            while(getchar() != '\n');
            printf("Invalid ID\n");
            continue;
        }
        
        if(targetID > 9999 || targetID < 1000)
        {
            printf("\nInvalid ID. It should be 4 digits only!\n");
            continue;
        }
    
        for(int i = 0; i < studentCount; i++)
        {
            if(student[i].studentID == targetID)
            {
                Index = i;
                break;
            }
        }
    
        if(Index != -1)
        {
            printf("\nStudent Found:\n");
            printf("Student ID: %d\nStudnet Name: %s\nStudent Age: %d\nStudent GPA: %.2f\n", student[Index].studentID, student[Index].name, student[Index].age, student[Index].gpa);
        }else{
            printf("Student not found.\n");
        }

        break;
    }
}
void displayAllStudents(Student *student)
{
    if(studentCount == 0)
    {
        printf("No students in the system.\n");
        return;
    }

    double GPAsum = 0;

    for(int i = 0; i < studentCount; i++)
    {
        GPAsum += student[i].gpa;
    }

    printf("\n=== ALL STUDENTS ===\n");
    
    printf("\n%-10s %-20s %-5s %-5s\n", "ID", "Name", "Age", "GPA");
    printf("-------------------------------------------------------------------\n");

    for(int i = 0; i < studentCount; i++)
    {
        printf("%-10d %-20s %-5d %-5.2f\n", student[i].studentID, student[i].name, student[i].age, student[i].gpa);
    }

    printf("\nTotal number of students: %d\n", studentCount);
    printf("Average GPA: %.2lf\n", GPAsum / studentCount);
    
}
void saveStudentRecords()
{
    FILE *out = fopen("students.bin", "wb");
    if(out == NULL)
    {
        printf("Error: Could not save records.\n");
        return;
    }
    
    fwrite(students, sizeof(Student), studentCount, out);

    fclose(out);


    printf("%d student record(s) saved to students.bin\n", studentCount);
}