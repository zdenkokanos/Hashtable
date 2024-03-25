#include <stdio.h>
#include <stdlib.h>

#define N 10000

typedef struct node {
    int balance;
    char searchBy[100];
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

double getFloat(char *balance) {
    int i = 0;
    double result;
    while (balance[i] != ',') {
        i++;
    }
    balance[i] = '.';
    sscanf(balance, "%lf", &result);
    return result;
}

int toInt(char *balance){
    return getFloat(balance)*100;
}

int compareStrings(char *string1, char *string2) {
    int i = 0;
    while (string1[i] != '\0' || string2[i] != '\0') {
        if (string1[i] != string2[i]) {
            return 1;
        }
        i++;
    }
    return 0;
}

char *remove_spaces(char *firstname, char *surname, char *date) {
    int i = 0;
    int j = 0;
    int total_length = string_length(firstname) + string_length(surname) + string_length(date);
    char *string = (char *)malloc((total_length + 1) * sizeof(char));
    while (firstname[i] != '\0') {
        string[i] = firstname[i];
        i++;
    }
    while (surname[j] != '\0') {
        string[i] = surname[j];
        i++;
        j++;
    }
    j = 0;
    while (date[j] != '\0') {
        string[i] = date[j];
        i++;
        j++;
    }
    string[i] = '\0';
    return string;
}

NODE *search(long long key, NODE **array, char *searchBy, int print, int *printed) {
    NODE *current = array[key];
    if (current == NULL) {
        return NULL;
    }
    if (compareStrings(current->searchBy, searchBy) == 0) {
        if(print==0){
            if(*printed==0){
                printf("%d,%02d", current->balance/100,current->balance % 100);
                *printed = 1;
            }
            else{
                printf("\n%d,%02d", current->balance/100,current->balance % 100);
            }
        }
    }
    else {
        while (compareStrings(current->searchBy, searchBy) != 0 && current!=NULL) {
            current = current->next;
        }
        if (current != NULL && print == 0){
            if(*printed==0){
                printf("%d,%02d", current->balance/100,current->balance%100);
                *printed = 1;
            }
            else{
                printf("\n%d,%02d", current->balance/100,current->balance%100);
            }
        }
    }
    return current;
}

void delete(NODE* toDelete) {
    if (toDelete->previous != NULL) {
        toDelete->previous->next = toDelete->next;
    }
    if (toDelete->next != NULL) {
        toDelete->next->previous = toDelete->previous;
    }
    free(toDelete);
}

unsigned long long int hash(char *string) {
    unsigned long long int result = 0;
    for (int i = 0; i < string_length(string); i++) {
        result = 31 * result + string[i];
    }
    return result % N;
}


void update(int balanceDifference, NODE *toUpdate) {
    int newBalance = toUpdate->balance + balanceDifference;
    if (newBalance < 0) {
        printf("\nupdate failed");
    } else {
        toUpdate->balance = newBalance;
    }
}

void insert(long long key, NODE **array, int balance, char *searchBy) {
    NODE *newNode = (NODE *) malloc(sizeof(NODE));
    newNode->balance =  balance;
    toString(newNode->searchBy, searchBy);
    newNode->next = NULL;
    newNode->previous = NULL;
    NODE *current = array[key];
    if (current != NULL) {
        //collisions are dealt with chaining
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
        current->next->previous = current;
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
    char *searchBy;
    int printed = 0;
    NODE **array = (NODE **) malloc(N * sizeof(NODE *));

    while (scanf(" %c", &input) == 1) {
        switch (input) {
            case 's':
                scanf("%s %s %s", firstname, surname, date);
                searchBy = remove_spaces(firstname, surname, date);
                if(search(hash(searchBy), array, searchBy,0, &printed)==NULL){
                    printf("\nsearch failed");
                }
                free(searchBy);
                break;
            case 'd':
                scanf("%s %s %s", firstname, surname, date);
                searchBy = remove_spaces(firstname, surname, date);
                delete(search(hash(searchBy), array, searchBy,1, &printed));
                free(searchBy);
                break;
            case 'i':
                scanf("%s %s %s %s", firstname, surname, date, balance);
                searchBy = remove_spaces(firstname, surname, date);
                insert(hash(searchBy), array, toInt(balance), searchBy);
                free(searchBy);
                break;
            case 'u':
                scanf("%s %s %s %s", firstname, surname, date, balance);
                searchBy = remove_spaces(firstname, surname, date);
                NODE *toUpdate = search(hash(searchBy), array, searchBy,1, &printed);
                if(toUpdate == NULL){
                    printf("\nupdate failed");
                }
                else{
                    update(toInt(balance), toUpdate);
                }
                free(searchBy);
                break;
            default:
                break;
        }
    }

    return 0;
}