# Employee Management System – C

A simple terminal-based Employee Management System developed in **C**. The project demonstrates structured programming, file handling, CRUD operations, input validation, searching, filtering, and basic employee reporting.

## Features

- Environment-configured login
- Add employee records
- Delete employee records
- Modify employee records
- Search employees by ID
- Display all employees
- Display basic employee information
- Display contact information
- Filter employees by gender
- Filter employees by district
- Filter employees by branch
- Binary file storage for employee records
- Cross-platform terminal input handling

## Technologies

- **Language:** C
- **Standard:** C11
- **Storage:** Binary file (`employee.dat`)
- **Concepts:** Structures, functions, file handling, CRUD operations, searching, filtering, input validation

## Project Structure

```text
employee-management-system-c/
├── employee_management_system.c
├── README.md
├── .gitignore
└── employee.dat          # Generated at runtime; not committed
```

## How to Run

### 1. Compile

Using GCC:

```bash
gcc -std=c11 -Wall -Wextra -pedantic employee_management_system.c -o employee_management_system
```

On Windows:

```bash
gcc -std=c11 -Wall -Wextra -pedantic employee_management_system.c -o employee_management_system.exe
```

### 2. Configure Login

The application does not store a password directly in the source code.

Set the password using an environment variable before running.

**Linux/macOS:**

```bash
export EMS_USERNAME=admin
export EMS_PASSWORD=your_password
./employee_management_system
```

**Windows Command Prompt:**

```cmd
set EMS_USERNAME=admin
set EMS_PASSWORD=your_password
employee_management_system.exe
```

The username defaults to `admin` if `EMS_USERNAME` is not set. A password must be configured through `EMS_PASSWORD`.

## Menu Options

1. Add Employee  
2. Delete Employee  
3. Modify Employee  
4. Display All Employees  
5. Search Employee  
6. Display Basic Information  
7. Display Contact Information  
8. List Male Employees  
9. List Female Employees  
10. List Employees from Dhaka  
11. List Employees from Other Districts  
12. List Employees from Main Branch  
13. List Employees from Other Branches  
0. Exit

## Data Storage

Employee records are stored in a local binary file named `employee.dat`. The file is generated automatically when the application is first run.

**Note:** Binary storage is used for persistence; it should not be considered encryption or secure database storage.

## Learning Outcomes

This project demonstrates practical use of:

- C structures
- Functions and modular programming
- File I/O using `fopen`, `fread`, `fwrite`, `fseek`, and `fclose`
- CRUD operations
- String handling and case-insensitive searching
- Input validation
- Temporary-file based record deletion
- Environment variables for configuration
- Basic cross-platform terminal handling

## Future Improvements

- Replace binary-file storage with a relational database
- Add stronger authentication and password hashing
- Prevent duplicate employee IDs
- Add role-based access control
- Add automated tests
- Add a graphical or web-based interface

## Author

**D. Siddharth Patel**

- LinkedIn: https://www.linkedin.com/in/siddharth-durgam-878632263/
