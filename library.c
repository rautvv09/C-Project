#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITLE 100
#define MAX_AUTHOR 100
#define FILE_NAME "library.dat"

typedef struct {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
} Book;

void addBook();
void displayBooks();
void searchBook();
void deleteBook();

int main() {
    int choice;
    while (1) {
        printf("\n==== Library Management System ====\n");
        printf("1. Add Book\n");
        printf("2. Display Books\n");
        printf("3. Search Book by ID\n");
        printf("4. Delete Book by ID\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: searchBook(); break;
            case 4: deleteBook(); break;
            case 5: exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

void addBook() {
    Book book;
    FILE *fp = fopen(FILE_NAME, "ab");

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Book ID: ");
    scanf("%d", &book.id);
    getchar();

    printf("Enter Title: ");
    fgets(book.title, MAX_TITLE, stdin);
    strtok(book.title, "\n"); // remove newline

    printf("Enter Author: ");
    fgets(book.author, MAX_AUTHOR, stdin);
    strtok(book.author, "\n"); // remove newline

    fwrite(&book, sizeof(Book), 1, fp);
    fclose(fp);
    printf("Book added successfully!\n");
}

void displayBooks() {
    Book book;
    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL) {
        printf("No books found.\n");
        return;
    }

    printf("\n--- Book List ---\n");
    while (fread(&book, sizeof(Book), 1, fp)) {
        printf("ID: %d\nTitle: %s\nAuthor: %s\n\n", book.id, book.title, book.author);
    }

    fclose(fp);
}

void searchBook() {
    int id, found = 0;
    Book book;
    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL) {
        printf("No books found.\n");
        return;
    }

    printf("Enter Book ID to search: ");
    scanf("%d", &id);

    while (fread(&book, sizeof(Book), 1, fp)) {
        if (book.id == id) {
            printf("Book Found:\nID: %d\nTitle: %s\nAuthor: %s\n", book.id, book.title, book.author);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Book with ID %d not found.\n", id);

    fclose(fp);
}

void deleteBook() {
    int id, found = 0;
    Book book;
    FILE *fp = fopen(FILE_NAME, "rb");
    FILE *temp = fopen("temp.dat", "wb");

    if (fp == NULL || temp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter Book ID to delete: ");
    scanf("%d", &id);

    while (fread(&book, sizeof(Book), 1, fp)) {
        if (book.id == id) {
            found = 1;
            continue;
        }
        fwrite(&book, sizeof(Book), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove(FILE_NAME);
    rename("temp.dat", FILE_NAME);

    if (found)
        printf("Book deleted successfully.\n");
    else
        printf("Book with ID %d not found.\n", id);
}
