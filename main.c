#include <stdio.h>
#include <stdlib.h>

#define N 10000
#define M 31


typedef struct person
{
    char firstname[20];
    char lastname[20];
    char date[12];
    int eur;
    int centy;
} PERSON;

typedef struct node
{
    PERSON person;
    struct node *next;
} NODE;

int string_length(const char *string)
{
    int i;
    for (i = 0; string[i] != '\0'; i++);
    return i;
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


int search(int key, NODE **array, int *printed, char *firstname, char *surname, char *date)
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
                printf("%d,%02d",current->person.eur, current->person.centy);
                *printed = 1;
            }
            else
            {
                printf("\n%d,%02d", current->person.eur, current->person.centy);
            }
            return 0;
        }
        current = current->next;
    }

    return 1;
}

int delete(int key, NODE **array, char *firstname, char *surname, char *date)
{
    NODE *current = array[key];
    NODE *previous = NULL;

    while (current != NULL)
    {
        if (compareStrings(current->person.firstname, firstname) == 0 &&
            compareStrings(current->person.lastname, surname) == 0 &&
            compareStrings(current->person.date, date) == 0)
        {

            if (current == array[key])
            {
                array[key] = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            free(current);
            return 0;
        }
        previous = current;
        current = current->next;
    }

    return 1;
}

int hash(char *firstname, char *lastname, char *date)
{
    int result = 0;
    for (int i = 0; i < string_length(firstname); i++)
    {
        result = M * result + firstname[i];
    }
    for (int i = 0; i < string_length(lastname); i++)
    {
        result = M * result + lastname[i];
    }
    for (int i = 0; i < string_length(date) + 1; i++)
    {
        result = M * result + date[i];
    }
    if (result < 0)
    {
        result *= -1;
    }
    return result % N;
}


int update(int eur, int centy, NODE **array, unsigned long long key, char *firstname, char *surname, char *date)
{
    NODE *current = array[key];
    while (current != NULL)
    {
        if (compareStrings(current->person.firstname, firstname) == 0 &&
            compareStrings(current->person.lastname, surname) == 0 &&
            compareStrings(current->person.date, date) == 0)
        {
            if(eur<0){
                centy*=-1;
            }
            int newCenty = current->person.centy + centy;
            int newEur = current->person.eur + eur;

            if (newCenty >= 100)
            {
                newCenty -= 100;
                newEur++;
            }
            else if (newCenty < 0)
            {
                newCenty += 100;
                newEur--;
            }

            if (newEur >= 0)
            {
                current->person.eur = newEur;
                current->person.centy = newCenty;
                return 0;
            }
        }
        current = current->next;
    }

    return 1;
}


int insert(int key, NODE **array, int eur, int centy, char *firstname, char *surname, char *date)
{
    if (eur < 0)
    {
        return 1;
    }


    NODE *current = array[key];
    while (current != NULL)
    {
        if (compareStrings(current->person.firstname, firstname) == 0 &&
            compareStrings(current->person.lastname, surname) == 0 &&
            compareStrings(current->person.date, date) == 0)
        {
            return 1;
        }
        current = current->next;
    }


    NODE *newNode = (NODE *) malloc(sizeof(NODE));
    if (newNode == NULL)
    {
        return 1;
    }


    newNode->person.eur = eur;
    newNode->person.centy = centy;
    toString(newNode->person.firstname, firstname);
    toString(newNode->person.lastname, surname);
    toString(newNode->person.date, date);
    newNode->next = NULL;


    if (array[key] == NULL)
    {
        array[key] = newNode;
    }
    else
    {
        newNode->next = array[key];
        array[key] = newNode;
    }

    return 0;
}


int main()
{
    char input;
    int eur;
    char date[12];
    char firstname[20];
    char surname[20];
    int centy;
    int printed = 0;
    NODE **array = (NODE **) malloc(N * sizeof(NODE *));

    while (scanf("%c", &input) == 1)
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
                scanf("%s %s %s %d,%d", firstname, surname, date, &eur, &centy);

                if (insert(hash(firstname, surname, date), array, eur,centy, firstname, surname, date) == 1)
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
                scanf("%s %s %s %d,%d", firstname, surname, date, &eur,&centy);
                if (update(eur,centy, array, hash(firstname, surname, date), firstname, surname, date) == 1)
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