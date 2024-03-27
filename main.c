#include <stdio.h>
#include <stdlib.h>

#define N 10000

//ked insertujem check ci tam uz nie je

typedef struct person {
    char firstname[20];
    char lastname[20];
    char date[13];
    int balance;
} PERSON;

typedef struct node {
    PERSON *person;
    struct node *next;
    struct node *previous;
} NODE;

int string_length(const char *string) {
    int i;
    for (i = 0; string[i] != '\0'; i++);
    return i;
}

void toString(char *N_string, char *string) {
    int i;
    for (i = 0; string[i] != '\0'; i++) {
        N_string[i] = string[i];
    }
    N_string[i] = '\0';
}

void freeNodes(NODE **array) {
    for (int i = 0; i < N; i++) {
        NODE *current = array[i];
        while (current != NULL) {
            NODE *temp = current;
            current = current->next;
            if(current!=NULL){
                free(temp->person);
                free(temp);
            }
        }
        array[i] = NULL;
    }
}

int toInt(const char *balance) {
    int result = 0;
    int i = 0;
    int minus = 1;
    if (balance[i] == '-') {
        i++;
        minus = -1;
    }
    while (balance[i] != '\0') {
        if (balance[i] == ',') {
            i++;
            continue;
        } else {
            result = result * 10 + (balance[i] - '0');
        }
        i++;
    }
    return minus * result;
}

int compareStrings(char *string1, char *string2) {
    int i = 0;
    if (string_length(string1) != string_length(string2)) {
        return 1;
    }
    while (string1[i] != '\0' || string2[i] != '\0') {
        if (string1[i] != string2[i]) {
            return 1;
        }
        i++;
    }
    return 0;
}


NODE *search(unsigned int key, NODE **array, int print, int *printed, char *firstname, char *surname, char *date) {
    NODE *current = array[key];
    if (current == NULL) {
        return NULL;
    }
    if (compareStrings(current->person->firstname, firstname) == 0 &&
        compareStrings(current->person->lastname, surname) == 0 &&
        compareStrings(current->person->date, date) == 0) {
        if (print == 0) {
            if (*printed == 0) {
                printf("%d,%02d", current->person->balance / 100, current->person->balance % 100);
                *printed = 1;
            } else {
                printf("\n%d,%02d", current->person->balance / 100, current->person->balance % 100);
            }
        }
    }
    else {
        while ((compareStrings(current->person->firstname, firstname) == 0 &&
                compareStrings(current->person->lastname, surname) == 0 &&
                compareStrings(current->person->date, date) == 0) && current != NULL) {
            current = current->next;
        }
        if (current != NULL && print == 0) {
            if (*printed == 0) {
                printf("%d,%02d", current->person->balance / 100, current->person->balance % 100);
                *printed = 1;
            } else {
                printf("\n%d,%02d", current->person->balance / 100, current->person->balance % 100);
            }
        }
    }
    return current;
}

void delete(NODE *toDelete) {
    if (toDelete->previous != NULL) {
        toDelete->previous->next = toDelete->next;
    }
    if (toDelete->next != NULL) {
        toDelete->next->previous = toDelete->previous;
    }
    if(toDelete!=NULL){
        //free(toDelete->person);
        //free(toDelete);
    }
    toDelete = NULL;
}

unsigned int hash(char *firstname, char *lastname, char *date) {
    unsigned int result = 0;
    for (int i = 0; i < string_length(firstname); i++) {
        result = 33 * result + firstname[i];
    }
    for (int i = 0; i < string_length(lastname); i++) {
        result = 33 * result + lastname[i];
    }
    for (int i = 0; i < string_length(date); i++) {
        result = 33 * result + date[i];
    }
    return result % N;
}


void update(int balanceDifference, NODE *toUpdate) {
    int newBalance = toUpdate->person->balance + balanceDifference;
    if (newBalance < 0) {
        printf("\nupdate failed");
    } else {
        toUpdate->person->balance = newBalance;
    }
}

void insert(unsigned int key, NODE **array, int balance, char *firstname, char *lastname, char *date) {
    NODE *newNode = (NODE *) malloc(sizeof(NODE));
    PERSON *newPerson = (PERSON *) malloc(sizeof(PERSON));
    newPerson->balance = balance;
    toString(firstname, newPerson->firstname);
    toString(lastname, newPerson->lastname);
    toString(date, newPerson->date);
    newNode->person = newPerson;
    newNode->next = NULL;
    newNode->previous = NULL;
    NODE *current = array[key];
    if (current != NULL) {
        while (current != NULL) {
            if (compareStrings(current->person->firstname, firstname) == 0 &&
                compareStrings(current->person->lastname, lastname) == 0 &&
                compareStrings(current->person->date, date) == 0) {
                printf("\ninsert failed");
                free(newPerson);
                free(newNode);
                return;
            }
            current = current->next;
        }
        current = array[key];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
        newNode->previous = current;
    } else {
        array[key] = newNode;
    }
}


int main() {
    char input;
    char balance[15];
    char date[12];
    char firstname[15];
    char surname[15];
    int printed = 0;
    NODE *array[N] = {NULL};

    while (scanf(" %c", &input) == 1) {
        switch (input) {
            case 's':
                scanf("%s %s %s", firstname, surname, date);
                if (search(hash(firstname, surname, date), array, 0, &printed, firstname,surname, date) == NULL) {
                    printf("\nsearch failed");
                }
                break;
            case 'd':
                scanf("%s %s %s", firstname, surname, date);
                NODE *toDelete = search(hash(firstname, surname, date), array, 1, &printed, firstname, surname, date);
                if (toDelete != NULL) {
                    delete(toDelete);
                } else {
                    printf("\ndelete failed");
                }
                break;
            case 'i':
                scanf("%s %s %s %s", firstname, surname, date, balance);
                insert(hash(firstname, surname, date), array, toInt(balance), firstname, surname, date);
                break;
            case 'u':
                scanf("%s %s %s %s", firstname, surname, date, balance);
                NODE *toUpdate = search(hash(firstname, surname, date), array, 1, &printed, firstname, surname,date);
                if (toUpdate == NULL) {
                    printf("\nupdate failed");
                } else {
                    update(toInt(balance), toUpdate);
                }
                break;
            default:
                break;
        }
    }
    freeNodes(array);
    return 0;
}