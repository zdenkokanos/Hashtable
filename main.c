#include <stdio.h>
#include <stdlib.h>

#define N 10000

//ked insertujem check ci tam uz nie je

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

void toString(char *N_string, char *string)
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
            current = current->next;
            if (current != NULL)
            {
                free(temp);
            }
        }
        array[i] = NULL;
    }
    free(array);
}

int toInt(const char *balance)
{
    int result = 0;
    int i = 0;
    int minus = 1;
    if (balance[i] == '-')
    {
        i++;
        minus = -1;
    }
    while (balance[i] != '\0')
    {
        if (balance[i] == ',')
        {
            i++;
            continue;
        }
        else
        {
            result = result * 10 + (balance[i] - '0');
        }
        i++;
    }
    return minus * result;
}

int compareStrings(char *string1, char *string2)
{
    int i = 0;
    if (string_length(string1) != string_length(string2))
    {
        return 1;
    }
    while (string1[i] != '\0' || string2[i] != '\0')
    {
        if (string1[i] != string2[i])
        {
            return 1;
        }
        i++;
    }
    return 0;
}


NODE *search(unsigned int key, NODE **array, int print, int *printed, char *firstname, char *surname, char *date)
{
    int found = 1;
    NODE *current = array[key];
    if (current == NULL)
    {
        return NULL;
    }
    if (compareStrings(current->person.firstname, firstname) == 0 &&
        compareStrings(current->person.lastname, surname) == 0 &&
        compareStrings(current->person.date, date) == 0)
    {
        if (print == 0)
        {
            if (*printed == 0)
            {
                printf("%d,%02d", current->person.balance / 100, current->person.balance % 100);
                *printed = 1;
            }
            else
            {
                printf("\n%d,%02d", current->person.balance / 100, current->person.balance % 100);
            }
        }
        found = 0;
    }
    else
    {
        while ((compareStrings(current->person.firstname, firstname) != 0 &&
                compareStrings(current->person.lastname, surname) != 0 &&
                compareStrings(current->person.date, date) != 0) && current->next != NULL)
        {
            current = current->next;
        }
        if (compareStrings(current->person.firstname, firstname) == 0 &&
            compareStrings(current->person.lastname, surname) == 0 &&
            compareStrings(current->person.date, date) == 0)
        {
            if (print == 0)
            {
                if (*printed == 0)
                {
                    printf("%d,%02d", current->person.balance / 100, current->person.balance % 100);
                    *printed = 1;
                }
                else
                {
                    printf("\n%d,%02d", current->person.balance / 100, current->person.balance % 100);
                }
            }
            found = 0;
        }
    }
    if (found == 0)
    {
        return current;
    }
    else
    {
        return NULL;
    }
}

void delete(NODE *toDelete)
{
    if (toDelete->previous != NULL)
    {
        toDelete->previous->next = toDelete->next;
    }
    if (toDelete->next != NULL)
    {
        toDelete->next->previous = toDelete->previous;
    }
    if (toDelete != NULL)
    {
        //free(toDelete);
    }
    toDelete = NULL;
}

unsigned int hash(char *firstname, char *lastname, char *date)
{
    unsigned int result = 0;
    for (int i = 0; i < string_length(firstname); i++)
    {
        result = 33 * result + firstname[i];
    }
    for (int i = 0; i < string_length(lastname); i++)
    {
        result = 33 * result + lastname[i];
    }
    for (int i = 0; i < string_length(date); i++)
    {
        result = 33 * result + date[i];
    }
    return result % N;
}


void update(int balanceDifference, NODE *toUpdate, int *printed)
{
    int newBalance = toUpdate->person.balance + balanceDifference;
    if (newBalance < 0)
    {
        if (*printed == 0)
        {
            printf("update failed");
            *printed = 1;
        }
        else
        {
            printf("\nupdate failed");
        }
    }
    else
    {
        toUpdate->person.balance = newBalance;
    }
}

void insert(unsigned int key, NODE **array, int balance, char *firstname, char *lastname, char *date, int *printed)
{
    if (balance < 0)
    {
        if (*printed == 0)
        {
            printf("insert failed");
            *printed = 1;
        }
        else
        {
            printf("\ninsert failed");
        }
        return;
    }
    NODE *newNode = (NODE *) malloc(sizeof(NODE));
    if (newNode == NULL)
    {
        printf("\nMemory failed");
        return;
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
                if (*printed == 0)
                {
                    printf("insert failed");
                    *printed = 1;
                }
                else
                {
                    printf("\ninsert failed");
                }
                free(newNode);
                return;
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
                if (scanf("%s %s %s", firstname, surname, date) != 3)
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
                    break;
                }
                if (search(hash(firstname, surname, date), array, 0, &printed, firstname, surname, date) == NULL)
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
                if (scanf("%s %s %s", firstname, surname, date) != 3)
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
                NODE *toDelete = search(hash(firstname, surname, date), array, 1, &printed, firstname, surname, date);
                if (toDelete != NULL)
                {
                    delete(toDelete);
                }
                else
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
                }
                break;
            case 'i':
                if (scanf("%s %s %s %s", firstname, surname, date, balance) != 4)
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
                insert(hash(firstname, surname, date), array, toInt(balance), firstname, surname, date, &printed);
                break;
            case 'u':
                if (scanf("%s %s %s %s", firstname, surname, date, balance) != 4)
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
                    break;
                }
                NODE *toUpdate = search(hash(firstname, surname, date), array, 1, &printed, firstname, surname, date);
                if (toUpdate == NULL)
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
                else
                {
                    update(toInt(balance), toUpdate, &printed);
                }
                break;
            default:
                break;
        }
    }
    freeNodes(array);
    return 0;
}