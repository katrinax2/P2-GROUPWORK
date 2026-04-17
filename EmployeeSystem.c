/******************************************************************************
 * Care For You Hospital Employee Management System
 * CSV Format: id,firstname,lastname,role,department,payrate,status
 * Status can be: Active, OnLeave, Terminated
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//============= STRUCTS =============
typedef struct {
    int id;
    char fname[50];
    char lname[50];
    char role[50];
    char department[50];
    int pay_rate;
    char status[20];
} Employee;
 
//============= FUNCTIONS =============
void AdminSignIn(int *isLoggedin);
void AddEmployee(void);
void ListEmployees(void);
void FindEmployee(void);
void UpdateEmployee(void);
void DeleteEmployee(void);

 
/* Safely reads a line of text from user, removes newline */
static void readLine(char *buf, int size) {
    if (fgets(buf, size, stdin))
        buf[strcspn(buf, "\n")] = '\0';
}
 
/* Read an integer from stdin */
static int readInt(int *out) {
    char buf[32];
    readLine(buf, sizeof(buf));
    if (sscanf(buf, "%d", out) == 1) return 1;
    return 0;
}

 // ============= MAIN =============
 int main(void)
{
    int isLoggedin = 0;
    int choice;

    do {
        // Display menu
        printf("\n = = = = = = = = M   E   N   U  = = = = = = = = = = = =\n");
        printf(" 1 = = = = =   A D M I N   S I G N   I N\n");
        printf(" 2 = = = = =   A D D   E M P L O Y E E\n");
        printf(" 3 = = = = =   L I S T   E M P L O Y E E S\n");
        printf(" 4 = = = = =   F I N D   E M P L O Y E E  ( BY ID )\n");
        printf(" 5 = = = = =   U P D A T E   E M P L O Y E E\n");
        printf(" 6 = = = = =   D E L E T E   E M P L O Y E E\n");
        printf(" 7 = = = = =   E X I T\n");

        // Input validation loop
        do {
            printf("Please select a choice (1-7): ");

            if (!readInt(&choice)) {
                printf("Invalid input! Please enter a number.\n");
                choice = 0;
            }

            if (choice < 1 || choice > 7)
                printf("Invalid choice. Please try again.\n");

        } while (choice < 1 || choice > 7);

        // Handle menu choice
        switch (choice) {

            case 1:
                AdminSignIn(&isLoggedin);
                break;

            case 2:
                if (isLoggedin)
                    AddEmployee();
                else
                    printf("You must login first!\n");
                break;

            case 3:
                if (isLoggedin)
                    ListEmployees();
                else
                    printf("You must login first!\n");
                break;

            case 4:
                if (isLoggedin)
                    FindEmployee();
                else
                    printf("You must login first!\n");
                break;

            case 5:
                if (isLoggedin)
                    UpdateEmployee();
                else
                    printf("You must login first!\n");
                break;

            case 6:
                if (isLoggedin)
                    DeleteEmployee();
                else
                    printf("You must login first!\n");
                break;

            case 7:
                printf("E x i t i n g . . . .\n");
                break;
        }

    } while (choice != 7);

    return 0;
}
// -------------------- FUNCTION DEFINITIONS --------------------
void AdminSignIn(int *isLoggedin)
{
    char username[30], password[30];
    char fileusername[30], filepassword[30];
    FILE *fp;
    int  attempts = 0;
    const int MAX_ATTEMPTS = 3;

    // Open admin file to read stored username and password
    fp = fopen("admin.txt", "r");
    if (fp == NULL) { 
        printf("Error: admin.txt not found!\n"); 
        return; 
    }

    // Read username and password from file
    fscanf(fp, "%29s %29s", fileusername, filepassword);
    fclose(fp);

    // Allow user up to MAX_ATTEMPTS to login
    while (attempts < MAX_ATTEMPTS) {

        // Get user input
        printf("Enter your username: ");
        readLine(username, sizeof(username));

        printf("Enter your password: ");
        readLine(password, sizeof(password));

        // Check if entered credentials match file credentials
        if (strcmp(username, fileusername) == 0 &&
            strcmp(password, filepassword) == 0) {

            printf("Login successful!\n");

            // Set login flag to true (1)
            *isLoggedin = 1;
            return;
        }

        // Increase attempt count if login fails
        attempts++;

        // Show remaining attempts
        if (attempts < MAX_ATTEMPTS)
            printf("Invalid credentials! %d attempt(s) remaining.\n",
                   MAX_ATTEMPTS - attempts);
    }

    // If all attempts used
    printf("Too many failed attempts. Returning to menu.\n");
}

// -------------------- ADD EMPLOYEE --------------------
void AddEmployee(void)
{
    Employee emp, existing;
    int exists = 0;
    FILE *fp;

    // Ask user for employee ID
    printf("Enter the employee ID (integer): ");
    if (!readInt(&emp.id)) { 
        printf("Invalid ID!\n"); 
        return; 
    }

    // Open file to check if ID already exists
    fp = fopen("employee.csv", "r");

    if (fp != NULL) {
        // Read each record from file
        while (fscanf(fp, "%d,%49[^,],%49[^,],%49[^,],%49[^,],%d,%19[^\n]\n",
                      &existing.id, existing.fname, existing.lname,
                      existing.role, existing.department,
                      &existing.pay_rate, existing.status) == 7) {

            // Compare IDs
            if (existing.id == emp.id) { 
                exists = 1; 
                break; 
            }
        }
        fclose(fp);
    }

    // If ID already exists, stop
    if (exists) { 
        printf("Employee ID %d already exists!\n", emp.id); 
        return; 
    }

    // Input employee details
    printf("Enter employee's first name: ");
    readLine(emp.fname, sizeof(emp.fname));

    printf("Enter employee's last name: ");
    readLine(emp.lname, sizeof(emp.lname));

    printf("Enter employee's role: ");
    readLine(emp.role, sizeof(emp.role));

    printf("Enter employee's department: ");
    readLine(emp.department, sizeof(emp.department));

    printf("Enter employee's pay rate (per hour): ");
    if (!readInt(&emp.pay_rate)) { 
        printf("Invalid pay rate!\n"); 
        return; 
    }

    printf("Enter employee status (Active / OnLeave / Terminated): ");
    readLine(emp.status, sizeof(emp.status));

    // Open file in append mode to add new record
    fp = fopen("employee.csv", "a");
    if (fp == NULL) { 
        printf("Error opening file!\n"); 
        return; 
    }

    // Write new employee to file
    fprintf(fp, "%d,%s,%s,%s,%s,%d,%s\n",
            emp.id, emp.fname, emp.lname,
            emp.role, emp.department,
            emp.pay_rate, emp.status);

    fclose(fp);

    // Confirmation message
    printf("Employee added successfully!\n");
}
// --------------------- LIST EMPLOYEES ---------------------
void ListEmployees(void)
{
    Employee emp;
    FILE *fp;
    int count = 0;

    // Open employee file for reading
    fp = fopen("employee.csv", "r");

    // Check if file exists
    if (fp == NULL) { 
        printf("No employee records found!\n"); 
        return; 
    }

    // Print table header
    printf("\n%-6s  %-15s %-15s %-15s %-15s %-10s %-12s\n",
           "ID", "First Name", "Last Name", "Role", "Department", "PayRate", "Status");

    // Print separator lines for better readability 
    //  (Format %s -> print string, 15 means 15 characters wide, - means left align)
    printf("%-6s  %-15s %-15s %-15s %-15s %-10s %-12s\n",
           "------","---------------","---------------",
           "---------------","---------------","----------","------------");

    // Read each employee record from file
    while (fscanf(fp, "%d,%49[^,],%49[^,],%49[^,],%49[^,],%d,%19[^\n]\n",
                  &emp.id, emp.fname, emp.lname,
                  emp.role, emp.department,
                  &emp.pay_rate, emp.status) == 7) {

        // Display employee details in formatted table
        printf("%-6d  %-15s %-15s %-15s %-15s $%-9d %-12s\n",
               emp.id, emp.fname, emp.lname,
               emp.role, emp.department,
               emp.pay_rate, emp.status);

        count++; // increment employee count
    }

    // Close file after reading
    fclose(fp);

    // Check if any employees were found
    if (count == 0) 
        printf("No employees found!\n");
    else            
        printf("\nTotal employees: %d\n", count);
}
// --------------------- FIND EMPLOYEE ---------------------
void FindEmployee(void)
{
    Employee emp;
    FILE *fp;
    int targetID, found = 0;

    printf("Enter Employee ID to find: ");
    if (!readInt(&targetID)) { 
        printf("Invalid ID!\n"); 
        return; 
    }

    fp = fopen("employee.csv", "r");
    if (fp == NULL) { 
        printf("No employee records found!\n"); 
        return; 
    }

    while (fscanf(fp, "%d,%49[^,],%49[^,],%49[^,],%49[^,],%d,%19[^\n]\n",
                  &emp.id, emp.fname, emp.lname,
                  emp.role, emp.department,
                  &emp.pay_rate, emp.status) == 7) {

        if (emp.id == targetID) {
            found = 1;

            printf("\n=== Employee Found ===\n");
            printf("ID         : %d\n", emp.id);
            printf("Name       : %s %s\n", emp.fname, emp.lname);
            printf("Role       : %s\n", emp.role);
            printf("Department : %s\n", emp.department);
            printf("Pay Rate   : $%d/hr\n", emp.pay_rate);
            printf("Status     : %s\n", emp.status);

            break;
        }
    }

    fclose(fp);

    if (!found) 
        printf("Employee ID %d not found.\n", targetID);
}

// --------------------- UPDATE EMPLOYEE ---------------------
void UpdateEmployee(void)
{
    Employee emp;
    FILE *fp, *temp;
    int targetID, found = 0;
    char buf[50];

    // Ask user for the employee ID to update
    printf("Enter Employee ID to update: ");
    if (!readInt(&targetID)) { 
        printf("Invalid ID!\n"); 
        return; 
    }

    // Open original file for reading and temp file for writing updated data
    fp   = fopen("employee.csv", "r");
    temp = fopen("temp.csv",     "w");

    // Check if files opened successfully
    if (fp == NULL || temp == NULL) {
        printf("Error opening file!\n");
        if (fp)   fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    // Read each employee record from the file
    while (fscanf(fp, "%d,%49[^,],%49[^,],%49[^,],%49[^,],%d,%19[^\n]\n",
                  &emp.id, emp.fname, emp.lname,
                  emp.role, emp.department,
                  &emp.pay_rate, emp.status) == 7) {

        // Check if current record matches the ID to update
        if (emp.id == targetID) {
            found = 1;

            // Display current employee details
            printf("Updating employee %d (%s %s)\n",
                   emp.id, emp.fname, emp.lname);

            // Ask for new first name (press Enter to keep old value)
            printf("Enter new first name [%s]: ", emp.fname);
            readLine(buf, sizeof(buf));
            if (strlen(buf) > 0) strcpy(emp.fname, buf);

            // Ask for new last name
            printf("Enter new last name [%s]: ", emp.lname);
            readLine(buf, sizeof(buf));
            if (strlen(buf) > 0) strcpy(emp.lname, buf);

            // Ask for new role
            printf("Enter new role [%s]: ", emp.role);
            readLine(buf, sizeof(buf));
            if (strlen(buf) > 0) strcpy(emp.role, buf);

            // Ask for new department
            printf("Enter new department [%s]: ", emp.department);
            readLine(buf, sizeof(buf));
            if (strlen(buf) > 0) strcpy(emp.department, buf);

            // Ask for new pay rate (convert from string to integer)
            printf("Enter new pay rate [%d]: ", emp.pay_rate);
            readLine(buf, sizeof(buf));
            if (strlen(buf) > 0) emp.pay_rate = atoi(buf);

            // Ask for new status
            printf("Enter new status [%s]: ", emp.status);
            readLine(buf, sizeof(buf));
            if (strlen(buf) > 0) strcpy(emp.status, buf);
        }

        // Write (updated or unchanged) record to temp file
        fprintf(temp, "%d,%s,%s,%s,%s,%d,%s\n",
                emp.id, emp.fname, emp.lname,
                emp.role, emp.department,
                emp.pay_rate, emp.status);
    }

    // Close both files after processing
    fclose(fp);
    fclose(temp);

    // Replace old file with updated file
    remove("employee.csv");
    rename("temp.csv", "employee.csv");

    // Display result message
    if (found) 
        printf("Employee updated successfully.\n");
    else       
        printf("Employee ID %d not found.\n", targetID);
}
// --------------------- DELETE EMPLOYEE ---------------------
void DeleteEmployee(void)
{
    Employee emp;
    FILE *fp, *temp;
    int targetID, found = 0;
    char confirm[10];     //Used later to confirm deletion with user

    // Ask user for employee ID to delete
    printf("Enter Employee ID to delete: ");
    if (!readInt(&targetID)) { 
        printf("Invalid ID!\n"); 
        return; 
    }

    // Confirm deletion before proceeding
    printf("Are you sure you want to delete employee %d? (yes/no): ", targetID);
    readLine(confirm, sizeof(confirm));
    if (strcmp(confirm, "yes") != 0) { 
        printf("Delete cancelled.\n"); 
        return; 
    }

    // Open original file for reading and temp file for writing
    fp   = fopen("employee.csv", "r");
    temp = fopen("temp.csv",     "w");

    // Check if files opened successfully
    if (fp == NULL || temp == NULL) {
        printf("Error opening file!\n");
        if (fp)   fclose(fp);
        if (temp) fclose(temp);
        return;
    }

    // Read each employee record from file
    while (fscanf(fp, "%d,%49[^,],%49[^,],%49[^,],%49[^,],%d,%19[^\n]\n",
                  &emp.id, emp.fname, emp.lname,
                  emp.role, emp.department,
                  &emp.pay_rate, emp.status) == 7) {

        // If this is the employee to delete
        if (emp.id == targetID) {
            found = 1;
            // Do not write to temp file (this removes the employee)
        } else {
            // Write all other employees to temp file
            fprintf(temp, "%d,%s,%s,%s,%s,%d,%s\n",
                    emp.id, emp.fname, emp.lname,
                    emp.role, emp.department,
                    emp.pay_rate, emp.status);
        }
    }

    // Close both files
    fclose(fp);
    fclose(temp);

    // Replace original file with updated temp file
    remove("employee.csv");
    rename("temp.csv", "employee.csv");

    // Display result message
    if (found) 
        printf("Employee %d deleted successfully.\n", targetID);
    else       
        printf("Employee ID %d not found.\n", targetID);
}