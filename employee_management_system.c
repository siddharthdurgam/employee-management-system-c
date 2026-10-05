/*
 * Employee Management System
 * Simple terminal-based employee record management application in C.
 *
 * Features:
 * - Environment-configured login
 * - Binary file storage
 * - Add, delete, modify, search and display records
 * - Basic employee and contact reports
 * - Gender, district and branch filters
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#define DATA_FILE "employee.dat"
#define TEMP_FILE "employee.tmp"

#define NAME_SIZE 100
#define DESIGNATION_SIZE 50
#define DATE_SIZE 16
#define GENDER_SIZE 16
#define BRANCH_SIZE 50
#define ADDRESS_SIZE 200
#define PHONE_SIZE 20
#define EMAIL_SIZE 100
#define INPUT_SIZE 256

typedef struct {
    int id;
    char name[NAME_SIZE];
    char designation[DESIGNATION_SIZE];
    float salary;
    char joining_date[DATE_SIZE];
    char gender[GENDER_SIZE];
    char branch[BRANCH_SIZE];
    char present_address[ADDRESS_SIZE];
    char permanent_address[ADDRESS_SIZE];
    char phone[PHONE_SIZE];
    char email[EMAIL_SIZE];
} Employee;

static void print_header(const char *title);
static void pause_screen(void);
static void clear_screen(void);
static void clear_input_buffer(void);
static void read_line(const char *prompt, char *buffer, size_t size);
static int read_int(const char *prompt);
static float read_float(const char *prompt);
static void read_password(const char *prompt, char *buffer, size_t size);
static int strings_equal_ignore_case(const char *a, const char *b);
static int contains_ignore_case(const char *text, const char *query);
static int authenticate(void);
static int open_data_file(FILE **file);
static void add_employee(FILE *file);
static void delete_employee(FILE **file);
static void modify_employee(FILE *file);
static void display_all(FILE *file);
static void search_employee(FILE *file);
static void display_basic_info(FILE *file);
static void display_contact_info(FILE *file);
static void list_by_gender(FILE *file, const char *gender);
static void list_by_district(FILE *file, int is_dhaka);
static void list_by_branch(FILE *file, int is_main);
static void print_employee(const Employee *employee, int include_addresses);
static void print_summary(const Employee *employee);
static void read_employee_details(Employee *employee);
static int find_employee(FILE *file, int id, Employee *employee);
static void show_menu(void);

#ifdef _WIN32
static int read_key(void) {
    return _getch();
}
#else
static int read_key(void) {
    struct termios old_settings;
    struct termios new_settings;
    int ch;

    if (tcgetattr(STDIN_FILENO, &old_settings) != 0) {
        return getchar();
    }

    new_settings = old_settings;
    new_settings.c_lflag &= (tcflag_t) ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
    return ch;
}
#endif

static void clear_screen(void) {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J\033[H");
#endif
}

static void pause_screen(void) {
    printf("\nPress Enter to continue...");
    fflush(stdout);
    clear_input_buffer();
}

static void clear_input_buffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* Discard remaining input. */
    }
}

static void read_line(const char *prompt, char *buffer, size_t size) {
    if (size == 0) {
        return;
    }

    printf("%s", prompt);
    fflush(stdout);

    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        clearerr(stdin);
        return;
    }

    if (strchr(buffer, '\n') == NULL) {
        clear_input_buffer();
    }

    buffer[strcspn(buffer, "\n")] = '\0';
}

static int read_int(const char *prompt) {
    char input[INPUT_SIZE];
    char *end;
    long value;

    for (;;) {
        read_line(prompt, input, sizeof(input));
        value = strtol(input, &end, 10);

        while (isspace((unsigned char)*end)) {
            end++;
        }

        if (input[0] != '\0' && *end == '\0') {
            return (int)value;
        }

        printf("Invalid number. Please try again.\n");
    }
}

static float read_float(const char *prompt) {
    char input[INPUT_SIZE];
    char *end;
    float value;

    for (;;) {
        read_line(prompt, input, sizeof(input));
        value = strtof(input, &end);

        while (isspace((unsigned char)*end)) {
            end++;
        }

        if (input[0] != '\0' && *end == '\0') {
            return value;
        }

        printf("Invalid amount. Please try again.\n");
    }
}

static void read_password(const char *prompt, char *buffer, size_t size) {
    size_t index = 0;
    int key;

    if (size == 0) {
        return;
    }

    printf("%s", prompt);
    fflush(stdout);

    while (index < size - 1) {
        key = read_key();

        if (key == '\n' || key == '\r') {
            break;
        }

        if ((key == 8 || key == 127) && index > 0) {
            index--;
            printf("\b \b");
            fflush(stdout);
            continue;
        }

        if (isprint((unsigned char)key)) {
            buffer[index++] = (char)key;
            putchar('*');
            fflush(stdout);
        }
    }

    buffer[index] = '\0';
    putchar('\n');
}

static int strings_equal_ignore_case(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 0;
        }
        a++;
        b++;
    }

    return *a == '\0' && *b == '\0';
}

static int contains_ignore_case(const char *text, const char *query) {
    size_t text_len = strlen(text);
    size_t query_len = strlen(query);

    if (query_len == 0) {
        return 1;
    }

    if (query_len > text_len) {
        return 0;
    }

    for (size_t i = 0; i <= text_len - query_len; i++) {
        size_t j = 0;

        while (j < query_len &&
               tolower((unsigned char)text[i + j]) ==
               tolower((unsigned char)query[j])) {
            j++;
        }

        if (j == query_len) {
            return 1;
        }
    }

    return 0;
}

static void print_header(const char *title) {
    clear_screen();

    printf("============================================================\n");
    printf("              EMPLOYEE MANAGEMENT SYSTEM\n");
    printf("============================================================\n");

    if (title != NULL && title[0] != '\0') {
        printf("%s\n", title);
        printf("------------------------------------------------------------\n");
    }
}

static int authenticate(void) {
    const char *expected_username = getenv("EMS_USERNAME");
    const char *expected_password = getenv("EMS_PASSWORD");
    char username[64];
    char password[128];

    if (expected_username == NULL || expected_username[0] == '\0') {
        expected_username = "admin";
    }

    if (expected_password == NULL || expected_password[0] == '\0') {
        printf("Login is not configured.\n");
        printf("Set EMS_PASSWORD before running the application.\n");
        printf("Default username: %s\n", expected_username);
        return 0;
    }

    print_header("Login");
    read_line("Username: ", username, sizeof(username));
    read_password("Password: ", password, sizeof(password));

    if (strcmp(username, expected_username) == 0 &&
        strcmp(password, expected_password) == 0) {
        printf("Login successful.\n");
        return 1;
    }

    printf("Invalid username or password.\n");
    return 0;
}

static int open_data_file(FILE **file) {
    *file = fopen(DATA_FILE, "rb+");

    if (*file != NULL) {
        return 1;
    }

    *file = fopen(DATA_FILE, "wb+");

    if (*file == NULL) {
        perror("Unable to open data file");
        return 0;
    }

    return 1;
}

static void read_employee_details(Employee *employee) {
    employee->id = read_int("Employee ID: ");
    read_line("Full Name: ", employee->name, sizeof(employee->name));
    read_line("Designation: ", employee->designation, sizeof(employee->designation));
    employee->salary = read_float("Salary: ");
    read_line("Joining Date (DD-MM-YYYY): ",
              employee->joining_date,
              sizeof(employee->joining_date));
    read_line("Gender: ", employee->gender, sizeof(employee->gender));
    read_line("Branch: ", employee->branch, sizeof(employee->branch));
    read_line("Present Address: ",
              employee->present_address,
              sizeof(employee->present_address));
    read_line("Permanent Address: ",
              employee->permanent_address,
              sizeof(employee->permanent_address));
    read_line("Phone: ", employee->phone, sizeof(employee->phone));
    read_line("Email: ", employee->email, sizeof(employee->email));
}

static int find_employee(FILE *file, int id, Employee *employee) {
    rewind(file);

    while (fread(employee, sizeof(Employee), 1, file) == 1) {
        if (employee->id == id) {
            return 1;
        }
    }

    return 0;
}

static void add_employee(FILE *file) {
    Employee employee;

    print_header("Add Employee");
    read_employee_details(&employee);

    fseek(file, 0, SEEK_END);

    if (fwrite(&employee, sizeof(Employee), 1, file) != 1) {
        perror("Unable to save employee");
        return;
    }

    fflush(file);
    printf("\nEmployee added successfully.\n");
}

static void delete_employee(FILE **file) {
    FILE *source = *file;
    FILE *temp;
    Employee employee;
    int id;
    int found = 0;

    print_header("Delete Employee");
    id = read_int("Employee ID to delete: ");

    temp = fopen(TEMP_FILE, "wb");

    if (temp == NULL) {
        perror("Unable to create temporary file");
        return;
    }

    rewind(source);

    while (fread(&employee, sizeof(Employee), 1, source) == 1) {
        if (employee.id == id) {
            found = 1;
            continue;
        }

        if (fwrite(&employee, sizeof(Employee), 1, temp) != 1) {
            perror("Unable to write temporary data");
            fclose(temp);
            remove(TEMP_FILE);
            return;
        }
    }

    fclose(source);
    fclose(temp);

    if (!found) {
        *file = fopen(DATA_FILE, "rb+");
        remove(TEMP_FILE);
        printf("\nEmployee ID %d was not found.\n", id);
        return;
    }

    if (remove(DATA_FILE) != 0 || rename(TEMP_FILE, DATA_FILE) != 0) {
        perror("Unable to update data file");
        *file = fopen(DATA_FILE, "rb+");
        return;
    }

    *file = fopen(DATA_FILE, "rb+");

    if (*file == NULL) {
        perror("Unable to reopen data file");
        return;
    }

    printf("\nEmployee deleted successfully.\n");
}

static void modify_employee(FILE *file) {
    Employee employee;
    int id;

    print_header("Modify Employee");
    id = read_int("Employee ID to modify: ");

    if (!find_employee(file, id, &employee)) {
        printf("\nEmployee ID %d was not found.\n", id);
        return;
    }

    printf("\nEnter the updated details below.\n\n");
    read_employee_details(&employee);

    fseek(file, -(long)sizeof(Employee), SEEK_CUR);

    if (fwrite(&employee, sizeof(Employee), 1, file) != 1) {
        perror("Unable to update employee");
        return;
    }

    fflush(file);
    printf("\nEmployee updated successfully.\n");
}

static void print_employee(const Employee *employee, int include_addresses) {
    printf("\nID              : %d\n", employee->id);
    printf("Name            : %s\n", employee->name);
    printf("Designation     : %s\n", employee->designation);
    printf("Salary          : %.2f\n", employee->salary);
    printf("Joining Date    : %s\n", employee->joining_date);
    printf("Gender          : %s\n", employee->gender);
    printf("Branch          : %s\n", employee->branch);

    if (include_addresses) {
        printf("Present Address : %s\n", employee->present_address);
        printf("Permanent Addr. : %s\n", employee->permanent_address);
    }

    printf("Phone           : %s\n", employee->phone);
    printf("Email           : %s\n", employee->email);
    printf("------------------------------------------------------------\n");
}

static void print_summary(const Employee *employee) {
    printf("ID: %-6d | Name: %-24s | Designation: %-18s | Phone: %s\n",
           employee->id,
           employee->name,
           employee->designation,
           employee->phone);
}

static void display_all(FILE *file) {
    Employee employee;
    int count = 0;

    print_header("All Employees");
    rewind(file);

    while (fread(&employee, sizeof(Employee), 1, file) == 1) {
        print_employee(&employee, 1);
        count++;
    }

    printf("Total employees: %d\n", count);
}

static void search_employee(FILE *file) {
    Employee employee;
    int id;

    print_header("Search Employee");
    id = read_int("Employee ID to search: ");

    if (find_employee(file, id, &employee)) {
        print_employee(&employee, 1);
    } else {
        printf("\nEmployee ID %d was not found.\n", id);
    }
}

static void display_basic_info(FILE *file) {
    Employee employee;

    print_header("Basic Employee Information");
    rewind(file);

    while (fread(&employee, sizeof(Employee), 1, file) == 1) {
        printf("ID: %-6d | Name: %-24s | Designation: %-18s | Gender: %-10s | Branch: %s\n",
               employee.id,
               employee.name,
               employee.designation,
               employee.gender,
               employee.branch);
    }
}

static void display_contact_info(FILE *file) {
    Employee employee;

    print_header("Employee Contact Information");
    rewind(file);

    while (fread(&employee, sizeof(Employee), 1, file) == 1) {
        printf("ID: %-6d | Name: %-24s | Phone: %-16s | Email: %s\n",
               employee.id,
               employee.name,
               employee.phone,
               employee.email);
    }
}

static void list_by_gender(FILE *file, const char *gender) {
    Employee employee;
    int count = 0;

    print_header(gender);
    rewind(file);

    while (fread(&employee, sizeof(Employee), 1, file) == 1) {
        if (strings_equal_ignore_case(employee.gender, gender)) {
            print_summary(&employee);
            count++;
        }
    }

    printf("\nMatching employees: %d\n", count);
}

static void list_by_district(FILE *file, int is_dhaka) {
    Employee employee;
    int count = 0;
    const char *title = is_dhaka
        ? "Employees from Dhaka"
        : "Employees from Other Districts";

    print_header(title);
    rewind(file);

    while (fread(&employee, sizeof(Employee), 1, file) == 1) {
        int in_dhaka = contains_ignore_case(employee.permanent_address, "Dhaka");

        if (in_dhaka == is_dhaka) {
            print_summary(&employee);
            count++;
        }
    }

    printf("\nMatching employees: %d\n", count);
}

static void list_by_branch(FILE *file, int is_main) {
    Employee employee;
    int count = 0;
    const char *title = is_main
        ? "Employees in Main Branch"
        : "Employees in Other Branches";

    print_header(title);
    rewind(file);

    while (fread(&employee, sizeof(Employee), 1, file) == 1) {
        int in_main = strings_equal_ignore_case(employee.branch, "Main");

        if (in_main == is_main) {
            print_summary(&employee);
            count++;
        }
    }

    printf("\nMatching employees: %d\n", count);
}

static void show_menu(void) {
    print_header("Main Menu");

    printf("1.  Add Employee\n");
    printf("2.  Delete Employee\n");
    printf("3.  Modify Employee\n");
    printf("4.  Display All Employees\n");
    printf("5.  Search Employee\n");
    printf("6.  Display Basic Information\n");
    printf("7.  Display Contact Information\n");
    printf("8.  List Male Employees\n");
    printf("9.  List Female Employees\n");
    printf("10. List Employees from Dhaka\n");
    printf("11. List Employees from Other Districts\n");
    printf("12. List Employees from Main Branch\n");
    printf("13. List Employees from Other Branches\n");
    printf("0.  Exit\n");
}

int main(void) {
    FILE *file = NULL;
    int option;

    print_header("Welcome");
    printf("A simple terminal-based employee record management system.\n\n");

    if (!authenticate()) {
        return EXIT_FAILURE;
    }

    if (!open_data_file(&file)) {
        return EXIT_FAILURE;
    }

    for (;;) {
        show_menu();
        option = read_int("\nSelect an option: ");

        switch (option) {
            case 1:
                add_employee(file);
                break;
            case 2:
                delete_employee(&file);
                break;
            case 3:
                modify_employee(file);
                break;
            case 4:
                display_all(file);
                break;
            case 5:
                search_employee(file);
                break;
            case 6:
                display_basic_info(file);
                break;
            case 7:
                display_contact_info(file);
                break;
            case 8:
                list_by_gender(file, "Male");
                break;
            case 9:
                list_by_gender(file, "Female");
                break;
            case 10:
                list_by_district(file, 1);
                break;
            case 11:
                list_by_district(file, 0);
                break;
            case 12:
                list_by_branch(file, 1);
                break;
            case 13:
                list_by_branch(file, 0);
                break;
            case 0:
                fclose(file);
                printf("\nThank you for using Employee Management System.\n");
                return EXIT_SUCCESS;
            default:
                printf("\nInvalid option. Please choose a number from 0 to 13.\n");
                break;
        }

        pause_screen();
    }
}
