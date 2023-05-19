#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "students.h"
#include <time.h>

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
    node tmpNode = l->head;
    while (tmpNode != NULL) {
        printStudent(tmpNode->data);
        tmpNode = tmpNode->next;
    }
}

Result load(char *c, list *l) {
    return NO_ERR;
}

Result save(char *c, list l) {
    return NO_ERR;
}

Result addStudent(student s, list l) {
    node n = (node) malloc(sizeof(struct nodeR));
    if (n == NULL) {
        return MALLOC_ERR;
    }

    s.id = generateId();
    n -> data = s;
    n -> next = NULL;

    if (l->head == NULL) {
        l -> head = n;
        l -> tail = n;
    } else {
        l->tail->next = n;
        n->previous = l->tail;
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

    *n =NULL;
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

    s->id = -1;
    return SYNTAX_ERR;
}

Result deleteStudentById(unsigned long id, list l) {
    node tmpNode = l->head;
    int found = 0;
    while (tmpNode != NULL) {
        if (tmpNode->data.id == id) {
            found = 1;
            break;
        }
        tmpNode = tmpNode->next;
    }

    if (found == 1) {
        if (l->head == l->tail) {
            l->head = NULL;
            l->tail = NULL;
            free(tmpNode);
            return NO_ERR;  
        }
        if (l->head == tmpNode) {
            l->head = tmpNode->next;
            l->head->previous = NULL;
            free(tmpNode);
            return NO_ERR;
        }

        if (l->tail == tmpNode) {
            l->tail = tmpNode->previous;
            l->tail->next = NULL;
            free(tmpNode);
            return NO_ERR;
        }

        tmpNode->previous->next = tmpNode->next;
        tmpNode->next->previous = tmpNode->previous;
        free(tmpNode);
        return NO_ERR;
    }

    return SYNTAX_ERR;
}

Result updateStudent(student s, list l) {
    node n = (node) malloc(sizeof(struct nodeR));
    if (n == NULL) {
        return MALLOC_ERR;
    } 

    int result = findStudentNode(s.id, l, &n);
    if (result == NO_ERR) {
        n->data = s;
    }

    int isSameName = strcmp(n->data.name, s.name);
    if (isSameName == 0) {
        return NO_ERR;
    } else {
        return UNKNOWN_ERR;
    }
}

int isError(Result result) {
    return 0;
}

unsigned long generateId() {
    /*
        Χρησιμοποίησα το χρόνο σαν σπόρο γιατί στο μάθημα "Προσωμοίωση" μας είπαν 
        ότι αυτός είναι ένας καλός τρόπος για δημιουργία ψευδοτυχαίων αριθμών.
        Τον κώδικα όμως τον πήρα απ'το ChatGPT. 
    */
    unsigned long seed = time(NULL);
    srand(seed);
    
    return (unsigned long)rand();
}
