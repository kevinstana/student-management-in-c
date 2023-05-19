// Κάποια includes που χρησιμοποιούνται εδώ έχουν γίνει include στο students.c
// οπότε επειδή εδώ έχω κάνει include το students.c δεν ξανακάνω και αυτά
#include <ctype.h>
#include "students.c"

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
        Όμως σε σωστή μέγιστη είσοδο δεν θα πάρει την τιμή 1 το isTooLong γιατί δεν θα μπει στο while.)
    */
    if (nameArray[strlen(nameArray) - 1] == '\n') {
        nameArray[strlen(nameArray) - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            isInputTooLong = 1;
        }
    }

    if (strlen(nameArray) == 1 && nameArray[0] == '0') {
        return 2;
    }

    if (strlen(nameArray) < 5) {
        printf("The name you entered is too short.\n");
        return 0;
    }

    if (isInputTooLong) {
        printf("The name you entered is too long.\n");
        return 0;
    }

    int i = -1;
    while (nameArray[++i] != '\0') { // Το i θα ξεκινάει από το 0 γιατί πρώτα αυξάνεται η τιμή και μετά χρησιμοποιείται στην έκφραση
        if ((nameArray[i] < 65 && nameArray[i] != 32) || (nameArray[i] > 122)) { // Έλεγχος για το αν έχει δώσει μόνο γράμματα και κενό
            printf("The name can only contain letters.\n");
            return 0;
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
        return 0;
    } else if (spaceCounter < 1) {
        printf("There has to be a space between the first name and the last name.\n");
        return 0;
    }

    if (firstSpacePosition == 0) {
        printf("The name cannot begin with space.\n");
        return 0;
    } else if (firstSpacePosition == strlen(nameArray) - 1) {
        printf("The name cannot end with space.\n");
        return 0;
    }

    int f_name = isupper(nameArray[0]);
    int l_name = isupper(nameArray[++firstSpacePosition]);

    if (f_name == 0 || l_name == 0) {
        printf("The first name and the last name have to begin with a capital letter.\n");
        return 0;
    }

    return 1;
}

// Μέθοδος για έλεγχο του ID που εισάγει ο χρήστης
int validateIdInput(unsigned long *id, char *inputArray) {
    int isInputTooLong = 0;
    int isNotNumber;
    char maxUL_32bit[] = {"4294967295"}; // max unsigned long για 32bit
    char maxUL_64bit[] = {"18446744073709551615"}; // max unsigned int για 64bit

    if (inputArray[strlen(inputArray) - 1] == '\n') {
        inputArray[strlen(inputArray) - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            isInputTooLong = 1;
        }
    }

    if (isInputTooLong > 0) {
        printf("Input too long. Try again.\n");
        return 0;
    }

    int i = -1;
    isNotNumber = 0;
    while (inputArray[++i] != '\0') {
        if (inputArray[i] < 48 || inputArray[i] > 57) {
            isNotNumber = 1;
            printf("The ID has to contain only numbers. Try again\n");
            return 0;
        }
    }
    
    if ((sizeof(unsigned long) == 8) && (strlen(inputArray) == 20)) {
        for (int i = 0; i < 21; i++) {
            if ((inputArray[i] - maxUL_64bit[i]) < 0) {
                printf("The input is too massive to be stored.\n");
                return 0;
            }
        } 
    } else if ((sizeof(unsigned long) == 4) && (strlen(inputArray) == 10)) {
        for (int i = 0; i < 11; i++) {
            if ((inputArray[i] - maxUL_32bit[i])) {
                printf("The input is too massive to be stored.\n");
                return 0;
            }
        } 
    }

    char *endPtr = &inputArray[strlen(inputArray) + 1]; // Σε αυτό το στοιχείο είναι το '\0'
    *id = strtoul(inputArray, &endPtr, 10); // Το 10 είναι για το δεκαδικο σύστημα

    return 1;
}

int main() {
    printf("=============== Welcome to the student management program ===============\n");

    list studentList = (list) malloc(sizeof(struct listR));
    if (studentList == NULL) {
        printf("Seems like there isn't enough memory to create the student list. Aborting.\n");
        abort();
    }
    studentList -> head = NULL;
    studentList -> tail = NULL;

    int run = 1;
    char option;
    unsigned long id;
    char idStr[21];

    student s;
    node n = (node) malloc(sizeof(struct nodeR));
    if (n == NULL) {
        printf("Seems like there isn't enough memory. Aborting.\n");
        abort();
    }

    while (run) {
        printf("\n");
	    printf("Enter the number that corresponds to the option you want to perform.\n");
        printMenu();

        int inputCounter;  // Για να μετρήσει το μέγεθος του input
        
        do {
            inputCounter = 1;
            printf("Answer: ");
			scanf(" %c", &option);   
            
            /*
                Το while αυτό το πήρα από το ChatGPT. Είναι για να αδειάζει το input buffer.
                Αν έγραφα πχ. asdfasdf θα έπαιρνε σαν πρώρο input το a και στις επόμενες επαναλήψεις
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
            }    

        } while( (option < '1' || option > '6') || inputCounter != 1 );

        switch (option) {
            case '1':
                printf("\n");
                printf("(Tip: Enter 0 if you want to exit this option.)\n");
                
                int run1 = 1;
                while (run1) {
                    printf("\n");
                    printf("Enter the student's full name."); 
                    printf("The input can be from 5 to 19 characters long.\n");
                    printf("Student's full name: ");
                    fgets(s.name, sizeof(s.name), stdin); // fgets για να παίρνει string από τον χρήστη https://stackoverflow.com/questions/21691843/how-to-correctly-input-a-string-in-c

                    int isValidName = validateNameInput(s.name);
                    if (isValidName == 0) {
                        continue;
                    } else if (isValidName == 1) {
                        int addResult = addStudent(s, studentList);
                        switch (addResult) {
                        case NO_ERR:
                            printf("Student added successfully!\n");
                            run1 = 0;
                            break;
                        }
                    } else {
                        run1 = 0;
                    }
                }
                break;
            case '2':
                if (studentList->head == NULL) {
                    printf("There are no students as of now. Add some by selecting option 1.\n");
                    break;
                }

                printf("(Tip: Enter 0 if you want to exit this option.)\n");
                printf("\n");
                printf("Enter the ID of the student you want to delete.\n");

                int run2 = 1;
                while (run2) {
                    printf("Student's ID: ");
                    fgets(idStr, sizeof(idStr), stdin);

                    int isIdValid = validateIdInput(&id, idStr);
                    if (isIdValid == 0) {
                        continue;
                    }

                    if (id == 0) {
                        run2 = 0;
                    } else {
                        int found = deleteStudentById(id, studentList);
                        switch (found) {
                            case NO_ERR:
                                printf("Deleted student with ID: %lu.\n", id);
                                run2 = 0;
                                break;
                            case SYNTAX_ERR:
                                printf("Could not find the student with ID: %lu.\n", id);
                                run2 = 0;
                                break;
                        }
                    }
                }
                break;
            case '3':
                if (studentList->head == NULL) {
                    printf("There are no students as of now. Add some by selecting option 1.\n");
                    break;
                }

                printf("(Tip: Enter 0 if you want to exit this option.)\n");
                printf("\n");
                printf("Enter the ID of the student you want to find.\n");

                int run3 = 1;
                while (run3) {
                    printf("Student's ID: ");
                    fgets(idStr, sizeof(idStr), stdin);

                    int isIdValid = validateIdInput(&id, idStr);
                    if (isIdValid == 0) {
                        continue;
                    }

                    if (id == 0) {
                        run3 = 0;
                    } else {
                        int found = findStudent(id, studentList, &s);
                        switch (found) {
                            case NO_ERR:
                                printStudent(s);
                                run3 = 0;
                                break;
                            case SYNTAX_ERR:
                                printf("Could not find the student with ID: %lu.\n", id);
                                run3 = 0;
                                break;
                        }
                    }
                }
                break;
            case '4':
                if (studentList->head == NULL) {
                    printf("There are no students as of now. Add some by selecting option 1.\n");
                    break;
                }

                printf("(Tip: Enter 0 if you want to exit this option.)\n");
                printf("\n");
                printf("Enter the ID of the student you want to find.\n");

                int run4_a = 1;
                while (run4_a) {
                    printf("Student's ID: ");
                    fgets(idStr, sizeof(idStr), stdin);

                    int isIdValid = validateIdInput(&id, idStr);
                    if (isIdValid == 0) {
                        continue;
                    }

                    if (id == 0) {
                        run4_a = 0;
                    } else {
                        int found = findStudent(id, studentList, &s);
                        switch (found) {
                            case NO_ERR:
                                printStudent(s);
                                run4_a = 0;
                                break;
                            case SYNTAX_ERR:
                                printf("Could not find the student with ID: %lu.\n", id);
                                break;
                        }
                    }
                }

                if (id == 0) {
                    break;
                }

                printf("\n");
                printf("(Tip: Enter 0 if you want to exit this option.)\n");
                
                int run4_b = 1;
                while (run4_b) {
                    printf("\n");
                    printf("Enter the student's updated full name."); 
                    printf("The input can be from 5 to 19 characters long.\n");
                    printf("Student's updated full name: ");
                    fgets(s.name, sizeof(s.name), stdin);

                    int isValidName = validateNameInput(s.name);
                    if (isValidName == 0) {
                        continue;
                    } else if (isValidName == 1) {
                        int updateResult = updateStudent(s, studentList);
                        switch (updateResult) {
                        case NO_ERR:
                            printf("Student updated successfully!\n");
                            run4_b = 0;
                            break;
                        }
                    } else {
                        printf("Something went wrong updating the student.\n");
                        run4_b = 0;
                    }
                }
                break;
            case '5':
                if (studentList->head == NULL) {
                    printf("There are no students as of now. Add some by selecting option 1.\n");
                } else {
                    printf("\n");
                    printf("%-20s %s\n", "Name", "ID");
                    printStudents(studentList);
                }
                break;
            case '6':
                printf("Are you sure you want to quit? (y/n): ");
                char exitOption;
                int exitInputCounter;

                do {
                    exitInputCounter = 1;
			        scanf(" %c", &exitOption);   
                    int nextChar;

                    while ((nextChar = getchar()) != '\n' && nextChar != EOF) {
                        exitInputCounter++;
                    }

                    if ( (exitOption != 'y' && exitOption != 'n') || exitInputCounter != 1 ) {
                            printf("I'm silly. I understand only 'y' for yes and 'n' for no :(\n");
                    }        
                } while( (exitOption != 'y' && exitOption != 'n') || exitInputCounter != 1 );
                
                if (exitOption == 'y') {
                    printf("\n");
                    printf("Saving data to students.txt...\n");
                    printf("Done.\n");
                    printf("Exiting program...\n");
                    run = 0;
                    break;
                }
                break;
        }
    }
}