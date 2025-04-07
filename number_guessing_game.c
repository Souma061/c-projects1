#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main() {
  int random , guess;
  int number_guessed=0;
  srand(time(NULL));

  printf("welcome\n"); 
  random = rand() % 100 + 1;


  do {
    printf("ENTER YOUR GUESS BETWEEN(1-100): ");
    scanf("%d" , &guess);
    number_guessed++;
     
    if(guess < random) {
      printf("Guess larger number.\n");
    } else if (guess > random) {
      printf("Guess smaller number.\n");

    } else {
      printf("YOU HAVE SUCCESSFULLY GUEESED THE NUMBER IN %d attempts\n" , number_guessed);
    }


  } while (guess != random);
  printf("BYE BYE\n");
  printf("created by Souma_061");
  
  

}