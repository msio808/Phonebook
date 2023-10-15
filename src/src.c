/* 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * Created by MrMas on 07/10/2023 @ 02:31PM  * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
*/
#include "src.h"

//TODO: Utility function to delete 'n' characters from the console input.
void del_char(size_t n) {
    do {
        for (size_t i = 0; i < n; i++) {
            putchar('\b');  //* Move the cursor back by one position
            putchar(' ');   //* Replace the character with a space
            putchar('\b');  //* Move the cursor back again
        }
    } while (EXIT_SUCCESS);  //* This loop runs once.
}
// TODO : Case-insensitive substring search function.
/*
 * Searches for a substring 'needle' within the string 'haystack'.
 * Returns a pointer to the first occurrence of 'needle' in 'haystack' or NULL if not found.
*/
string stristr(string haystack, const string needle) {
    if (!haystack || !needle)
        return NULL;

    while (*haystack) {
        if (tolower(*haystack) == tolower(*needle)) {
            string temp_h = haystack;   //* Temporary pointer to iterate 'haystack'
            string temp_n = (string)needle;  //* Temporary pointer to iterate 'needle'

            //* Continue comparing characters while they match and both strings haven't reached their end.
            while (*temp_h && *temp_n && tolower(*temp_h) == tolower(*temp_n)) {
                temp_h++;
                temp_n++;
            }

            //* If 'temp_n' has reached the end, 'needle' is found in 'haystack'; return the pointer.
            if (!(*temp_n))
                return (string)haystack;
        }
        haystack++;
    }

    //* 'needle' not found in 'haystack'; return NULL.
    return NULL;
}
// TODO : Function to read password from user.
/* 
 * Reads a password from the console input while masking the characters with '*'.
 * Displays a prompt 'prompt' and stores the password in 'pass'.
 * Returns the length of the password entered.
*/
size_t getPass(const string prompt, string pass)
{
    if (prompt) {
        fputs(prompt, stdout); //* Display the provided prompt.
        fflush(stdout);
    }
    size_t len = 0;
    int c;

    //* Continue reading characters until the Enter key is pressed.
    while (ENTER_KEY != (c = cgetch())) {
        if (BACKSPACE_KEY == c && 0 < len) {
            del_char(1); //* Handle backspace: delete one character and decrement the length.
            len -= 1;
        }
        else if (CTRL_K('c') == c) {
            exit(EXIT_FAILURE); //* Handle Ctrl+C: Exit the program with a failure status.
        }
        else {
            *(pass + len) = (char)c;
            putchar('*'); //* Mask the entered character with '*'.
            len += 1;
        }
    }
    *(pass + len) = '\0'; //* Null-terminate the password string.
    return len; //* Return the length of the password entered.
}
// TODO : Function to read string input from specified stream.
/*
 * Reads a string from the specified stream 'STREAM' into 'array' with a specified size.
 * Displays a prompt 'prompt' if provided.
 * Returns the length of the string read (excluding the newline character).
*/
size_t input(const string prompt, string array, int size, FILE *STREAM) {
    if (prompt) {
        fputs(prompt, stdout); //* Display the provided prompt.
        fflush(stdout);
    }
    if (0 == size) { //* Handle invalid buffer size
        return EXIT_FAILURE; //* Exit the program if the specified size is 0 (no input can be read).
    }
    size_t len = 0;
    //* Read a line of text from 'STREAM' into 'array' with a maximum size of 'size'.
    if (NULL != fgets(array, size, STREAM)) {
        
        len = strlen(array);
        if (0 < len && '\n' == *(array + (len - 1))) {
            *(array + (len - 1)) = '\0'; //* Remove the trailing newline character, if present
            len -= 1;
        }
    }
    return len; //* Return the length of the string read (excluding the newline character).
}

//TODO : Function to handle the login process for the phonebook application.
void login() {
    clr();  //* Clear the console screen.
    short x = 29, y = 1;  // Initialize cursor position.
    size_t check;
    //* Hardcoded admin username & password.
    static const string uname = "admin";  
    static const string passwd = "admin";
    
    //* Set the cursor position & Display a banner.
    SetCursorPosition(x, y);
    print_banner(16, " AUTHORIZED USER'S ONLY ", 16);

    //* Allocate memory for a user contact structure.
    contact* user = (contact*)calloc(1 << 4, sizeof(contact));
    
    //* Handle memory allocation failure.
    if (NULL == user) {
        fprintf(stderr, "\n\t [!] - Error, Memory Allocation Failed!.");
        exit(EXIT_FAILURE);
    }

    do {
        //* Prompt the user for username and password.
        SetCursorPosition(x + 5, y += 2);
        input("Enter your username : ", user->creds.username, BUFFSIZE, stdin);

        SetCursorPosition(x + 5, y += 2);
        getPass("Enter your password : ", user->creds.password);

        //* Check if entered credentials match the hardcoded admin username or password.
        check = (0 == strcmp(user->creds.username, uname)) || (0 == strcmp(user->creds.password, passwd));
        if (1 != check)
        {
            SetCursorPosition(x + 6, y += 2);
            fprintf(stderr, "[!] Invalid Entry, Incorrect Username of Password.");
        }
    } while (1 != check);
    free(user); //* Free the allocated memory.
    menu();  //* Redirect to the main menu.
}

// TODO : Function to display the main menu of the phonebook application and handle user menu choices.
void menu(void) {
    short x = 29, y = 1;
    enum options {NEW = '1', EDIT = '2', LIST = '3', BROWSE = '4', DEL = '5', EXIT = '6', QUIT = CTRL_K('c')};
    clr();
    SetCursorPosition(x, y);
    print_banner(16, " PHONEBOOK DIRECTORY ", 16);
    SetCursorPosition(x + 37, y+=2);
    printf(" MAIN MENU ");
    SetCursorPosition(x + 7, y += 2);
    printf("[1]. ADD NEW CONTACT \t[2]. EDIT CONTACT    \t[3]. LIST CONTACT");
    SetCursorPosition(x + 7, y += 2);
    printf("[4]. BROWSE CONTACTS \t[5]. DELETE CONTACT  \t[6]. EXIT");

    int check = 0;
    //* Display the menu and handle user choices.
    do
    {
        SetCursorPosition(x + 5, y + 2);
        switch (cgetch()) {
            case NEW :
                add_contact();
                break;
            case EDIT :
                edit_contact();
                break;
            case LIST :
                list_contact();
                break;
            case BROWSE :
                search_contact();
                break;
            case DEL :
                delete_contact();
                break;
            case EXIT :
            case QUIT :
                exit_program();
                break;
            default :
                check = -1;
        }
    } while (-1 == check);
}

// TODO : Function to Parse a line into the 'Contact' structure.
int parse_line(string line, contact *Person) {
    return sscanf(line, "%19[^;]; %99[^;]; %99[^;]; %99[^;]; %199[^;];", Person->cuid, Person->name, Person->email, Person->phone, Person->address);
}

/*
 * Reads lines from the specified file and extracts contact information into the 'Person' structure.
 * Returns the highest 'cuid' found in the file.
*/
int getline_ID(contact *Person, FILE *cfPtr) {
    char _Buff[LINESIZE];
    int id, highestID = 0;
    fseek(cfPtr, 0, SEEK_SET); //* Move the file pointer to the beginning of the file.
    
    //* Read lines from the file and extract contact information.
    while (!feof(cfPtr) && input(NULL, _Buff, sizeof(_Buff), cfPtr) > 1) {
        if (5 == parse_line(_Buff, Person)) {
            id = atoi(Person->cuid); //* Convert the 'cuid' string to an integer.
            if (id > highestID)
                highestID = id; //* Update the highest 'cuid' if a higher one is found.
        }
    }
    return id; //* Return the highest 'cuid' found in the file.
}

/*
 * Checks for duplicate contacts in the phonebook.
 * Compares the provided 'Name' and 'Phone' with the contact information in the file.
 * Returns the count of matching contacts (duplicates).
*/
int check_duplicate(contact *Person, string _Name, string _Phone, FILE *cfPtr) {
    int flag = 0;
    char _Buff[LINESIZE];

    fseek(cfPtr, 0, SEEK_SET);  //* Move the file pointer to the beginning of the file.

    //* Read lines from the file and compare contact information.
    while (!feof(cfPtr) && input(NULL, _Buff, sizeof(_Buff), cfPtr) > 0) {
        if (5 == parse_line(_Buff, Person)) {

            char tmp1[strlen(_Name) + 1], tmp2[strlen(_Phone) + 1];
            strcpy(tmp1, _Name); strcpy(tmp2, _Phone);

            if (0 == strcmp(strUpr(tmp1), strUpr(Person->name)) || 0 == strcmp(tmp2, Person->phone)) {
                flag += 1; //* Increment the flag if a duplicate contact is found.
            }
        }
    }
    return flag; //* Return the count of duplicate contacts.
}

//TODO : Function to Convert a string to uppercase.
string strUpr(const string _Buff) {
    for (size_t i = 0; *(_Buff + i) != '\0'; i++) {
        if (islower(*(_Buff + i)))
            *(_Buff + i) = toupper(*(_Buff + i)); //* Convert each character to uppercase.
    }
    return _Buff;
}

//TODO : Print the details of a contact to the console.
void printline(contact *Person) {
    printf("\n\t\t\t\t\t");
    printf("-> Contact ID:\t%s", Person->cuid);
    printf("\n\t\t\t\t\t\t");
    printf("-> FULLNAME  :\t%s", Person->name);
    printf("\n\t\t\t\t\t\t");
    printf("-> EMAIL     :\t%s", Person->email);
    printf("\n\t\t\t\t\t\t");
    printf("-> PHONE     :\t%s", Person->phone);
    printf("\n\t\t\t\t\t\t");
    printf("-> ADDRESS   :\t%s", Person->address);
    printf("\n\t\t\t\t\t");
    for (size_t i = 0; i < 70; i++)printf("_"); //* Print a horizontal line as a separator.
}

// TODO : Function to search for occurence of specific keyword in the 'name' field from each lines in the file.
/*
 * The 'key' is the search term to look for in contact names.
 * 'Person' structure is used to store the contact details.
 * Returns the count of matching contact lines found.
*/
int findline(contact *Person, string _Buff, size_t _Bufsize_, string key, FILE *cfPtr) {
    int flag = 0;
    fseek(cfPtr, 0, SEEK_SET); //* Move the file pointer to the beginning of the file.
    while (!feof(cfPtr) && input(NULL, _Buff, _Bufsize_, cfPtr) > 0) {
        if (5 == parse_line(_Buff, Person)) {
            //* If the line is successfully parsed, check if 'key' is present in the 'name' field.
            if (NULL != stristr(Person->name, key)) {
                printline(Person); //* Print the details of the matching contact.
                flag += 1;  //* Increment the flag for each match found.
            }
        }
        else {
            fprintf(stderr, "\n\t\t [!] - Error parsing line.");
        }
        memset(Person, 0, 1 << 4); //* Clear the 'Person' structure for the next iteration.
    }
    return flag;  //* Return the count of matching contact lines found.
}

// TODO : Function to Rewrite a line in a file based on the specified 'mode'.
/*
 * If 'mode' is 'd', the function deletes the line where 'Old' matches 'key' and writes the rest of the lines.
 * If 'mode' is 'e', the function edits the line where 'Old' matches 'key' with the information in 'New' and writes the updated lines.
 * 'Old' is used to store the original contact, 'New' stores the updated contact, and 'key' is the identifier for the line to delete or edit.
 * 'cfPtr' is the original file, and 'tmp_cfPtr' is a temporary file for writing the updated data.
*/
void rewrite_line(contact *Old, contact *New, string _Buff, size_t _Bufsize_, string key, FILE *cfPtr, FILE *tmp_cfPtr, char mode) {
    
    fseek(cfPtr, 0, SEEK_SET); //* Move the file pointer to the beginning of the file.
    int n = 1;
    if ('d' == mode) {
        input(NULL, _Buff, _Bufsize_, cfPtr);
        fprintf(tmp_cfPtr, "%s\n", _Buff);
    }

    while (!feof(cfPtr) && input(NULL, _Buff, _Bufsize_, cfPtr) > 0) {
        if (5 == parse_line(_Buff, Old)) {
            int tmp1 = atoi(Old->cuid), key_tmp = atoi(key);
            if ('d' == mode) {
                if (tmp1 != key_tmp) {
                    sprintf(Old->cuid, "%06i", n++);
                    fprintf(tmp_cfPtr, "%s; %s; %s; %s; %s;\n", (Old->cuid), (Old->name), (Old->email), (Old->phone), (Old->address));
                }
            }
            else {
                if (tmp1 != key_tmp) {
                    fprintf(tmp_cfPtr, "%s; %s; %s; %s; %s;\n", (Old->cuid), (Old->name), (Old->email), (Old->phone), (Old->address));
                }
                else {
                    //* Edit the contact with the information in 'New'.
                    printf("\n \t\t \t\t ");
                    input("-> FULL NAME      : ", New->name, NAMESIZE, stdin);
                    printf("\n \t\t \t\t ");
                    input("-> EMAIL          : ", New->email, BUFFSIZE, stdin);
                    printf("\n \t\t \t\t ");
                    input("-> PHONE          : ", New->phone, BUFFSIZE, stdin);
                    printf("\n \t\t \t\t ");
                    input("-> ADDRESS        : ", New->address, MAXXBUFF, stdin);
                    sprintf(New->cuid, "%s", Old->cuid); //* Copy the 'cuid' from the original contact.
                    fprintf(tmp_cfPtr, "%s; %s; %s; %s; %s;\n", (New->cuid), (New->name), (New->email), (New->phone), (New->address));
                }
            }
        }
        else {
            fprintf(stderr, "\n\t\t [!] - Error parsing line."); //! Handle parsing errors.
        }
        memset(Old, 0, 1<<4); //* Clear the 'Old' structure for the next iteration.
    }
}

/*
 * The function below sets the console cursor position to the specified coordinates (x, y).
 * Works for both Windows and Unix-like systems.
*/
void SetCursorPosition(short x, short y) {
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
    COORD coordinates = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
#else
    printf("\033[%d;%dH", y + 1, x + 1); //* On non-Windows systems, use ANSI escape codes to set cursor position.
#endif
}

// TODO : Cross-platform function to get a character from the console without echoing.
int cgetch(void)
{
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
    //* On Windows, use the 'getch' function to get a character.
    return getch();
#else
    //* On non-Windows systems, set the terminal to non-canonical mode to get a character.
    struct termios old_tio, new_tio;
    int c;

    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    c = cgetchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
    return c;
#endif
}

/*
 * Prints a banner with custom text enclosed by parentheses.
 * The banner consists of angle brackets and a custom 'text' provided by the user.
 * The 'left' and 'right' parameters specify the number of parentheses on each side of the text.
*/
void print_banner(size_t left, string text, size_t right) {
    do {
        fputc('<', stdout);
        for (size_t i = 0; i < left; i++)
            fputs("()", stdout);
        if (NULL != text)
            fputs(text, stdout);
        for (size_t i = 0; i < right; i++)
            fputs("()", stdout);
        fputc('>', stdout); fflush(stdout);
    } while(EXIT_SUCCESS);
}