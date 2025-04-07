#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>

#define MAX_USERS 10
#define CREDENTIAL_LENGTH 30

typedef struct {
    char username[CREDENTIAL_LENGTH];
    char password[CREDENTIAL_LENGTH];
} User;

User users[MAX_USERS];
int user_count = 0;

void register_user();
int login_user();   // returns the user index
void fix_fgets_inputs(char*);
void input_credentials(char* username, char* password);

int main() {
    int option;
    int user_index;
    while (1) {
        printf("\nWelcome to the user management");
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
                    printf("\nLogin successful! Welcome, %s!\n", users[user_index].username);
                } else {
                    printf("\nLogin failed! Incorrect username or password.\n");
                }
                break;
            case 3:
                printf("\nExiting Program.\n");
                return 0;
            default:
                printf("\nInvalid option. Please try again.\n");
                break;
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

    printf("\nRegistration successful!!\n");
}

int login_user() {
    char password[CREDENTIAL_LENGTH];
    char username[CREDENTIAL_LENGTH];

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
    printf("\nEnter your username:\n");
    fgets(username, CREDENTIAL_LENGTH, stdin);
    fix_fgets_inputs(username);

    printf("Enter password: ");
    fflush(stdout);

    char ch;
    int i = 0;
    while (1) {
        ch = getch();  // Read single character without echoing
        if (ch == '\r') {  // Enter key pressed
            break;
        } else if (ch == '\b' && i > 0) {  // Handle backspace
            i--;
            printf("\b \b");
        } else if (ch != '\b') {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';  // Null-terminate the password
    printf("\n");  // Move to new line
}

void fix_fgets_inputs(char* string) {
    int index = strcspn(string, "\n");
    string[index] = '\0';
}
