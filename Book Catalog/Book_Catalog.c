#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_LENGTH	100
#define INPUT_OPERATION_LENGTH	4
#define	CURRENT_YEAR 2025

const int SUCCESS = 0;
const int EXIT_NUMBER = 6;
const int maxStringSize = 50;

typedef struct Book {
	int id;
	char title[MAX_LENGTH];
	char author[MAX_LENGTH];
	int	publication_year;
	struct Book* next;
} Book;

// Declared function prototypes
void addBook(Book** head, int id, const char* title, const char* author, int publication_year);
void viewBooks(Book* head);
void updateBook(Book* head, int id);
void deleteBook(Book** head, int id);
void searchBooks(Book* head, const char* title);

// Declared function prototypes for operation and validation
void displayMenu(void);
void getMenuNum(int* menuNum);
void getParameters(int menuNum, int* id, char* title, char* author, int* publication_year);
void getId(int* id);
void getTitleOrAuthor(int typeOfData, char* str, int strLength);
void getPublicationYear(int* publication_year);
void freeCatalog(Book* head);
bool isHeadNull(Book* head);
char* trimWhitespace(char* str);
void clearNewLineChar(char* str);

// enum for menu
enum functions {
	ADD,
	VIEW,
	UPDATE,
	DELETE,
	SEARCH,
	EXIT		
} OPERATIONS;

// enum for string types
enum stringTypes {
	TITLE,
	AUTHOR
} STRING_TYPES;

int main(void) {
	Book* head = NULL; // Initialize linked list

	int menuNum = 0;

	int id = 0;
	char title[MAX_LENGTH] = "";
	char author[MAX_LENGTH] = "";
	int publication_year = 0;

	while (true) {
		displayMenu();
		getMenuNum(&menuNum);
		getParameters(menuNum, &id, title, author, &publication_year);

		OPERATIONS = menuNum - 1;
		switch (OPERATIONS)
		{
		case ADD:
			addBook(&head, id, title, author, publication_year);
			break;
		case VIEW:
			viewBooks(head);
			break;
		case UPDATE:
			updateBook(head, id);
			break;
		case DELETE:
			deleteBook(&head, id);
			break;
		case SEARCH:
			searchBooks(head, title);
			break;
		default:
			break;
		}

		if (menuNum == EXIT_NUMBER) {
			freeCatalog(head);
			printf("Thank you for using. Have a good day!\n");
			break;
		}
	}

	return SUCCESS;
}


/* FUNCTION: addBook
   DESCRIPTION	: Appends a new book to the end of the linked list after verifying that the book ID is unique.
   PARAMETERS	: Book** head           : A pointer to the head pointer of the linked list.
				  int id                : The unique identifier for the new book.
				  const char* title     : The title of the new book.
				  const char* author    : The author of the new book.
				  int publication_year  : The publication year of the new book.
   RETURNS		: void
*/

void addBook(Book** head, int id, const char* title, const char* author, int publication_year) {
	// Check for copied ID
	struct Book* current = *head;

	if (!isHeadNull(*head)) {
		while (current->next != NULL) {
			if (current->id == id) {
				printf("Error: Book ID %d already exists.\n", id);
				return;
			}
			current = current->next;
		}

		// Checked last book id
		if (current->id == id) {
			printf("Error: Book ID %d already exists.\n", id);
			return;
		}
	}

	// Allocate memory for new book
	struct Book* newBook = (struct Book*)calloc(1, sizeof(struct Book));
	if (newBook == NULL) {
		fprintf(stderr, "Cannot create book '%s' (ID:%d). Memory exhausted.\n", title, id);
		return;
	}

	// Initialize new book data
	newBook->id = id;
	strncpy(newBook->title, title, MAX_LENGTH);
	newBook->title[MAX_LENGTH - 1] = '\0';  // Ensure null-termination
	strncpy(newBook->author, author, MAX_LENGTH);
	newBook->author[MAX_LENGTH - 1] = '\0';
	newBook->publication_year = publication_year;
	newBook->next = NULL;

	// Add to the end of the list
	if (*head == NULL) {
		*head = newBook;
	}
	else {
		current->next = newBook;
	}

	printf("Successfully added a Book.\n");
}

//
// FUNCTION		: viewBooks
// DESCRIPTION	: Prints list of all (if any) Books
// PARAMETERS	: Book* head	:	Pointer to linked list of Books
// RETURNS		: None
//
void viewBooks(Book* head) {
	Book* current = head;

	//Displays an error message if there are no books in the catalog
	if (isHeadNull(head)) {
		printf("Error: There are currently no books in the catalog.\n");
		return;
	}
	// Runs a loop to cycle through all the books in the catalog and display their details
	else {
		printf("\n=================================================================================================\n");
		printf("%-10s%-40s%-30s%-20s\n", (char*)"Book ID", (char*)"Title", (char*)"Author", (char*)"Publication Year");
		printf("=================================================================================================\n");
		while (current != NULL) {
			printf("%-10d%-40s%-30s%-10d\n", current->id, current->title, current->author, current->publication_year);
			current = current->next;
		}
		printf("=================================================================================================\n");
	}
	return;
}

/*
   FUNCTION     : updateBook
   DESCRIPTION  : Searches for a book by its id in the linked list and updates its title, author, and publication year based on user input.
   PARAMETERS   : Book* head : Pointer to the head of the linked list of books
				  int id     : The identifier of the book to be updated
   RETURNS      : void
*/
void updateBook(Book* head, int id) {
	// Initialize pointer to the head of the list
	Book* current = head;

	// Using while loop until the end of the list is reached
	while (current != NULL) {
		// Using if statement to check if the current book's id matches the target id
		if (current->id == id) {

			// Print that book was found to the screen
			printf("Book found: ID: %d.\n", id);

			// Read the new title from input
			getTitleOrAuthor(TITLE, current->title, MAX_LENGTH);

			// Read the new author from input
			getTitleOrAuthor(AUTHOR, current->author, MAX_LENGTH);

			// Read the publication year from input and check for valid integer input
			getPublicationYear(&current->publication_year);

			// Print that book was updated to the screen
			printf("Successfully updated a Book.\n");

			// Exit the function
			return;
		}
		// Move to the next book in the list
		current = current->next;
	}
	// Print the word to the screen to indicate that no book with the given ID was found.
	printf("Book ID %d not found.\n", id);
}

//
// DESCRIPTION	: This function deletes a book from linked list using the given Book id.
// PARAMETERS	: Book* head	:	Pointer to the head of the linked list of books
//				  int id		:	The id entered by the user for deletion.
// RETURNS		: None
//
void deleteBook(Book** head, int id) {
	if (isHeadNull(*head)) {
		printf("\nNo data available for deletion.\n");
		return;
	}

	Book* current = *head;
	Book* mustBeFree = NULL; // Store Book to delete

	// Check Head
	if (current->id == id) {
		*head = current->next;
		free(current);
		printf("Book ID - %d is deleted\n", id);
		return;
	}
	// Iterate through Linked List
	while (current->next != NULL && current->next->id != id) {
		current = current->next;
	}
	// Checked Tail
	if (current->next == NULL) {
		printf("Book ID - %d does not exist in the catalog\n", id);
		return;
	}
	// Free node if it was deleted
	mustBeFree = current->next;
	current->next = mustBeFree->next;
	free(mustBeFree);

	printf("Book ID - %d is deleted\n", id);
}

//
// FUNCTION		: searchBooks
// DESCRIPTION	: This function allows the user to search a book by title that contains the string they have entered (case-insensitive)
// PARAMETERS	: Book* head			:	Pointer to the head of the linked list of books
//				  const char* title		:	The search query as inputted by the user
// RETURNS		:
//				  void
void searchBooks(Book* head, const char* title) {
	// Check if list is empty - if so, return
	if (head == NULL) {
		printf("No Books to search.\n");
		return;
	}

	int numSearchResults = 0; // Store number of results
	// Store search query into char array and convert to lowercase
	char query[MAX_LENGTH] = "";
	strncpy(query, title, MAX_LENGTH);
	query[MAX_LENGTH - 1] = '\0'; // Ensure null-termination from strncpy()
	_strlwr(query);
	int titleLength = (int)strlen(title); // Store length of search query title

	// Check if search query is empty - if so, return
	if (titleLength == 0) {
		printf("Search query cannot be empty.\n");
		return;
	}

	bool isShowHeaderTemplate = true; // Show header if there are search results
	char currentTitle[MAX_LENGTH] = ""; // Store current title in char array
	int currentTitleLength = 0; // Store length of current title string

	Book* current = head;
	while (current != NULL) {
		// Store current title and its length
		strncpy(currentTitle, current->title, MAX_LENGTH);
		currentTitle[MAX_LENGTH - 1] = '\0'; // Ensure null-termination from strncpy()
		_strlwr(currentTitle); // Convert title to lowercase
		currentTitleLength = (int)strlen(currentTitle);

		// If there substring (query) is found in title, print result
		if (strstr(currentTitle, query) != NULL) {
			if (isShowHeaderTemplate) {
				printf("\n=================================================================================================\n");
				printf("%-10s%-40s%-30s%-20s\n", (char*)"Book ID", (char*)"Title", (char*)"Author", (char*)"Publication Year");
				printf("=================================================================================================\n");
				isShowHeaderTemplate = false;
			}
			// If there are no mismatches, then the query is in the title, and print result
			printf("%-10d%-40s%-30s%-10d\n", current->id, current->title, current->author, current->publication_year);
			numSearchResults++;
		}
		current = current->next;
	}

	// Print if there were no search results to console
	if (numSearchResults == 0)
	{
		printf("\nNo books found.\n\n");
	}
	else {
		printf("=================================================================================================\n\n");
	}

	return;
}

//
// FUNCTION		: displayMenu
// DESCRIPTION	: Displays the menu of operations for the user
// PARAMETERS	: None
// RETURNS		: None
//

void displayMenu(void) {
	printf("1. addBook\n");
	printf("2. viewBooks\n");
	printf("3. updateBook\n");
	printf("4. deleteBook\n");
	printf("5. searchBooks\n");
	printf("6. exit program\n");
}

//
// FUNCTION		: getMenuNum
// DESCRIPTION	: Asks the user to input a number for menu operation
// PARAMETERS	: int* menuNum	:	Integer of menu operation
// RETURNS		: None
//
void getMenuNum(int* menuNum) {
	char caMenuNum[INPUT_OPERATION_LENGTH] = "";
	const int kAllowedInputLength = 2;
	const int kStartMenuNum = 1;
	const int kEndMenuNum = 5;
	bool loopFlag = true;

	while (loopFlag) {
		printf("Please input a number for operation (only one digit): ");
		fgets(caMenuNum, sizeof(caMenuNum), stdin);

		if (strlen(caMenuNum) > kAllowedInputLength || sscanf_s(caMenuNum, "%d", menuNum) == 0) {
			printf("Wrong number was inputted.\n");
			continue;
		}

		if (!(*menuNum >= kStartMenuNum && *menuNum <= kEndMenuNum) && *menuNum != EXIT_NUMBER) {
			printf("Invalid menu number.\n");
			continue;
		}

		break;
	}
}

//
// FUNCTION		: getParameters
// DESCRIPTION	: Calls functions to obtain parameters for operation depending on user function
//				  and stores them in variables
// PARAMETERS	: int menuNum			:	Menu operation from getMenuNum
//				  int* id				:	Pointer to integer variable storing book ID
//				  char* title			:	String variable storing book title
//				  char* author			:	String variable  storing author
//				  int* publication_year	:	Pointer to integer variable storing publication year
// RETURNS		: None
//
void getParameters(int menuNum, int* id, char* title, char* author, int* publication_year) {
	enum functions menu = menuNum - 1;
	switch (menu)
	{
	case ADD:
		getId(id);
		getTitleOrAuthor(TITLE, title, MAX_LENGTH);
		getTitleOrAuthor(AUTHOR, author, MAX_LENGTH);
		getPublicationYear(publication_year);
		break;
	case UPDATE:
	case DELETE:
		getId(id);
		break;
	case SEARCH:
		getTitleOrAuthor(TITLE, title, MAX_LENGTH);
		break;
	default:
		break;
	}
}

//
// FUNCTION		: getID
// DESCRIPTION	: Get integer for book ID from user onput
// PARAMETERS	: int* id : Pointer to integer containing book ID
// RETURNS		: None
//
void getId(int* id) {
	char caIdBuff[MAX_LENGTH] = ""; // Buffer for user input
	char extraChar;
	bool validID = false; // Flag for valid ID

	while (!validID) {
		// Prompt user for book ID
		printf("Please input Book ID: ");
		fgets(caIdBuff, sizeof(caIdBuff), stdin);
		clearNewLineChar(caIdBuff);

		// If converting failed from user's input to integer - print error
		if (sscanf_s(caIdBuff, "%d %c", id, &extraChar, (int)sizeof(char)) != 1) {
			printf("Invalid input. Please enter a valid positive integer for Book ID. \n");
		}
		else {
			// Check if ID number is positive
			if (*id < 0) {
				printf("Invalid input: Please enter a positive number for Book ID.\n");
			}
			else { // ID is valid, break loop
				validID = true;
			}
		}
	}
}

//
// FUNCTION		: getTitleOrAuthor
// DESCRIPTION	: Get a text from user
// PARAMETERS	: int* publication_year : It will be changed by user input.
// RETURNS		: None
//
void getTitleOrAuthor(int typeOfData, char* str, int strLength) {
	bool validData = false; // Flag for whether input was valid

	while (!validData) {
		// Prompt user for Book or Author title
		if (typeOfData == TITLE) {
			printf("Please input Book title: ");
		}
		else if (typeOfData == AUTHOR) {
			printf("Please input Author name: ");
		}

		// Get user input
		fgets(str, strLength, stdin);
		clearNewLineChar(str);
		
		// Validate user input
		str = trimWhitespace(str); // Trim whitespace from input
		if (strlen(str) == 0) { // If string is empty, ask for input again
			printf("Input cannot be empty.\n");
		}
		else { // Input is valid, break loop
			validData = true;
		}
	}
}

//
// FUNCTION		: getPublicationYear
// DESCRIPTION	: Get publish year from user input
// PARAMETERS	: int* publication_year : It will be changed by user input.
// RETURNS		: None
//
void getPublicationYear(int* publication_year) {
	char caPublishedYearBuff[MAX_LENGTH] = ""; // Buffer for user input
	char extraChar;
	bool validYear = false;

	while (!validYear) {
		// Prompt user for year of publication
		printf("Please input year book was published: ");
		fgets(caPublishedYearBuff, sizeof(caPublishedYearBuff), stdin);

		clearNewLineChar(caPublishedYearBuff);

		// If converting failed from user's input to integer  - print error
		if (sscanf_s(caPublishedYearBuff, "%d %c", publication_year, &extraChar, (int)sizeof(char)) != 1) {
			printf("Invalid input. Please enter year in YYYY format.\n");
		}
		else {
			// Check if year is valid number
			if (*publication_year > CURRENT_YEAR) {
				printf("Invalid year. Please enter year in YYYY format.\n");
			}
			else { // Year is valid, break loop
				validYear = true;
			}
		}
	}
}

//
// FUNCTION		: freeCatalog
// DESCRIPTION	: Releases memory allocated for the linked list of Books
// PARAMETERS	: Book* head	:	Pointer to linked list
// RETURNS		: None
//
void freeCatalog(Book* head) {
	Book* mustBeFree = NULL;

	if (isHeadNull(head)) {
		printf("List is already empty\n");
	}
	else {
		while (head != NULL) {
			mustBeFree = head;
			head = head->next;
			free(mustBeFree);
		}
	}
}

//
// FUNCTION		: isHeadNull
// DESCRIPTION	: Returns boolean of whether linked list of Books is empty by checking if head is null
// PARAMETERS	: Book* head	:	Pointer to linked list of Books
// RETURNS		: bool
//
bool isHeadNull(Book* head) {
	return head == NULL;
}

//
// FUNCTION		: clearNewLineChar
// DESCRIPTION	: Removes the trailing newline character \n from fgets input
// PARAMETERS	: char* str		:	String to be modified
// RETURNS		: None
//
void clearNewLineChar(char* str) {
	str[strlen(str) - 1] = '\0';
}

//
// FUNCTION		: trimWhitespace
// DESCRIPTION	: Trims any leading and/or trailing whitespace in a string
// PARAMETERS	: char* str		:	String to be modified
// RETURNS		: char*			:	Trimmed string
//
char* trimWhitespace(char* str) {
	int strLength = (int)strlen(str); // Store length of string
	int trimmedStrLength = 0; // Store new length of string

	// Check if string is empty
	if (strLength == 0) {
		return str;
	}
	else
	{
		// Remove leading whitespace
		while (isspace(*str)) {
			str++; // Increment pointer of string to skip leading spaces
		}
		trimmedStrLength = (int)strlen(str); // Calculate new length of string

		// Remove trailing whitespace
		while (isspace(str[trimmedStrLength - 1]) && trimmedStrLength > 0) {
			trimmedStrLength--; // While last character is a space, decrement string length
		}

		// Null-terminate string at first non-space character from end of string
		str[trimmedStrLength] = '\0';
	}

	return str;
}