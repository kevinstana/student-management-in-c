#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "students.h"

Result result;

void printMenu() {
    printf("\n");
    printf("1. Add a new student.\n");
    printf("2. Delete a student by ID.\n");
    printf("3. Find a student by ID.\n");
    printf("4. Update a student by ID.\n");
    printf("5. Print all students.\n");
    printf("6. Quit.\n");
    printf("\n");
}

void printStudent(student s) {
        printf("%-20s %lu\n", s.name, s.id);
}

void printStudents(list l) {
    int i = 0;
    node tmpNode = l->head;
    while (tmpNode != NULL) {
        if (i < 9) {
            printf("%d%-3s ", ++i, ")");
        } else {
            printf("%d%-2s ", ++i, ")");
        }
        printStudent(tmpNode->data);
        tmpNode = tmpNode->next;
    }
}

Result load(char *c, list *l) {
    char line[100];
    int result;
    
    FILE *fp = fopen(c, "r");
    if (fp == NULL) {
        return UNKNOWN_ERR;
    }

    student *s = (student*) malloc(sizeof(student));
    if (s == NULL) {
        return MALLOC_ERR;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strlen(line) < 2 || !isdigit(line[0]))
            continue;

        int maxFname = MAXSTRING - 3;
        int maxLname = MAXSTRING - 3;

        char fname[maxFname];
        char lname[maxLname];

        result = sscanf(line, "%*d) %s %s %lu", fname, lname, &s->id);
        if (result <= 0) {
            return F_READ_ERR;
        }

        strcpy(s->name, fname);
        strcat(s->name, " ");
        strcat(s->name, lname);

        addStudent(*s, *l);
    }

    free(s);
    s = NULL;
    
    if ((*l)->head == NULL) {
        return NON_INIT_LIST;
    }

    if (fclose(fp) == EOF) {
        return UNKNOWN_ERR;
    };

    return NO_ERR;
}

Result save(char *c, list l) {
    FILE *fp = fopen(c, "w");
    if (fp == NULL) {
        return UNKNOWN_ERR;
    }

    size_t result = fprintf(fp, "%-4s %-20s %s\n", "", "Name", "ID");
    if(result < 1) {
        return F_WRITE_ERR;
    }

    int i = 0;
    node currentNode = l->head;

    while(currentNode != NULL) {       
        if (i < 9) {
            result = fprintf(fp, "%d%-3s ", ++i, ")");
            if(result < 1) {
                return F_WRITE_ERR;
            }
        } else {
            result = fprintf(fp, "%d%-2s ", ++i, ")");
            if(result < 1) {
                return F_WRITE_ERR;
            }
        }

        result = fprintf(fp, "%-20s %-20lu\n", currentNode->data.name, currentNode->data.id);
        if(result < 1)
        {
            return F_WRITE_ERR;
        }

        currentNode = currentNode->next;
    }
    
    if (fclose(fp) == EOF) {
        return UNKNOWN_ERR;
    };

    return NO_ERR;
}

Result addStudent(student s, list l) {
    node n = (node) malloc(sizeof(struct nodeR));
    if (n == NULL) {
        return MALLOC_ERR;
    }

    if (s.id == 0) {
        s.id = generateId();
    }

    n->data.id = s.id;
    strcpy(n->data.name, s.name);

    if (l->head == NULL) {
        n->next = NULL;
        n->previous = NULL;
        l->head = n;
        l->tail = n;
    } else {
        n->next = NULL;
        n->previous = l->tail;
        l->tail->next = n;
        l->tail = n;
    }

    return NO_ERR;
}

Result findStudentNode(unsigned long id, list l, node *n) {
    node tmpNode = l->head;
    while (tmpNode != NULL) {
        if (tmpNode->data.id == id) {
            *n = tmpNode;
            return NO_ERR;
        }
        tmpNode = tmpNode->next;
    }

    *n = NULL;
    return SYNTAX_ERR;
}

Result findStudent(unsigned long id, list l, student *s) {
    node tmpNode = l->head;
    while (tmpNode != NULL) {
        if (tmpNode->data.id == id) {
            *s = tmpNode->data;
            return NO_ERR;
        }
        tmpNode = tmpNode->next;
    }
    
    return SYNTAX_ERR;
}

Result deleteStudentById(unsigned long id, list l) {
    node n = l->head;
    Result result = findStudentNode(id, l, &n);
    if (isError(result)) {
        return result;
    }

    if (l->head == l->tail) {
        free(n);
        n = NULL;
        l->head = NULL;
        l->tail = NULL;
        return NO_ERR;
    }

    if (n == l->head) {
        l->head = n->next;
        l->head->previous = NULL;
        free(n);
        n = NULL;
        return NO_ERR;
    }

    if (n == l->tail) {
        l->tail = n->previous;
        l->tail->next = NULL;
        free(n);
        n = NULL;
        return NO_ERR;
    }

    n->previous->next = n->next;
    n->next->previous = n->previous;
    free(n);
    n = NULL;
    return NO_ERR;
}

Result updateStudent(student s, list l) {
    node n = l->head;
    Result result = findStudentNode(s.id, l, &n);
    if (isError(result)) {
        return result;
    }

    strcpy(n->data.name, s.name);
    return NO_ERR;
}

int isError(Result result) {
    if (result != NO_ERR) {
        return TRUE;
    }

    return FALSE;
}

unsigned long generateId() {
    unsigned long seed = time(NULL);
    srand(seed);
    
    return (unsigned long)rand();
}
