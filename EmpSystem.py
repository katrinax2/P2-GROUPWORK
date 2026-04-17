import csv
import os
from datetime import datetime

# Functions            
def load_employee():
    global system_records
    
    # check if file exists before trying to open it
    
    if not os.path.exists('employee.csv'):
        print("Error: employee.csv is not found!")
        return
    
    with open ('employee.csv',mode='r') as file:
        reader = list(csv.reader(file))#Convert the Csv file into a python
        
        # Validation:check if the file has data
        
        if not reader:
            print("No employee found in the file.")
        else:
            system_records = reader
            print(f"Successfully loaded {len(system_records)} records.")
            
            
def view_employee_list():
    if not system_records:
        print("No data loaded.Please load employee data first.")
        return
    # user Select which group of employee to see
    print("Options: 1.Active | 2. On Leave | 3.Terminated.")
    choice = input("Select status to view:")
    
    
    status_map = {"1": "Active","2": "On Leave", "3": "Terminated"}
    target_status = status_map.get(choice,"Active")
    
    
    print(f"\n---Showing {target_status} Employees ---")
    for emp in system_records:
      if len(emp) >= 7 and emp[6].strip().capitalize()==target_status:
            print(f"ID: {emp[0]}    | Name : {emp[1]} {emp[2]}     | Role: {emp[3]}     | Dept: {emp[4]}")
            

def calculate_payroll():
    global system_records
    # check if the global variable system_records has data before proceeding
    
    if not system_records:
        print("\n No employee data available.")
        return
    # to capture the current system date and time
    run_date = datetime.now().strftime("%y-%m-%d")
    
    #open in append mode
    with open('payroll_history.csv', mode='a', newline='') as file:
        writer = csv.writer(file)
        print(f"\n---Payroll Processing ({run_date})--")
        
        # to ensure the row has enough column
        for emp in system_records:
            if len(emp)<7:
                continue
            
            
            emp_id,f_name,l_name,role,rate,status= emp[0],emp[1],emp[2],emp[3],emp[5],emp[6]
            #only active employee are eligible
            if status.strip().capitalize()== "Active":
                try:
                    print(f"\nprocessing: {f_name},{l_name} ({role})")
                     # to get user input
                    hrs=float(input(f"Enter hours worked for {f_name}:  "))
                    hourly_rate=float(rate)#use the rate stored in employee.csv
                    
                    
                    gross =hrs *hourly_rate
                    nis = gross *0.025
                    ed_tax = (gross -nis)* 0.00225
                    net = gross -nis -ed_tax
                    
                    #Save the transaction data as a new row
                    writer.writerow([run_date,emp_id,f_name,l_name,role,hrs,hourly_rate,gross,nis,ed_tax,net])
                    print(f"Saved! Net pay: ${net:.2f}")
                except ValueError:
                     print("Invalid input.")# to handle the non-numeric values
            else:
               print(f" Skipped: {f_name} {l_name} (Status: {status} - ELIGIBILITY BLOCKED)")# to skip employee who are not active


def view_payroll_summary_report():
    total_net =0.0
    total_deductions=0.0
    role_totals ={}# dictionary to store the net pay total
    

    # to make sure the history file exists before trying to read it

    if not os.path.exists('payroll_history.csv'):
        print("\nNO payroll history found.Please process the payroll first.")
        return
    
    print(f"\n---Care for you Medical Clinic: Summary Report---")
    
    
    try:
        with open('payroll_history.csv', mode='r') as file:
            reader =csv.reader(file)
            for row in reader:
            
                # Extract specific number from the csv column
                net = float(row[10])
                nis = float(row[8])
                tax = float(row[9])
                role = row[4]
                #Accumulate the global totals
                total_net +=net
                total_deductions += (nis + tax)
                role_totals[role]=role_totals.get(role,0)+ net
                
                #display the aggregated financial summary
        print(f"Total Net Payout: $ {total_net:.2f}")
        print(f"Total Taxes Collected: ${total_deductions:.2f}")
    
            # Display the category report
        print("\n Category Report: Spending by Role")
        for role,amount in role_totals.items():
            print(f"-{role}: ${amount: .2f}")

    except Exception as e:
          print(f"Error reading report: {e}")
    
#Global list to store employee data
system_records = []


# MAIN MENU
def main_menu():
    while True:
        print("\n------- Care for you Medical Clinic Payroll system --------")
        print("\t1.Load Employee data")
        print("\t2.Filter/View Employees")
        print("\t3.Calculate Payroll")
        print("\t4.View Payroll Summary report")
        print("\t5.Exit")
        
        choice=input("Select an option: ")
        
        if  choice =='1':
            load_employee()
        elif choice== '2':
            view_employee_list()
        elif choice == '3':
            calculate_payroll()
        elif choice =='4':
            view_payroll_summary_report()
        elif choice == '5':
            print("Exiting system")
            break
 
    
#start the program
if __name__=="__main__":
    main_menu()