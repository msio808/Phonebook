# Phonebook Application

The Phonebook Application is a terminal-based contact management system that allows users to store, edit, list, search, and delete contact information in a file.

## Features

- **Add New Contact**: Add a new contact with details such as name, email, phone, and address.

- **Edit Contact**: Modify existing contact details.

- **List Contacts**: View a list of all stored contacts.

- **Search Contacts**: Find specific contacts by name.

- **Delete Contact**: Remove unwanted contacts.

## Prerequisites

- [GCC](https://gcc.gnu.org/) or any other C compiler for building the project.

## Installation

1. Clone the repository to your local machine:

   ```bash
   git clone https://github.com/mrmas-io/phonebook.git
   cd phonebook/config
   ```
2. Build using **`GCC`**:

   ```bash
   ./build.sh --run
   ```


## Usage

1. Run the application:
   1. **On Windows:**
        ```bash
        .\main.exe
        ```
    2. **On Unix:**
        ```bash
        ./main
        ```

2. You will be prompted to log in with a predefined username and password. The default is "admin" for both.

3. Use the menu to navigate through the application's features.

## Function Descriptions

- **`add_contact()`**: Adds a new contact to the phonebook.

- **`edit_contact()`**: Edits an existing contact's details.

- **`list_contact()`**: Lists all contacts in the phonebook.

- **`search_contact()`**: Searches for a contact by name.

- **`delete_contact()`**: Deletes a contact from the phonebook.

- **`exit_program(int)`**: Exits the program.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.