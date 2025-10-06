#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>

struct books {
    int id;
    char bookName[50];
    char AuthorName[50];
} b;

struct student {
    int id;
    char sName[50];
    char sClass[50];
    int sRoll;
    char bookname[50];
} s;

FILE *fp;

void AddBook();
void BookList();
void RemoveBook();
void IssueBook();
void IssueList();
void Search();

int main() {
    while(1) {
        int ch;
        printf("Library Management System\n");
        printf("1. Add book\n");
        printf("2. Book list\n");
        printf("3. Remove book\n");
        printf("4. Issue book\n");
        printf("5. Issued book list\n");
        printf("6. Search a book\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &ch);
        switch(ch) {
            case 1: AddBook(); break;
            case 2: BookList(); break;
            case 3: RemoveBook(); break;
            case 4: IssueBook(); break;
            case 5: IssueList(); break;
            case 6: Search(); break;
            case 7: exit(0);
            default: printf("Please enter a number from 1 to 7\n");
        }
        printf("Press any key to continue.");
        getch();
    }
    return 0;
}

void AddBook() {
    int flag = 0;
    FILE *temp = fopen("books.txt", "rb");
    printf("Enter the ID of the Book: ");
    scanf("%d", &b.id);
    getchar();
    if (temp != NULL) {
        struct books check;
        while (fread(&check, sizeof(check), 1, temp)) {
            if (check.id == b.id) {
                flag = 1;
                break;
            }
        }
        fclose(temp);
    }

    if (flag) {
        printf("A book with this ID already exists.\n");
        return;
    }
    printf("Enter the book name: ");
    scanf(" %[^\n]", b.bookName);
    getchar();
    printf("Enter the Author name: ");
    scanf(" %[^\n]", b.AuthorName);
    getchar();
    fp = fopen("books.txt", "ab");
    fwrite(&b, sizeof(b), 1, fp);
    fclose(fp);
}

void BookList() {
    fp = fopen("books.txt", "rb");
    printf("All the books available are -->\n");
    while(fread(&b, sizeof(b), 1, fp) == 1) {
        printf("ID: %d, Name: %s, Author: %s\n", b.id, b.bookName, b.AuthorName);
    }
    fclose(fp);
}

void RemoveBook() {
    int id, f = 0;
    printf("Enter the id of the book to remove: ");
    scanf("%d", &id);

    FILE *ft;
    fp = fopen("books.txt", "rb");
    ft = fopen("temp.txt", "wb");

    while(fread(&b, sizeof(b), 1, fp) == 1) {
        if(id == b.id) f = 1;
        else fwrite(&b, sizeof(b), 1, ft);
    }

    if(f)
        printf("The book was removed successfully\n");
    else
        printf("Book not found\n");

    fclose(fp);
    fclose(ft);
    remove("books.txt");
    rename("temp.txt", "books.txt");
}

void IssueBook() {
    FILE *fl;
    int f = 0;
    printf("Issue Book -->\n");
    printf("Enter the id of the Book: ");
    scanf("%d", &s.id);

    fp = fopen("books.txt", "rb");
    while(fread(&b, sizeof(b), 1, fp) == 1) {
        if(b.id == s.id) {
            strcpy(s.bookname, b.bookName);
            f = 1;
            break;
        }
    }
    fclose(fp);

    if(!f) {
        printf("The book is not found, try again...\n");
        return;
    }

    fl = fopen("issue.txt", "ab");
    getchar();
    printf("Enter the student name: ");
    scanf(" %[^\n]s", s.sName);
    printf("Enter the class of the Student: ");
    scanf(" %[^\n]s", s.sClass);
    printf("Enter the Student roll no: ");
    scanf("%d", &s.sRoll);
    printf("The book is issued successfully on %s %s.\n", __DATE__,__TIME__);
    fwrite(&s, sizeof(s), 1, fl);
    fclose(fl);
}

void IssueList() {
    printf("Issue list -->\n");
    printf("ID  Name  Class  Roll  Book Name\n");
    fp = fopen("issue.txt", "rb");
    while(fread(&s, sizeof(s), 1, fp) == 1) {
        printf("%d    %s    %s    %d    %s\n", s.id, s.sName, s.sClass, s.sRoll, s.bookname);
    }
    fclose(fp);
}
void Search() {
    int cho;
    printf("1. Search by Book id:\n");
    printf("2. Search by Book Name:\n");
    printf("Enter the choice: ");
    scanf("%d", &cho);

    int id, f = 0;
    char bn[100];

    fp = fopen("books.txt", "rb");
    if (fp == NULL) {
        printf("Book file not found.\n");
        return;
    }

    switch (cho) {
        case 1:
            printf("Enter the Book id to be searched: ");
            scanf("%d", &id);
            while (fread(&b, sizeof(b), 1, fp) == 1) {
                if (id == b.id) {
                    f = 1;
                    break;
                }
            }
            break;

        case 2:
            getchar(); // consume newline
            printf("Enter the Book name to be searched: ");
            scanf(" %[^\n]s", bn);
            while (fread(&b, sizeof(b), 1, fp) == 1) {
                if (strcmp(bn, b.bookName) == 0) {
                    f = 1;
                    break;
                }
            }
            break;

        default:
            printf("Invalid choice.\n");
            fclose(fp);
            return;
    }

    fclose(fp);

    if (f)
        printf("The book was found successfully.\n");
    else
        printf("Book not found\n");
}
