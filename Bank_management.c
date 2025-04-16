#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char name[50];
    int acc_no;
    float balance;
    char password[20];
} Account;

const char* ACCOUNT_FILE = "account.dat";
const char* LOG_FILE = "transactions.txt";

bool is_unique_account(int acc_no);
bool login(int acc_no, const char* password);
void log_transaction(const char* type, int acc_no, float amount);

void create_account();
void deposit_money();
void withdraw_money();
void check_balance();
void admin_menu();
void view_all_accounts();
void delete_account();

int main() {
    while (1) {
        int choice;
        printf("\n\n--- Welcome to Our Bank ---");
        printf("\n1. Create your account");
        printf("\n2. Deposit money");
        printf("\n3. Withdraw money");
        printf("\n4. Check bank balance");
        printf("\n5. Admin login");
        printf("\n6. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: create_account(); break;
            case 2: deposit_money(); break;
            case 3: withdraw_money(); break;
            case 4: check_balance(); break;
            case 5: admin_menu(); break;
            case 6: printf("\nThanks for visiting!\n"); return 0;
            default: printf("\nInvalid choice!\n"); break;
        }
    }
}

bool is_unique_account(int acc_no) {
    FILE* file = fopen(ACCOUNT_FILE, "rb");
    if (!file) return true;
    Account acc;
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no) {
            fclose(file);
            return false;
        }
    }
    fclose(file);
    return true;
}

void create_account() {
    Account acc;
    FILE* file = fopen(ACCOUNT_FILE, "ab+");
    if (!file) {
        printf("\nUnable to open file!");
        return;
    }

    getchar(); // clear newline
    printf("Enter your name: ");
    fgets(acc.name, sizeof(acc.name), stdin);
    acc.name[strcspn(acc.name, "\n")] = '\0';

    while (1) {
        printf("Enter your account number: ");
        scanf("%d", &acc.acc_no);
        if (is_unique_account(acc.acc_no)) break;
        else printf("Account number already exists. Try again.\n");
    }

    printf("Set a password for your account: ");
    scanf("%s", acc.password);
    acc.balance = 0.0;

    fwrite(&acc, sizeof(acc), 1, file);
    fclose(file);
    printf("Account created successfully!\n");
}

bool login(int acc_no, const char* password) {
    FILE* file = fopen(ACCOUNT_FILE, "rb");
    if (!file) return false;

    Account acc;
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no && strcmp(acc.password, password) == 0) {
            fclose(file);
            return true;
        }
    }
    fclose(file);
    return false;
}

void deposit_money() {
    int acc_no;
    char password[20];
    float money;

    printf("Enter account number: ");
    scanf("%d", &acc_no);
    printf("Enter password: ");
    scanf("%s", password);

    if (!login(acc_no, password)) {
        printf("Login failed!\n");
        return;
    }

    FILE* file = fopen(ACCOUNT_FILE, "rb+");
    Account acc;
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no) {
            printf("Enter amount to deposit: ");
            scanf("%f", &money);
            acc.balance += money;

            fseek(file, -sizeof(acc), SEEK_CUR);
            fwrite(&acc, sizeof(acc), 1, file);
            fclose(file);
            log_transaction("Deposit", acc_no, money);

            printf("Deposited Rs. %.2f. New balance: Rs. %.2f\n", money, acc.balance);
            return;
        }
    }
    fclose(file);
    printf("Account not found.\n");
}

void withdraw_money() {
    int acc_no;
    char password[20];
    float money;

    printf("Enter account number: ");
    scanf("%d", &acc_no);
    printf("Enter password: ");
    scanf("%s", password);

    if (!login(acc_no, password)) {
        printf("Login failed!\n");
        return;
    }

    FILE* file = fopen(ACCOUNT_FILE, "rb+");
    Account acc;
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no) {
            printf("Enter amount to withdraw: ");
            scanf("%f", &money);
            if (acc.balance >= money) {
                acc.balance -= money;
                fseek(file, -sizeof(acc), SEEK_CUR);
                fwrite(&acc, sizeof(acc), 1, file);
                fclose(file);
                log_transaction("Withdraw", acc_no, money);

                printf("Withdrawn Rs. %.2f. New balance: Rs. %.2f\n", money, acc.balance);
            } else {
                printf("Insufficient balance!\n");
            }
            return;
        }
    }
    fclose(file);
    printf("Account not found.\n");
}

void check_balance() {
    int acc_no;
    char password[20];

    printf("Enter account number: ");
    scanf("%d", &acc_no);
    printf("Enter password: ");
    scanf("%s", password);

    if (!login(acc_no, password)) {
        printf("Login failed!\n");
        return;
    }

    FILE* file = fopen(ACCOUNT_FILE, "rb");
    Account acc;
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no == acc_no) {
            printf("Hello, %s\nYour current balance is Rs. %.2f\n", acc.name, acc.balance);
            fclose(file);
            return;
        }
    }
    fclose(file);
    printf("Account not found.\n");
}

void log_transaction(const char* type, int acc_no, float amount) {
    FILE* log = fopen(LOG_FILE, "a");
    if (!log) return;
    fprintf(log, "%s - Acc No: %d - Rs. %.2f\n", type, acc_no, amount);
    fclose(log);
}

void admin_menu() {
    char username[20], password[20];
    printf("Enter admin username: ");
    scanf("%s", username);
    printf("Enter admin password: ");
    scanf("%s", password);

    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
        int ch;
        while (1) {
            printf("\n--- Admin Menu ---\n");
            printf("1. View all accounts\n");
            printf("2. Delete an account\n");
            printf("3. View transaction log\n");
            printf("4. Exit admin panel\n");
            printf("Enter choice: ");
            scanf("%d", &ch);

            switch (ch) {
                case 1: view_all_accounts(); break;
                case 2: delete_account(); break;
                case 3: {
                    char line[256];
                    FILE* log = fopen(LOG_FILE, "r");
                    if (!log) {
                        printf("No logs found.\n");
                        break;
                    }
                    printf("\n--- Transaction Log ---\n");
                    while (fgets(line, sizeof(line), log)) {
                        printf("%s", line);
                    }
                    fclose(log);
                    break;
                }
                case 4: return;
                default: printf("Invalid choice.\n");
            }
        }
    } else {
        printf("Invalid admin credentials!\n");
    }
}

void view_all_accounts() {
    FILE* file = fopen(ACCOUNT_FILE, "rb");
    if (!file) {
        printf("No accounts found.\n");
        return;
    }
    Account acc;
    printf("\n--- All Accounts ---\n");
    while (fread(&acc, sizeof(acc), 1, file)) {
        printf("Name: %s | Acc No: %d | Balance: Rs. %.2f\n", acc.name, acc.acc_no, acc.balance);
    }
    fclose(file);
}

void delete_account() {
    int acc_no;
    printf("Enter account number to delete: ");
    scanf("%d", &acc_no);

    FILE* file = fopen(ACCOUNT_FILE, "rb");
    FILE* temp = fopen("temp.dat", "wb");
    if (!file || !temp) {
        printf("Error opening files.\n");
        return;
    }

    Account acc;
    bool found = false;
    while (fread(&acc, sizeof(acc), 1, file)) {
        if (acc.acc_no != acc_no) {
            fwrite(&acc, sizeof(acc), 1, temp);
        } else {
            found = true;
        }
    }

    fclose(file);
    fclose(temp);
    remove(ACCOUNT_FILE);
    rename("temp.dat", ACCOUNT_FILE);

    if (found)
        printf("Account deleted successfully.\n");
    else
        printf("Account not found.\n");
}
