#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>

#define MAX_USERS 10
#define CREDENTIAL_LENGTH 30
#define ACCOUNT_FILE "account.dat"

typedef struct {
    char username[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];
} User;

typedef struct {
    char name[50];
    int acc_no;
    float balance;
} Account;

User users[MAX_USERS];
int user_count = 0;
int isAdmin = 0; // Flag for admin login

// Function Prototypes
void register_user();
int login_user();
void input_credentials(char* username, char* password);
void fix_fgets_inputs(char* string);
void create_account();
void deposit_money();
void withdraw_money();
void check_balance();
void view_all_accounts();

int main() {
    int option, user_index;

    while (1) {
        printf("\nWelcome to the user management system");
        printf("\n1. Register");
        printf("\n2. Login");
        printf("\n3. Exit");
        printf("\nChoose an option: ");
        scanf("%d", &option);
        getchar();

        switch (option) {
            case 1:
                register_user();
                break;
            case 2:
                user_index = login_user();
                if (user_index >= 0) {
                    if (strcmp(users[user_index].username, "admin") == 0) {
                        isAdmin = 1;
                        printf("\nAdmin login successful!\n");
                    } else {
                        printf("\nLogin successful! Welcome, %s!\n", users[user_index].username);
                    }
                } else {
                    printf("\nLogin failed! Incorrect username or password.\n");
                }
                break;
            case 3:
                printf("\nExiting Program.\n");
                return 0;
            default:
                printf("\nInvalid option. Please try again.\n");
        }

        while (user_index >= 0) {
            int choice;
            printf("\n\n--- Welcome to the Bank System ---");
            printf("\n1. Create your account.");
            printf("\n2. Deposit money.");
            printf("\n3. Withdraw money.");
            printf("\n4. Check balance.");
            if (isAdmin) {
                printf("\n5. View all accounts (Admin Only).");
            }
            printf("\n6. Logout.");
            printf("\nEnter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    create_account();
                    break;
                case 2:
                    deposit_money();
                    break;
                case 3:
                    withdraw_money();
                    break;
                case 4:
                    check_balance();
                    break;
                case 5:
                    if (isAdmin) {
                        view_all_accounts();
                    } else {
                        printf("\nInvalid choice!\n");
                    }
                    break;
                case 6:
                    printf("\nLogging out...\n");
                    user_index = -1;
                    isAdmin = 0;
                    break;
                default:
                    printf("\nInvalid choice!\n");
            }
        }
    }

    return 0;
}

void register_user() {
    if (user_count == MAX_USERS) {
        printf("\nMaximum %d users are allowed!\n", MAX_USERS);
        return;
    }

    int new_index = user_count;
    printf("Register a new user. ");
    input_credentials(users[new_index].username, users[new_index].password);
    user_count++;
    printf("\nRegistration successful!\n");
}

int login_user() {
    char username[CREDENTIAL_LENGTH], password[CREDENTIAL_LENGTH];

    input_credentials(username, password);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(username, users[i].username) == 0 &&
            strcmp(password, users[i].password) == 0) {
            return i;
        }
    }

    return -1;
}

void input_credentials(char* username, char* password) {
    printf("\nEnter your username: ");
    fgets(username, CREDENTIAL_LENGTH, stdin);
    fix_fgets_inputs(username);

    printf("Enter password: ");
    fflush(stdout);

    char ch;
    int i = 0;
    while (1) {
        ch = getch();
        if (ch == '\r') {
            break;
        } else if (ch == '\b' && i > 0) {
            i--;
            printf("\b \b");
        } else if (ch != '\b') {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}

void fix_fgets_inputs(char* string) {
    int index = strcspn(string, "\n");
    string[index] = '\0';
}

// Bank Functions

void create_account() {
    Account acc;
    FILE *file = fopen(ACCOUNT_FILE, "ab+");
    if (file == NULL) {
        printf("\nUnable to open file!\n");
        return;
    }

    printf("\nEnter your name: ");
    fgets(acc.name, sizeof(acc.name), stdin);
    fix_fgets_inputs(acc.name);
    printf("\nEnter your account number: ");
    scanf("%d", &acc.acc_no);
    acc.balance = 0;

    fwrite(&acc, sizeof(acc), 1, file);
    fclose(file);
    printf("\nAccount created successfully!\n");
}

void deposit_money() {
    FILE *file = fopen(ACCOUNT_FILE, "rb+");
    if (file == NULL) {
        printf("\nUnable to open file!\n");
        return;
    }

    int acc_no;
    float money;
    Account acc_to_read;
    printf("\nEnter your account No.: ");
    scanf("%d", &acc_no);
    printf("\nEnter the amount to deposit: ");
    scanf("%f", &money);

    while (fread(&acc_to_read, sizeof(acc_to_read), 1, file)) {
        if (acc_to_read.acc_no == acc_no) {
            acc_to_read.balance += money;
            fseek(file, -sizeof(acc_to_read), SEEK_CUR);
            fwrite(&acc_to_read, sizeof(acc_to_read), 1, file);
            fclose(file);
            printf("\nSuccessfully deposited Rs.%.2f. New balance is Rs. %.2f\n", money, acc_to_read.balance);
            return;
        }
    }
    fclose(file);
    printf("\nAccount No. %d not found!\n", acc_no);
}

void withdraw_money() {
    FILE *file = fopen(ACCOUNT_FILE, "rb+");
    if (file == NULL) {
        printf("\nUnable to open file!\n");
        return;
    }

    int acc_no;
    float money;
    Account acc_to_read;
    printf("\nEnter your account No.: ");
    scanf("%d", &acc_no);
    printf("\nEnter the amount to withdraw: ");
    scanf("%f", &money);

    while (fread(&acc_to_read, sizeof(acc_to_read), 1, file)) {
        if (acc_to_read.acc_no == acc_no) {
            if (acc_to_read.balance >= money) {
                acc_to_read.balance -= money;
                fseek(file, -sizeof(acc_to_read), SEEK_CUR);
                fwrite(&acc_to_read, sizeof(acc_to_read), 1, file);
                fclose(file);
                printf("\nSuccessfully withdrew Rs.%.2f. New balance is Rs.%.2f\n", money, acc_to_read.balance);
                return;
            } else {
                printf("\nInsufficient balance!\n");
                fclose(file);
                return;
            }
        }
    }
    fclose(file);
    printf("\nAccount not found!\n");
}

void check_balance() {
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (file == NULL) {
        printf("\nUnable to open file!\n");
        return;
    }

    int acc_no;
    Account acc_read;
    printf("\nEnter your account No.: ");
    scanf("%d", &acc_no);

    while (fread(&acc_read, sizeof(acc_read), 1, file)) {
        if (acc_read.acc_no == acc_no) {
            printf("\nYour current balance is Rs.%.2f\n", acc_read.balance);
            fclose(file);
            return;
        }
    }
    fclose(file);
    printf("\nAccount not found!\n");
}

void view_all_accounts() {
    FILE *file = fopen(ACCOUNT_FILE, "rb");
    if (file == NULL) {
        printf("\nUnable to open file!\n");
        return;
    }

    Account acc;
    printf("\n--- All Accounts ---\n");
    while (fread(&acc, sizeof(acc), 1, file)) {
        printf("Name: %s, Account No: %d, Balance: Rs.%.2f\n", acc.name, acc.acc_no, acc.balance);
    }
    fclose(file);
}
