#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <math.h>

void start_menu();
void create_txt_file(char name[], char id[], char acc_type[], char pin[]);

struct variables {
    //Details - Create New Bank Acc
    char name[30];
    char id[12];
    char acc_type[20];
    char pin[5];
    float amount;
    
    //File Management
    char filename[7];
};

void start_menu(){
    printf("========================================\n");
    printf("BANKING SYSTEM APPLICATION\n");
    printf("========================================\n");
    char *actions[] = {
        "1. Create New Bank Account",
        "2. Delete Bank Account",
        "3. Deposit Money",
        "4. Withdraw Money",
        "5. Remittance Money",
        "6. Exit"
    };

    int num_actions = sizeof(actions)/sizeof(actions[0]);

    for(int i=0;i<num_actions;i++){
        printf("%s\n",actions[i]);
    }
}

void create_txt_file(char name[], char id[], char acc_type[], char pin[]){
    FILE *fp;
    struct variables variable;
    srand(time(NULL));

    char acc_num[7] = "";
    variable.amount = 0;

    for(int i=0;i<6;i++){
        int rand_num = rand() % 9 + 1;
        char digit[2];
        sprintf(digit,"%d", rand_num);
        strcat(acc_num, digit);
    }

    sprintf(variable.filename,"database/%s.txt",acc_num);
    fp = fopen(variable.filename,"w");

    if (fp == NULL) {
        printf("Error! File cannot be opened!\n");
    }

    fprintf(fp,"%s\n",acc_num);
    fprintf(fp,"%s\n",name);
    fprintf(fp,"%s\n",id);
    fprintf(fp,"%s\n",acc_type);
    fprintf(fp,"%s\n",pin);
    fprintf(fp,"%.2f\n",variable.amount);
    fclose(fp);
}

void create_new_bank_acc(){
    struct variables variable;
    int numPin = 0;
    printf("\n--------CREATE NEW BANK ACCOUNT---------\n");
    printf("Please insert your personal details below\n");

    printf("Name: ");
    fgets(variable.name, sizeof(variable.name), stdin);
    variable.name[strcspn(variable.name, "\n")] = '\0';

    printf("Identification Number: ");
    fgets(variable.id, sizeof(variable.id), stdin);
    variable.id[strcspn(variable.id, "\n")] = '\0';

    printf("Type of Account(Savings or Current): ");
    fgets(variable.acc_type, sizeof(variable.acc_type), stdin);
    variable.acc_type[strcspn(variable.acc_type, "\n")] = '\0';

    while(numPin < 4){
        printf("4-digit Pin: ");
        fgets(variable.pin, sizeof(variable.pin), stdin);
        variable.pin[strcspn(variable.pin, "\n")] = '\0';

        numPin = strlen(variable.pin);
        if (numPin < 4) {
            printf("4 Digit Pin is required. Please retype.\n\n");
            continue;
        } else {
            break;
        }
    }
    
    create_txt_file(variable.name,variable.id,variable.acc_type,variable.pin);
}

bool check_last4Digits(char filename[],char inputLast4[10]){
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL){
        return false;
    }

    char content[256];
    int lineNum = 0;
    int targetLine = 3;

    //Read content of every line
    while (fgets(content, sizeof(content), fp)){
        lineNum++;
        //Stop at line 3
        if (lineNum == targetLine){
            break;
        }
    }
    
    content[strcspn(content, "\n")] = '\0';

    int size = strlen(content);
    if (strcmp(content + (size - 4), inputLast4) == 0){
        return true;
    } else {
        return false;
    }
}

bool check_pin(char filename[], char inputPin[]) {
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return false;
    }

    char content[256];
    int lineNum = 0;
    int targetLine = 5;
    int numCorrect = 0;

    while (fgets(content, sizeof(content), fp)){
        lineNum++;
        //Stop at line 5
        if (lineNum == targetLine){
            break;
        }
    }

    content[strcspn(content, "\n")] = '\0';
    if(strcmp(content,inputPin) == 0){
        return true;
    } else {
        return false;
    }
}

void delete_bank_acc(){
    DIR *d;
    struct dirent *dir;
    struct variables variable;
    char accNum[16];
    char pin[6];
    char last4Digits[6];
    char filename[20];
    printf("\n----------DELETE BANK ACCOUNT-----------\n");
    d = opendir("database"); 
    if (d == NULL) {
        printf("Cannot open directory\n");
    }

    //List current bank accounts
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;

        printf("%s\n", dir->d_name);
    }
    closedir(d);

    while(true){
        printf("\nBank Account Number: ");
        if (!fgets(accNum, sizeof(accNum), stdin)) continue;
        accNum[strcspn(accNum, "\n")] = '\0'; 

        printf("Last 4-digit of ID Number: ");
        if (!fgets(last4Digits, sizeof(last4Digits), stdin)) continue;
        last4Digits[strcspn(last4Digits, "\n")] = '\0';

        printf("Pin: ");
        if (!fgets(pin, sizeof(pin), stdin)) continue;
        pin[strcspn(pin, "\n")] = '\0';

        snprintf(filename, sizeof(filename), "database/%s.txt", accNum);
        printf("%s\n",filename);

        //Validate id last 4 digit and pin 
        if(check_last4Digits(filename,last4Digits) && check_pin(filename,pin)) {
            if (remove(filename) == 0){
                printf("ACCOUNT DELETED\n\n");
                break;
            } else {
                printf("COULDN'T DELETE\n\n");
            }
        } else {
            printf("WRONG INFO\n");
            continue;
        }
    }
}

bool check_digitNumber(float amount){
    int amount_scaled = amount * 100;
    float diff = fabs(amount_scaled - roundf(amount_scaled));

    if (diff < 0.0001f){
        return true;
    } else {
        return false;
    }
}

void deposit(){
    char filename[30];
    char content[256];
    int lineNum = 0;
    int targetLine = 6;

    char accNum[8];
    char pin[6];
    char inputAmount[6];
    float amountToDeposit = 0.00;

    char *endptr;

    struct variables variable;

    printf("\n----------DEPOSIT MONEY-----------\n");

    while(true){
        printf("Bank Account Number: ");
        fgets(accNum, sizeof(accNum), stdin);
        accNum[strcspn(accNum, "\n")] = '\0';

        sprintf(filename,"database/%s.txt",accNum);

        FILE *fp;
        fp = fopen(filename, "r");

        if (fp == NULL){
            printf("NO CURRENT ACCOUNT. TRY AGAIN\n");
            continue;
        } else {
            //Get pin from user
            printf("Pin: ");
            fgets(pin, sizeof(pin), stdin);
            pin[strcspn(pin, "\n")] = '\0';

            //Check if pin is valid
            if(check_pin(filename,pin)){
                while (fgets(content,sizeof(content),fp)){
                    lineNum++;
                    if (lineNum == targetLine){
                        variable.amount = strtof(content,NULL);
                        break;
                    }
                }

                printf("Current Amount (RM): %.2f\n",variable.amount);
                while(true){
                    printf("Enter Amount To Deposit (RM): ");
                    fgets(inputAmount, sizeof(inputAmount), stdin);
                    inputAmount[strcspn(inputAmount, "\n")] = '\0';

                    amountToDeposit = strtof(inputAmount,&endptr);

                    if (endptr == inputAmount || *endptr != '\0') {
                        
                    }
                    if (amountToDeposit < 0 || amountToDeposit > 50000){
                        printf("EXCEEDED AMOUNT. ONLY RM 0 - RM50,000 IS ALLOWED PER OPERATION\n");
                        continue;
                    } else if (!check_digitNumber(amountToDeposit)){
                        printf("INVALID FORMAT. ONLY XX.XX IS ACCEPTED\n");
                        continue;
                    } else {
                        variable.amount += amountToDeposit;

                        FILE *fp2 = fopen(filename, "r");
                        if (fp2 == NULL) {
                            printf("Error. Unable to open file for editing\n");
                            break;
                        }
                        
                        char lines[10][256]; //An array of 10 strings with 256 len
                        int count = 0;

                        //Write line contents(txt file) to content
                        while(fgets(lines[count], sizeof(lines[count]), fp2)) {
                            count++;
                        }
                        fclose(fp2);

                        //Overwrite content on line 5(Money Amount)
                        sprintf(lines[5], "%.2f\n", variable.amount);
                        fp2 = fopen(filename, "w");
                        if (fp2 == NULL) {
                            printf("Error. Unable to open file for writing\n");
                            break;
                        }
                        for(int i=0; i<count; i++){
                            fputs(lines[i], fp2);
                        }
                        fclose(fp2);

                        printf("DEPOSIT SUCCESSFUL!\n");
                        return;
                    }
                }
            } else {
                printf("WRONG PIN. TRY AGAIN\n");
                continue;
            }
        }
        fclose(fp);
    }
}

void withdrawal(){
    char filename[30];
    char content[256];
    int lineNum = 0;
    int targetLine = 6;

    char accNum[8];
    char pin[6];
    char inputAmount[6];
    float amountToWithdraw = 0.00;

    struct variables variable;

    printf("\n----------DEPOSIT MONEY-----------\n");

    while(true){
        printf("Bank Account Number: ");
        fgets(accNum, sizeof(accNum), stdin);
        accNum[strcspn(accNum, "\n")] = '\0';

        sprintf(filename,"database/%s.txt",accNum);

        FILE *fp;
        fp = fopen(filename, "r");

        if (fp == NULL){
            printf("NO CURRENT ACCOUNT. TRY AGAIN\n");
            continue;
        } else {
            //Get pin from user
            printf("Pin: ");
            fgets(pin, sizeof(pin), stdin);
            pin[strcspn(pin, "\n")] = '\0';

            //Check if pin is valid
            if(check_pin(filename,pin)){
                while (fgets(content,sizeof(content),fp)){
                    lineNum++;
                    if (lineNum == targetLine){
                        variable.amount = strtof(content,NULL);
                        break;
                    }
                }

                while(true){
                    printf("Current Amount (RM): %.3f\n",variable.amount);
                    printf("Enter Amount To Withdraw (RM): ");
                    fgets(inputAmount, sizeof(inputAmount), stdin);
                    inputAmount[strcspn(inputAmount, "\n")] = '\0';
                    printf("inputAmount: %s\n",inputAmount);
                    amountToWithdraw = strtof(inputAmount,NULL);
                    printf("%f\n",amountToWithdraw);

                    
                    variable.amount -= amountToWithdraw;

                    FILE *fp2 = fopen(filename, "r");
                    if (fp2 == NULL) {
                        printf("Error. Unable to open file for editing\n");
                        break;
                    }
                    
                    char lines[10][256]; //An array of 10 strings with 256 len
                    int count = 0;

                    //Write line contents(txt file) to content
                    while(fgets(lines[count], sizeof(lines[count]), fp2)) {
                        count++;
                    }
                    fclose(fp2);

                    //Overwrite content on line 5(Money Amount)
                    sprintf(lines[5], "%.2f\n", variable.amount);
                    fp2 = fopen(filename, "w");
                    if (fp2 == NULL) {
                        printf("Error. Unable to open file for writing\n");
                        break;
                    }
                    for(int i=0; i<count; i++){
                        fputs(lines[i], fp2);
                    }
                    fclose(fp2);

                    printf("WITHDRAW SUCCESSFUL!\n");
                    return;
                }
            } else {
                printf("WRONG PIN. TRY AGAIN\n");
                continue;
            }
        }
        fclose(fp);
    }  
}

void remittance(){
    char sender_accNum[8];
    char sender_pin[5];
    char receive_accNum[8];
    char filename[20];

    float amount;

    printf("Sender's Bank Account Number: ");
    fgets(sender_accNum, sizeof(sender_accNum), stdin);
    sender_accNum[strcspn(sender_accNum, "\n")] = '\0';

    sprintf(filename,sizeof(filename),"database/%s.txt", accNum);

    printf("Pin: ");
    fgets(sender_pin, sizeof(sender_pin), stdin);
    sender_pin[strcspn(sender_pin, "\n")] = '\0';

    printf("Receiver's Bank Account Number: ");
    fgets(receive_accNum, sizeof(receive_accNum), stdin);
    receive_accNum[strcspn(receive_accNum, "\n")] = '\0';

    
}

int main(){
    FILE *fp;
    srand(time(NULL));
    struct variables variable;

    char *actions[6];
    char input[20];
    int option = 0;
    char *end;
    bool keepGoing = true;

    start_menu();
    printf("Please type the number of actions(1-6) to proceed: ");
    
    //Body - Main Part
    while (keepGoing){
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        option = strtol(input, &end, 10);

        if (*end == '\0'){
            switch(option){
                case 1: 
                    create_new_bank_acc();

                    printf("\n----------BANK ACCOUNT CREATED----------\n\n");
                    break;
                case 2:
                    delete_bank_acc();
                    break;
                case 3: 
                    deposit();
                    break;
                case 4:
                    withdrawal();
                    break;
                case 5:
                    remittance();
                    break;
                case 6:
                    printf("\n----------------------------------------\n");
                    printf("Program Exited");
                    return 0;
                default: 
                    printf("Error! Please retype\n");
                    break;
            }
        } else {
            //Change the string input to lowercase
            for(int i=0;input[i];i++){
                input[i] = tolower(input[i]);
            }

            if (strcmp(input, "create") == 0){
                create_new_bank_acc();
            } else if(strcmp(input,"exit") == 0){
                return 0;
            }
        }

        start_menu();
        printf("Please type the number of actions(1-6) to proceed: ");
        int ch;
        while((ch=getchar()) != '\n' && ch != EOF);

    }
}