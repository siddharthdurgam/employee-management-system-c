# 👥 Employee Management System — C

> **Terminal-based employee management application demonstrating C programming, CRUD operations, file handling, searching, filtering, validation, and configuration management.**

![CI](https://github.com/siddharthdurgam/employee-management-system-c/actions/workflows/validate.yml/badge.svg)
![C](https://img.shields.io/badge/C-C11-A8B9CC?style=for-the-badge&logo=c&logoColor=black)
![GCC](https://img.shields.io/badge/GCC-Compiler-4EAA25?style=for-the-badge&logo=gnu&logoColor=white)

## 🎯 Project Overview

This project is a command-line **Employee Management System** built in C. It demonstrates how structured programming concepts can be combined with persistent file storage to create a practical CRUD application.

## ✨ Features

- 🔐 Environment-configured login
- ➕ Add, modify and delete employee records
- 🔎 Search employees by ID
- 📋 Display employee information
- 📞 Display contact information
- ⚧️ Filter by gender
- 📍 Filter by district
- 🏢 Filter by branch
- 💾 Binary-file persistence
- 🖥️ Cross-platform terminal input handling

## 🧩 Core Concepts Demonstrated

| Concept | Implementation |
|---|---|
| Structures | Employee record modelling |
| Functions | Modular application logic |
| File I/O | `fopen`, `fread`, `fwrite`, `fseek`, `fclose` |
| CRUD | Create, read, update and delete records |
| Searching | Employee ID and text-based searches |
| Filtering | Gender, district and branch |
| Validation | User-input validation |
| Configuration | Environment variables |
| Persistence | Binary file storage |

## 🗂️ Project Structure

```text
employee-management-system-c/
├── .github/workflows/validate.yml
├── employee_management_system.c
├── README.md
└── .gitignore
```

`employee.dat` is generated locally at runtime and should not be treated as a source-controlled application database.

## ▶️ Build & Run

### Compile with GCC

```bash
gcc -std=c11 -Wall -Wextra -pedantic employee_management_system.c -o employee_management_system
```

Windows:

```bash
gcc -std=c11 -Wall -Wextra -pedantic employee_management_system.c -o employee_management_system.exe
```

### Configure Login

Credentials are supplied through environment variables rather than hard-coded in the source.

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

## 📋 Application Menu

```text
1.  Add Employee
2.  Delete Employee
3.  Modify Employee
4.  Display All Employees
5.  Search Employee
6.  Display Basic Information
7.  Display Contact Information
8.  List Male Employees
9.  List Female Employees
10. List Employees from Dhaka
11. List Employees from Other Districts
12. List Employees from Main Branch
13. List Employees from Other Branches
0.  Exit
```

## 💾 Data Storage

Employee records are persisted in a local binary file named `employee.dat`.

> **Security note:** binary-file storage provides persistence, not encryption or database-level security.

## 🧪 Automated Validation

GitHub Actions compiles the application with strict compiler warnings on pushes and pull requests, then verifies that the executable is produced successfully.

## 🚀 Future Improvements

- Replace binary storage with a relational database.
- Add password hashing and stronger authentication.
- Prevent duplicate employee IDs.
- Introduce role-based access control.
- Add automated functional tests.
- Build a GUI or web interface.
- Add database-backed reporting.

## 👤 Author

**D. Siddharth Patel**  
B.Tech — Computer Science & Engineering | Business Analyst | Data Analytics | Python | SQL

[LinkedIn](https://www.linkedin.com/in/siddharth-durgam-878632263/) · [GitHub](https://github.com/siddharthdurgam)

⭐ **If you find this project useful, consider starring the repository.**