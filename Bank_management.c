#include<stdio.h>
#include<string.h>

typedef struct {
  char name[50];
  int acc_no;
  float balance;
} Account;

void create_account();
void deposit_money();
void withdraw_money();
void check_balance();

const char* ACCOUNT_FILE = "account.dat";



int main() {

  while(1) {
    int choice;
    printf("\n\n---welcome to in our bank.---");
    printf("\n1. Create your account.");
    printf("\n2. Deposit money: ");
    printf("\n3. Withdraw money: ");
    printf("\n4. Check Bank balance: ");
    printf("\n5. Exit. ");
    printf("\nEnter your choice: ");
    scanf("%d" , &choice);

    switch(choice)
    {
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
      printf("\nClosing your bank, Thanks for your visit!!\n");
      return 0;
      break;
      default:
      printf("\nInvalid choice!!\n");
      break;


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

  char c;
  do {
    c = getchar();
  } while(c != '\n' && c != EOF);

  printf("\nEnter your name: ");
  fgets(acc.name , sizeof(acc.name) , stdin);
  int ind = strcspn(acc.name , "\n");
  acc.name[ind] = '0';
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
  printf("\nEbter your account No.: ");
  scanf("%d" , &acc_no);
  printf("\nEnter the amount to deposit: ");
  scanf("%f" , &money);

  while(fread(&acc_to_read , sizeof(acc_to_read) , 1 , file)) {
    if(acc_to_read.acc_no == acc_no ) {
      acc_to_read.balance += money;
      fseek(file , -sizeof(acc_to_read) , SEEK_CUR);
      fwrite(&acc_to_read , sizeof(acc_to_read) , 1 , file);
      fclose(file);
      printf("\nSuccessfully deposited Rs.%.2f  New balance is Rs. %.2f" , money , acc_to_read.balance);
      return;
    }
  }
  fclose(file);
  printf("\nMoney could not be deposited as the account No. %d was not found in the records." , acc_no);

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
  printf("\nEbter your account No.: ");
  scanf("%d" , &acc_no);
  printf("\nEnter the amount you wish to withdraw: ");
  scanf("%f" , &money);

  while(fread(&acc_to_read , sizeof(acc_to_read) , 1 , file) != EOF) {
    if(acc_to_read.acc_no == acc_no) {
      if(acc_to_read.balance >= money ) {
        acc_to_read.balance -= money;
        fseek(file , -sizeof(acc_to_read) , SEEK_CUR);
        fwrite(&acc_to_read , sizeof(acc_to_read) , 1 , file);
        fclose(file);
        printf("\nSuccessfully withdrawed Rs.%.2f. New balance is Rs.%.2f" , money , acc_to_read.balance);
      } else {
        printf("\nInsufficient balance!!!");
      }
      fclose(file);
      return;
    }
  }
  fclose(file);
  printf("\nMoney could not be withdrawed as the account No. %d was not found in the records." , acc_no);
  
}
void check_balance() {
   FILE *file = fopen( ACCOUNT_FILE , "rb");
    if(file == NULL) {
    printf("\nUnable to open file! ");
    return;
  }
  int acc_no;
  Account acc_read;
  printf("\nEnter your account no. : ");
  scanf("%d" , &acc_no);

  while(fread(&acc_read , sizeof(acc_read) , 1 , file)) {
    if(acc_read.acc_no == acc_no) {
      printf("\nYour current balance is Rs.%.2f" , acc_read.balance);
      fclose(file);
      return;
    }
  }
  fclose(file);
  printf("\nAccount No.:%d was not found.\n" , acc_no);
}