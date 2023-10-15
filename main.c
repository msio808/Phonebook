/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * Created by MrMas on 07/10/2023 @ 08:28PM  * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*/

#include "./src/src.c"

//TODO : The entry point of the phonebook application.
int main() {
    clr();      //* Clear the console screen.
    login();    //* Call the login function to start the application.
    return 0;   //* Return 0 to indicate successful program execution.
}

// TODO : Function that allows the user to add a new contact to the phonebook.
void add_contact() {
    short x = 29, y = 0;
    contact *user, *dupl;

    //* Allocate memoryto the user & dupl structure.
    user = (contact*)calloc(1<<4, sizeof(contact));
    dupl = (contact*)calloc(1<<4, sizeof(contact));

    //* Handle Memory Allocation Errors.
    if (NULL == user || NULL == dupl) {
        fprintf(stderr, "\n\t\t[!] - Error Allocating Memory!...");
        exit(EXIT_FAILURE); //! Exit the program if memory is not allocated
    }

    do {
        clr();
        SetCursorPosition(x, y+=1);
        print_banner(18, " ADD NEW CONTACT ", 18);

        FILE* db;
        //* Attempt to open the database file in read/write mode.
        if (NULL == (db = fopen("./src/db.bin", "rb+"))) {
            fprintf(stderr, "\n\t[!] - Error Opening File : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        SetCursorPosition(x += 5, y+=2);
        input("-> FULL NAME      : ", user->name, NAMESIZE, stdin);
        SetCursorPosition(x, y += 1);
        input("-> EMAIL          : ", user->email, BUFFSIZE, stdin);
        SetCursorPosition(x, y += 1);
        input("-> PHONE          : ", user->phone, BUFFSIZE, stdin);
        SetCursorPosition(x, y += 1);
        input("-> ADDRESS        : ", user->address, MAXXBUFF, stdin);
        sprintf(user->cuid, "%06i", getline_ID(dupl, db) + 1);

        //* Check for duplicate contacts with the same name or phone number.
        if (check_duplicate(dupl, user->name, user->phone, db) > 0) {
            printf("\n\t\t Contact with Name %s or Phone %s already exists in the file", user->name, user->phone);
        } else {
            //* Write the new contact information to the database file.
            fprintf(db, "%s; %s; %s; %s; %s;\n", user->cuid, user->name, user->email, user->phone, user->address);
        }
        fflush(db);
        fclose(db);
        memset(user, 0, 1<<4);
        
        SetCursorPosition(x -= 5, y += 4);
        printf("Press Y add another contact, or any key to go back to the main menu : ");
    } while ('Y' == toupper(cgetch()));
    free(user);
    free(dupl);
    menu(); //* Redirects to the main menu.
}

// TODO : Function that allows the user to edit an existing contact in the phonebook.
void edit_contact() {
    short x = 29, y = 1;
    char line[LINESIZE];
    char keyword[NAMESIZE];
    contact *user, *tmp_user;
 
    user = (contact*)calloc(1<<4, sizeof(contact));
    tmp_user = (contact*)calloc(1<<4, sizeof(contact));
    if (NULL == user) {
        fprintf(stderr, "\n\t\t[!] - Error Allocating Memory!...");
        exit(EXIT_FAILURE);
    }

    do {
        clr();
        SetCursorPosition(x, y);
        print_banner(18, " EDIT CONTACT ", 18);
        FILE *db, *tmp_db;

        //* Attempt to open the database file in read mode and a temporary file in read/write mode.
        if (NULL == (db = fopen("./src/db.bin", "rb")) || NULL == (tmp_db = fopen("./src/tmp.bin", "ab+"))) {
            fprintf(stderr, "\n\t[!] - Error Opening File : %s\n", strerror(errno));
            exit(EXIT_FAILURE); //! Exit the program if file can't be opened.
        }

        SetCursorPosition(x += 5, y += 2);
        input("Search For Contact You Want To Modify (search by name) : ", keyword, NAMESIZE, stdin);

        //* Find the contact to modify based on the search keyword.
        if (0 != findline(user, line, LINESIZE, keyword, db)) {
            printf("\n\n\t\t\t\t\t");
            input("Select the contact you want to modify by entering the Contact ID : ", keyword, 16, stdin);
            
            //* Rewrite the contact's information in the temporary database file.
            rewrite_line(user, tmp_user, line, LINESIZE, keyword, db, tmp_db, 'e');
        }
        else {
            fprintf(stderr, "\n\t\t\t\t\t\t[!] - Error, Contact record not found.");
        }
        fflush(db);
        fflush(tmp_db);
        fclose(db);
        fclose(tmp_db);

        int r1, r2;
        //* Remove the original database file and rename the temporary file to replace it.
        if (0 != (r1 = remove("./src/db.bin")))
            fprintf(stderr, "\n\t\t [!] - Error removing file.");
        if(0 != (r2 = rename("./src/tmp.bin", "./src/db.bin")))
            fprintf(stderr, "\n\t\t [!] - Error renaming file.");
        if (0 != r1 || 0 != r2)
            printf("\n\t\t\t\t\t\t[!] - Error Modifying Contact, Press any key to go back to the main menu : ");
        else
            printf("\n\t\t\t\t\t-> Contact edited successfully, Press any key to go back to the main menu : ");
    } while ('Y' == toupper(cgetch()));
    free(user);
    menu();
}

// TODO : Function to displays a list of contacts from the phonebook.
void list_contact() {
    short x = 29, y = 1;
    contact* user;
    char line[LINESIZE];

    user = (contact*)calloc(1<<4, sizeof(contact));
    if (NULL == user) {
        fprintf(stderr, "\n\t\t[!] - Error Allocating Memory!...");
        exit(EXIT_FAILURE);
    }

    clr();
    SetCursorPosition(x, y);
    print_banner(18, " LIST OF CONTACTS ", 18);

    FILE* db;
    //* Attempt to open the database file in read mode.
    if (NULL == (db = fopen("./src/db.bin", "rb"))) {
        fprintf(stderr, "\n\t\t[!] - Error Opening File : %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    fseek(db, 0, SEEK_SET); //* Move the file pointer to the beginning of the file.
    input(NULL, line, LINESIZE, db);
    printf("\n\n\t\t\t\t\t");
    for (size_t i = 0; i < 70; i++)printf("_"); //* Print a separator line.

    //* Read and display contact information from the database file.
    while (!feof(db) && input(NULL, line, LINESIZE, db) > 0) {
        if (5 == parse_line(line, user)) {
            printline(user);
        }
        else {
            printf("\n\t\t [!] - Error parsing line.");
        }
        memset(user, 0, 1<<4);
    }
    fflush(db);
    fclose(db);
    free(user);

    printf("\n\t\t\t\t\t\t");
    printf("-> Press any key to go back to the main menu : ");

    cgetch();
    menu();
}

// TODO : Function that allows the user to search for contacts in the phonebook by name.
void search_contact() {
    short x = 29, y = 1;
    contact *user;
    char line[LINESIZE];
    char keyword[NAMESIZE];
    
    user = (contact*)calloc(1<<4, sizeof(contact));
    
    if (NULL == user) {
        //! Handle memory allocation failure.
        fprintf(stderr, "\n\t\t[!] - Error Allocating Memory!...");
        exit(EXIT_FAILURE);
    }
    do {
        clr();
        SetCursorPosition(x, y);
        print_banner(18, " SEARCH CONTACTS ", 18);

        FILE* db;
        //* Attempt to open the database file in read mode.
        if (NULL == (db = fopen("./src/db.bin", "rb"))) {
            //! Handle file opening failure.
            fprintf(stderr, "\n\t[!] - Error Opening File : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        SetCursorPosition(x += 5, y += 2);
        input("Search contact by name : ", keyword, NAMESIZE, stdin);
        //* Search for contacts by the provided name keyword.
        int flag = findline(user, line, LINESIZE, keyword, db);

        fflush(db);
        fclose(db);

        //* Display a message when no matching contact is found.
        if (0 == flag)
            printf("\n\t\t\t\t\t\tContact record not found.\n");
        printf("\n\t\t\t\t\t\t");
        printf("Press any key to go back to the main menu : ");
    } while ('Y' == toupper(cgetch()));
    //* Deallocate the memory and return to the main menu.
    free(user);
    menu();
}

// TODO : Function that allows the user to delete a contact from the phonebook.
void delete_contact() {
    short x = 29, y = 1;
    char line[LINESIZE];
    char keyword[NAMESIZE];
    contact *user, *tmp_user;

    //* Allocate Memory to the user and temporary user structures.
    user = (contact*)calloc(1<<5, sizeof(contact));
    tmp_user = (contact*)calloc(1<<5, sizeof(contact));

    if (NULL == user || NULL == tmp_user) {
        //! Handle memory allocation failure.
        fprintf(stderr, "\n\t\t[!] - Error Allocating Memory!...");
        exit(EXIT_FAILURE);
    }
    do {
        clr();
        SetCursorPosition(x, y);
        print_banner(18, " DELETE CONTACT ", 18);

        FILE *db, *tmp_db;
        //* Attempt to open the database file in read mode and the temporary database file in append mode.
        if (NULL == (db = fopen("./src/db.bin", "rb")) || NULL == (tmp_db = fopen("./src/tmp.bin", "ab+"))) {
            //! Handle file opening failure.
            fprintf(stderr, "\n\t[!] - Error Opening File : %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        SetCursorPosition(x += 5, y += 2);
        input("Search For Contact You Want To Delete (search by name) : ", keyword, NAMESIZE, stdin);

        if (0 != findline(user, line, LINESIZE, keyword, db)) {
            printf("\n\n\t\t\t\t\t");
            input("Select the contact you want to delete by entering the Contact ID : ", keyword, 16, stdin);
            //* Rewrite the contact list, excluding the selected contact for deletion.
            rewrite_line(user, tmp_user, line, LINESIZE, keyword, db, tmp_db, 'd');
        }
        else {
            fprintf(stderr, "\n\t\t\t\t\t\t[!] - Error, Contact record not found.");
        }
        fflush(db);
        fflush(tmp_db);
        fclose(db);
        fclose(tmp_db);

        int r1, r2;
        //* Remove the old database file and rename the temporary file.
        if (0 != (r1 = remove("./src/db.bin"))) {
            fprintf(stderr, "\n\t\t [!] - Error removing file.");
        }
        if(0 != (r2 = rename("./src/tmp.bin", "./src/db.bin"))) {
            fprintf(stderr, "\n\t\t [!] - Error renaming file.");
        }
        if (0 != r1 || 0 != r2) {
            printf("\n\t\t\t\t\t[!] - Error Deleting Contact, Press any key to go back to the main menu : ");
        } else {
            printf("\n\t\t\t\t\t-> Contact Deleted successfully, Press any key to go back to the main menu : ");
        }
    } while ('Y' == toupper(cgetch()));
    free(user);
    menu();
}

//* Exits the program with a success status code.
void exit_program(void) {
    exit(EXIT_SUCCESS);
}