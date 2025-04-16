#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME_LEN 50
#define LOG_FILE "transaction.log"

typedef struct {
  char name[MAX_NAME_LEN];
  int acc_no;
  float balance;
} Account;

void create_account();
void deposit_money();
void withdraw_money();
void check_balance();
void admin_menu();
void view_all_accounts();
void delete_account();
void reset_data();

const char* ACCOUNT_FILE = "account.dat";

int main() {
  while(1) {
    int choice;
    printf("\n\n--- Welcome to Our Bank ---");
    printf("\n1. Create your account.");
    printf("\n2. Deposit money: ");
    printf("\n3. Withdraw money: ");
    printf("\n4. Check Bank balance: ");
    printf("\n5. Admin login");
    printf("\n6. Exit. ");
    printf("\nEnter your choice: ");
    scanf("%d" , &choice);

    switch(choice) {
      case 1: create_account(); break;
      case 2: deposit_money(); break;
      case 3: withdraw_money(); break;
      case 4: check_balance(); break;
      case 5: admin_menu(); break;
      case 6:
        printf("\nClosing your bank, Thanks for your visit!!\n");
        return 0;
      default:
        printf("\nInvalid choice!!\n");
    }
  }
}

void create_account() {
  Account acc;
  FILE *file = fopen(ACCOUNT_FILE , "ab+");
  if(file == NULL) {
    printf("\nUnable to open file! ");
    return;
  }

  getchar();
  printf("\nEnter your name: ");
  fgets(acc.name , sizeof(acc.name) , stdin);
  acc.name[strcspn(acc.name , "\n")] = '\0';

  printf("\nEnter your account number: ");
  scanf("%d" , &acc.acc_no);
  acc.balance = 0;

  fwrite(&acc , sizeof(acc) , 1 , file);
  fclose(file);
  printf("\nAccount created successfully!!");
}

void deposit_money() {
  FILE *file = fopen(ACCOUNT_FILE , "rb+");
  if(file == NULL) {
    printf("\nUnable to open file! ");
    return;
  }

  int acc_no;
  float money;
  Account acc_to_read;
  printf("\nEnter your account No.: ");
  scanf("%d" , &acc_no);
  printf("\nEnter the amount to deposit: ");
  scanf("%f" , &money);

  while(fread(&acc_to_read , sizeof(acc_to_read) , 1 , file)) {
    if(acc_to_read.acc_no == acc_no ) {
      acc_to_read.balance += money;
      fseek(file , -sizeof(acc_to_read) , SEEK_CUR);
      fwrite(&acc_to_read , sizeof(acc_to_read) , 1 , file);
      fclose(file);
      FILE *log = fopen(LOG_FILE, "a");
      if (log) {
        fprintf(log, "Deposited Rs.%.2f to Acc No. %d\n", money, acc_no);
        fclose(log);
      }
      printf("\nSuccessfully deposited Rs.%.2f. New balance is Rs. %.2f" , money , acc_to_read.balance);
      return;
    }
  }
  fclose(file);
  printf("\nAccount No. %d not found.", acc_no);
}

void withdraw_money() {
  FILE *file = fopen(ACCOUNT_FILE , "rb+");
  if(file == NULL) {
    printf("\nUnable to open file! ");
    return;
  }

  int acc_no;
  float money;
  Account acc_to_read;
  printf("\nEnter your account No.: ");
  scanf("%d" , &acc_no);
  printf("\nEnter the amount you wish to withdraw: ");
  scanf("%f" , &money);

  while(fread(&acc_to_read , sizeof(acc_to_read) , 1 , file)) {
    if(acc_to_read.acc_no == acc_no) {
      if(acc_to_read.balance >= money ) {
        acc_to_read.balance -= money;
        fseek(file , -sizeof(acc_to_read) , SEEK_CUR);
        fwrite(&acc_to_read , sizeof(acc_to_read) , 1 , file);
        fclose(file);
        FILE *log = fopen(LOG_FILE, "a");
        if (log) {
          fprintf(log, "Withdrew Rs.%.2f from Acc No. %d\n", money, acc_no);
          fclose(log);
        }
        printf("\nSuccessfully withdrew Rs.%.2f. New balance is Rs.%.2f" , money , acc_to_read.balance);
      } else {
        printf("\nInsufficient balance!!!");
      }
      fclose(file);
      return;
    }
  }
  fclose(file);
  printf("\nAccount No. %d not found.", acc_no);
}

void check_balance() {
  FILE *file = fopen( ACCOUNT_FILE , "rb");
  if(file == NULL) {
    printf("\nUnable to open file! ");
    return;
  }

  int acc_no;
  Account acc_read;
  printf("\nEnter your account no.: ");
  scanf("%d" , &acc_no);

  while(fread(&acc_read , sizeof(acc_read) , 1 , file)) {
    if(acc_read.acc_no == acc_no) {
      printf("\nYour current balance is Rs.%.2f" , acc_read.balance);
      fclose(file);
      return;
    }
  }
  fclose(file);
  printf("\nAccount No. %d not found.\n" , acc_no);
}

void admin_menu() {
  char username[20], password[20];
  printf("\nEnter admin username: ");
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
      printf("5. Reset all data\n");
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
        case 5: reset_data(); break;
        default: printf("Invalid choice.\n");
      }
    }
  } else {
    printf("Invalid admin credentials!\n");
  }
}

void view_all_accounts() {
  FILE *file = fopen(ACCOUNT_FILE, "rb");
  if (!file) {
    printf("Unable to open account file.\n");
    return;
  }
  Account acc;
  printf("\n--- All Accounts ---\n");
  while (fread(&acc, sizeof(acc), 1, file)) {
    if (acc.acc_no <= 0 || acc.balance < 0) {
      printf("Invalid or corrupt record found. Skipping...\n");
      continue;
    }
    printf("Name: %s | Acc No: %d | Balance: Rs. %.2f\n", acc.name, acc.acc_no, acc.balance);
  }
  fclose(file);
}

void delete_account() {
  int acc_no;
  printf("Enter account number to delete: ");
  scanf("%d", &acc_no);

  FILE *file = fopen(ACCOUNT_FILE, "rb");
  FILE *temp = fopen("temp.dat", "wb");
  if (!file || !temp) {
    printf("File error.\n");
    return;
  }

  Account acc;
  int found = 0;
  while (fread(&acc, sizeof(acc), 1, file)) {
    if (acc.acc_no == acc_no) {
      found = 1;
      continue;
    }
    fwrite(&acc, sizeof(acc), 1, temp);
  }

  fclose(file);
  fclose(temp);
  remove(ACCOUNT_FILE);
  rename("temp.dat", ACCOUNT_FILE);

  if (found) {
    printf("Account deleted successfully.\n");
  } else {
    printf("Account not found.\n");
  }
}

void reset_data() {
  FILE *file = fopen(ACCOUNT_FILE, "wb");
  FILE *log = fopen(LOG_FILE, "w");
  if (file && log) {
    fclose(file);
    fclose(log);
    printf("\nAll data has been reset successfully.\n");
  } else {
    printf("\nError resetting data!\n");
  }
}
