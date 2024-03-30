#include <stdio.h>
#include <stdlib.h>

#define N 10000
#define M 33


typedef struct person
{
    char firstname[26];
    char lastname[26];
    char date[12];
    int balance;
} PERSON;

typedef struct node
{
    PERSON person;
    struct node *next;
    struct node *previous;
} NODE;

int string_length(const char *string)
{
    int i;
    for (i = 0; string[i] != '\0'; i++);
    return i;
}

char *floatToString(float balance)
{
    char *string = (char *)malloc(20 * sizeof(char));
    if (string == NULL) {
        return NULL;
    }

    sprintf(string, "%.2f", balance);

    for (int i = 0; string[i] != '\0'; i++) {
        if (string[i] == '.') {
            string[i] = ',';
            break;
        }
    }
    return string;
}

int toInt(float balance){
    return (balance)*100;
}

void toString(char *N_string, const char *string)
{
    int i;
    for (i = 0; string[i] != '\0'; i++)
    {
        N_string[i] = string[i];
    }
    N_string[i] = '\0';
}

void freeNodes(NODE **array)
{
    for (int i = 0; i < N; i++)
    {
        NODE *current = array[i];
        while (current != NULL)
        {
            NODE *temp = current;
            if (current != NULL)
            {
                free(temp);
            }
            current = current->next;
        }
        array[i] = NULL;
    }
    free(array);
}

float getFloat(char *balance) {
    int i = 0;
    float result;
    while (balance[i] != ',') {
        i++;
    }
    balance[i] = '.';
    sscanf(balance, "%f", &result);

    return result;
}

int compareStrings(char *string1, char *string2)
{
    int i = 0;
    if (string_length(string1) != string_length(string2))
    {
        return 1;
    }
    while (string1[i] != '\0' && string2[i] != '\0')
    {
        if (string1[i] != string2[i])
        {
            return 1;
        }
        i++;
    }
    return 0;
}


int search(unsigned long long key, NODE **array, int *printed, char *firstname, char *surname, char *date)
{
    NODE *current = array[key];
    if (current == NULL)
    {
        return 1;
    }
    while (current != NULL)
    {
        if (compareStrings(current->person.firstname, firstname) == 0 &&
            compareStrings(current->person.lastname, surname) == 0 &&
            compareStrings(current->person.date, date) == 0)
        {

            if (*printed == 0)
            {
                printf("%d,%02d", current->person.balance/100,current->person.balance%100);
                *printed = 1;
            }
            else
            {
                printf("\n%d,%02d", current->person.balance/100,current->person.balance%100);
            }
            return 0;
        }
        current = current->next;
    }

    return 1;
}


int delete(unsigned long long key, NODE **array, char *firstname, char *surname, char *date)
{
    NODE *current = array[key];
    if (array[key] == NULL)
    {
        return 1;
    }
    else if (compareStrings(current->person.firstname, firstname) == 0 &&
             compareStrings(current->person.lastname, surname) == 0 &&
             compareStrings(current->person.date, date) == 0 && current->next != NULL)
    {
        array[key] = current->next;
        array[key]->previous = NULL;
        free(current);
        return 0;
    }
    else
    {
        while ((compareStrings(current->person.firstname, firstname) != 0 ||
                compareStrings(current->person.lastname, surname) != 0 ||
                compareStrings(current->person.date, date) != 0) && current->next != NULL)
        {
            current = current->next;
        }
        if (compareStrings(current->person.firstname, firstname) == 0 &&
            compareStrings(current->person.lastname, surname) == 0 &&
            compareStrings(current->person.date, date) == 0)
        {
            if (current->previous != NULL)
            {
                if (current->next != NULL)
                {
                    current->next->previous = current->previous;
                }
                current->previous->next = current->next;
            }
            free(array[key]);
            array[key] = NULL;
            return 0;
        }
    }
    return 1;
}

unsigned long long hash(char *firstname, char *lastname, char *date)
{
    unsigned long long result = 0;
    for (int i = 0; i < string_length(firstname); i++)
    {
        result = M * result + firstname[i];
    }
    for (int i = 0; i < string_length(lastname); i++)
    {
        result = M * result + lastname[i];
    }
    for (int i = 0; i < string_length(date); i++)
    {
        result = M * result + date[i];
    }
    return result % N;
}


int update(int balanceDifference, NODE **array, unsigned long long key, char *firstname, char *surname, char *date)
{
    NODE *current = array[key];

    while (current != NULL)
    {
        if (compareStrings(current->person.firstname, firstname) == 0 &&
            compareStrings(current->person.lastname, surname) == 0 &&
            compareStrings(current->person.date, date) == 0)
        {

            int newBalance = current->person.balance + balanceDifference;
            if (newBalance < 0)
            {
                return 1;
            }
            current->person.balance = newBalance;
            return 0;
        }
        current = current->next;
    }

    return 1;
}

int insert(unsigned long long key, NODE **array, int balance, char *firstname, char *lastname, char *date)
{
    if (balance < 0)
    {
        return 1;
    }

    NODE *newNode = (NODE *) malloc(sizeof(NODE));
    if (newNode == NULL)
    {
        return 1;
    }

    newNode->person.balance = balance;
    toString(newNode->person.firstname, firstname);
    toString(newNode->person.lastname, lastname);
    toString(newNode->person.date, date);
    newNode->next = NULL;
    newNode->previous = NULL;

    NODE *current = array[key];
    if (current != NULL)
    {
        while (current != NULL)
        {
            if (compareStrings(current->person.firstname, firstname) == 0 &&
                compareStrings(current->person.lastname, lastname) == 0 &&
                compareStrings(current->person.date, date) == 0)
            {
                free(newNode);
                return 1;
            }
            current = current->next;
        }

        current = array[key];
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
        newNode->previous = current;
    }
    else
    {
        array[key] = newNode;
    }

    return 0;
}


int main()
{
    char input;
    char balance[20];
    char date[12];
    char firstname[26];
    char surname[26];
    int printed = 0;
    NODE **array = (NODE **) malloc(N * sizeof(NODE *));

    while (scanf(" %c", &input) == 1)
    {
        switch (input)
        {
            case 's':
                scanf("%s %s %s", firstname, surname, date);
                if (search(hash(firstname, surname, date), array, &printed, firstname, surname, date) == 1)
                {
                    if (printed == 0)
                    {
                        printf("search failed");
                        printed = 1;
                    }
                    else
                    {
                        printf("\nsearch failed");
                    }
                }
                break;
            case 'd':
                scanf("%s %s %s", firstname, surname, date);
                if (delete(hash(firstname, surname, date), array, firstname, surname, date) == 1)
                {
                    if (printed == 0)
                    {
                        printf("delete failed");
                        printed = 1;
                    }
                    else
                    {
                        printf("\ndelete failed");
                    }
                    break;
                }
                break;
            case 'i':
                scanf("%s %s %s %s", firstname, surname, date, balance);

                if (insert(hash(firstname, surname, date), array, toInt(getFloat(balance)), firstname, surname, date) == 1)
                {
                    if (printed == 0)
                    {
                        printf("insert failed");
                        printed = 1;
                    }
                    else
                    {
                        printf("\ninsert failed");
                    }
                    break;
                }

                break;
            case 'u':
                scanf("%s %s %s %s", firstname, surname, date, balance);
                if (update(toInt(getFloat(balance)), array, hash(firstname, surname, date), firstname, surname, date) == 1)
                {
                    if (printed == 0)
                    {
                        printf("update failed");
                        printed = 1;
                    }
                    else
                    {
                        printf("\nupdate failed");
                    }
                }
                break;
            default:
                break;
        }
    }
    freeNodes(array);
    return 0;
}