#include <stdlib.h>

#include "io.h"

#include <unistd.h>

#include "mm.h"




/**
 * @name  main
 * @brief This function is the entry point to your program
 * @return 0 for success, anything else for failure
 *
 *
 * Then it has a place for you to implementation the command 
 * interpreter as  specified in the handout.
 */


typedef struct Node {
    int data;
    struct Node *next;
} Node;

Node *createNode(int data) {
    Node *newNode = simple_malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void insertAtEnd(Node **head, int data) {
    Node *newNode = createNode(data);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    Node *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

void deleteFromEnd(Node **head) {
    if (*head == NULL) {
        return;
    }
    Node *temp = *head;
    if (temp->next == NULL) {
        simple_free(temp);
        *head = NULL;
        return;
    }
    while (temp->next->next != NULL) {
        temp = temp->next;
    }
    simple_free(temp->next);
    temp->next = NULL;
}

void itoaAndReverse(int n) {
    int numbers[100];
    int i = 0;

    do
    {
        numbers[i++] = n % 10;
        n /= 10;
    }
    while (n > 0);
    for (int j = i-1; j >= 0; j--) {
        write_int(numbers[j]);
    }
}

void printLinkedList(Node *head) {
    Node *temp = head;
    while (temp != NULL) {
        if (temp->data > 9) {
            itoaAndReverse(temp->data);
        } else {
            write_int(temp->data);
        }
        //Undgå at skrive komma, hvis kun et tal
        if (temp->next != NULL) {
            write_char(',');
        }
        temp = temp->next;
    }
    write_char(';');
    simple_free(temp);
}

ssize_t flushInputBuffer() {
    char buf[1000];
    ssize_t bytes_read = read(STDIN_FILENO, buf, sizeof(buf));
    // Returnér bytes_read for at undgå advarsel for ubrugt variabel
    return bytes_read;
}

int main() {


    Node *head = NULL;
    int count = 0;

    // cmd bliver initialiseret her, så while do-while
    // kan "se" ændringerne
    char cmd;

    do
    {
        cmd = read_char();

        if (cmd == 'a') {
            insertAtEnd(&head, count);
            count++;
        } else if (cmd == 'b') {
            count++;
        } else if (cmd == 'c') {
            deleteFromEnd(&head);
            count++;
        }
    }
    while (cmd == 'a' || cmd == 'b' || cmd == 'c');

    printLinkedList(head);


    // Vi tjekker om cmd er EOF, hvilket ellers ville
    // fryse programmet i flushInputBuffer funktionen
    if (cmd != EOF && cmd != '\n') {
        flushInputBuffer(); // Vi sikrer os, at inputbufferen ikke påvirker vores terminal
    }

    return 0;
}
