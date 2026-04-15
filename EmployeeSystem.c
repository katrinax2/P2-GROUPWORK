/******************************************************************************
 * Care For You Hospital Employee Management System
 * CSV Format: id,firstname,lastname,role,department,payroll
 ******************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//============= STRUCTS =============
typedef struct {
    int  id;
    char firstname[30];
    char lastname[30];
    char role[30];
    char department[30];
    int  payroll;
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
        printf("\n = = = = = = = = M   E   N   U  = = = = = = = = = = = =\n");
        printf(" 1 = = = = =   A D M I N   S I G N   I N\n");
        printf(" 2 = = = = =   A D D   E M P L O Y E E\n");
        printf(" 3 = = = = =   L I S T   E M P L O Y E E S\n");
        printf(" 4 = = = = =   F I N D   E M P L O Y E E  ( BY ID )\n");
        printf(" 5 = = = = =   U P D A T E   E M P L O Y E E\n");
        printf(" 6 = = = = =   D E L E T E   E M P L O Y E E\n");
        printf(" 7 = = = = =   E X I T\n");
 
        do {
            printf("Please select a choice (1-7): ");
            if (!readInt(&choice)) {
                printf("Invalid input! Please enter a number.\n");
                choice = 0;
            }
            if (choice < 1 || choice > 7)
                printf("Invalid choice. Please try again.\n");
        } while (choice < 1 || choice > 7);
 
        // Handle menu choice and Call functions accordingly
        switch (choice) {
            case 1: AdminSignIn(&isLoggedin);                                         break;
            case 2: isLoggedin ? AddEmployee()    : puts("You must login first!");    break;
            case 3: isLoggedin ? ListEmployees()  : puts("You must login first!");    break;
            case 4: isLoggedin ? FindEmployee()   : puts("You must login first!");    break;
            case 5: isLoggedin ? UpdateEmployee() : puts("You must login first!");    break;
            case 6: isLoggedin ? DeleteEmployee() : puts("You must login first!");    break;
            case 7: puts("E x i t i n g . . . ."); break;
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
 
    // Read admin details from file admin.txt (format: username password)
    fp = fopen("admin.txt", "r");
    if (fp == NULL) { printf("Error: admin.txt not found!\n"); return; }
    fscanf(fp, "%29s %29s", fileusername, filepassword);
    fclose(fp);
 
    while (attempts < MAX_ATTEMPTS) {
        printf("Enter your username: ");
        readLine(username, sizeof(username));
        printf("Enter your password: ");
        readLine(password, sizeof(password));
 
        if (strcmp(username, fileusername) == 0 &&
            strcmp(password, filepassword) == 0) {
            printf("Login successful!\n");
            *isLoggedin = 1;
            return;
        }
        attempts++;
        if (attempts < MAX_ATTEMPTS)
            printf("Invalid credentials! %d attempt(s) remaining.\n",
                   MAX_ATTEMPTS - attempts);
    }
    printf("Too many failed attempts. Returning to menu.\n");
}


// -------------------- ADD EMPLOYEE --------------------
void AddEmployee(void)
{
    Employee emp, existing;
    int exists = 0;
    FILE *fp;
 
    printf("Enter the employee ID (integer): ");
    if (!readInt(&emp.id)) { printf("Invalid ID!\n"); return; }
 
    fp = fopen("employee.csv", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d,%29[^,],%29[^,],%29[^,],%29[^,],%d\n",
                      &existing.id, existing.firstname, existing.lastname,
                      existing.role, existing.department, &existing.payroll) == 6) {
            if (existing.id == emp.id) { exists = 1; break; }
        }
        fclose(fp);
    }
    if (exists) { printf("Employee ID %d already exists!\n", emp.id); return; }
 
    printf("Enter employee's first name: ");
    readLine(emp.firstname, sizeof(emp.firstname));
 
    printf("Enter employee's last name: ");
    readLine(emp.lastname, sizeof(emp.lastname));
 
    printf("Enter employee's role: ");
    readLine(emp.role, sizeof(emp.role));
 
    printf("Enter employee's department: ");
    readLine(emp.department, sizeof(emp.department));
 
    printf("Enter employee's payroll (integer): ");
    if (!readInt(&emp.payroll)) { printf("Invalid payroll!\n"); return; }
 
    fp = fopen("employee.csv", "a");
    if (fp == NULL) { printf("Error opening file!\n"); return; }
    fprintf(fp, "%d,%s,%s,%s,%s,%d\n",
            emp.id, emp.firstname, emp.lastname,
            emp.role, emp.department, emp.payroll);
    fclose(fp);
 
    printf("Employee added successfully!\n");
}

// --------------------- LIST EMPLOYEES ---------------------
void ListEmployees(void)
{
    Employee emp;
    FILE *fp;
    int count = 0;
 
    fp = fopen("employee.csv", "r");
    if (fp == NULL) { printf("No employee records found!\n"); return; }
 
    printf("\n%-6s  %-15s %-15s %-15s %-15s %s\n",
           "ID", "First Name", "Last Name", "Role", "Department", "Payroll");
    printf("%-6s  %-15s %-15s %-15s %-15s %s\n",
           "------","---------------","---------------",
           "---------------","---------------","----------");
 
    while (fscanf(fp, "%d,%29[^,],%29[^,],%29[^,],%29[^,],%d\n",
                  &emp.id, emp.firstname, emp.lastname,
                  emp.role, emp.department, &emp.payroll) == 6) {
        printf("%-6d  %-15s %-15s %-15s %-15s $%d\n",
               emp.id, emp.firstname, emp.lastname,
               emp.role, emp.department, emp.payroll);
        count++;
    }
    fclose(fp);
 
    if (count == 0) printf("No employees found!\n");
    else            printf("\nTotal employees: %d\n", count);
}
 

// --------------------- FIND EMPLOYEE ---------------------

void FindEmployee(void)
{
    Employee emp;
    FILE *fp;
    int targetID, found = 0;
 
    printf("Enter Employee ID to find: ");
    if (!readInt(&targetID)) { printf("Invalid ID!\n"); return; }
 
    fp = fopen("employee.csv", "r");
    if (fp == NULL) { printf("No employee records found!\n"); return; }
 
    while (fscanf(fp, "%d,%29[^,],%29[^,],%29[^,],%29[^,],%d\n",
                  &emp.id, emp.firstname, emp.lastname,
                  emp.role, emp.department, &emp.payroll) == 6) {
        if (emp.id == targetID) {
            found = 1;
            printf("\n=== Employee Found ===\n");
            printf("ID         : %d\n",    emp.id);
            printf("Name       : %s %s\n", emp.firstname, emp.lastname);
            printf("Role       : %s\n",    emp.role);
            printf("Department : %s\n",    emp.department);
            printf("Payroll    : $%d\n",   emp.payroll);
            break;
        }
    }
    fclose(fp);
 
    if (!found) printf("Employee ID %d not found.\n", targetID);
}


// --------------------- UPDATE EMPLOYEE ---------------------
void UpdateEmployee(void)
{
    Employee emp;
    FILE *fp, *temp;
    int targetID, found = 0;
    char buf[30];
 
    printf("Enter Employee ID to update: ");
    if (!readInt(&targetID)) { printf("Invalid ID!\n"); return; }
 
    fp   = fopen("employee.csv", "r");
    temp = fopen("temp.csv",     "w");
    if (fp == NULL || temp == NULL) {
        printf("Error opening file!\n");
        if (fp)   fclose(fp);
        if (temp) fclose(temp);
        return;
    }
 
    while (fscanf(fp, "%d,%29[^,],%29[^,],%29[^,],%29[^,],%d\n",
                  &emp.id, emp.firstname, emp.lastname,
                  emp.role, emp.department, &emp.payroll) == 6) {
 
        if (emp.id == targetID) {
            found = 1;
            printf("Updating employee %d (%s %s)\n",
                   emp.id, emp.firstname, emp.lastname);
 
            printf("Enter new first name [%s]: ", emp.firstname);
            readLine(buf, sizeof(buf));
            if (strlen(buf) > 0) strcpy(emp.firstname, buf);
 
            printf("Enter new last name [%s]: ", emp.lastname);
            readLine(buf, sizeof(buf));
            if (strlen(buf) > 0) strcpy(emp.lastname, buf);
 
            printf("Enter new role [%s]: ", emp.role);
            readLine(buf, sizeof(buf));
            if (strlen(buf) > 0) strcpy(emp.role, buf);
 
            printf("Enter new department [%s]: ", emp.department);
            readLine(buf, sizeof(buf));
            if (strlen(buf) > 0) strcpy(emp.department, buf);
 
            printf("Enter new payroll [%d]: ", emp.payroll);
            readLine(buf, sizeof(buf));
            if (strlen(buf) > 0) emp.payroll = atoi(buf);
        }
 
        fprintf(temp, "%d,%s,%s,%s,%s,%d\n",
                emp.id, emp.firstname, emp.lastname,
                emp.role, emp.department, emp.payroll);
    }
 
    fclose(fp);
    fclose(temp);
    remove("employee.csv");
    rename("temp.csv", "employee.csv");
 
    if (found) printf("Employee updated successfully.\n");
    else       printf("Employee ID %d not found.\n", targetID);
}
 
// --------------------- DELETE EMPLOYEE ---------------------
void DeleteEmployee(void)
{
    Employee emp;
    FILE *fp, *temp;
    int targetID, found = 0;
    char confirm[10];
 
    printf("Enter Employee ID to delete: ");
    if (!readInt(&targetID)) { printf("Invalid ID!\n"); return; }
 
    printf("Are you sure you want to delete employee %d? (yes/no): ", targetID);
    readLine(confirm, sizeof(confirm));
    if (strcmp(confirm, "yes") != 0) { printf("Delete cancelled.\n"); return; }
 
    fp   = fopen("employee.csv", "r");
    temp = fopen("temp.csv",     "w");
    if (fp == NULL || temp == NULL) {
        printf("Error opening file!\n");
        if (fp)   fclose(fp);
        if (temp) fclose(temp);
        return;
    }
 
    while (fscanf(fp, "%d,%29[^,],%29[^,],%29[^,],%29[^,],%d\n",
                  &emp.id, emp.firstname, emp.lastname,
                  emp.role, emp.department, &emp.payroll) == 6) {
        if (emp.id == targetID) {
            found = 1;
        } else {
            fprintf(temp, "%d,%s,%s,%s,%s,%d\n",
                    emp.id, emp.firstname, emp.lastname,
                    emp.role, emp.department, emp.payroll);
        }
    }
 
    fclose(fp);
    fclose(temp);
    remove("employee.csv");
    rename("temp.csv", "employee.csv");
 
    if (found) printf("Employee %d deleted successfully.\n", targetID);
    else       printf("Employee ID %d not found.\n", targetID);
}