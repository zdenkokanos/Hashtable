#include <stdio.h>
#include <stdlib.h>

#define N 100

typedef struct node {
    char balance[50];
    char searchBy[100];
    struct node *next;
} NODE;

int string_length(char *string) {
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

int compareStrings(char *string1, char *string2) {
    int i = 0;
    while (string1[i] != '\0' || string2[i] != '\0') {
        if (string1[i] != string2[i]) {
            return 1;
        }
    }
    return 0;
}

char *remove_spaces(char *firstname, char *surname, char *date) {
    int i = 0;
    int j = 0;
    char string[50];
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
    printf("\n%s\n", string);
    return string;
}

NODE *search(long long key, NODE **array, char *searchBy) {
    NODE *current = array[key];
    if (compareStrings(current->searchBy, searchBy) == 0) {
        printf("%s", current->balance);
    } else {
        while (compareStrings(current->searchBy, searchBy) != 0) {
            current = current->next;
        }
        printf("%s", current->balance);
    }
    return current;
}

void delete() {

}

unsigned long long int hash(char *string) {
    unsigned long long int result = 0;
    for (int i = 0; i < string_length(string); i++) {
        result = 32 * result + string[i];
    }
    return result % N;
}

double getFloat(char *balance) {
    int i = 0;
    double result;
    while (balance[i] != ',') {
        i++;
    }
    balance[i] = '.';
    sscanf(balance, "%lf", result);
    return result;
}


void update(char *balance, char *balanceDifference, NODE **array) {
    double balanceInDouble = getFloat(balance);
    double balanceDifferenceDouble = getFloat(balanceDifference);
    double result = balance - balanceDifference;
    if (result < 0) {
        printf("update failed");
    } else {

    }
}

void insert(long long key, NODE **array, char *balance, char *searchBy) {
    NODE *newNode = (NODE *) malloc(sizeof(NODE));
    toString(newNode->balance, balance);
    toString(newNode->searchBy, searchBy);
    newNode->next = NULL;
    NODE *current = array[key];
    if (current != NULL) {
        //collisions are dealt with chaining
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    } else {
        array[key] = newNode;
    }

}


void print(NODE **array) {
    for (int i = 0; i < N; i++) {
        if (array[i] != NULL) {
            printf("%s\n", array[i]->balance);
        }
    }
}


int main() {
    char input;
    char balance[15];
    char date[12];
    char firstname[15];
    char surname[15];
    char *searchBy;
    NODE **array = (NODE **) malloc(N * sizeof(NODE *));

    while (scanf(" %c", &input) == 1) {
        switch (input) {
            case 's':
                scanf("%s %s %s", firstname, surname, date);
                searchBy = remove_spaces(firstname, surname, date);
                search(hash(searchBy), array, searchBy);
                break;
            case 'd':
                break;
            case 'i':
                scanf("%s %s %s %s", firstname, surname, date, balance);
                searchBy = remove_spaces(firstname, surname, date);
                insert(hash(searchBy), array, balance, searchBy);
                break;
            case 'u':
                scanf("%s %s %s %s", firstname, surname, date, balance);
                searchBy = remove_spaces(firstname, surname, date);
                break;
            case 'p':
                print(array);
            default:
                break;
        }
    }

    return 0;
}
