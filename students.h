#define TRUE 1
#define FALSE 0
#define MAXSTRING 20

typedef enum {
	NO_ERR,
	MALLOC_ERR,
    F_READ_ERR,
    F_WRITE_ERR,
	NON_INIT_LIST,
    SYNTAX_ERR,
    UNKNOWN_ERR
} Result;

typedef struct nodeR* node;
typedef struct listR* list;

typedef struct{
   char name[MAXSTRING];
   unsigned long id;
}student;

struct nodeR{
  student data;
  node next;
  node previous;
};

struct listR{
  node head;
  node tail;
};

void printMenu();

//Εκτυπώνει το id και το όνομα ενός φοιτητή st
void printStudent(student );

//Εκτυπώνει το id και το όνομα όλων των φοιτητών σε μία λίστα l
void printStudents(list );

//ανάκτηση λίστας φοιτητών από αρχείο
Result load(char *, list *);

//αποθήκευση λίστας φοιτητών σε αρχείο
Result save(char *, list );

//προσθήκη νέου student στο τέλος της λίστας
Result addStudent(student , list );

//αναζήτηση του student από τη λίστα με βάση το id. Aνάθεση της δ/νσης του κόμβου που περιέχει το student με το σωστό id στο np. To np γίνεται NULL αν δε βρεθεί.
Result findStudentNode(unsigned long , list , node* );

//αναζήτηση του student από τη λίστα με βάση το id. Aνάθεση της δ/νσης του student με το σωστό id στο st. Το st.id γίνεται -1 αν δε βρεθεί
Result findStudent(unsigned long , list , student *);

//διαγραφή από τη λίστα με βάση το id
Result deleteStudentById(unsigned long , list );

//ανανέωση των στοιχείων του student στη λίστα l με βάση το st.id
Result updateStudent(student , list );

//Επιστρέφει TRUE αν το σφάλμα δεν είναι NO_ERR
int isError(Result );

//Παράγει ένα ψευδοτυχαίο και με μεγάλη πιθανότητα μοναδικό id
unsigned long generateId();