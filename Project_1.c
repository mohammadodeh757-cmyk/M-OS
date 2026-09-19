#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
typedef struct{
    int pageLength;
    char *pageContent;
}pages;
typedef struct{
    char bookName[40];
    int bookID;
    int pageNumber;
    char bookType[30];
    int bookMark;
    pages page[200];
}library;
typedef struct{
    char taskName[30];
    int taskID;
    char taskDescription[80];
    char taskPriority[15];
    char taskStatus[15];
}task;
typedef struct{
    int noteID;
    char noteTitle[30];
    char noteContent[250];
    char noteDate[11];
}note;
typedef struct{
    int transactionID;
    char transactionType[10];
    float amount;
    char transactionDate[11];
}transaction;
typedef struct{
    int eventID;
    char eventName[40];
    char eventDate[11];
    int day;
    int month;
    int year;
}event;
event events[50];
transaction transactions[150];
library books[15];
task tasks[25];
note notes[30];
int bookCount = 0;
int taskCount = 0;
int noteCount = 0;
int eventCount = 0;
int pendingTasks = 0;
int upcomingEvents = 0;
int nextID = 1000;
int N_nextID = 0;
int T_nextID = 0;
int E_nextID = 0;
int transactionCount = 0;
float balance = 0;
char buffer[250];
int lastDay = 0;
void loadSystem();
void initializeSystem();
void displayMainMenu();
void libraryMenu();
void displayBooks();
void readBook();
void addBook();
void deleteBook();
void searchBook();
void taskMenu();
void displayTasks();
void addTask();
void viewTask();
void completeTask();
void deleteTask();
void notesMenu();
void displayNotes();
void viewNote();
void addNote();
void deleteNote();
void financeMenu();
void addMoney();
void spendMoney();
void displayTransactions();
void displayPlanner();
void plannerMenu();
void addEvent();
void displayEvents();
void deleteEvent();
void updateUpcomingEvents();
void shutdownSystem();
int main(){
    int userChoice=7;
    loadSystem();
    initializeSystem();
    updateUpcomingEvents();
    while(userChoice != 0){
        displayMainMenu();
        while(1){
            if((scanf("%d",&userChoice))!=1){
                printf("Invalid input. Please enter a number from 0 to 5\n");
                getchar();
            }
            else{
                break;
            }
        }
        fgets(buffer,sizeof(buffer),stdin);
        switch(userChoice){
            case 0:
            system("cls");
            shutdownSystem();
            userChoice = 0;
            break;
            case 1:
            system("cls");
            libraryMenu();
            break;
            case 2:
            system("cls");
            taskMenu();
            break;
            case 3:
            system("cls");
            notesMenu();
            break;
            case 4:
            system("cls");
            financeMenu();
            break;
            case 5:
            system("cls");
            plannerMenu();
            break;
            default:
            printf("Invalid input. Please enter a number from 0 to 5\n");
            continue;
        }
        if(userChoice == 0){
            break;
        }
        system("cls");
        initializeSystem();
    }
    return 0;
}
void loadSystem(){
    time_t secs = time(0);
    struct tm *full_time = localtime(&secs);
    FILE *fpt_books = fopen("books.bin", "rb");
    if(fpt_books == NULL){
        bookCount = 0;
    }
    else{
        for(int i=0;i<15;i++){
            if(fread(&books[i].bookID, sizeof(int), 1, fpt_books) != 1){
            break;
            }
            else{
                bookCount++;
            }
            fread(books[i].bookName,sizeof(char),40,fpt_books);
            fread(&books[i].pageNumber,sizeof(int),1,fpt_books);
            fread(books[i].bookType,sizeof(char),30,fpt_books);
            fread(&books[i].bookMark,sizeof(int),1,fpt_books);
        }
        fclose(fpt_books);
    }
    FILE *fpt_tasks = fopen("tasks.bin", "rb");
    if(fpt_tasks == NULL){
        taskCount = 0;
    }
    else{
        taskCount = fread(tasks, sizeof(task), 25, fpt_tasks);
        fclose(fpt_tasks);
    }
    if(taskCount == 0){
        nextID = 1000;
    }
    else{
        nextID = tasks[taskCount-1].taskID + 1;
    }
    for(int i=0;i<taskCount;i++){
        if(strcmp(tasks[i].taskStatus,"Pending") == 0){
            pendingTasks++;
        }
    }
    FILE *fpt_notes = fopen("notes.bin", "rb");
    if(fpt_notes == NULL){
        noteCount = 0;
    }
    else{
        noteCount = fread(notes, sizeof(note), 30, fpt_notes);
        fclose(fpt_notes);
    }
    if(noteCount == 0){
        N_nextID = 1;
    }
    else{
        N_nextID = notes[noteCount-1].noteID + 1;
    }
    FILE *fpt_finance = fopen("finance.bin", "rb");
    if(fpt_finance == NULL){
        transactionCount = 0;
        balance = 0;
    }
    else{
        transactionCount = fread(transactions, sizeof(transaction), 150, fpt_finance);
        for(int i=0;i<transactionCount;i++){
            balance += transactions[i].amount;
        }
        fclose(fpt_finance);
    }
    if(transactionCount == 0){
        T_nextID = 1;
        balance = 0;
    }
    else{
        T_nextID = transactions[transactionCount-1].transactionID + 1;
    }
    FILE *fpt_planner = fopen("planner.bin", "rb");
    if(fpt_planner == NULL){
        E_nextID = 1;
    }
    else{
        eventCount = fread(events, sizeof(event), 50, fpt_planner);
        fclose(fpt_planner);
    }
    if(eventCount == 0){
        E_nextID = 1;
    }
    else{
        E_nextID = events[eventCount-1].eventID + 1;
    }
    for(int i=0;i<eventCount;i++){
        if(events[i].month == (full_time->tm_mon + 1)
        && events[i].year == (full_time->tm_year + 1900)){
            upcomingEvents++;
        }
    }
}
void initializeSystem(){
    printf("================================================\n");
    printf("                     M-OS v0.1\n");
    printf("           Personal Management System\n");
    printf("================================================\n\n");
    printf("                 Welcome, Mohammed\n\n");
    printf("------------------------------------------------\n");
    printf("Dashboard:\n\n");
    printf("Books        : %d\n",bookCount);
    printf("Pending Tasks: %d\n",pendingTasks);
    printf("Notes        : %d\n",noteCount);
    printf("Balance      : %.1f ILS\n",balance);
    printf("Upcoming     : %d events\n\n",upcomingEvents);
    printf("------------------------------------------------\n\n");
}
void displayMainMenu(){
    printf("Choose a module:\n\n");
    printf("[1] Personal Library\n");
    printf("[2] Task Manager\n");
    printf("[3] Notes\n");
    printf("[4] Finance Tracker\n");
    printf("[5] Personal Planner\n\n");
    printf("[0] Shutdown\n");
}
void libraryMenu(){
    int choice=7;
    printf("================================================\n");
    printf("                     M-OS\n");
    printf("               Personal Library\n");
    printf("================================================\n\n");
    printf("Welcome to your personal library.\n\n");
    printf("------------------------------------------------\n\n");
    printf("Total Books : %d\n\n",bookCount);
    printf("------------------------------------------------\n");
    printf("1. View All Books\n");
    printf("2. Read a Book\n");
    printf("3. Add New Book\n");
    printf("4. Delete Book\n");
    printf("5. Search Book\n");
    printf("0. Back to Main Menu\n");
    while(choice!=0){
        while(1){
            if((scanf("%d",&choice))!=1){
                printf("Invalid input. Please enter a number from 0 to 6\n");
                getchar();
            }
            else{
                break;
            }
        }
        fgets(buffer,sizeof(buffer),stdin);
        switch(choice){
            case 0:
            return ;
            case 1:
            system("cls");
            displayBooks();
            break;
            case 2:
            system("cls");
            readBook();
            break;
            case 3:
            system("cls");
            addBook();
            break;
            case 4:
            system("cls");
            deleteBook();
            break;
            case 5:
            system("cls");
            searchBook();
            break;
            default:
            printf("Invalid input. Please enter a number from 0 to 6\n");
            continue;
        }
        printf("1. View All Books\n");
        printf("2. Read a Book\n");
        printf("3. Add New Book\n");
        printf("4. Delete Book\n");
        printf("5. Search Book\n");
        printf("0. Back to Main Menu\n");
    }
}
void displayBooks(){
    if(bookCount == 0){
        printf("No books in the system.\n");
        return;
    }
    printf("Book ID\t\tBook Name\tType\t\tPages\n");
    printf("-----------------------------------------------------\n");
    for(int i=0;i<bookCount;i++){
        printf("%d\t\t",books[i].bookID);
        if(strlen(books[i].bookName)>=16){
            printf("%s",books[i].bookName);
        }
        else if(strlen(books[i].bookName)>=8 && strlen(books[i].bookName)<16){
            printf("%s\t",books[i].bookName);
        }
        else{
            printf("%s\t\t",books[i].bookName);
        }
        if(strlen(books[i].bookType)<8){
            printf("%s\t\t",books[i].bookType);
        }
        else{
            printf("%s\t",books[i].bookType);
        }
        printf("%d\n",books[i].pageNumber);
    }
    printf("-----------------------------------------------------\n");
}
void addBook(){
    FILE*fpt_out;
    int ID,check=1,choice;
    char Name[40],Type[30],content[1500],fileName[15];
    printf("Enter Book ID: ");
    while(check){
        check=0;
        if((scanf("%d",&ID))!=1){
            printf("Book ID must consist of digits only. Please try again\n");
            check=1;
            getchar();
            continue;
        }
        if(ID<1000 || ID>9999){
            printf("Invalid ID. It should be 4 digits.\n");
            check=1;
            continue;
        }
        for(int i=0;i<bookCount;i++){
            if(ID==books[i].bookID){
                printf("Book ID already exists. Please try again\n");
                check=1;
                break;
            }
        }
    }
    check = 1;
    books[bookCount].bookID = ID;
    fgets(buffer,sizeof(buffer),stdin);
    printf("Enter Book Name: ");
    fgets(Name,sizeof(Name),stdin);
    int length = strlen(Name);
    while(length-1==0){
        printf("Book Name cannot be empty. Please try again\n");
        printf("Enter Book Name: ");
        fgets(Name,sizeof(Name),stdin);
        length = strlen(Name);
    }
    strncpy(books[bookCount].bookName,Name,length-1);
    books[bookCount].bookName[length - 1] = '\0';
    printf("Enter Book Type: ");
    fgets(Type,sizeof(Type),stdin);
    int length2 = strlen(Type);
    while(length2-1==0){
        printf("Book Type cannot be empty. Please try again\n");
        printf("Enter Type Name: ");
        fgets(Type,sizeof(Type),stdin);
        length2 = strlen(Type);
    }
    strncpy(books[bookCount].bookType,Type,length2-1);
    books[bookCount].bookType[length2 - 1] = '\0';
    sprintf(fileName,"Book_%d",books[bookCount].bookID);
    fpt_out = fopen(fileName,"wb");
    fwrite(&books[bookCount].bookID,sizeof(int),1,fpt_out);
    fwrite(books[bookCount].bookName,sizeof(books[bookCount].bookName),1,fpt_out);
    fwrite(books[bookCount].bookType,sizeof(books[bookCount].bookType),1,fpt_out);
    fclose(fpt_out);
    books[bookCount].pageNumber = 0;
    books[bookCount].bookMark = 0;
    for(int i=0;i<200;i++){
        printf("Enter page %d content: ",i+1);
        if(fgets(content, sizeof(content), stdin) == NULL){
            printf("Input error!\n");
            bookCount++;
            return;
        }
        books[bookCount].page[i].pageContent=malloc((strlen(content)+1)*sizeof(char));
        if(books[bookCount].page[i].pageContent == NULL){
            printf("Memory allocation failed!\n");
            bookCount++;
            return;
        }
        strcpy(books[bookCount].page[i].pageContent, content);
        books[bookCount].page[i].pageLength = strlen(content)+1;
        printf("Enter 0 to return or 1 to continue: ");
        scanf("%d",&choice);
        fgets(buffer, sizeof(buffer), stdin);
        fpt_out = fopen(fileName,"ab");
        fwrite(&books[bookCount].page[i].pageLength,sizeof(int),1,fpt_out);
        fwrite(books[bookCount].page[i].pageContent,sizeof(char),books[bookCount].page[i].pageLength,fpt_out);
        books[bookCount].pageNumber++;
        fclose(fpt_out);
        free(books[bookCount].page[i].pageContent);
        if(choice == 0){
            bookCount++;
            return;
        }
        else{
            system("cls");
        }
    }
    bookCount++;
}
void readBook(){
    int ID,check=1,i,j,k,pageNum,length,choice,bookID;
    char fileName[15],bookName[40],Type[30],content[1500];
    printf("Enter book ID: ");
    while(check){
        check=0;
        if((scanf("%d",&ID))!=1){
            printf("Book ID must consist of digits only. Please try again\n");
            check=1;
            getchar();
            continue;
        }
        if(ID<1000 || ID>9999){
            printf("Invalid ID. It should be 4 digits.\n");
            check=1;
            continue;
        }
        for(j=0;j<bookCount;j++){
            if(ID == books[j].bookID){
                printf("Book ID exists\n");
                break;
            }
        }
    }
    check = 1;
    fgets(buffer,sizeof(buffer),stdin);
    if(j == bookCount){
        printf("Invalid ID\n");
        return;
    }
    sprintf(fileName,"Book_%d",ID);
    FILE *fpt_in = fopen(fileName,"rb");
    if(fpt_in == NULL){
        printf("Unable to open the book file. The book may not exist.\n");
        return;
    }
    printf("Enter starting page: ");
    while(check){
        check=0;
        if((scanf("%d",&pageNum))!=1){
            printf("Page number must consist of digits only. Please try again\n");
            check=1;
            getchar();
            continue;
        }
        if(pageNum<=0){
            printf("Invalid input. Page number must be positive integer.\n");
            check=1;
            continue;
        }
        pageNum-=1;
    }
    fgets(buffer,sizeof(buffer),stdin);
    fread(&bookID,sizeof(int),1,fpt_in);
    fread(bookName,sizeof(bookName),1,fpt_in);
    fread(Type,sizeof(Type),1,fpt_in);
    for(k=0;k<pageNum;k++){
        fread(&length,sizeof(int),1,fpt_in);
        fread(content,sizeof(char),length,fpt_in);
    }
    for(;k<books[j].pageNumber;k++){
        fread(&length,sizeof(int),1,fpt_in);
        fread(content,sizeof(char),length,fpt_in);
        printf("%s\n",content);
        printf("Enter 0 to return or 1 to continue: ");
        scanf("%d",&choice);
        fgets(buffer,sizeof(buffer),stdin);
        if(choice == 0){
            fclose(fpt_in);
            books[j].bookMark = k;
            return;
        }
        else{
            system("cls");
        }
    }
    books[j].bookMark = k;
    fclose(fpt_in);
}
void deleteBook(){
    int ID,check=1,j,choice;
    char fileName[15];
    printf("Enter book ID: ");
    while(check){
        check=0;
        if((scanf("%d",&ID))!=1){
            printf("Book ID must consist of digits only. Please try again\n");
            check=1;
            getchar();
            continue;
        }
        if(ID<1000 || ID>9999){
            printf("Invalid ID. It should be 4 digits.\n");
            check=1;
            continue;
        }
        for(j=0;j<bookCount;j++){
            if(ID == books[j].bookID){
                printf("Book ID exists\n");
                break;
            }
        }
    }
    fgets(buffer,sizeof(buffer),stdin);
    check = 1;
    if(j == bookCount){
        printf("Error: Book ID not found.\n");
        return;
    }
    printf("Are you sure you want to delete this book? %s\n",books[j].bookName);
    while(check){
        check=0;
        printf("Enter 0 to confirm or 1 to cancel: ");
        if((scanf("%d",&choice))!=1){
            printf("Invalid input. Please enter a number.\n");
            check=1;
            getchar();
            continue;
        }
        if(choice != 0 && choice != 1){
            printf("Invalid choice. Please enter 0 or 1.\n");
            check=1;
        }
    }
    fgets(buffer,sizeof(buffer),stdin);
    if(choice == 1){
        return;
    }
    sprintf(fileName,"Book_%d",ID);
    if(remove(fileName) == 0){
        printf("Book deleted successfully.\n");
    }
    else{
        printf("Failed to delete book file.\n");
        return;
    }
    for(;j<bookCount-1;j++){
        books[j].bookID =  books[j+1].bookID;
        books[j].bookMark = books[j+1].bookMark;
        books[j].pageNumber = books[j+1].pageNumber;
        strncpy(books[j].bookName,books[j+1].bookName,40);
        strncpy(books[j].bookType,books[j+1].bookType,30);
    }
    bookCount--;
}
void searchBook(){
    int ID,check=1,j;
    printf("Enter book ID: ");
    while(check){
        check=0;
        if((scanf("%d",&ID))!=1){
            printf("Book ID must consist of digits only. Please try again\n");
            check=1;
            getchar();
            continue;
        }
        if(ID<1000 || ID>9999){
            printf("Invalid ID. It should be 4 digits.\n");
            check=1;
            continue;
        }
        for(j=0;j<bookCount;j++){
            if(ID == books[j].bookID){
                printf("Book ID exists\n");
                break;
            }
        }
    }
    fgets(buffer,sizeof(buffer),stdin);
    check = 1;
    if(j == bookCount){
        printf("Error: Book ID not found.\n");
        return;
    }
    printf("Book Name: %s\n",books[j].bookName);
    printf("Book Type: %s\n",books[j].bookType);
    printf("Book ID: %d\n",books[j].bookID);
    printf("Number of pages: %d\n",books[j].pageNumber);
    printf("Book Mark: page %d\n",books[j].bookMark+1);
}
void taskMenu(){
    int choice=7;
    printf("================================================\n");
    printf("                     M-OS\n");
    printf("                 Task Manager\n");
    printf("================================================\n\n");
    printf("Welcome to your task manager.\n\n");
    printf("------------------------------------------------\n\n");
    printf("Total Tasks : %d\n\n",taskCount);
    printf("------------------------------------------------\n");
    printf("1. Display All Tasks\n");
    printf("2. Add New Task\n");
    printf("3. View Task\n");
    printf("4. Complete Task\n");
    printf("5. Delete Task\n");
    printf("0. Back to Main Menu\n");
    while(choice!=0){
        while(1){
            if((scanf("%d",&choice))!=1){
                printf("Invalid input. Please enter a number from 0 to 5\n");
                getchar();
            }
            else{
                break;
            }
        }
        fgets(buffer,sizeof(buffer),stdin);
        switch(choice){
            case 0:
            return;
            case 1:
            system("cls");
            displayTasks();
            break;
            case 2:
            system("cls");
            addTask();
            break;
            case 3:
            system("cls");
            viewTask();
            break;
            case 4:
            system("cls");
            completeTask();
            break;
            case 5:
            system("cls");
            deleteTask();
            break;
            default:
            printf("Invalid input. Please enter a number from 0 to 5\n");
            continue;
        }
        printf("1. Display All Tasks\n");
        printf("2. Add New Task\n");
        printf("3. View Task\n");
        printf("4. Complete Task\n");
        printf("5. Delete Task\n");
        printf("0. Back to Main Menu\n");
    }
}
void displayTasks(){
    if(taskCount == 0){
        printf("No tasks in the system.\n");
        return;
    }
    printf("ID\tTask Name\t\tPriority\tStatus\n");
    printf("---------------------------------------------------------\n");
    for(int i=0;i<taskCount;i++){
        printf("%d\t",tasks[i].taskID);
        if(strlen(tasks[i].taskName)>=24){
            printf("%s",tasks[i].taskName);
        }
        else if(strlen(tasks[i].taskName)>=16 && strlen(tasks[i].taskName)<24){
            printf("%s\t",tasks[i].taskName);
        }
        else if(strlen(tasks[i].taskName)>=8 && strlen(tasks[i].taskName)<16){
            printf("%s\t\t",tasks[i].taskName);
        }
        else{
            printf("%s\t\t\t",tasks[i].taskName);
        }
        if(strlen(tasks[i].taskPriority)>=8){
            printf("%s\t",tasks[i].taskPriority);
        }
        else{
            printf("%s\t\t",tasks[i].taskPriority);
        }
        printf("%s\n",tasks[i].taskStatus);
    }
    printf("---------------------------------------------------------\n\n");
    printf("Total tasks: %d\n",taskCount);
}
void addTask(){
    char Name[30],status[15]={"Pending"},content[80];
    int length,choice,check=1;
    char options[4][15] = {"Low","Medium","High","Urgent"};
    printf("Enter Task Name: ");
    fgets(Name, sizeof(Name), stdin);
    length = strlen(Name);
    while(length-1 == 0){
        printf("Task Name cannot be empty. Please try again\n");
        printf("Enter Task Name: ");
        fgets(Name,sizeof(Name),stdin);
        length = strlen(Name);
    }
    Name[length-1] = '\0';
    strcpy(tasks[taskCount].taskName, Name);
    printf("Select Task Priority:\n");
    printf("1. Low\n");
    printf("2. Medium\n");
    printf("3. High\n");
    printf("4. Urgent\n");
    while(check){
        check = 0;
        if((scanf("%d",&choice))!=1){
            printf("Invalid input. Please enter a number.\n");
            check=1;
            getchar();
            continue;
        }
        if(choice<1 || choice>4){
            printf("Invalid choice. Please enter a number between 1 and 4.\n");
            check=1;
        }
    }
    fgets(buffer,sizeof(buffer),stdin);
    strcpy(tasks[taskCount].taskPriority,options[choice - 1]);
    strcpy(tasks[taskCount].taskStatus,status);
    fgets(buffer, sizeof(buffer), stdin);
    printf("Enter task content: ");
    if(fgets(content, sizeof(content), stdin) == NULL){
        printf("Input error!\n");
        return;
    }
    length = strlen(content);
    while(length-1 == 0){
        printf("Task content cannot be empty. Please try again\n");
        printf("Enter Task content: ");
        fgets(content,sizeof(content),stdin);
        length = strlen(content);
    }
    content[length-1] = '\0';
    strcpy(tasks[taskCount].taskDescription, content);
    tasks[taskCount].taskID = nextID;
    nextID++;
    pendingTasks++;
    taskCount++;
}
void completeTask(){
    int ID,check,i;
    char status[15] = {"Completed"};
    printf("Enter the Task ID to complete: ");
    while(check){
        check=0;
        if((scanf("%d",&ID))!=1){
            printf("Task ID must consist of digits only. Please try again\n");
            check=1;
            getchar();
            continue;
        }
        if(ID < 1000){
            printf("Task ID must be greater than 1000. Please try again.\n");
            check=1;
        }
    }
    for(i=0;i<taskCount;i++){
        if(ID == tasks[i].taskID){
            printf("Task ID exists\n");
            break;
        }
    }
    fgets(buffer,sizeof(buffer),stdin);
    if(i == taskCount){
        printf("Error: Task ID not found.\n");
        return;
    }
    strcpy(tasks[i].taskStatus,status);
    printf("Task completed successfully!\n");
}
void deleteTask(){
    int ID,check=1,i;
    printf("Enter the Task ID to delete: ");
    while(check){
        check=0;
        if((scanf("%d",&ID))!=1){
            printf("Task ID must consist of digits only. Please try again\n");
            check=1;
            getchar();
            continue;
        }
        if(ID < 1000){
            printf("Task ID must be greater than 1000. Please try again.\n");
            check=1;
        }
    }
    for(i=0;i<taskCount;i++){
        if(ID == tasks[i].taskID){
            printf("Task ID exists\n");
            break;
        }
    }
    fgets(buffer,sizeof(buffer),stdin);
    if(i == taskCount){
        printf("Error: Task ID not found.\n");
        return;
    }
    for(;i<taskCount-1;i++){
        tasks[i].taskID = tasks[i+1].taskID;
        strncpy(tasks[i].taskName,tasks[i+1].taskName,30);
        strncpy(tasks[i].taskPriority,tasks[i+1].taskPriority,15);
        strncpy(tasks[i].taskStatus,tasks[i+1].taskStatus,15);
        strncpy(tasks[i].taskDescription,tasks[i+1].taskDescription,80);
    }
    taskCount--;
    printf("Task deleted successfully!\n");
}
void viewTask(){
    int ID,check=1,j;
    printf("Enter task ID: ");
    while(check){
        check=0;
        if((scanf("%d",&ID))!=1){
            printf("Task ID must consist of digits only. Please try again\n");
            check=1;
            getchar();
            continue;
        }
        if(ID<1000){
            printf("Task ID must be greater than 1000. Please try again.\n");
            check=1;
            continue;
        }
        for(j=0;j<taskCount;j++){
            if(ID == tasks[j].taskID){
                break;
            }
        }
    }
    fgets(buffer,sizeof(buffer),stdin);
    check = 1;
    if(j == taskCount){
        printf("Error: Task ID not found.\n");
        return;
    }
    printf("Task Name: %s\n",tasks[j].taskName);
    printf("Task ID: %d\n",ID);
    printf("Task Priority: %s\n",tasks[j].taskPriority);
    printf("Task Status: %s\n",tasks[j].taskStatus);
    printf("Task Content: %s\n",tasks[j].taskDescription);
}
void notesMenu(){
    int choice=7;
    printf("================================================\n");
    printf("                     M-OS\n");
    printf("                Notes Manager\n");
    printf("================================================\n\n");
    printf("Welcome to your Notes manager.\n\n");
    printf("------------------------------------------------\n\n");
    printf("Total Notes : %d\n\n",noteCount);
    printf("------------------------------------------------\n");
    printf("1. Display All Notes\n");
    printf("2. Add New Note\n");
    printf("3. View Note\n");
    printf("4. Delete Note\n");
    printf("0. Back to Main Menu\n");
    while(choice!=0){
        while(1){
            if((scanf("%d",&choice))!=1){
                printf("Invalid input. Please enter a number from 0 to 4\n");
                getchar();
            }
            else{
                break;
            }
        }
        fgets(buffer,sizeof(buffer),stdin);
        switch(choice){
            case 0:
            return;
            case 1:
            system("cls");
            displayNotes();
            break;
            case 2:
            system("cls");
            addNote();
            break;
            case 3:
            system("cls");
            viewNote();
            break;
            case 4:
            system("cls");
            deleteNote();
            break;
            default:
            printf("Invalid input. Please enter a number from 0 to 4\n");
            continue;
        }
        printf("1. Display All Notes\n");
        printf("2. Add New Note\n");
        printf("3. View Note\n");
        printf("4. Delete Note\n");
        printf("0. Back to Main Menu\n");
    }
}
void displayNotes(){
    if(noteCount == 0){
        printf("No notes in the system.\n");
        return;
    }
    printf("ID\tNote Title\t\t\tDate\n");
    printf("--------------------------------------------------\n");
    for(int i=0;i<noteCount;i++){
        printf("%d\t",notes[i].noteID);
        if(strlen(notes[i].noteTitle)>=32){
            printf("%s",notes[i].noteTitle);
        }
        else if(strlen(notes[i].noteTitle)>=24 && strlen(notes[i].noteTitle)<32){
            printf("%s\t",notes[i].noteTitle);
        }
        else if(strlen(notes[i].noteTitle)>=16 && strlen(notes[i].noteTitle)<24){
            printf("%s\t\t",notes[i].noteTitle);
        }
        else if(strlen(notes[i].noteTitle)>=8 && strlen(notes[i].noteTitle)<16){
            printf("%s\t\t\t",notes[i].noteTitle);
        }
        else{
            printf("%s\t\t\t\t",notes[i].noteTitle);
        }
        printf("%s\n",notes[i].noteDate);
    }
    printf("--------------------------------------------------\n");
    printf("Total notes: %d\n",noteCount);
}
void addNote(){
    char Name[35],content[250];
    printf("Enter Note Title: ");
    fgets(Name, sizeof(Name), stdin);
    int length = strlen(Name);
    while(length-1 == 0){
        printf("Note title cannot be empty. Please try again\n");
        printf("Enter Note Title: ");
        fgets(Name,sizeof(Name),stdin);
        length = strlen(Name);
    }
    Name[length-1] = '\0';
    strcpy(notes[noteCount].noteTitle, Name);
    printf("Enter note content: ");
    if(fgets(content, sizeof(content), stdin) == NULL){
        printf("Input error!\n");
        return;
    }
    length = strlen(content);
    while(length-1 == 0){
        printf("Note content cannot be empty. Please try again\n");
        printf("Enter Note content: ");
        fgets(content,sizeof(content),stdin);
        length = strlen(content);
    }
    content[length-1] = '\0';
    strcpy(notes[noteCount].noteContent, content);
    notes[noteCount].noteID = N_nextID;
    N_nextID++;
    time_t currentTime;
    struct tm *localTime;
    time(&currentTime);
    localTime = localtime(&currentTime);
    sprintf(notes[noteCount].noteDate, "%02d/%02d/%04d",
        localTime->tm_mday,
        localTime->tm_mon + 1,
        localTime->tm_year + 1900);
    noteCount++;
}
void viewNote(){
    int ID,check=1,j;
    printf("Enter note ID: ");
    while(check){
        check=0;
        if((scanf("%d",&ID))!=1){
            printf("Note ID must consist of digits only. Please try again\n");
            check=1;
            getchar();
            continue;
        }
        if(ID<1){
            printf("Note ID must be a positive integer. Please try again.\n");
            check=1;
            continue;
        }
        for(j=0;j<noteCount;j++){
            if(ID == notes[j].noteID){
                break;
            }
        }
    }
    fgets(buffer,sizeof(buffer),stdin);
    check = 1;
    if(j == noteCount){
        printf("Error: Note ID not found.\n");
        return;
    }
    printf("Note Title: %s\n",notes[j].noteTitle);
    printf("Note ID: %d\n",ID);
    printf("Date: %s\n",notes[j].noteDate);
    printf("Content: %s\n",notes[j].noteContent);
}
void deleteNote(){
    int ID,check=1,i;
    printf("Enter the Note ID to delete: ");
    while(check){
        check=0;
        if((scanf("%d",&ID))!=1){
            printf("Note ID must consist of digits only. Please try again\n");
            check=1;
            getchar();
            continue;
        }
        if(ID < 1){
            printf("Note ID must be a positive integer. Please try again.\n");
            check=1;
        }
    }
    for(i=0;i<noteCount;i++){
        if(ID == notes[i].noteID){
            break;
        }
    }
    fgets(buffer,sizeof(buffer),stdin);
    if(i == noteCount){
        printf("Error: Note ID not found.\n");
        return;
    }
    for(;i<noteCount-1;i++){
        notes[i].noteID = notes[i+1].noteID;
        strncpy(notes[i].noteTitle, notes[i+1].noteTitle,35);
        strncpy(notes[i].noteDate,notes[i+1].noteDate,11);
        strncpy(notes[i].noteContent,notes[i+1].noteContent,250);
    }
    noteCount--;
    printf("Note deleted successfully!\n");
}
void financeMenu(){
int choice=7;
    printf("================================================\n");
    printf("                     M-OS\n");
    printf("               Finance Manager\n");
    printf("================================================\n\n");
    printf("Welcome to your Finance manager.\n\n");
    printf("------------------------------------------------\n\n");
    printf("Current Balance: %.1f ILS\n\n",balance);
    printf("------------------------------------------------\n");
    printf("1. Display Balance\n");
    printf("2. Add Money\n");
    printf("3. Spend Money\n");
    printf("4. Display Transactions\n");
    printf("0. Back to Main Menu\n");
    while(choice!=0){
        while(1){
            if((scanf("%d",&choice))!=1){
                printf("Invalid input. Please enter a number from 0 to 4\n");
                getchar();
            }
            else{
                break;
            }
        }
        fgets(buffer,sizeof(buffer),stdin);
        switch(choice){
            case 0:
            return;
            case 1:
            system("cls");
            printf("Current Balance: %.1f ILS\n",balance);
            break;
            case 2:
            system("cls");
            addMoney();
            break;
            case 3:
            system("cls");
            spendMoney();
            break;
            case 4:
            system("cls");
            displayTransactions();
            break;
            default:
            printf("Invalid input. Please enter a number from 0 to 4\n");
            continue;
        }
        printf("1. Display Balance\n");
        printf("2. Add Money\n");
        printf("3. Spend Money\n");
        printf("4. Display Transactions\n");
        printf("0. Back to Main Menu\n");
    }
}
void addMoney(){
    int check=1;
    float amount;
    char type[10] = {"Income"};
    transactions[transactionCount].transactionID = T_nextID;
    T_nextID++;
    time_t currentTime;
    struct tm *localTime;
    time(&currentTime);
    localTime = localtime(&currentTime);
    sprintf(transactions[transactionCount].transactionDate, "%02d/%02d/%04d",
        localTime->tm_mday,
        localTime->tm_mon + 1,
        localTime->tm_year + 1900);
    while(check){
        printf("Enter amount to add: ");
        check=0;
        if((scanf("%f",&amount))!=1){
            printf("Invalid input. Please enter a number.\n");
            check=1;
            getchar();
            continue;
        }
        if(amount <=0){
            printf("Invalid amount. Please enter an amount greater than zero.\n");
            check=1;
        }
    }
    fgets(buffer,sizeof(buffer),stdin);
    transactions[transactionCount].amount = amount;
    strncpy(transactions[transactionCount].transactionType, type, 10);
    transactionCount++;
    printf("Money added successfully!\n");
    balance += amount;
    printf("Current Balance: %.1f ILS\n",balance);
}
void spendMoney(){
    int check=1;
    float amount;
    char type[10] = {"Expense"};
    transactions[transactionCount].transactionID = T_nextID;
    T_nextID++;
    time_t currentTime;
    struct tm *localTime;
    time(&currentTime);
    localTime = localtime(&currentTime);
    sprintf(transactions[transactionCount].transactionDate, "%02d/%02d/%04d",
        localTime->tm_mday,
        localTime->tm_mon + 1,
        localTime->tm_year + 1900);
    while(check){
        printf("Enter amount to spend: ");
        check=0;
        if((scanf("%f",&amount))!=1){
            printf("Invalid input. Please enter a number.\n");
            check = 1;
            getchar();
            continue;
        }
        if(amount <=0){
            printf("Invalid amount. Please enter an amount greater than zero.\n");
            check = 1;
            continue;
        }
        if(amount > balance){
            printf("Insufficient balance. You cannot spend more than your current balance.\n");
            check = 1;
        }
    }
    fgets(buffer,sizeof(buffer),stdin);
    transactions[transactionCount].amount = -amount;
    strncpy(transactions[transactionCount].transactionType, type, 10);
    transactionCount++;
    printf("Money spent successfully!\n");
    balance -= amount;
    printf("Current Balance: %.1f ILS\n",balance);
}
void displayTransactions(){
    if(transactionCount== 0){
        printf("No transactions in the system.\n");
        return;
    }
    printf("================================================\n");
    printf("              Transaction History\n");
    printf("================================================\n\n");
    printf("ID\tType\t\tAmount\t\tDate\n");
    printf("--------------------------------------------------\n");
    for(int i=0;i<transactionCount;i++){
        printf("%d\t",transactions[i].transactionID);
        printf("%s\t\t",transactions[i].transactionType);
        printf("%.1f\t\t",transactions[i].amount);
        printf("%s\n",transactions[i].transactionDate);
    }
    printf("--------------------------------------------------\n");
    printf("Current Balance: %.1f ILS\n",balance);
}
void displayPlanner(){
    int days;
    char month[10];
    time_t secs = time(0);
    struct tm *full_time = localtime(&secs);
    struct tm *first_day = localtime(&secs);
    int today = full_time->tm_mday;
    int year = full_time->tm_year + 1900;
    first_day->tm_mday = 1;
    mktime(first_day);
    int firstDayOfWeek = first_day->tm_wday;
    switch (full_time->tm_mon+1)
    {
    case 1:
        days = 31;
        strcpy(month, "January");
        break;
    case 2:
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            days = 29;
        }
        else {
            days = 28;
        }
        strcpy(month, "February");
        break;
    case 3:
        days = 31;
        strcpy(month, "March");
        break;
    case 4:
        days = 30;
        strcpy(month, "April");
        break;
    case 5:
        days = 31;
        strcpy(month, "May");
        break;
    case 6:
        days = 30;
        strcpy(month, "June");
        break;
    case 7:
        days = 31;
        strcpy(month, "July");
        break;
    case 8:
        days = 31;
        strcpy(month, "August");
        break;
    case 9:
        days = 30;
        strcpy(month, "September");
        break;
    case 10:
        days = 31;
        strcpy(month, "October");
        break;
    case 11:
        days = 30;
        strcpy(month, "November");
        break;
    case 12:
        days = 31;
        strcpy(month, "December");
        break;
    }
    int counter = 1;
    printf("================================================\n");
    printf("                    M-OS\n");
    printf("                   Planner\n");
    printf("================================================\n\n");
    printf("------------------------------------------------\n");
    switch (strlen(month))
    {
    case 9:
        printf("               %s %d\n", month,year);
        break;
    case 8:
        printf("               %s %d\n", month,year);
        break;
    case 7:
        printf("                %s %d\n", month,year);
        break;
    case 6:
        printf("                 %s %d\n", month,year);
        break;
    case 5:
        printf("                 %s %d\n", month,year);
        break;
    case 4:
        printf("                  %s %d\n", month,year);
        break;
    default:
        printf("                  %s %d\n", month,year);
        break;
    }
    printf("\n");
    printf("Sun   Mon   Tue   Wed   Thu   Fri   Sat\n");
    printf("------------------------------------------------\n");
    int i=0,j=0,k=0,found = 0;;
    for(;i<firstDayOfWeek;i++){
        printf("      ");
    }
    for(i=firstDayOfWeek;i<7;i++){
        if(eventCount>0){
            for(;j<eventCount;j++){
                if(events[j].day == counter &&
                events[j].month == full_time->tm_mon+1 &&
                events[j].year == year){
                    found = 1;
                }
                if(j == eventCount-1){
                    j = 0;
                    break;
                }
            }
        }
        if(found && counter == today){
            printf("[%d*] ",counter);
        }
        else if(found){
            printf("%d*   ",counter);
        }
        else if(counter == today){
            printf("[%d]  ",counter);
        }
        else{
            printf("%d    ",counter);
        }
        if(counter<10){
            printf(" ");
        }
        counter++;
        found = 0;
    }
    printf("\n");
    for(i=0;i<4;i++){
        for(j=0;j<7 && counter<=days;j++){
            if(eventCount>0){
                for(;k<eventCount;k++){
                    if(events[k].day == counter &&
                    events[k].month == full_time->tm_mon+1 &&
                    events[k].year == year){
                        found = 1;
                    }
                    if(k == eventCount-1){
                    k = 0;
                    break;
                    }
                }
            }
            if(found && counter == today){
                printf("[%d*] ",counter);
            }
            else if(found){
                printf("%d*   ",counter);
            }
            else if(counter == today){
                printf("[%d]  ",counter);
            }
            else{
                printf("%d    ",counter);
            }
            if(counter<10){
                printf(" ");
            }
            counter++;
            found = 0;
        }
        printf("\n");
    }
    lastDay = counter - 1;
    printf("------------------------------------------------\n");
    printf("Upcoming Events: %d\n\n",upcomingEvents);
    for(int i=0;i<eventCount;i++){
        if(events[i].month == (full_time->tm_mon + 1)
        && events[i].year == (full_time->tm_year + 1900)
        && events[i].day >= today){
            printf("%s %s\n",events[i].eventDate,events[i].eventName);
        }
    }
    printf("------------------------------------------------\n");
}
void plannerMenu(){
    displayPlanner();
    printf("1. Add New Event\n");
    printf("2. Display All Events\n");
    printf("3. Delete Event\n");
    printf("4. Display Planner\n");
    printf("0. Back to Main Menu\n");
    int choice=7;
    while(choice!=0){
        while(1){
            if((scanf("%d",&choice))!=1){
                printf("Invalid input. Please enter a number from 0 to 3\n");
                getchar();
            }
            else{
                break;
            }
        }
        fgets(buffer,sizeof(buffer),stdin);
        switch(choice){
            case 0:
            return;
            case 1:
            system("cls");
            addEvent();
            updateUpcomingEvents();
            break;
            case 2:
            system("cls");
            displayEvents();
            break;
            case 3:
            system("cls");
            deleteEvent();
            break;
            case 4:
            system("cls");
            displayPlanner();
            break;
            default:
            printf("Invalid input. Please enter a number from 0 to 4\n");
            continue;
        }
        printf("1. Add New Event\n");
        printf("2. Display All Events\n");
        printf("3. Delete Event\n");
        printf("4. Display Planner\n");
        printf("0. Back to Main Menu\n");
    }
}
void addEvent(){
    time_t secs = time(0);
    struct tm *full_time = localtime(&secs);
    int day,month,year,check=1,j;
    events[eventCount].eventID = E_nextID;
    E_nextID++;
    char Name[40],date[11];
    printf("Enter Event Title: ");
    fgets(Name, sizeof(Name), stdin);
    int length = strlen(Name);
    while(length-1 == 0){
        printf("Event title cannot be empty. Please try again\n");
        printf("Enter Event Title: ");
        fgets(Name,sizeof(Name),stdin);
        length = strlen(Name);
    }
    Name[length-1] = '\0';
    strcpy(events[eventCount].eventName, Name);
    printf("Enter Event Date: \n");
    printf("Day: ");
    while(check){
        check=0;
        if((scanf("%d",&day))!=1){
            printf("Event Day must consist of digits only. Please try again\n");
            check=1;
            getchar();
            continue;
        }
        if(day<1 || day>lastDay){
            printf("Event Day must be greater than 1 and less than %d. Please try again.\n",lastDay);
            check=1;
        }
    }
    events[eventCount].day = day;
    printf("Month: ");
    check = 1;
    while(check){
        check=0;
        if((scanf("%d",&month))!=1){
            printf("Event Month must consist of digits only. Please try again\n");
            check=1;
            getchar();
            continue;
        }
        if(month<1 || month>12){
            printf("Event Month must be greater than 1 and less than 12. Please try again.\n");
            check=1;
        }
    }
    events[eventCount].month = month;
    printf("Year: ");
    check = 1;
    while(check){
        check=0;
        if((scanf("%d",&year))!=1){
            printf("Event year must consist of digits only. Please try again\n");
            check=1;
            getchar();
            continue;
        }
        if(year<(full_time->tm_year+1900)){
            printf("Event Year must be greater than or equal %d. Please try again.\n",full_time->tm_year+1900);
            check=1;
        }
    }
    fgets(buffer,sizeof(buffer),stdin);
    events[eventCount].year = year;
    sprintf(events[eventCount].eventDate, "%02d/%02d/%04d",day,month,year);
    eventCount++;
    printf("ُEvent added successfully!\n");
}
void displayEvents(){
    if(eventCount == 0){
        printf("No events in the system.\n");
        return;
    }
    printf("ID\tEvent Title\t\t\tDate\n");
    printf("--------------------------------------------------\n");
    for(int i=0;i<eventCount;i++){
        printf("%d\t",events[i].eventID);
        if(strlen(events[i].eventName)>=32){
            printf("%s",events[i].eventName);
        }
        else if(strlen(events[i].eventName)>=24 && strlen(events[i].eventName)<32){
            printf("%s\t",events[i].eventName);
        }
        else if(strlen(events[i].eventName)>=16 && strlen(events[i].eventName)<24){
            printf("%s\t\t",events[i].eventName);
        }
        else if(strlen(events[i].eventName)>=8 && strlen(events[i].eventName)<16){
            printf("%s\t\t\t",events[i].eventName);
        }
        else{
            printf("%s\t\t\t\t",events[i].eventName);
        }
        printf("%s\n",events[i].eventDate);
    }
    printf("--------------------------------------------------\n");
    printf("Total Events: %d\n",eventCount);
}
void deleteEvent(){
    int ID,check=1,i;
    printf("Enter the Event ID to delete: ");
    while(check){
        check=0;
        if((scanf("%d",&ID))!=1){
            printf("Event ID must consist of digits only. Please try again\n");
            check=1;
            getchar();
            continue;
        }
        if(ID < 1){
            printf("Event ID must be a positive integer. Please try again.\n");
            check=1;
        }
    }
    for(i=0;i<eventCount;i++){
        if(ID == events[i].eventID){
            break;
        }
    }
    fgets(buffer,sizeof(buffer),stdin);
    if(i == eventCount){
        printf("Error: Event ID not found.\n");
        return;
    }
    for(;i<eventCount-1;i++){
        events[i].eventID = events[i+1].eventID;
        strncpy(events[i].eventName, events[i+1].eventName,40);
        strncpy(events[i].eventDate,events[i+1].eventDate,11);
        events[i].day = events[i+1].day;
        events[i].month = events[i+1].month;
        events[i].year = events[i+1].year;
    }
    eventCount--;
    printf("Event deleted successfully!\n");
}
void updateUpcomingEvents(){
    time_t secs = time(0);
    struct tm *full_time = localtime(&secs);
    upcomingEvents = 0;
    for(int i=0;i<eventCount;i++){
        if(events[i].month == full_time->tm_mon + 1 &&
           events[i].year == full_time->tm_year + 1900){
            upcomingEvents++;
        }
    }
}
void shutdownSystem(){
    FILE *fpt_books = fopen("books.bin", "wb");
    if(fpt_books == NULL){
        printf("Error: Could not save book records\n");
        return;
    }
    else{
        for(int i=0;i<bookCount;i++){
            if(fwrite(&books[i].bookID, sizeof(int), 1, fpt_books) != 1){
            break;
            }
            fwrite(books[i].bookName,sizeof(char),40,fpt_books);
            fwrite(&books[i].pageNumber,sizeof(int),1,fpt_books);
            fwrite(books[i].bookType,sizeof(char),30,fpt_books);
            fwrite(&books[i].bookMark,sizeof(int),1,fpt_books);
        }
        fclose(fpt_books);
    }
    FILE *fpt_tasks = fopen("tasks.bin", "wb");
    if(fpt_tasks == NULL){
        printf("Error: Could not save task records\n");
        return;
    }
    else{
        fwrite(tasks, sizeof(task), taskCount, fpt_tasks);
        fclose(fpt_tasks);
    }
    FILE *fpt_notes = fopen("notes.bin", "wb");
    if(fpt_notes == NULL){
        printf("Error: Could not save note records\n");
        return;
    }
    else{
        fwrite(notes, sizeof(note), noteCount, fpt_notes);
        fclose(fpt_notes);
    }
    FILE *fpt_finance = fopen("finance.bin", "wb");
    if(fpt_finance == NULL){
        printf("Error: Could not save finance records\n");
        return;
    }
    else{
        fwrite(transactions, sizeof(transaction), transactionCount, fpt_finance);
        fclose(fpt_finance);
    }
    FILE *fpt_planner = fopen("planner.bin", "wb");
    if(fpt_planner == NULL){
        printf("Error: Could not save event records\n");
        return;
    }
    else{
        fwrite(events, sizeof(event), eventCount, fpt_planner);
        fclose(fpt_planner);
    }
}