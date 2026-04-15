import csv
import os
# Global list to store employee data so both members can access it.
system_records = []



def main_menu():
    while True:
        print("\n--- Health & Medicine Payroll System ---")
        print("1. Load Employee Data (Member 3)")
        print("2. Filter/View Employees (Member 3)")
        print("3. Process Active Payroll (Member 4)")
        print("4. Exit")
       
        choice = input("Select an option: ")
       
        if choice == '1':
            load_employees()
        elif choice == '2':
            view_employee_list()
        elif choice == '3':
            calculate_payroll()
        elif choice == '4':
            print("Exiting system...")
            break


def load_employees():
   
   
    global system_records
   
    # check if file exists before trying to open it
    if not os.path.exists('employee.csv'):
        print("Error: employee.csc is not found! Please to run the C program.")
        return
   
    with open('employee.csv', mode='r') as file:
        #Convert the Csv file into a python
        reader = list(csv.reader(file))
       
        # Validation:check if the file has data
        if not reader:
            print("No employees found in the file.")
        else:
            system_records = reader
            print(f"Successfully loaded {len(system_records)}reccords.")

def view_employee_list():
   
    if not system_records:
        print("No data loaded.")
        return
   
    # user Select which group of employee to see
    print("\n options: 1. Active | 2. On Leave | 3.Terminated")
    choice=input("Select status to view: ")
   
    status_map = {"1": "Active", "2": "On Leave", "3": "Terminated"}
    target_status =status_map.get(choice, "Active")
   
    print(f"\n showing{target_status}Employees  ")
    for emp in system_records:
        if emp[4] == target_status:
            print(f"ID: {emp[0]} | Name : {emp[1]} {emp[2]} | Role: {emp[3]}")
           
           
   
   
   



if __name__ == "__main__":
    main_menu()