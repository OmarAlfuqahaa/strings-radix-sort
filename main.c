// Omar Alfoqahaa 1200207
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // to import isalpha

#define MAX_STRING_SIZE 31    // declare a constant (max is 30 and we need one more for \0)
#define NUM_OF_CHARACTERS 256 // declare a constant (for ASCII)

// structure for doubly linked list (node)
typedef struct Node {
    char data[MAX_STRING_SIZE];
    struct Node *prev;
    struct Node *next;
} Node;

// doubly linked list structure
typedef struct {
    Node *head;
    Node *tail;
} DoublyLinkedList;

// function to initialize an empty doubly linked list
void initializeList(DoublyLinkedList *list) {
    list->head = NULL;
    list->tail = NULL;
}

// function to create a new node
Node *createNode(const char data[]) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("memory allocation error!!!\n");
        exit(0);
    }

    // Ensure the data fits within the specified size
    strcpy(newNode->data, data);
    newNode->data[MAX_STRING_SIZE - 1] = '\0'; // null-terminate the string

    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}

// function to delete word
void delete(DoublyLinkedList *list, char name[31]) {
    Node *current = list->head;

    while (current != NULL) {
        if (!strcmp(current->data, name)) {
            if (current == list->head) {           // delete first
                current->next->prev = NULL;
                list->head = current->next;
                free(current);
            } else if (current == list->tail) {    // delete last
                current->prev->next = NULL;
                list->tail = current->prev;
                free(current);
            } else {                                // delete from middle
                current->next->prev = current->prev;
                current->prev->next = current->next;
                free(current);
            }


            return;
        }

        current = current->next;
    }

}

// function to insert a node at the end of the list
void insertLast(DoublyLinkedList *list, const char data[]) {
    if(!isalpha(data[0])){
        printf("There are problem in below line, it should start with letter.\n%s\n\n", data);
        return;
    }
    Node *newNode = createNode(data);

    if (list->tail == NULL) {
        // if the list is empty, set both head and tail to the new node
        list->head = newNode;
        list->tail = newNode;
    } else {
        // otherwise, update the links and move the tail to the new node
        newNode->prev = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
    }
}

// function to print the non-sorted elements of the list
void printList(DoublyLinkedList *list) {
    Node *current = list->head;

    printf("List: \n");
    while (current != NULL) {
        printf("%s\n", current->data);
        current = current->next;
    }
}

// function to print the sorted elements of the list
void printSortedList(DoublyLinkedList *sortedStringsList, DoublyLinkedList *stringsList) {
    if (stringsList->head == NULL){
        printf("You should read file before print sorted or your file is empty");
        return;
    }
    else if (sortedStringsList->head == NULL){
        printf("You should make sort before print sorted, please choose third choice to sort");
        return;
    }


    Node *current = sortedStringsList->head;

    printf("List: \n");
    while (current != NULL) {
        printf("%s\n", current->data);
        current = current->next;
    }
}
// function to convert string to length 30 by adding spaces
void convert(char line[31]) {
    unsigned long len = strlen(line);

    int numOfSpaces = MAX_STRING_SIZE - len;
    while (numOfSpaces--)
        strcat(line, " ");

    line[30] = '\0';
}
// function to read data from file
void readFile(DoublyLinkedList *list, const char *filename) {
    FILE *file = fopen(filename, "r");
    char line[31];
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character

        convert(line);
        insertLast(list, line);
    }

    printf("File Loaded successfully!\n");

}

// function to free the memory allocated for the list
void freeList(DoublyLinkedList *list) {
    Node *current = list->head;
    Node *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    // Set the list to an empty state
    list->head = NULL;
    list->tail = NULL;
}

// function for radix sort
DoublyLinkedList radixSort(DoublyLinkedList *list) {
    // create array for each char and initialize all
    DoublyLinkedList radixArray[NUM_OF_CHARACTERS];
    for (int i = 0; i < NUM_OF_CHARACTERS; ++i)
        initializeList(&radixArray[i]);

    // make first iteration of radix sort
    int currentIndex = 29;
    Node *current = list->head;
    while (current != NULL) {
        insertLast(&radixArray[current->data[currentIndex]], current->data);
        current = current->next;
    }

    // make other iterations of radix sort
    while (--currentIndex >= 0) {
        // create new array to move the data that in previous radix array
        DoublyLinkedList newRadixArray[NUM_OF_CHARACTERS];
        for (int i = 0; i < NUM_OF_CHARACTERS; ++i)
            initializeList(&newRadixArray[i]);

        // move the data from previous radix array to new radix array
        for (int i = 0; i < NUM_OF_CHARACTERS; ++i) {
            current = radixArray[i].head;
            while (current != NULL) {
                // insert data to new radix array
                insertLast(&newRadixArray[current->data[currentIndex]], current->data);
                current = current->next;
            }
        }
        // to free the old radix array and move data from new radix array to old radix array.
        // this function to prevent the error "out of memory" in limited pc's
        for (int i = 0; i < NUM_OF_CHARACTERS; ++i) {
            freeList(&radixArray[i]);
            radixArray[i] = newRadixArray[i];
        }

    }

    // fill the final result (sorted string) into doubly linked list
    DoublyLinkedList result;
    initializeList(&result);
    for (int i = 0; i < NUM_OF_CHARACTERS; ++i) {
        current = radixArray[i].head;
        while (current != NULL) {
            insertLast(&result, current->data);
            current = current->next;
        }
    }


    return result;
}

// function to save to the output file
void saveToFile(DoublyLinkedList *list, const char *filename) {
    FILE *out = fopen(filename, "w");
    Node *current = list->head;

    while (current != NULL) {
        fprintf(out, "%s\n", current->data);
        current = current->next;
    }

    fclose(out);
    printf("saved successfully");
}

int main() {

    // Create and initialize a doubly linked list
    DoublyLinkedList stringsList;
    DoublyLinkedList sortedStringsList;

    initializeList(&stringsList);
    initializeList(&sortedStringsList);


    char string[31]; // string to read in it when add or delete

    char choice;

    do {
        printf("\nMenu:\n");
        printf("1. Load the strings\n");
        printf("2. Print the strings before sorting\n");
        printf("3. Sort the strings\n");
        printf("4. Print the sorted strings\n");
        printf("5. Add a new word to the list of sorted strings (and sort it)\n");
        printf("6. Delete a word from the sorted strings\n");
        printf("7. Save to output file\n");
        printf("8. Exit\n");

        printf("Enter your choice:");
        scanf(" %c", &choice);

        switch (choice) {
            case '1': // Load the strings
                readFile(&stringsList, "input.txt");
                break;
            case '2': // Print the strings before sorting
                printList(&stringsList);
                break;
            case '3': // Sort the strings
                freeList(&sortedStringsList);
                sortedStringsList = radixSort(&stringsList);
                printf("Strings sorted successfully!\n");
                break;
            case '4': // Print the sorted strings
                printSortedList(&sortedStringsList, &stringsList);
                break;
            case '5': // Add a new word to the list of sorted strings (and sort it)
                printf("enter name to add it: ");
                fflush(stdin); // as Refresh The console
                gets(string);
                convert(string);
                insertLast(&stringsList, string);
                freeList(&sortedStringsList);
                sortedStringsList = radixSort(&stringsList);
                break;
            case '6': // Delete a word from the sorted strings
                printf("enter name to delete it: ");
                fflush(stdin);
                gets(string);
                convert(string);
                delete(&stringsList, string);
                delete(&sortedStringsList, string);
                break;
            case '7': // Save to output file
                if (sortedStringsList.head != NULL)
                    saveToFile(&sortedStringsList, "output.txt");
                else if (stringsList.head == NULL)
                    printf("You should read file before save to file or your file is empty");
                else
                    printf("You should make sort before save to file, please press 3");

                break;
            case '8': // Exit
                printf("Exiting the program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }

    } while (choice != 8);


    return 0;
}