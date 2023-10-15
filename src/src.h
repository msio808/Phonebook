/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * Created by MrMas on 07/10/2023 @ 02:29PM  * * * * * * * * * * * * * * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef PHONEBOOK_SRC_H
#define PHONEBOOK_SRC_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <stdbool.h>

#define BUFFSIZE 0x20
#define NAMESIZE 0x40
#define MAXXBUFF 0x80
#define LINESIZE 0x1000
typedef char *string;

#define DELIMITER "\", \""
#define clr() printf("\033c")
#define CTRL_K(key) ((key) & 0x1f)

typedef struct contacts {
    char cuid[1<<3];
    char name[NAMESIZE];
    char email[BUFFSIZE];
    char phone[BUFFSIZE];
    char address[MAXXBUFF];
    struct credentials {
        char username[BUFFSIZE];
        char password[BUFFSIZE];
    } creds;
} contact;

//TODO: Function declarations with comments

//* Displays the main menu of the phonebook application.
void menu();
//* Handles the login process for the phonebook application.
void login();
//* Adds a new contact to the phonebook.
void add_contact();
//* Lists all the contacts in the phonebook.
void list_contact();
//* Edits an existing contact in the phonebook.
void edit_contact();
//* Searches for a contact in the phonebook by name or email.
void search_contact();
//* Deletes a contact from the phonebook.
void delete_contact();
//* Exits the phonebook application.
void exit_program();

//* Utility function to delete a character at a given position from a string.
void del_char(size_t);
//* Prints the details of a contact to the console.
void printline(contact*);
//* Sets the cursor position on the terminal.
void SetCursorPosition(short, short);
//* Prints a banner to the console.
void print_banner(size_t, string, size_t);
//* Rewrites/Deletes a line in a CSV file.
void rewrite_line(contact*, contact*, string, size_t, string, FILE*, FILE*, char);

//* Cross-platform getch function for reading characters from the console.
int cgetch(void);
//* Reads a line from a CSV file and returns new ID.
int getline_ID(contact*, FILE*);
//* Parses a line from a file into a contact structure.
int parse_line(string, contact*);
//* Searches for a specific line in a CSV file by name or email.
int findline(contact*, string, size_t, string, FILE*);

//* Checks for duplicate contacts in the phonebook.
int check_duplicate(contact*, string, string, FILE*);
//* Utility function to convert a string to uppercase.
string strUpr(string);
//* Utility function to add quotes to a string.
string quote(const string);
//* Case-insensitive string search function.
string stristr(string, const string);
//* Reads input from the user with a specified prompt and validation.
size_t input(const string, string, int, FILE*);

//* Platform-specific includes for Windows and Unix-like systems.
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
#include <windows.h>
#include <conio.h>
#define ENTER_KEY 0x0D
#define BACKSPACE_KEY 0x08
#else
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#define ENTER_KEY 0x0A
#define BACKSPACE_KEY 0x7F
#endif

#endif //PHONEBOOK_SRC_H
