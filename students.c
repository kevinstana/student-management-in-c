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
    int i = 0;
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

        char fname[MAXSTRING - 4];
        char lname[MAXSTRING - 4];
        char fullName[31]; // Είχε θέμα o compiler χωρίς αυτό (fullName), έλεγε ότι πάω να βάλω στο s.name που έχει μέγεθος 20 το lname
                           // με το fname που έχουν μέγεθος 16 το καθένα (σύνολο 32, 31 βασικά χωρίς το '\0').
                           // Αρχικά δεν έβαλα το fullName με το σκεπτικό ότι έχω κάνει ελέγχους στην είσοδο, οπότε ξέρω το πιθανό max μήκος 
                           // του input (19 χαρακτήρες συνολικά το όνομα, 1 πιάνει το κενό που δεν το παίρνει σαν string η sscanf από κάτω, 
                           // και τουλάχιστον 2 χαρακτήρες το fname ή lname οπότε 16 για το υπόλοιπο) και το concatenation τους 
                           // θα χόραγε στο s.name
        sscanf(line, "%*d) %s %s %lu", fname, lname, &s->id);
        result = snprintf(fullName, sizeof(fullName), "%s %s", fname, lname);
        if (result == 0) {
            return F_READ_ERR;
        }

        if (strlen(fullName) < 20) {
            strcpy(s->name, fullName);
        }

        addStudent(*s, *l);
    }

    free(s);
    s = NULL; // https://stackoverflow.com/questions/1025589/setting-variable-to-null-after-free
    
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

    int result = fprintf(fp, "%-4s %-20s %s\n", "", "Name", "ID");
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

        // Είδα τον τρόπο που γίνεται αυτό με την fwrite στις διαφάνειες για I/O στη σελίδα 36 αλλά ήθελα να φαίνονται κάπως πιο ωραία
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

    if (l->head == NULL) { // Προσθήκη στην αρχή της λίστας
        n->next = NULL;
        n->previous = NULL;
        l->head = n;
        l->tail = n;
    } else { // Προσθήκη στο τέλος της λίστας
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

    // s->id = -1; // Το id είναι unsgigned long, δεν μπορεί να γίνει -1, γιατί θα πάρει πολύ μεγάλη τιμή
    return SYNTAX_ERR;
}

Result deleteStudentById(unsigned long id, list l) {
    node n = l->head;
    Result result = findStudentNode(id, l, &n);
    if (isError(result)) {
        return result;
    }

    if (l->head == l->tail) { // Αν η λίστα έχει μόνο 1 κόμβο
        free(n);
        n = NULL;
        l->head = NULL;
        l->tail = NULL;
        return NO_ERR;
    }

    if (n == l->head) { // Αν ο κόμβος προς διαγραφή είναι στην αρχή
        l->head = n->next;
        l->head->previous = NULL;
        free(n);
        n = NULL;
        return NO_ERR;
    }

    if (n == l->tail) { // Αν ο κόμβος προς διαγραφή είναι στο τέλος
        l->tail = n->previous;
        l->tail->next = NULL;
        free(n);
        n = NULL;
        return NO_ERR;
    }

    // Αν ο κόμβος προς διαγραφή είναι μεταξύ δύο άλλων κόμβων
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
    /*
        Χρησιμοποίησα το χρόνο σαν σπόρο γιατί στο μάθημα "Προσωμοίωση" μας είπαν 
        ότι αυτός είναι ένας καλός τρόπος για δημιουργία ψευδοτυχαίων αριθμών.
        Τον κώδικα όμως τον πήρα απ'το ChatGPT.
    */
    unsigned long seed = time(NULL);
    srand(seed);
    
    return (unsigned long)rand();
}
