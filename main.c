#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ON_SHELF 1  // Book is available on the shelf.
#define IN_USER 0   // Book is currently borrowed by a user.
#define MAX_LOAN_DAYS 15 // Maximum loan period in days.
#define MAX_USERS 100 // Maximum number of users.
#define MAX_BOOKS 500 // Maximum number of books.

typedef struct{
 char name[100];
 char writer[100];
 char category[30];
 int  status; // ON_SHELF OR IN_USER
 int borrowerId;
 char loanDate[11]; // YYYY-MM-DD
 int id; // book-specific id.
}books;

typedef struct{
char name[100];
char password[20];
int numberOfBooks; // Number of books borrowed by the user.
int books[3]; // Can borrow three books at the same time.
int id; // user-specific id.
}users;

typedef  struct{
char name[100];
char password[20];
}admin;


//This function ensures that all necessary data files exist.
// If they don't, it creates them.
void prepareFiles(){
FILE* fptr;
fptr= fopen("users.txt" ,"a");
if(fptr==NULL){
        printf("Failed to open file\n");
   }else{
        fclose(fptr);
   }

fptr= fopen("books.txt" ,"a");
if(fptr==NULL){
        printf("Failed to open file\n");
   }else{
        fclose(fptr);
   }

   fptr= fopen("loans.txt","a");
if(fptr==NULL){
        printf("Failed to open file\n");
   }else{
        fclose(fptr);
   }

   fptr= fopen("admin.txt" ,"a");
if(fptr==NULL){
        printf("Failed to open file\n");
   }else{
        fclose(fptr);
   }

   fptr= fopen("messages.txt" ,"a");
if(fptr==NULL){
        printf("Failed to open file\n");
   }else{
        fclose(fptr);
   }


}

//Attempts to log in a user or an admin
//Return 0 for admin, userID for user, -1 for failure
// The reason return the userID is to recognize the user by their ID number when they log in.
// The user and admin have three attempts to log in.
int loginUser(){

 char tempUserName[100];  // Temporary storage for username input
 char tempPassword[20];   // Temporary storage for password input
 int attempts = 0;

 while(attempts<3){
printf("Please enter the username as shown. (example: Ahmet_Hamdi - use '_' instead of space)\n");
printf("Enter user name:\n");
scanf("%99s",tempUserName);
printf("Enter password:\n");
scanf("%19s",tempPassword);

FILE* fadmin = fopen("admin.txt","r");
FILE* fusers = fopen("users.txt","r");

if(fadmin==NULL || fusers==NULL){
    printf("Failed to open file");
    return -1;
}

//Check if the logged in an admin.
admin tempAdmin;
//Reads to the end of the file.
while(fscanf(fadmin, "%s %s",
    tempAdmin.name,
    tempAdmin.password)!=EOF){
    // If the username and password match the admin's credentials,it will finish the loop.
    if(strcmp(tempAdmin.name,tempUserName)==0 && strcmp(tempAdmin.password,tempPassword)==0){
        fclose(fadmin);
        fclose(fusers);
        return 0;
    }
}

//Check if the logged in a user
users tempUsers;
//Reads to the end of the file.
while(fscanf(fusers,"%s %s %d %d %d %d %d",
             tempUsers.name,
             tempUsers.password,
             &tempUsers.numberOfBooks,
             &tempUsers.books[0],
             &tempUsers.books[1],
             &tempUsers.books[2],
             &tempUsers.id)!=EOF){
                    // If the username and password match the user's credentials, it will finish the loop.
                 if(strcmp(tempUsers.name,tempUserName)==0 && strcmp(tempUsers.password,tempPassword)==0){
                    fclose(fadmin);
                    fclose(fusers);
                    return tempUsers.id;
                 }
}

  fclose(fadmin);
  fclose(fusers);
  attempts++;
  // If the username or password is incorrect, it will print the number of attempts left.
  printf("Incorrect username or password. Attempts left: %d\n", 3 - attempts);

}
printf("Too many failed attempts. Login blocked.\n");
return -1; // Login failed after 3 attempts.
}

//Function saves the user data to the "users.txt" file.
//I did this to reduce code duplication.
void saveUsers(users *userArray, int count) {
    FILE *f = fopen("users.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(f, "%s %s %d %d %d %d %d\n",
                userArray[i].name,
                userArray[i].password,
                userArray[i].numberOfBooks,
                userArray[i].books[0],
                userArray[i].books[1],
                userArray[i].books[2],
                userArray[i].id);
    }
    fclose(f);
}

//Function saves the book data to the "books.txt" file.
// I did this to reduce code duplication.
void saveBooks(books *bookArray, int count) {
    FILE *f = fopen("books.txt", "w");
    for (int i = 0; i < count; i++) {
        fprintf(f, "%s %s %s %d %d %s %d\n",
                bookArray[i].name,
                bookArray[i].writer,
                bookArray[i].category,
                bookArray[i].status,
                bookArray[i].borrowerId,
                bookArray[i].loanDate,
                bookArray[i].id);
    }
    fclose(f);
}

//Function saves the message data to the "messages.txt" file.
// I did this to reduce code duplication.
void saveMessages(char messages[][256], int count) {
    FILE *f = fopen("messages.txt", "w");
    if (!f) {
        printf("Failed to open messages file!\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(f, "%s\n", 
            messages[i]);
    }
    fclose(f);
}

//Function saves the loan data to the "loans.txt" file.
// I did this to reduce code duplication.
void saveLoans(int bookIDs[], int userIDs[], char loanDates[][12], int count) {
    FILE *f = fopen("loans.txt", "w");
    if (!f) {
        printf("Failed to open loans file!\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(f, "%d %d %s\n",
            bookIDs[i],
            userIDs[i],
            loanDates[i]);
    }
    fclose(f);
}

//Function that reads user data from file and stores it in an array.
// It returns the number of users loaded.
// I did this to reduce code duplication.
int loadUsers(users* arr) {
    FILE* f = fopen("users.txt", "r");
    if (!f) {
        printf("Failed to open users.txt\n");
        return 0;
    }

    int count = 0;
    // Read user data from the file and store it in the array
    while (fscanf(f, "%s %s %d %d %d %d %d",
                  arr[count].name,
                  arr[count].password,
                  &arr[count].numberOfBooks,
                  &arr[count].books[0],
                  &arr[count].books[1],
                  &arr[count].books[2],
                  &arr[count].id) != EOF) {
        count++;
    }

    fclose(f);
    return count;
}

//Function that reads book data from file and stores it in an array.
// It returns the number of books loaded.
// I did this to reduce code duplication.
int loadBooks(books* arr) {
    FILE* f = fopen("books.txt", "r");
    if (!f) {
        printf("Failed to open books.txt\n");
        return 0;
    }

    int count = 0;
    // Read book data from the file and store it in the array.
    while (fscanf(f, "%s %s %s %d %d %s %d",
                  arr[count].name,
                  arr[count].writer,
                  arr[count].category,
                  &arr[count].status,
                  &arr[count].borrowerId,
                  arr[count].loanDate,
                  &arr[count].id) != EOF) {
        count++;
    }

    fclose(f);
    return count;
}

//Function that reads loan data from file and stores it in arrays.
// It returns the number of loans loaded.
// I did this to reduce code duplication.
int loadLoans(int* bookIDs, int* userIDs, char loanDates[][12]) {
    FILE* f = fopen("loans.txt", "r");
    if (!f) return 0;
    int count = 0;
    while (fscanf(f, "%d %d %s",
        &bookIDs[count],
        &userIDs[count],
        loanDates[count]) != EOF)
        count++;
    fclose(f);
    return count;
}

//Function that reads messages from file and stores them in an array.
/// I did this to reduce code duplication.
int loadMessages(char messages[][256]) {
    FILE* f = fopen("messages.txt", "r");
    if (!f) {
        printf("Failed to open messages.txt\n");
        return 0;
    }

    int count = 0;
    while (fgets(messages[count], 256, f)) {
        count++;
    }

    fclose(f);
    return count;
}

// Function that adds data to the loan file for the user
// I did this to reduce code duplication.
void addLoan(int bookID, int userID, const char* loanDate) {
    int bookIDs[MAX_BOOKS], userIDs[MAX_BOOKS];
    char loanDates[MAX_BOOKS][12];
    int loanCount = loadLoans(bookIDs, userIDs, loanDates);

    bookIDs[loanCount] = bookID;
    userIDs[loanCount] = userID;
    strcpy(loanDates[loanCount], loanDate);
    loanCount++;

    saveLoans(bookIDs, userIDs, loanDates, loanCount);
}

// Function that deletes data from loan file for the user
// I did this to reduce code duplication.
void removeLoan(int bookID, int userID) {
    int bookIDs[MAX_BOOKS], userIDs[MAX_BOOKS];
    char loanDates[MAX_BOOKS][12];
    int loanCount = loadLoans(bookIDs, userIDs, loanDates);
    int newLoanCount = 0;
    for (int i = 0; i < loanCount; i++) {
        // Just copy the loans that do not match the bookID and userID to a new array.
        if (!(bookIDs[i] == bookID && userIDs[i] == userID)) {
            bookIDs[newLoanCount] = bookIDs[i];
            userIDs[newLoanCount] = userIDs[i];
            strcpy(loanDates[newLoanCount], loanDates[i]);
            newLoanCount++;
        }
    }
    saveLoans(bookIDs, userIDs, loanDates, newLoanCount);
}

// Check of which books were borrowed and when.
void checkLoanedBooks(){
       books tempBooks[MAX_BOOKS];
    users tempUsers[MAX_USERS];
    int bookcount = loadBooks(tempBooks);
    int userscount = loadUsers(tempUsers);

    int bookID, userID;
    char date[12];
    FILE* floans = fopen("loans.txt", "r");
    if (!floans) {
        printf("Failed to open loans.txt\n");
        return;
    }
    printf("\n------------------------------Borrowed Books------------------------------------\n");
    while (fscanf(floans, "%d %d %s",
        &bookID,
        &userID,
        date) != EOF) {
        char* bookName = NULL;
        char* userName = NULL;
        //User and book names are searched in the temporary arrays.
        for (int i = 0; i < bookcount; i++)
            if (tempBooks[i].id == bookID) {
                bookName = tempBooks[i].name;
                break;
            }
        for (int i = 0; i < userscount; i++)
            if (tempUsers[i].id == userID) {
                 userName = tempUsers[i].name;
                 break;
                }
        if (bookName && userName)
            printf("Book: %-30s | User: %-20s | Date:%s\n", bookName, userName, date);
    }
    fclose(floans);
}

// Listing function of books on the shelf.
void checkShelfBooks(){
    books tempBooks[MAX_BOOKS];
    int bookcount = loadBooks(tempBooks); // Kitapları dosyadan oku

    printf("\n------------------------------Available Books------------------------------------\n");
    // Loop through the temporary array and print the details of books that are on the shelf.
    for(int i=0; i<bookcount; i++){
        if(tempBooks[i].status == ON_SHELF){
            printf("Available Book:\n");
            printf("ID: %d | Book: %-20s | Writer: %-20s | Category:%s\n",
                tempBooks[i].id,
                tempBooks[i].name,
                tempBooks[i].writer,
                tempBooks[i].category);
        }
    }
}

//Book loan period is 15 days.
//Function that sends a message to users whose loan period exceeds 15 days.
//If no user has exceeded 15 days, no warning message is sent and the message no overdue books is displayed on the screen.
void notifyUser(){
users tempUsers[MAX_USERS];
    int userscount = loadUsers(tempUsers); // Load existing users from file
    int bookIDs[500], userIDs[500], loanCount = 0;
    char loanDates[500][12];
    loanCount = loadLoans(bookIDs, userIDs, loanDates); // Load existing loans from file
    FILE* fmessages = fopen("messages.txt", "a");
    if (!fmessages) {
        printf("Failed to open file.\n");
        return;
    }
    int overduecount = 0;
    for (int i = 0; i < loanCount; i++) {
        char* userName = NULL;
        for (int j = 0; j < userscount; j++) {
            if (tempUsers[j].id == userIDs[i]) {
                userName = tempUsers[j].name;
                break;
            }
        }

         if (userName != NULL) {
            int year, month, day;
            sscanf(loanDates[i], "%4d-%2d-%2d", &year, &month, &day);
            struct tm loanDate = {0};
            loanDate.tm_year = year - 1900;
            loanDate.tm_mon = month - 1;
            loanDate.tm_mday = day;
            time_t loanTime = mktime(&loanDate);
            time_t currentTime = time(NULL);
            double passDays = difftime(currentTime, loanTime) / (60 * 60 * 24);
            if (passDays > MAX_LOAN_DAYS) {
                fprintf(fmessages, "User: %-20s | Book ID: %d | Date:%s | Message: Your loan period has expired return the book.\n", userName, bookIDs[i], loanDates[i]);
                overduecount++;
            }
        }
    }
    fclose(fmessages);
    if (overduecount == 0) {
        printf("No overdue books found.\n");
    } else {
        printf("%d overdue notifications sent.\n", overduecount);
    }
}

//Function to add new users to the system by admin.
//If the username you wantt to add is already registered in the system, a message is sent This username is registered in the system please enter another username on the screen.
void addUser() {

    users tempUsers[MAX_USERS];
    int userCount = loadUsers(tempUsers); // Load existing users from file
    int lastID = 0;
    for (int i = 0; i < userCount; i++) {
        if (tempUsers[i].id > lastID) lastID = tempUsers[i].id;
    }
    // Check if the user count exceeds the maximum limit
    if (userCount >= MAX_USERS) {
        printf("User limit reached. Cannot add more users.\n");
        return ;
    }
    users newUser; // New user to be added
    int isDuplicate;
    do {
        isDuplicate = 0;
        printf("Please enter the username as shown. (example: Ahmet_Hamdi - use '_' instead of space)\n");
        printf("Enter user name:\n");
        scanf("%99s", newUser.name);

        // Check for duplicate usernames
        for (int i = 0; i < userCount; i++) {
            if (strcmp(tempUsers[i].name, newUser.name) == 0) {
                printf("This username is already registered. Please enter another.\n");
                isDuplicate = 1;
                break;
            }
        }
    } while (isDuplicate);

    printf("Enter password:\n");
    scanf("%19s", newUser.password);

   // Empty new user data
    newUser.numberOfBooks = 0;
    newUser.books[0] = -1;
    newUser.books[1] = -1;
    newUser.books[2] = -1;
    newUser.id = lastID + 1;

    tempUsers[userCount++] = newUser; // Add new user to temporary array

    saveUsers(tempUsers, userCount); // Save all users to file

    printf("User added successfully.\n");
}

//Function to delete users from the system by admin.
//Deletion is done with user ID.
//If the user has borrowed books, a message is sent to the screen that this user has borrowed books and cannot be deleted.
void deleteUser(){

    users tempUsers[MAX_USERS];
    int userCount = loadUsers(tempUsers); // Load existing users from file
    int deleteUserId;
    printf("Enter user id to delete:\n");
    scanf("%d", &deleteUserId);

    int found = 0, loanedBook = 0;
    for (int i = 0; i < userCount; i++) {
        if (tempUsers[i].id == deleteUserId) {
            found = 1;
            if (tempUsers[i].numberOfBooks > 0) loanedBook = 1;
            if (!loanedBook) {
                   //Delete data from loans file.
                  for (int b = 0; b < 3; b++) {
                    if (tempUsers[i].books[b] != -1) {
                        removeLoan(tempUsers[i].books[b], deleteUserId);
                    }
                }
                // If the user has not borrowed any books, we can delete them
                for (int j = i; j < userCount - 1; j++) {
                    tempUsers[j] = tempUsers[j + 1];
                }
                userCount--;
            }
            break;
        }
    }
    if (found){
        if(loanedBook){
            printf("This user has borrowed books and cannot be deleted.\n");
        }else{
            saveUsers(tempUsers, userCount);
            printf("User %d deleted successfully.\n", deleteUserId);
        }
    }else{
        printf("User %d not found.\n", deleteUserId);
    }
}

//Book adding function by admin.
//If the book you want to add is already registered in the system, a message is sent This book is already registered, you cannot add it again on the screen.
void addBook() {
  books tempBooks[MAX_BOOKS];
    int bookCount = loadBooks(tempBooks); // Load existing books from file
    int lastID = 0;
    for (int i = 0; i < bookCount; i++) {
        if (tempBooks[i].id > lastID) lastID = tempBooks[i].id;
    }
    // Check if the book count exceeds the maximum limit
    if (bookCount >= MAX_BOOKS) {
        printf("Book limit reached. Cannot add more books.\n");
        return;
    }

  // Check for duplicate book entries
    books newBook; // New book to be added
    int isDuplicate;
    do {
        isDuplicate = 0;
        printf("Please enter the book name as shown. (example: Karamazov_Kardesler - use '_' instead of space)\n");
        printf("Enter book name:\n");
        scanf("%99s", newBook.name);

        printf("Please enter the writer name as shown. (example: Ahmet_Hamdi_Tanpinar - use '_' instead of space)\n");
        printf("Enter writer name:\n");
        scanf("%99s", newBook.writer);

        printf("\nCategories: Fiction, Classic, Adventure, Distopya, Fantastic, Literature, Attempt, Drama, Love, Modern\n");
        printf("You can search by these categories. Pay attention to uppercase and lowercase letters\n");
        printf("Enter category:\n");
        scanf("%29s", newBook.category);

        for (int i = 0; i < bookCount; i++) {
            if (strcmp(tempBooks[i].name, newBook.name) == 0 &&
                strcmp(tempBooks[i].writer, newBook.writer) == 0) {
                printf("This book is already registered, you cannot add it again.\n");
                isDuplicate = 1;
                break;
            }
        }
    } while (isDuplicate);
    //Empty new book data
    newBook.id = lastID + 1;
    newBook.status = ON_SHELF;
    newBook.borrowerId = -1;
    strcpy(newBook.loanDate, "-");

    tempBooks[bookCount++] = newBook;

    saveBooks(tempBooks, bookCount);

    printf("Book added successfully.\n");
}

//Function to delete books from the system by admin.
//Deletion is done with book ID.
//If the book has borrowed by user, a message is sent to the screen that this book has borrowed and cannot be deleted.
void deleteBook(){
     books tempBooks[MAX_BOOKS];
    int bookCount = loadBooks(tempBooks); // Load existing books from file
    int deleteBookID;

    printf("Enter book id to delete:\n");
    scanf("%d", &deleteBookID);
    
    int found = 0, inUser = 0;
    for (int i = 0; i < bookCount; i++) {
        if (tempBooks[i].id == deleteBookID) {
            found = 1;
            if (tempBooks[i].status == IN_USER) inUser = 1;
            if (!inUser) {
                // Delete data from loans file.
                 for (int u = 0; u < MAX_USERS; u++) {
                    removeLoan(deleteBookID, u);
                }

                // If the book is not borrowed by any user, we can delete it
                for (int j = i; j < bookCount - 1; j++) {
                    tempBooks[j] = tempBooks[j + 1];
                }
                bookCount--;
            }
            break;
        }

    }

    if(found){
        if(inUser){
            printf("This book is currently borrowed by a user and cannot be deleted.\n");
            return;
        }else{
            saveBooks(tempBooks, bookCount);
            printf("Book with ID %d deleted successfully.\n", deleteBookID);
        }
    }else{
        printf("Book with ID %d not found.\n", deleteBookID);
    }
}

//Function to update book information by admin.
//The user is asked to enter the ID of the book they want to update.
//If the user enters 0 the update operation is cancelled.
void updateBook(){
    int targetID;  // Variable to store the ID of the book to be updated
    int found = 0;  // Flag to check if the book is found
    books tempBooks[MAX_BOOKS];  // Temporary array to hold existing books
    int booksCount = 0; // Counter for the number of books

    // Load existing books from file
    booksCount = loadBooks(tempBooks);

    // Print the current books on the shelf.
    checkShelfBooks();

    printf("Enter the ID of the book you want to update: (If you want to cancel, enter 0):\n");
    scanf("%d", &targetID);

    // If the user enters 0, cancel the update operation.
    if (targetID == 0) {
        printf("Update operation cancelled.\n");
        return;
    }

    // Prompt the user to enter new values for the book.
    // If the user enters '-', the current value will be kept.
    // If the user enters a new value, it will be updated.
    for (int i = 0; i < booksCount; i++) {
        if (tempBooks[i].id == targetID) {
            printf("Current Book Details:\n");
            printf("Name: %s\n", tempBooks[i].name);
            printf("Writer: %s\n", tempBooks[i].writer);
            printf("Category: %s\n", tempBooks[i].category);
            printf("Status: %s\n", tempBooks[i].status == ON_SHELF ? "On Shelf" : "Borrowed");
            printf("Borrower ID: %d\n", tempBooks[i].borrowerId);
            printf("Loan Date: %s\n", tempBooks[i].loanDate);

            printf("\nEnter new values (type '-' to keep current):\n");

            char newName[100], newWriter[100], newCategory[30], input[10];
            int newStatus, newBorrowerId;

            printf("New Name: ");
            scanf("%s", newName);
            if (strcmp(newName, "-") != 0)
                strcpy(tempBooks[i].name, newName);

            printf("New Writer: ");
            scanf("%s", newWriter);
            if (strcmp(newWriter, "-") != 0)
                strcpy(tempBooks[i].writer, newWriter);

            printf("New Category: ");
            scanf("%s", newCategory);
            if (strcmp(newCategory, "-") != 0)
                strcpy(tempBooks[i].category, newCategory);

            printf("New Status (1=On Shelf, 0=Borrowed): ");
            scanf("%s", input);
            if (strcmp(input, "-") != 0) {
                newStatus = atoi(input);
                if (newStatus == ON_SHELF || newStatus == IN_USER) {
                    tempBooks[i].status = newStatus;
                    if (newStatus == IN_USER) {
                        printf("Enter Borrower ID: ");
                        scanf("%d", &newBorrowerId);
                        tempBooks[i].borrowerId = newBorrowerId;

                        // Update loan date to today's date
                        time_t t = time(NULL);
                        struct tm tm = *localtime(&t);
                        sprintf(tempBooks[i].loanDate, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
                    } else {
                        tempBooks[i].borrowerId = -1;
                        strcpy(tempBooks[i].loanDate, "-");
                    }
                }
            }

            printf("Book updated successfully.\n");
            break;
        }
    }
    // Save the updated book data back to the file.
    saveBooks(tempBooks, booksCount);
}

//Function that searches by book name.
//If the book is borrowed by a user, a message is sent to screen that the book is borrowed and cannot be borrowed at the moment.
//If the book is not currently borrowed by anyone, the user is asked whether they want to borrow the book and the action is taken according to the user's answer.
//If the number of books borrowed by the user is 3, a message is sent to the screen maximum number of books has been reached and no more books can be borrowed will be displayed and the book borrowing processing will be cancelled.
void searchBookName(int userID) {
    books tempBooks[MAX_BOOKS]; // Temporary array to hold book data
    users tempUsers[MAX_USERS];  // Temporary array to hold user data
    int booksCount = loadBooks(tempBooks), usersCount = loadUsers(tempUsers);
    char tempBookName[100];  // Temporary storage for book name input
    int found = 0; // Flag to check if the book is found
    int bookIDs[500], userIDs[500]; // Arrays to hold book and user IDs for loans
    char loanDates[500][12];
    loadLoans(bookIDs, userIDs, loanDates);

    // Find the user index based on userID
    int userindex = -1;
    for (int i = 0; i < usersCount; i++) {
        if (tempUsers[i].id == userID) {
            userindex = i;
            break;
        }
    }
    if (userindex == -1) {
        printf("User not found!\n");
        return;
    }

    // Prompt the user to enter the book name
    printf("Please enter the book name as shown. (example: Karamazov_Kardesler - use '_' instead of space)\n");
    printf("Enter the book name:\n");
    scanf("%99s", tempBookName);

    // Search for the book in the temporary array
    for (int i = 0; i < booksCount; i++) {
        if (strcmp(tempBooks[i].name, tempBookName) == 0) {
            found = 1;
            if (tempBooks[i].status == ON_SHELF) {
                printf("\n  Book is avaliable on the Shelf. You can borrow this book.\n");
                printf("Book name: %s\n", tempBooks[i].name);
                printf("Writer name: %s\n", tempBooks[i].writer);
                printf("Category: %s\n", tempBooks[i].category);

                if (tempUsers[userindex].numberOfBooks < 3) {
                    int choice;
                    printf("Do you want to borrow this book? (1 for Yes, 0 for No): ");
                    scanf("%d", &choice);
                    if (choice == 1) {
                        tempBooks[i].status = IN_USER;
                        tempBooks[i].borrowerId = tempUsers[userindex].id;

                        // Today's date
                        time_t t = time(NULL);
                        struct tm tm = *localtime(&t);
                        sprintf(tempBooks[i].loanDate, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

                        tempUsers[userindex].books[tempUsers[userindex].numberOfBooks] = tempBooks[i].id;
                        tempUsers[userindex].numberOfBooks++;

                        // Write books to file
                       saveBooks(tempBooks, booksCount);
                        // Write users to file
                        saveUsers(tempUsers, usersCount);
                         // Write loans to file

                         addLoan(tempBooks[i].id, tempUsers[userindex].id, tempBooks[i].loanDate);

                        printf("Book borrowed successfully.\n");
                    } else {
                        printf("Book borrowing cancelled.\n");
                    }
                } else {
                    printf("You have reached the maximum number of borrowed books. You cannot borrow more.\n");
                }
            } else {
                printf("This book is not available on the shelf.\n");
            }
            break;
        }
    }
    if (!found) {
        printf("The book you are looking for could not be found.\n");
    }
}

//Function that searches by writer name.
//All books by the writer are listedi
//Questions to the user whether they want to borrow a book or not.
//If the user does not want to borrow a book, the borrowing process is canceled; if the user wants to borrow a book, the ID of the book he/she wants to borrow is asked.
//If the book is borrowed by a user, a message is sent to screen that the book is borrowed and cannot be borrowed at the moment.
//If the number of books borrowed by the user is 3, a message is sent to the screen maximum number of books has been reached and no more books can be borrowed will be displayed and the book borrowing processing will be cancelled.
void searchWriterName(int userID) {
    books tempBooks[500]; // Temporary array to hold book data
    users tempUsers[100]; // Temporary array to hold user data
    int booksCount = loadBooks(tempBooks), usersCount = loadUsers(tempUsers);
    char tempBookWriter[100]; // Temporary storage for writer name input
    int found = 0;  // Flag to check if any books by the writer are found
    int bookIDs[MAX_BOOKS], bookIndexes[MAX_BOOKS], listCount = 0;

    // Find the user index based on userID
    int userindex = -1;
    for (int i = 0; i < usersCount; i++) {
        if (tempUsers[i].id == userID) {
            userindex = i;
            break;
        }
    }
    if (userindex == -1) {
        printf("User not found!\n");
        return;
    }
    printf("Please enter the author name as shown. (example: Ahmet_Hamdi_Tanpinar - use '_' instead of space)\n");
    printf("Enter the writer name:\n");
    scanf("%99s", tempBookWriter);

    // List all books by the writer.
    printf("Books by %s:\n", tempBookWriter);
    for (int i = 0; i < booksCount; i++) {
        if (strcmp(tempBooks[i].writer, tempBookWriter) == 0) {
            printf("ID: %d | Book: %s | Category: %s | Status: %s\n",
                tempBooks[i].id,
                tempBooks[i].name,
                tempBooks[i].category,
                tempBooks[i].status == ON_SHELF ? "On Shelf" : "Borrowed");
            bookIDs[listCount] = tempBooks[i].id;
            bookIndexes[listCount] = i;
            listCount++;
            found = 1;
        }
    }

    if (!found) {
        printf("No books found for this writer.\n");
        return;
    }

    printf("Do you want to borrow a book? (1 for Yes, 0 for No): ");
    int borrowChoice;
    scanf("%d", &borrowChoice);
    if (borrowChoice != 1) {
        printf("Book borrowing cancelled.\n");
        return;
    }

    printf("Enter the ID of the book you want to borrow: ");
    int selectedID, selectedIndex = -1;
    scanf("%d", &selectedID);
    for (int i = 0; i < listCount; i++) {
        if (bookIDs[i] == selectedID) {
            selectedIndex = bookIndexes[i];
            break;
        }
    }
    if (selectedIndex == -1) {
        printf("Invalid book ID selection.\n");
        return;
    }

    // Controlls if the book is available for borrowing.
    if (tempBooks[selectedIndex].status != ON_SHELF) {
        printf(" Book is not available on the shelf. Book is currently borrowed at the moment.\n");
        return;
    }
    if (tempUsers[userindex].numberOfBooks >= 3) {
        printf("You have reached the maximum number of borrowed books. You cannot borrow more.\n");
        return;
    }

    // Today's date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(tempBooks[selectedIndex].loanDate, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    tempBooks[selectedIndex].status = IN_USER;
    tempBooks[selectedIndex].borrowerId = tempUsers[userindex].id;
    tempUsers[userindex].books[tempUsers[userindex].numberOfBooks] = tempBooks[selectedIndex].id;
    tempUsers[userindex].numberOfBooks++;

    // Write books to file
    saveBooks(tempBooks, booksCount);

    // Write users to file
    saveUsers(tempUsers, usersCount);

    // Write loans to file
    addLoan(tempBooks[selectedIndex].id, tempUsers[userindex].id, tempBooks[selectedIndex].loanDate);

    printf("Book borrowed successfully.\n");
}

//Function that searches by category.
//All books by the category are listed.
//Questions to the user whether they want to borrow a book or not.
//If the user does not want to borrow a book, the borrowing process is canceled; if the user wants to borrow a book, the ID of the book he/she wants to borrow is asked.
//If the book is borrowed by a user, a message is sent to screen that the book is borrowed and cannot be borrowed at the moment.
//If the number of books borrowed by the user is 3, a message is sent to the screen maximum number of books has been reached and no more books can be borrowed will be displayed and the book borrowing processing will be cancelled.
void searchCategory(int userID) {
    books tempBooks[MAX_BOOKS];  // Temporary array to hold book data
    users tempUsers[MAX_USERS];  // Temporary array to hold user data
    int booksCount = loadBooks(tempBooks), usersCount = loadUsers(tempUsers);  // Counters for the number of books and users
    char tempBookCategory[100]; // Temporary storage for category input
    int found = 0;
    int bookIDs[MAX_BOOKS], bookIndexes[MAX_BOOKS], listCount = 0;

    // Find the user index based on userID
    int userindex = -1;
    for (int i = 0; i < usersCount; i++) {
        if (tempUsers[i].id == userID) {
            userindex = i;
            break;
        }
    }
    if (userindex == -1) {
        printf("User not found!\n");
        return;
    }
    printf("\nCategories: Fiction, Classic, Adventure, Distopya, Fantastic, Literature, Attempt, Drama, Love, Modern\n");
    printf("You can search by these categories. Pay attention to uppercase and lowercase letters\n");
    printf("Enter the category name:\n");
    scanf("%29s", tempBookCategory);

    // List all books by the category.
    printf("Books in category %s:\n", tempBookCategory);
    for (int i = 0; i < booksCount; i++) {
        if (strcmp(tempBooks[i].category, tempBookCategory) == 0) {
            printf("ID: %d | Book: %s | Writer: %s | Status: %s\n",
                tempBooks[i].id,
                tempBooks[i].name,
                tempBooks[i].writer,
                tempBooks[i].status == ON_SHELF ? "On Shelf" : "Borrowed");
            bookIDs[listCount] = tempBooks[i].id;
            bookIndexes[listCount] = i;
            listCount++;
            found = 1;
        }
    }

    if (!found) {
        printf("No books found for this writer.\n");
        return;
    }

    printf("Do you want to borrow a book? (1 for Yes, 0 for No): ");
    int borrowChoice;
    scanf("%d", &borrowChoice);
    if (borrowChoice != 1) {
        printf("Book borrowing cancelled.\n");
        return;
    }

    printf("Enter the ID of the book you want to borrow: ");
    int selectedID, selectedIndex = -1;
    scanf("%d", &selectedID);
    for (int i = 0; i < listCount; i++) {
        if (bookIDs[i] == selectedID) {
            selectedIndex = bookIndexes[i];
            break;
        }
    }
    if (selectedIndex == -1) {
        printf("Invalid book ID selection.\n");
        return;
    }

    //Controlls.
    //If the book is available for borrowing.
    if (tempBooks[selectedIndex].status != ON_SHELF) {
        printf("Book is not available on the shelf. Book is currently borrowed at the moment.\n");
        return;
    }
    if (tempUsers[userindex].numberOfBooks >= 3) {
        printf("You have reached the maximum number of borrowed books. You cannot borrow more.\n");
        return;
    }

    // Today's date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(tempBooks[selectedIndex].loanDate, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    tempBooks[selectedIndex].status = IN_USER;
    tempBooks[selectedIndex].borrowerId = tempUsers[userindex].id;
    tempUsers[userindex].books[tempUsers[userindex].numberOfBooks] = tempBooks[selectedIndex].id;
    tempUsers[userindex].numberOfBooks++;

    // Write books to file
    saveBooks(tempBooks, booksCount);

    // Write users to file
    saveUsers(tempUsers, usersCount);

    // Write loans to file
    addLoan(tempBooks[selectedIndex].id, tempUsers[userindex].id, tempBooks[selectedIndex].loanDate);
    printf("Book borrowed successfully.\n");
}

//Return loaned book function.
//If the user has no borrowed books, a message is sent to the screen that you have no borrowed books to return.
//If the user has borrowed books, the books borrowed by the user are listed first, then the user is asked to select the book they want to return.
 void returnBook(int userID) {
    users tempUsers[MAX_USERS]; // Temporary array to hold user data
    books tempBooks[MAX_BOOKS];  // Temporary array to hold book data
    int usersCount = loadUsers(tempUsers), booksCount = loadBooks(tempBooks); // Counters for the number of users and books
    int bookID; // Variable to store the ID of the book to be returned

    // Find the user index based on userID
    int userIndex = -1;
    for (int i = 0; i < usersCount; i++) {
        if (tempUsers[i].id == userID) {
            userIndex = i;
            break;
        }
    }
    if (userIndex == -1) {
        printf("User not found!\n");
        return;
    }

    if (tempUsers[userIndex].numberOfBooks == 0) {
        printf("You have no borrowed books to return.\n");
        return;
    }

     // List borrowed books
    printf("Your borrowed books:\n");
    for (int i = 0; i < tempUsers[userIndex].numberOfBooks; i++) {
        int bID = tempUsers[userIndex].books[i];
        for (int j = 0; j < booksCount; j++) {
            if (tempBooks[j].id == bID) {
                printf("ID: %d | Book: %s | Writer: %s | Category: %s | Loan Date: %s\n",
                       tempBooks[j].id,
                       tempBooks[j].name,
                       tempBooks[j].writer,
                       tempBooks[j].category,
                       tempBooks[j].loanDate);
            }
        }
    }
    printf("Enter the book ID you want to return:\n");
    scanf("%d", &bookID);


    int bookIndex = -1;
    for (int i = 0; i < booksCount; i++) {
        if (tempBooks[i].id == bookID && tempBooks[i].borrowerId == userID) {
            bookIndex = i;
            break;
        }
    }

     if (bookIndex == -1) {
        printf("Book not found or not borrowed by user.\n");
        return;
    }
    // Book return process
    tempBooks[bookIndex].status = ON_SHELF;
    tempBooks[bookIndex].borrowerId = -1;
    strcpy(tempBooks[bookIndex].loanDate, "-");

    // Remove book from user and decrease numberOfBooks
    int foundBook = 0;
    for (int i = 0; i < tempUsers[userIndex].numberOfBooks; i++) {
        if (tempUsers[userIndex].books[i] == bookID) {
            foundBook = 1;
            for (int j = i; j < tempUsers[userIndex].numberOfBooks - 1; j++) {
                tempUsers[userIndex].books[j] = tempUsers[userIndex].books[j + 1];
            }
            tempUsers[userIndex].books[tempUsers[userIndex].numberOfBooks - 1] = -1;
            tempUsers[userIndex].numberOfBooks--;
            break;
        }
    }

     //Delete the loan record and update the loan file.
    removeLoan(bookID, userID);

    // Delete the related messages
    char messages[MAX_BOOKS * 3][256];
    int msgCount = loadMessages(messages);
    char pattern[120]; //user Id and book id pattern.
    sprintf(pattern, "User: %-20s | Book ID: %d", tempUsers[userIndex].name, bookID);
    int newMsgCount = 0;
    for (int i = 0; i < msgCount; i++) {
        if (strstr(messages[i], pattern) == NULL) { // If the message does not match the pattern, keep it
            strcpy(messages[newMsgCount++], messages[i]);
        }
    }
    saveMessages(messages, newMsgCount);

    saveBooks(tempBooks, booksCount); // Save updated book data to file
    saveUsers(tempUsers, usersCount); // Save updated user data to file
    printf("Book returned successfully.\n");
}

//Function that checks the borrowed books of the user.
//If the user has no borrowed books, a message is sent to the screen that you have not borrowed any books.
//If the user has borrowed books, the books borrowed by the user are listed.
 void checkBorrowedBooks(int userID) {
    users tempUsers[MAX_USERS];  // Temporary array to hold user data
    books tempBooks[MAX_BOOKS];  // Temporary array to hold book data
    int usersCount = loadUsers(tempUsers), booksCount = loadBooks(tempBooks);

    // Find the user index based on userID
    int userIndex = -1;
    for (int i = 0; i < usersCount; i++) {
        if (tempUsers[i].id == userID) {
            userIndex = i;
            break;
        }
    }
    if (userIndex == -1) {
        printf("User not found!\n");
        return;
    }
    if (tempUsers[userIndex].numberOfBooks == 0) {
        printf("You have not borrowed any books.\n");
        return;
    }

    printf("----------------------------Borrowed books:---------------------------------\n");
    for (int i = 0; i < tempUsers[userIndex].numberOfBooks; i++) {
        int bookID = tempUsers[userIndex].books[i];
        for (int j = 0; j < booksCount; j++) {
            if (tempBooks[j].id == bookID) {
                printf("ID: %d | Book: %s | Writer: %s | Category: %s | Status: %s | Borrower ID: %d | Loan Date: %s\n",
                       tempBooks[j].id,
                       tempBooks[j].name,
                       tempBooks[j].writer,
                       tempBooks[j].category,
                       (tempBooks[j].status == ON_SHELF) ? "Available" : "Not Available",
                       tempBooks[j].borrowerId,
                       tempBooks[j].loanDate);
            }
        }
    }
}

//Function that views the messages of the user.
//If the user has no messages, a message is sent to the screen that you have no messages.
void viewMessages(int userID){
    users tempUsers[MAX_USERS];  // Temporary array to hold user data
    int usersCount = loadUsers(tempUsers);   // Counter for the number of users
    char messages[MAX_BOOKS * 3][256];
    int msgCount = loadMessages(messages);
    int found = 0;
    char pattern[120];

    // Find the user index based on userID
    int userIndex = -1;
    for (int i = 0; i < usersCount; i++) {
        if (tempUsers[i].id == userID) {
            userIndex = i;
            break;
        }
    }
    if (userIndex == -1) {
        printf("User not found!\n");
        return;
    }

    sprintf(pattern, "User: %-20s", tempUsers[userIndex].name);
    for (int i = 0; i < msgCount; i++) {
        if (strstr(messages[i], pattern)) {
            printf("%s", messages[i]);
            found = 1;
        }
    }

    if (!found) {
        printf("You have no messages.\n");
    }
}

//Function that changes the password of the user.
//If the user enters the same password as the old password, a message is sent to the screen that the new password cannot be the same as the old password and the process is cancelled.
//If the user enters a different password, the password is changed and a message is sent to the screen that the password has been changed successfully.
//"The username is fixed in the system.The user can only change their password.
//This is done to protect system integrity and facilitate authorization control.
void changeUserPassword(int userID) {
    users tempUser[MAX_USERS]; // Temporary array to hold user data
    int userCount = loadUsers(tempUser), userIndex = -1;

    char userName[100], oldPassword[20], newPassword[20];
    printf("Enter your username: ");
    scanf("%99s", userName);

for (int i = 0; i < userCount; i++) {
    if (tempUser[i].id == userID) {
        userIndex = i;
        break;
    }
}
if (userIndex == -1) {
    printf("User not found!\n");
    return;
}

    printf("Enter your current password: ");
    scanf("%19s", oldPassword);
    if (strcmp(tempUser[userIndex].password, oldPassword) != 0) {
        printf("Current password is incorrect.\n");
        return;
    }

    printf("Enter new password for user %s: ", tempUser[userIndex].name);
    scanf("%19s", newPassword);

    if (strcmp(tempUser[userIndex].password, newPassword) == 0) {
        printf("New password cannot be the same as the old password. Please enter a different password.\n");
        return;
    }
    // Update the password in the temporary array
    strcpy(tempUser[userIndex].password, newPassword);

    saveUsers(tempUser, userCount);
    printf("Password changed successfully for user %s.\n", tempUser[userIndex].name);
}

// Function that changes the password of the admin.
//If the admin enters the same password as the old password, a message is sent to the screen that the new password cannot be the same as the old password and the process is cancelled.
//If the admin enters a different password, the password is changed and a message is sent to the screen that the password has been changed successfully.
void changeAdminPassword() {

    admin tempAdmins;
    char newPassword[20], oldPassword[20];  // Temporary storage for admin name and old password

    FILE *fadmin = fopen("admin.txt", "r");
    if (!fadmin) {
        printf("Admin file could not be opened!\n");
        return;
    }
    // Read admin data from the file and store it in the temporary array.
    //Just one admin is stored in the file, so we read it directly.
   if (fscanf(fadmin, "%s %s", tempAdmins.name, tempAdmins.password) != 2) {
        fclose(fadmin);
        printf("No admin found!\n");
        return;
    }
    fclose(fadmin);

    printf("Enter your admin username: ");
    char adminName[100];
    scanf("%99s", adminName);

    if (strcmp(tempAdmins.name, adminName) != 0) {
        printf("Admin not found!\n");
        return;
    }

    printf("Enter your current password: ");
    scanf("%19s", oldPassword);
   if (strcmp(tempAdmins.password, oldPassword) != 0) {
        printf("Current password is incorrect.\n");
        return;
    }


    printf("Enter new password for admin %s: ", tempAdmins.name);
    scanf("%19s", newPassword);

    if (strcmp(tempAdmins.password, newPassword) == 0) {
        printf("New password cannot be the same as the old password. Please enter a different password.\n");
        return;
    }

    strcpy(tempAdmins.password, newPassword);

    fadmin = fopen("admin.txt", "w");
    if (!fadmin) {
        printf("Admin file could not be opened for writing!\n");
        return;
    }

        fprintf(fadmin, "%s %s\n", tempAdmins.name, tempAdmins.password);

    fclose(fadmin);
    printf("Password changed successfully for admin %s.\n", tempAdmins.name);
}

// Function that deletes the user account.
//If the user has borrowed books, a message is sent to the screen that you have borrowed books and you cannot delete your account.
//If the user has no borrowed books, the user is asked to confirm the deletion of the account.
//If the user confirms the deletion, the account is deleted from the file and a message is sent to the screen that your account has been deleted successfully.
void deleteUserAccount(int userID) {
    printf("Are you sure you want to delete your account? (y/n): ");
    char confirm;
    scanf(" %c", &confirm);
    if (confirm != 'y' && confirm != 'Y') {
        printf("Account deletion cancelled.\n");
        return;
    }

    users tempUser[MAX_USERS]; // Temporary array to hold user data
    int usersCount = loadUsers(tempUser);
    int found = 0;

    for (int i = 0; i < usersCount; i++) {
        if (tempUser[i].id == userID) {
            found = 1;
            if (tempUser[i].numberOfBooks > 0) {
                printf("You have borrowed books. Please return them before deleting your account.\n");
                return;
            }
              //Delete data from the loans file.
            for (int b = 0; b < 3; b++) {
                if (tempUser[i].books[b] != -1) {
                    removeLoan(tempUser[i].books[b], userID);
                }
            }

            // Shift remaining users to remove the deleted user
            for (int j = i; j < usersCount - 1; j++) {
                tempUser[j] = tempUser[j + 1];
            }
            usersCount--;
            break;
        }
    }
  if (found) {
        saveUsers(tempUser, usersCount);
        printf("Your account has been deleted successfully.\n");
        exit(0);
    } else {
        printf("Account not found.\n");
    }
}

int main()
{   int number;
    prepareFiles();
    int userID=loginUser();
    if (userID==0){
    printf("Login successful. You are an admin.\n");
    printf("__________________________________________________________________________________________________\n");
    char again='y';
    while(again=='y' || again=='Y'){
    printf("Press 1 to edit user information.\n");
    printf("Press 2 to edit book information.\n");
    printf("Press 3 to check borrowed information.\n");
    printf("Press 4 to view books on the shelf.\n");
    printf("Press 5 to notify users of book delays.\n");
    printf("Press 6 to change admin password.\n");
    printf("___________________________________________________________________________________________________\n");
    scanf("%d",&number);

    switch(number){
          case 1:
              printf("Press 1 to add user.\n");
              printf("Press 2 to delete user.\n");
              scanf("%d",&number);
              printf("_________________________________________________________________________________________\n");
              if(number==1){
                printf("Adding user.\n");
                addUser();

              }else if(number==2){
                printf("Deleting user.\n");
                deleteUser();

              }else{
                printf("Invalid number please try again.\n");
                printf("_______________________________________________________________________________________\n");
              }

              break;
          case 2:
              printf("Press 1 to add book.\n");
              printf("Press 2 to delete book.\n");
              printf("Press 3 to update book information.\n");
              scanf("%d",&number);
              printf("_____________________________________________________________________________________________\n");
              if(number==1){
                printf("Adding book.\n");
                addBook();

              }else if(number==2){
                printf("Deleting book.\n");
                deleteBook();

              }else if(number==3){
                printf("Updating book information.\n");
                updateBook();

              }else{
                printf("Invalid number please try again.\n");
                printf("______________________________________________________________________________________________\n");
              }

              break;
          case 3:
             checkLoanedBooks();
             break;

          case 4:
             checkShelfBooks();
              break;

          case 5:
                notifyUser();
              break;

          case 6:
                    printf("Changing admin password.\n");
                    changeAdminPassword();
                    break;
          default:
            printf("Invalid number please try again.");
            printf("______________________________________________________________________________________________________\n");

    }
    printf("Do you want to continue? If yes, press y or Y If no, press other.\n");
    scanf(" %c", &again);
    printf("_____________________________________________________________________________________________________________\n");
    }


    }else if(userID >0){
    printf("Login successful. You are a user.\n");
    printf("__________________________________________________________________________________________________\n");
    char again='y';
    while(again=='y' || again=='Y'){
    printf("Press 1 to search for books.\n");
    printf("Press 2 to return a book.\n");
    printf("Press 3 to check borrowed information.\n");
    printf("Press 4 to view messages.\n");
    printf("Press 5 to view books on the shelf.\n");
    printf("Press 6 to change password.\n");    
    printf("Press 7 to delete account.\n");
    printf("__________________________________________________________________________________________________\n");
    scanf("%d",&number);

    switch(number){
          case 1:
        printf("Press 1 to search for book name.\n");
        printf("Press 2 to search for writer name.\n");
        printf("Press 3 to search for category.\n");
        scanf("%d",&number);
        printf("__________________________________________________________________________________________________\n");
        if(number==1){
            printf("Searching by book name.\n");
            searchBookName(userID);

        }else if(number==2){
            printf("Searching by writer name.\n");
            searchWriterName(userID);

        }else if(number==3){
            printf("Searching by category.\n");
            searchCategory(userID);

        }else{
            printf("Invalid number please try again.\n");
            printf("__________________________________________________________________________________________________\n");

        }
              break;
          case 2:
                printf("Returning book.\n");
                returnBook(userID);

              break;
          case 3:
                printf("Checking borrowed information.\n");
                checkBorrowedBooks(userID);
              break;
          case 4:
                printf("Viewing messages.\n");
                viewMessages(userID);
              break;
          case 5:
                    printf("Viewing books on the shelf.\n");
                    checkShelfBooks();
                    break;
          case 6:
                printf("Changing password.\n");
                changeUserPassword(userID);
                break;
          case 7:
                printf("Deleting account.\n");
                deleteUserAccount(userID);
                break;
          default:
              printf("Invalid number please try again.\n");
                printf("__________________________________________________________________________________________________\n");
    }

        printf("Do you want to continue? If yes, press y or Y If no, press other.\n");
        scanf(" %c", &again);
        printf("__________________________________________________________________________________________________\n");
    }

    }
    else{
    printf("\nlogin failed");
    printf("__________________________________________________________________________________________________\n");
    }
    return 0;
}
