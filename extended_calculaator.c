#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

// Function declarations
double division(double, double);
double modulus(int, int);
void print_menu();
void conversion_menu();
int is_valid_binary(const char*);
int binary_to_decimal(const char*);
void binary_to_hex(const char*);
void decimal_to_binary(const char*);
void hex_to_binary(const char*);
void octal_to_binary(const char*);

int main() {
    int choice;
    double first, second, result;

    while (1) {
        print_menu();
        scanf("%d", &choice);

        if (choice == 8) {
            printf("\nExiting calculator. Goodbye!\n");
            break;
        }

        if (choice < 1 || choice > 8) {
            fprintf(stderr, "Invalid Menu Choice.\n");
            continue;
        }

        if (choice == 7) {
            conversion_menu();
            continue;
        }

        printf("\nEnter the first number: ");
        scanf("%lf", &first);
        printf("Enter the second number: ");
        scanf("%lf", &second);

        switch (choice) {
            case 1: result = first + second; break;
            case 2: result = first - second; break;
            case 3: result = first * second; break;
            case 4: result = division(first, second); break;
            case 5: result = modulus((int)first, (int)second); break;
            case 6: result = pow(first, second); break;
        }

        if (!isnan(result)) {
            printf("\nResult: %.2f\n", result);
        }
    }

    return 0;
}

// Arithmetic functions
double division(double a, double b) {
    if (b == 0) {
        fprintf(stderr, "Error: Division by zero.\n");
        return NAN;
    }
    return a / b;
}

double modulus(int a, int b) {
    if (b == 0) {
        fprintf(stderr, "Error: Modulus by zero.\n");
        return NAN;
    }
    return a % b;
}

void print_menu() {
    printf("\n\n================== Calculator ==================");
    printf("\n1. Add");
    printf("\n2. Subtract");
    printf("\n3. Multiply");
    printf("\n4. Division");
    printf("\n5. Modulus");
    printf("\n6. Power");
    printf("\n7. Number System Conversions");
    printf("\n8. Exit");
    printf("\n===============================================");
    printf("\nEnter your choice: ");
}

// Number system conversions
void conversion_menu() {
    int sub_choice;
    char input[65];

    printf("\n\n====== Number System Conversion Menu ======");
    printf("\n1. Binary to Decimal");
    printf("\n2. Binary to Hexadecimal");
    printf("\n3. Binary to Octal");
    printf("\n4. Decimal to Binary");
    printf("\n5. Hexadecimal to Binary");
    printf("\n6. Octal to Binary");
    printf("\nEnter your choice: ");
    scanf("%d", &sub_choice);

    switch (sub_choice) {
        case 1:
            printf("Enter a binary number: ");
            scanf("%s", input);
            if (!is_valid_binary(input)) {
                fprintf(stderr, "Invalid binary input.\n");
                return;
            }
            printf("Decimal: %d\n", binary_to_decimal(input));
            break;

        case 2:
            printf("Enter a binary number: ");
            scanf("%s", input);
            if (!is_valid_binary(input)) {
                fprintf(stderr, "Invalid binary input.\n");
                return;
            }
            binary_to_hex(input);
            break;

        case 3:
            printf("Enter a binary number: ");
            scanf("%s", input);
            if (!is_valid_binary(input)) {
                fprintf(stderr, "Invalid binary input.\n");
                return;
            }
            printf("Octal: %o\n", binary_to_decimal(input));
            break;

        case 4:
            printf("Enter a decimal number: ");
            scanf("%s", input);
            decimal_to_binary(input);
            break;

        case 5:
            printf("Enter a hexadecimal number: ");
            scanf("%s", input);
            hex_to_binary(input);
            break;

        case 6:
            printf("Enter an octal number: ");
            scanf("%s", input);
            octal_to_binary(input);
            break;

        default:
            fprintf(stderr, "Invalid conversion choice.\n");
    }
}

int is_valid_binary(const char *bin) {
    for (int i = 0; bin[i] != '\0'; i++) {
        if (bin[i] != '0' && bin[i] != '1') return 0;
    }
    return 1;
}

int binary_to_decimal(const char *bin) {
    return (int)strtol(bin, NULL, 2);
}

void binary_to_hex(const char *bin) {
    int decimal = binary_to_decimal(bin);
    printf("Hexadecimal: %X\n", decimal);
}

void decimal_to_binary(const char *input) {
    int num = atoi(input);
    if (num < 0) {
        fprintf(stderr, "Negative values not supported.\n");
        return;
    }

    char binary[65];
    int i = 0;

    if (num == 0) {
        printf("Binary: 0\n");
        return;
    }

    while (num > 0) {
        binary[i++] = (num % 2) + '0';
        num /= 2;
    }
    binary[i] = '\0';

    // Reverse binary string
    for (int j = 0; j < i / 2; j++) {
        char temp = binary[j];
        binary[j] = binary[i - j - 1];
        binary[i - j - 1] = temp;
    }

    printf("Binary: %s\n", binary);
}

void hex_to_binary(const char *hex) {
    printf("Binary: ");
    for (int i = 0; hex[i] != '\0'; i++) {
        char ch = toupper(hex[i]);
        switch (ch) {
            case '0': printf("0000"); break;
            case '1': printf("0001"); break;
            case '2': printf("0010"); break;
            case '3': printf("0011"); break;
            case '4': printf("0100"); break;
            case '5': printf("0101"); break;
            case '6': printf("0110"); break;
            case '7': printf("0111"); break;
            case '8': printf("1000"); break;
            case '9': printf("1001"); break;
            case 'A': printf("1010"); break;
            case 'B': printf("1011"); break;
            case 'C': printf("1100"); break;
            case 'D': printf("1101"); break;
            case 'E': printf("1110"); break;
            case 'F': printf("1111"); break;
            default:
                fprintf(stderr, "\nInvalid hex character: %c\n", ch);
                return;
        }
    }
    printf("\n");
}

void octal_to_binary(const char *octal) {
    int decimal = (int)strtol(octal, NULL, 8);
    if (decimal < 0) {
        fprintf(stderr, "Invalid octal input.\n");
        return;
    }
    decimal_to_binary(octal);
}
