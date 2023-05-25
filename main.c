#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "students.h"

// Μέθοδος για έλεγχο ονόματος που εισάγει ο χρήστης
int validateNameInput(char *nameArray) {
    int isInputTooLong = 0;
    int spaceCounter = 0;
    int firstSpacePosition = 0;

    /*  
        Η fgets αποθηκεύει στο s.name έως 19 χαρακτήρες και στο 20ο βάζει το '\0'.
        Αν η είσοδος είναι μικρότερη από 19 χαρακτήρες, αποθηκεύεται και το '\n', 
        οπότε αυτό αντικαθιστάται με το '\0'. Σε περίπτωση εισόδου μεγαλύτερης 
        των 19 χαρακτήρων, δηλαδή αν δεν υπάρχει το '\n', αδειάζει το buffer του input.
        (εδώ βασικά και σωστό μέγιστο μέγεθος να δώσει ο χρήστης, πάλι θα πάει στο else 
        γιατί η συνθήκη για να μπει στο else είναι απλά να μην είναι '\n' το στοιχείο πριν το '\0'.
        Όμως σε σωστή μέγιστη είσοδο δεν θα πάρει την τιμή 1 το isTooLong.)
    */
    if (nameArray[strlen(nameArray) - 1] == '\n') {
        nameArray[strlen(nameArray) - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            isInputTooLong = TRUE;
        }
    }

    if (strlen(nameArray) == 1 && nameArray[0] == '0') {
        return 2; // 2 σημαίνει ότι έδωσε 0 για είσοδο άρα θέλει να επιστρέψει στο menu
    }

    if (strlen(nameArray) < 5) {
        printf("The name you entered is too short.\n");
        return FALSE;
    }

    if (isInputTooLong) {
        printf("The name you entered is too long.\n");
        return FALSE;
    }

    int i = -1;
    while (nameArray[++i] != '\0') {
        // Ο κώδικας μεγαλωνει προς τα δεξιά αλλά έπρεπε να το ελέγξω... :|
        if ( (nameArray[i] < 65 && nameArray[i] != 32) || ((nameArray[i] > 90) && nameArray[i] < 97) || nameArray[i] > 122) {
            printf("The name can only contain letters.\n");
            return FALSE;
        }

        if (nameArray[i] == 32) {
            spaceCounter++;
            if (spaceCounter == 1) {
                firstSpacePosition = i;
            }
        }
    }

    if (spaceCounter > 1) {
        printf("Too many spaces.\n");
        return FALSE;
    } else if (spaceCounter < 1) {
        printf("There has to be a space between the first name and the last name.\n");
        return FALSE;
    }

    if (firstSpacePosition == 0) {
        printf("The name cannot begin with space.\n");
        return FALSE;
    } else if (firstSpacePosition == strlen(nameArray) - 1) {
        printf("The name cannot end with space.\n");
        return FALSE;
    }

    int fNameUpper = isupper(nameArray[0]);
    int lNameUpper = isupper(nameArray[++firstSpacePosition]);

    if (fNameUpper == 0 || lNameUpper == 0) {
        printf("The first name and the last name have to begin with a capital letter.\n");
        return FALSE;
    }

    if (nameArray[1] == 32) {
        printf("The first name can be 2 or more characters long.\n");
        return FALSE;
    }

    if (nameArray[strlen(nameArray) - 2] == 32) {
        printf("The last name can be 2 or more characters long.\n");
        return FALSE;
    }

    return TRUE;
}

// Μέθοδος για έλεγχο του ID που εισάγει ο χρήστης
int validateIdInput(unsigned long *id, char *inputArray) {
    int isInputTooLong = 0;
    int isNotNumber;

    // Έλεγχος αν ο χρήστης έδωσε λιγότερους από 20 χαρακτήρες και αντικατάσταση του '\n' με '\0' αν αυτό συνέβη
    if (inputArray[strlen(inputArray) - 1] == '\n') {
        inputArray[strlen(inputArray) - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            isInputTooLong = TRUE;
        }
    }

    // Tο inputArray χωράει 20 χαρακτήρες (χωρίς το '\0'). Αν το σύστημα είναι 32bit το μέγιστο unsigned long έχει 10 ψηφία
    // οπότε αν το inputArray έχει πάνω από 10 χαρακτήρες τότε επιστρέφεται μήνυμα λάθους
    if ( (sizeof(unsigned long) == 4) && (strlen(inputArray) > 10) ) {
        printf("Input too long. Try again.\n");
        return FALSE;
    }
    
    // Έλεγχος για μεγάλη είσοδο σε 64bit σύστημα
    if (isInputTooLong) {
        printf("Input too long. Try again.\n");
        return FALSE;
    }

    if (strlen(inputArray) == 0) {
        printf("Input cannot be empty.\n");
        return FALSE;
    }

    // Έλεγχος για μόνο αριθμούς
    int i = -1;
    isNotNumber = 0;
    while (inputArray[++i] != '\0') {
        if (inputArray[i] < 48 || inputArray[i] > 57) {
            isNotNumber = 1;
            printf("The ID can contain only numbers. Try again\n");
            return FALSE;
        }
    }

    /*
        Η είσοδος του χρήστη για το id αρχικά αποθηκεύεται σε string. Για να μην γίνει μετατροπή
        σε unsigned long μιας τιμής μεγαλύτερης από αυτήν που μπορεί να κρατήσει το σύστημα
        αποθηκεύεται η μέγιστη τιμή unsigned long του συστήματος σαν string. Μετά γίνεται σύγκριση 
        αν το max value είναι μικρότερο από το id που έδωσε ο χρήστης. 
    */
    if ((sizeof(unsigned long) == 8) && (strlen(inputArray) == 20)) {
        char maxUL_64bit[] = {"18446744073709551615"}; // max unsigned long για 64bit

            if ( (strcmp(maxUL_64bit, inputArray)) < 0) {
                printf("The input is too massive to be stored.\n");
                return FALSE;
            }

    } else if ((sizeof(unsigned long) == 4) && (strlen(inputArray) == 10)) {
        char maxUL_32bit[] = {"4294967295"}; // max unsigned long για 32bit

        if ( (strcmp(maxUL_32bit, inputArray)) < 0) {
                printf("The input is too massive to be stored.\n");
                return FALSE;
            }
    }

    char *endPtr = &inputArray[strlen(inputArray) + 1]; // Σε αυτό το στοιχείο είναι το '\0'
    *id = strtoul(inputArray, &endPtr, 10); // Μετατροπή του string στο inputArray σε unsigned long. Το 10 είναι για το δεκαδικο σύστημα

    return TRUE;
}

int main(int argc, char *argv[]) {

    Result result;

    list studentList = (list) malloc(sizeof(struct listR));
    if (studentList == NULL) {
        printf("Seems like there isn't enough memory to create the student list. Aborting.\n");
        abort();
    }
    studentList->head = NULL;
    studentList->tail = NULL;

    switch (argc) {
        case 2:
            // Έλεγχος αν υπάρχει το αρχείο
            if (access(argv[1], F_OK) == 0) { // https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c
                printf("Opening %s...\n", argv[1]);
                
                result = load(argv[1], &studentList);
                if (isError(result)) {
                    switch(result) {
                        case MALLOC_ERR:
                            printf("There isn't enough memory to load the file data.\n");
                            return 0;
                        case F_READ_ERR:
                            printf("There was a problem while reading from %s.\n", argv[1]);
                            return 0;
                        case NON_INIT_LIST:
                            printf("No student data was found in %s.\n", argv[1]);
                            break;
                        case UNKNOWN_ERR:
                            printf("Something went wrong while interacting with %s.\n", argv[1]);
                            return 0;
                    }
                } else {
                    printf("Loading data from %s ...\n", argv[1]);
                    printf("Done!\n");
                }
            } else {
                printf("File %s cannot be found.\n", argv[1]); 
                printf("It will be created for data storing before exiting the program.\n");
            }
            break;
        default:
            printf("The program requires the name of a file for data storing.\n");
            printf("If the file you pass as an argument does not exist, it will be created by the program before exiting.\n");
            printf("For example, try running ./a.out students.txt\n");
            return 0;
    }

    printf("\n");
    printf("=============== Welcome to the student management program ===============\n");

    student *s = (student*) malloc(sizeof(student));
    if (s == NULL) {
        printf("Seems like there isn't enough memory to run the program. Aborting.\n");
        abort();
    }

    int run = 1;
    char option;
    unsigned long id;
    char idStr[21]; // Εδώ(idStr) αποθηκεύεται το id που εισάγει ο χρήστης, για έλεγχο πριν την μετατροπή σε unsigned long.
                    // Ο πίνακας έχει 21 θέσεις γιατί για 64bit η μέγιστη τιμή unsigned long έχει 20 ψηφία, άρα το 
                    // 21ο είναι για το '\0'. Αν το σύστημα είναι 32bit τα ψηφία θα είναι 10, άρα < 20 οπότε θα χωράει εδώ.

    while (run) {
        printf("\n");
	    printf("Enter the number that corresponds to the option you want to perform.\n");
        printMenu();

        int inputCounter;
        int invalidOption = 1;
        
        do {
            inputCounter = 1;
            printf("Answer: ");
			scanf(" %c", &option);   
            
            /*
                Το while αυτό το πήρα από το ChatGPT. Είναι για να αδειάζει το input buffer.
                Αν έγραφα πχ. asdfasdf θα έπαιρνε σαν πρώτο input το a και στις επόμενες επαναλήψεις
                της scanf θα συνέχιζε με τη σειρά το input, δηλαδή s μετά d, μετά f κλπ. 
                Έτσι δηλαδή κρατάει μόνο το 1ο χαρακτήρα και "πετάει" τους υπόλοιπους. 
                Επίσης έτσι μπορώ να μετρήσω και τους χαρακρήρες εισόδου μέσα στο while.
            */
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
                inputCounter++;
            }

            if ( (option < '1' || option > '6') || inputCounter != 1 ) {
                printf("Oops. Invalid input. You have to enter one of the numbers below.\n");
                printMenu();
                continue;
            }    

            invalidOption = 0;

        } while(invalidOption);

        switch (option) {
            case '1':
                printf("\n");
                printf("(Tip: Enter 0 to return to the main menu.)\n");
                printf("\n");
                printf("Enter the student's full name."); 
                printf("The input can be from 5 to 19 characters long.\n");
                
                int run1 = 1;
                while (run1) {
                    printf("Student's full name: ");
                    fgets(s->name, sizeof(s->name), stdin); // fgets για να παίρνει string από τον χρήστη https://stackoverflow.com/questions/21691843/how-to-correctly-input-a-string-in-c

                    int isValidName = validateNameInput(s->name);

                    // ξεχώρισα τα if γιατί πολλά if, else εμφωλευμένα με μπέρδευαν
                    if (isValidName == FALSE) {
                        continue;
                    } 
                    
                    if (isValidName == TRUE) {
                        s->id = 0;
                        result = addStudent(*s, studentList);
                        if (isError(result)) {
                            switch (result) {
                                case MALLOC_ERR:
                                    printf("There isn't enough memory to add the student.\n");
                                    abort();
                                }
                        } else {
                            printf("Student added successfully!\n");
                            run1 = 0;
                        }
                    }  
                    
                    if (isValidName == 2) { // Αν ο χρήστης έδωσε 0, επιστροφή στο menu
                        run1 = 0;
                    }
                }
                break;
            case '2':
                if (studentList->head == NULL) {
                    printf("There are no students as of now. Add some by selecting option 1.\n");
                    break;
                }

                printf("(Tip: Enter 0 to return to the main menu.)\n");
                printf("\n");
                printf("Enter the ID of the student you want to delete.\n");

                int run2 = 1;
                while (run2) {
                    printf("Student's ID: ");
                    fgets(idStr, sizeof(idStr), stdin);

                    int isIdValid = validateIdInput(&id, idStr);
                    if (isIdValid == FALSE) {
                        continue;
                    }

                    if (id == 0) { // επιστροφή στο menu
                        run2 = 0;
                        continue;
                    }
                    
                    result = deleteStudentById(id, studentList);
                    if (isError(result)) {
                        switch (result) {
                            case SYNTAX_ERR: // Εδώ έβαλα SYNTAX_ERR εννοώντας λάθος στο id, με το σκεπτικό ότι 
                                             // αν υπήρχε syntax error στον κώδικα δεν θα έκανε compile
                                printf("Could not find the student with ID: %lu.\n", id);
                                run2 = 0;
                                break;
                            }
                    } else {
                        printf("Deleted student with ID: %lu.\n", id);
                        run2 = 0;
                    }
                }
                break;
            case '3':
                if (studentList->head == NULL) {
                    printf("There are no students as of now. Add some by selecting option 1.\n");
                    break;
                }

                printf("(Tip: Enter 0 to return to the main menu.)\n");
                printf("\n");
                printf("Enter the ID of the student you want to find.\n");

                int run3 = 1;
                while (run3) {
                    printf("Student's ID: ");
                    fgets(idStr, sizeof(idStr), stdin);

                    int isIdValid = validateIdInput(&id, idStr);
                    if (isIdValid == FALSE) {
                        continue;
                    }

                    if (id == 0) {
                        run3 = 0;
                        continue;
                    }
                        
                    result = findStudent(id, studentList, s);
                    if (isError(result)) {
                        switch (result) {
                            case SYNTAX_ERR:
                                printf("Could not find the student with ID: %lu.\n", id);
                                run3 = 0;
                                break;
                        }
                    } else {
                        printf("Found the following student:\n");
                        printf("\n");
                        printf("%-20s %s\n", "Name", "ID");
                        printStudent(*s);
                        run3 = 0;
                    }
                }
                break;
            case '4':
                if (studentList->head == NULL) {
                    printf("There are no students as of now. Add some by selecting option 1.\n");
                    break;
                }

                printf("(Tip: Enter 0 to return to the main menu.)\n");
                printf("\n");
                printf("Enter the ID of the student you want to update.\n");

                int goToMenu = 0;
                int run4_a = 1;

                while (run4_a) {
                    printf("Student's ID: ");
                    fgets(idStr, sizeof(idStr), stdin);

                    int isIdValid = validateIdInput(&id, idStr);
                    if (isIdValid == FALSE) {
                        continue;
                    }

                    if (id == 0) {
                        goToMenu = 1;
                        run4_a = 0;
                        continue;
                    }

                    result = findStudent(id, studentList, s);
                    if (isError(result)) {
                        switch (result) {
                            case SYNTAX_ERR:
                                printf("Could not find the student with ID: %lu.\n", id);
                                goToMenu = 1;
                                run4_a = 0;
                                break;
                        }
                    } else {
                        printf("Found the following student:\n");
                        printf("\n");
                        printf("%-20s %s\n", "Name", "ID");
                        printStudent(*s);
                        run4_a = 0;
                    }
                }

                if (goToMenu) {
                    break;
                }

                printf("\n");
                printf("(Tip: Enter 0 to return to the main menu.)\n");
                printf("\n");
                printf("Enter the student's updated full name."); 
                printf("The input can be from 5 to 19 characters long.\n");
                
                int run4_b = 1;
                while (run4_b) {
                    printf("Student's updated full name: ");
                    fgets(s->name, sizeof(s->name), stdin);

                    int isValidName = validateNameInput(s->name);
                    if (isValidName == FALSE) {
                        continue;
                    } 
                    
                    if (isValidName == TRUE) {
                        s->id = id;
                        result = updateStudent(*s, studentList);
                        if (isError(result)) {
                            switch (result) {
                            case SYNTAX_ERR:
                                printf("Something went wrong updating the student.\n");
                                run4_b = 0;
                                break;
                            }
                        } else {
                            printf("Student updated successfully!\n");
                            run4_b = 0;
                        }
                    } 
                    
                    // Αν ο χρήστης εισάγει 0 για επιστροφή στο menu
                    if (isValidName == 2) {
                        run4_b = 0;
                    }
                }
                break;
            case '5':
                if (studentList->head == NULL) {
                    printf("There are no students as of now. Add some by selecting option 1.\n");
                } else {
                    printf("\n");
                    printf("%-4s %-20s %s\n", "", "Name", "ID");
                    printStudents(studentList);
                }
                break;
            case '6':
                printf("Are you sure you want to quit? (y/n): ");
                char exitOption;
                int exitInputCounter;
                int run6 = 1;

                do {
                    exitInputCounter = 1;
			        scanf(" %c", &exitOption);   
                    int nextChar;

                    while ((nextChar = getchar()) != '\n' && nextChar != EOF) {
                        exitInputCounter++;
                    }

                    if ( (exitOption != 'y' && exitOption != 'n') || exitInputCounter != 1 ) {
                            printf("I'm silly. I understand only 'y' for yes and 'n' for no :(\n");
                            continue;
                    }

                    run6 = 0;
                } while(run6);
                
                if (exitOption == 'y') {
                    printf("\n");
                    printf("Saving data to %s...\n", argv[1]);
                    result = save(argv[1], studentList);
                    if (isError(result)) {
                        switch (result) {
                            case F_WRITE_ERR:
                                printf("There was a problem while writing to %s.\n", argv[1]);
                                return 0;
                            case UNKNOWN_ERR:
                                printf("Something went wrong while opening or closing %s.\n", argv[1]);
                                return 0;   
                        }
                    } else {
                        printf("Done.\n");
                    }

                    node tmpNode = studentList->head;
                    while (tmpNode != NULL) {
                        studentList->head = tmpNode->next;
                        free(tmpNode);
                        tmpNode = studentList->head;
                    }

                    free(studentList);
                    studentList = NULL;
                    free(s);
                    s = NULL;

                    run = 0;
                    printf("Exiting program...\n");
                }
                break;
        }
    }
}