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

float global_amount;

struct variables {
    //Details - Create New Bank Acc
    char name[256];
    char id[256];
    char acc_type[256];
    char pin[256];

    //File Management
    char filename[7];
};

void printError(int errorID) {
    switch(errorID) {
        // ===== CREATE =====
        case 101:
            printf("Error: Unable to open file\n");
            break;
        case 102:
            printf("Error: Name exceeds 255 characters. Please enter a valid name\n");
            break;
        case 103:
            printf("Error: Invalid name format. Only alphabets and '-' are allowed\n");
            break;
        case 104:
            printf("Error: ID number exceeds 12 characters. Please enter a valid ID\n");
            break;
        case 105:
            printf("Error: Invalid ID number format. Only numeric characters are allowed\n");
            break;
        case 106:
            printf("Error: Account type input exceeds maximum allowed length\n");
            break;
        case 107:
            printf("Error: Invalid account type format\n");
            break;
        case 108:
            printf("Error: PIN must be exactly 4 digits\n");
            break;
        case 109:
            printf("Error: Invalid PIN format. Only numeric characters are allowed\n");
            break;
        case 110:
            printf("Error: ID number below 5 characters. Please enter a valid ID\n");
            break;

        // ===== DELETE =====
        case 201:
            printf("Error: Unable to access directory\n");
            break;
        case 202:
            printf("Error: Account number must be 6 digits. Please enter a valid account number\n");
            break;
        case 203:
            printf("Error: Invalid account number format. Only numeric characters are allowed\n");
            break;
        case 204:
            printf("Error: Incorrect last 4 digit of id number. Please try again\n");
            break;
        case 205:
            printf("Error: Incorrect pin. Please try again\n");
            break;
        case 206:
            printf("Error: Unable to remove account\n");
            printf("Leaving action...\n\n");
            break;
        case 207:
            printf("Error: No matching bank account was found. Please enter a valid account number\n");
            break;

        // ===== DEPOSIT & WITHDRAW =====
        case 301:
            printf("Error: Account not found. Please try again\n");
            break;
        case 302:
            printf("Error: Amount exceeds allowed range. Only RM 0 - RM 50,000 per operation\n");
            break;
        case 303:
            printf("Error: Incorrect PIN. Please try again\n");
            break;
        case 304:
            printf("Error: Invalid format. Only XXXXX.XX is accepted\n");
            break;
        case 305:
            printf("Error: Insufficient funds. The withdraw amount exceeds you available balance\n");
            break;

        // ===== REMITTANCE =====
        case 401:
            printf("Error: Invalid input. Account file not found\n");
            break;
        case 402:
            printf("Error: Invalid PIN\n");
            break;
        case 403:
            printf("Error: Insufficient funds. The remittance amount exceeds your available balance\n");
            break;
        case 404:
            printf("Error: Invalid amount format. Only XXXXX.XX is accepted\n");
            break;
        case 405:
            printf("Error: Invalid input. Please select a valid option (accept[1]/reject[0])\n");
            break;
        case 406:
            printf("Transaction rejected. Please re-enter\n\n");
            break;

        // ===== START MENU =====
        case 501:
            printf("Error: Invalid selection. Please enter a number between 1 and 6\n");
            break;

        default:
            break;
    }
}

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
    global_amount = 0;

    for(int i=0;i<6;i++){
        int rand_num = rand() % 9 + 1;
        char digit[2];
        sprintf(digit,"%d", rand_num);
        strcat(acc_num, digit);
    }

    sprintf(variable.filename,"database/%s.txt",acc_num);
    fp = fopen(variable.filename,"w");

    if (fp == NULL) {
        printError(101); //File cannot be opened
    }

    fprintf(fp,"%s\n",acc_num);
    fprintf(fp,"%s\n",name);
    fprintf(fp,"%s\n",id);
    fprintf(fp,"%s\n",acc_type);
    fprintf(fp,"%s\n",pin);
    fprintf(fp,"%.2f\n",global_amount);
    fclose(fp);
}

bool onlyAplhabets(const char *str){
    //Reject empty string
    if (*str == '\0'){
        return false;
    }

    while((*str != '\0' && *str != '\n')){
        //Accepts "-" in name
        if(*str == '-'){
            str++;
            continue;
        }

        //Check if it's not alphabet
        if (!isalpha((unsigned char)*str)){
            return false;
        }

        str++;
    }

    //Return true if everything is alphabet and contains "-"
    return true;
}

bool onlyDigits(const char *str){
    //Reject empty string
    if (*str == '\0'){
        return false;
    }

    //Return false if any chracters is not digit
    while (*str != '\0'){
        if (!isdigit((unsigned char)*str)){
            return false;
        }
        str++;
    }

    return true;
}

void toLowerCase(char *str){
    while(*str){
        *str = tolower((unsigned char)*str);
        str++;
    }
}

void create_new_bank_acc(){
    struct variables variable;
    int numPin = 0;
    char loweredAccType[20];
    printf("\n--------CREATE NEW BANK ACCOUNT---------\n");
    printf("Please insert your personal details below\n");

    while (true){
        printf("Name: ");
        fgets(variable.name, sizeof(variable.name), stdin);
        variable.name[strcspn(variable.name, "\n")] = '\0';

        int size_name = strlen(variable.name);
        if (size_name > 255){
            printError(102); //Name exceeds 255 characters
            continue;
        }

        if (onlyAplhabets(variable.name)) {
            break;
        } else {
            printError(103); //Invalid name format
            continue;
        }
    }

    while (true){
        printf("Identification Number: ");
        fgets(variable.id, sizeof(variable.id), stdin);
        variable.id[strcspn(variable.id, "\n")] = '\0';

        int size_id = strlen(variable.id);
        if (size_id > 12){
            printError(104); //ID number exceeds 12 characters
            continue;
        } else if (size_id < 5) {
            printError(110); //ID number below 5 characters
            continue;
        }

        if(onlyDigits(variable.id)){
            break;
        } else {
            printError(105); //Invalid ID format
            continue;
        }
    }
    
    while(true){
        printf("Type of Account(Savings[1] OR Current[2]): ");
        fgets(variable.acc_type, sizeof(variable.acc_type), stdin);
        variable.acc_type[strcspn(variable.acc_type, "\n")] = '\0';
        
        int size_accType = strlen(variable.acc_type);
        if (size_accType > 8){
            printError(106); //Account type input exceed maximum allowed length
            continue;
        }

        if(onlyAplhabets(variable.acc_type)){
            strcpy(loweredAccType,variable.acc_type);
            toLowerCase(loweredAccType);
            if (strcmp(loweredAccType,"savings") == 0 || strcmp(loweredAccType,"current") == 0){
                break;
            } else {
                printError(107); //Invalid account type format
                continue;
            }
        } else {
            if (strcmp(variable.acc_type,"1") == 0){
                strcpy(variable.acc_type,"Savings");
                break;
            } else if(strcmp(variable.acc_type,"2") == 0){
                strcpy(variable.acc_type,"Current");
                break;
            } else {
                printError(107); //Invalid account type format
                continue;
            }
        }
    }
    
    while(true){
        printf("4 Digit Pin: ");
        fgets(variable.pin, sizeof(variable.pin), stdin);
        variable.pin[strcspn(variable.pin, "\n")] = '\0';

        numPin = strlen(variable.pin);
        if (numPin != 4) {
            printError(108); //Exceed 4 digit
            continue;
        } else {
            if(!onlyDigits(variable.pin)){
                printError(109); //Invalid pin format
                continue;
            } else {
                break;
            }
        }
    }
    
    create_txt_file(variable.name,variable.id,variable.acc_type,variable.pin);

    printf("\n----------BANK ACCOUNT CREATED----------\n\n");
}

bool check_accNum(char filename[]){
    FILE *fp = fopen(filename,"r");
    if (fp == NULL){
        return false;
    } else {
        fclose(fp);
        return true;
    }
}

bool check_last4Digits(char filename[],char inputLast4[5]){
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("unable to open file\n");
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
    fclose(fp);

    content[strcspn(content, "\n")] = '\0';

    int size = strlen(content);

    // Safety check (line must be >= 4)
    if (size < 4) return false;

    // Extract last 4 digits
    char contentLast4[5];
    strncpy(contentLast4, content + size - 4, 4);
    contentLast4[4] = '\0';   // null termination

    // Compare with input
    return strcmp(contentLast4, inputLast4) == 0;

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
        //Stop at Pin Line (5)
        if (lineNum == targetLine){
            break;
        }
    }
    fclose(fp);

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
    char accNum[256];
    char pin[256];
    char last4Digits[256];
    char filename[30];

    bool last4Correct = false;
    bool pinCorrect = false;
    printf("\n----------DELETE BANK ACCOUNT-----------\n");
    d = opendir("database"); 
    if (d == NULL) {
        printError(201); //Unable to access directory
    }

    //List current bank accounts
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;

        printf("%s\n", dir->d_name);
    }
    closedir(d);

    printf("\n");

    while(true){
        printf("Bank Account Number: ");
        if (!fgets(accNum, sizeof(accNum), stdin)) continue;
        accNum[strcspn(accNum, "\n")] = '\0';
        
        int sizeAccNum = strlen(accNum);
        if (sizeAccNum != 6){
            printError(202); //Acc num not 6 digit number
            continue;
        }

        if(onlyDigits(accNum)){
            sprintf(filename, "database/%s.txt", accNum);

            if (check_accNum(filename)){
                break;
            } else {
                printError(207); //No matching bank account number
                continue;
            }
        } else {
            printError(203); //Invalid account number format
        }
    }

    while(true){
        printf("Last 4-digit of ID Number: ");
        if (!fgets(last4Digits, sizeof(last4Digits), stdin)) continue;
        last4Digits[strcspn(last4Digits, "\n")] = '\0';

        if(check_last4Digits(filename,last4Digits)){
            last4Correct = true;
            break;
        } else {
            printError(204); //Invalid last 4 digit
            continue;
        }
    }

    while(true){
        printf("Pin: ");
        if (!fgets(pin, sizeof(pin), stdin)) continue;
        pin[strcspn(pin, "\n")] = '\0';

        if (check_pin(filename,pin)){
            pinCorrect = true;
            break;
        } else {
            printError(205); //Incorrect pin
            continue;
        }
    }

    if (last4Correct && pinCorrect){
        if (remove(filename) == 0){
            printf("----------ACCOUNT DEELETED----------\n\n");
        } else if (remove(filename) != 0) {
            perror("Remove failed");
        }
    } else {
        printError(206);
    }
}

bool check_digitNumber(char amount[]){
    //123.567
    int len = strlen(amount);
    int digitNumber;
    bool hasDecimal = false;

    for(int i=0;i<len;i++){
        if (amount[i] == '.'){
            hasDecimal = true;
            int dotNum = i + 1;
            digitNumber = len - dotNum;
            break;
        }
    }

    if(!hasDecimal){
        return true;
    }

    if (digitNumber > 2){
        return false;
    } else {
        return true;
    }
}

bool check_amountFormat(char strAmount[]){
    char *endptr;
    strtof(strAmount,&endptr);

    if ((endptr == strAmount || *endptr != '\0')) {
        return false;
    } else {
        return true;
    }
}

void getAmount(char filename[]){
    struct variables variable;
    FILE *fp = fopen(filename,"r");

    char content[256];

    int lineNum = 0;
    int targetLine = 6;
    while (fgets(content,sizeof(content),fp)){
        lineNum++;
        if (lineNum == targetLine){
            global_amount = strtof(content,NULL);
            break;
        }
    }
    fclose(fp);
}

void updateAmount(char filename[], float newAmount){
    char lines[10][256]; //An array of 10 strings with 256 len
    int count = 0;

    FILE *fp = fopen(filename,"r");
    if (fp == NULL){
        return;
    }

    //Write line contents(txt file) to content
    while(fgets(lines[count], sizeof(lines[count]), fp)) {
        count++;
    }
    fclose(fp);

    //Overwrite content on line 5(Money Amount)
    sprintf(lines[5], "%.2f\n", newAmount);

    fp = fopen(filename,"w");
    if (fp == NULL){
        return;
    }
    for(int i=0; i<count; i++){
        fputs(lines[i], fp);
    }
    fclose(fp);
}

void deposit(){
    char filename[30];
    char content[256];

    char accNum[8];
    char pin[6];
    char inputAmount[20];
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
            printError(301); //Account not found
            continue;
        } else {
            //Get pin from user
            printf("Pin: ");
            fgets(pin, sizeof(pin), stdin);
            pin[strcspn(pin, "\n")] = '\0'; 

            //Check if pin is valid
            if(check_pin(filename,pin)){
                printf("LOG IN SUCCESFUL\n");
                getAmount(filename);
                
                fclose(fp);
                while(true){
                    printf("Current Amount: RM %.2f\n",global_amount);
                    printf("Enter Amount To Deposit: RM ");
                    fgets(inputAmount, sizeof(inputAmount), stdin);
                    inputAmount[strcspn(inputAmount, "\n")] = '\0';

                    amountToDeposit = strtof(inputAmount,&endptr);

                    if (check_amountFormat(inputAmount) && check_digitNumber(inputAmount)){
                        if (amountToDeposit < 0 || amountToDeposit > 50000){
                            printError(302); //Amount exceed allowed range
                            continue;
                        } else {
                            global_amount += amountToDeposit;
                            updateAmount(filename,global_amount);
                            
                            printf("----------DEPOSIT SUCCESSFUL----------\n");
                            getAmount(filename);
                            printf("Updated Amount: RM %.2f\n\n", global_amount);
                            return;
                        }
                    } else {
                        printError(304); //Invalid format.
                        continue;
                    }
                }
            } else {
                printError(303); //Incorrect Pin
                continue;
            }
        }
    }
}

void withdraw(){
    char filename[30];
    char content[256];
    int lineNum = 0;
    int targetLine = 6;

    char accNum[8];
    char pin[6];
    char inputAmount[20];
    float amountToWithdraw = 0.00;

    struct variables variable;

    printf("\n----------WITHDRAW MONEY-----------\n");

    while(true){
        printf("Bank Account Number: ");
        fgets(accNum, sizeof(accNum), stdin);
        accNum[strcspn(accNum, "\n")] = '\0';

        sprintf(filename,"database/%s.txt",accNum);

        FILE *fp;
        fp = fopen(filename, "r");

        if (fp == NULL){
            printError(301); //Account not found
            continue;
        } else {
            //Get pin from user
            printf("Pin: ");
            fgets(pin, sizeof(pin), stdin);
            pin[strcspn(pin, "\n")] = '\0';

            //Check if pin is valid
            if(check_pin(filename,pin)){
                printf("LOG IN SUCCESFUL\n");
                getAmount(filename);
                
                fclose(fp);
                while(true){
                    printf("Current Amount: RM %.2f\n",global_amount);
                    printf("Enter Amount To Withdraw: RM ");
                    fgets(inputAmount, sizeof(inputAmount), stdin);
                    inputAmount[strcspn(inputAmount, "\n")] = '\0';
                    printf("Amount to withdraw: RM %s\n",inputAmount);
                    amountToWithdraw = strtof(inputAmount,NULL);

                    if (check_amountFormat(inputAmount) && check_digitNumber(inputAmount)){
                        if(amountToWithdraw < 0){
                            printError(302); //Amount exceed allowed range
                            continue;
                        } 
                        
                        if (amountToWithdraw > global_amount){
                            printError(305); //Withdraw amount exceed available balance
                            continue;
                        }
                        
                        global_amount -= amountToWithdraw;
                        updateAmount(filename,global_amount);

                        printf("----------WITHDRAW SUCCESSFUL----------\n");
                        getAmount(filename);
                        printf("Updated Amount: RM %.2f\n\n", global_amount);
                        return;
                    } else {
                        printError(304); //Invalid format.
                        continue;
                    }
                }
            } else {
                printError(303); //Incorrect Pin
                continue;
            }
        }
        fclose(fp);
    }
}

int check_accType(char send_filename[], char rec_filename[]){
    FILE *send_fp = fopen(send_filename,"r");
    FILE *rec_fp = fopen(rec_filename, "r");
    if (send_fp == NULL || rec_fp == NULL){
        return 0;
    }

    int targetLine = 4;

    char send_content[256];
    int s_lineNum = 0;
    
    char rec_content[256];
    int r_lineNum = 0;

    char strSavings[8] = "Savings";
    char strCurrent[8] = "Current";

    //Read sender's account type
    while (fgets(send_content, sizeof(send_content), send_fp)){
        s_lineNum++;
        //Stop at Account Type Line (4)
        if (s_lineNum == targetLine){
            break;
        }
    }

    int sizeScontent = strlen(send_content);
    if (sizeScontent > 0){
        send_content[sizeScontent - 1] = '\0';
    }

    while (fgets(rec_content, sizeof(rec_content), rec_fp)){
        r_lineNum++;
        //Stop at Account Type Line (4)
        if (r_lineNum == targetLine){
            break;
        }
    }

    int sizeRcontent = strlen(rec_content);
    if (sizeRcontent > 0){
        rec_content[sizeRcontent - 1] = '\0';
    }

    if(strcmp(send_content,strSavings) == 0 && strcmp(rec_content,strCurrent) == 0){
        return 1;
    } else if(strcmp(send_content,strCurrent) == 0 && strcmp(rec_content,strSavings) == 0){
        return 2;
    } else if (strcmp(send_content,rec_content) == 0){
        return 3;
    } else {
        return 0;
    }
}

void remittance(){
    struct variables variable;

    char sender_accNum[8];
    char sender_pin[6];
    char sender_filename[20];
    char receiver_accNum[8];
    char receiver_filename[20];

    char inputAmount[20];
    float amountToRemit = 0.00;
    char *endptr;

    int remitStatus;
    float savToCur = 0.02;
    float curToSav = 0.03;
    float remitFee;

    char remitAccept[10];

    printf("\n----------REMITTANCE-----------\n");
    printf("REMITTANCE FEE:\n");
    printf("Savings to Current: 2%%\n");
    printf("Current to Savings: 3%%\n\n");

    //Check if sender's bank account number is valid
    while (true){
        printf("Sender's Bank Account Number: ");
        fgets(sender_accNum, sizeof(sender_accNum), stdin);
        sender_accNum[strcspn(sender_accNum, "\n")] = '\0';

        sprintf(sender_filename,"database/%s.txt", sender_accNum);

        if (check_accNum(sender_filename)){
            break;
        } else {
            printError(401); //Account not found
            continue;
        }
    }
    
    //Check if pin is valid
    while(true) {
        printf("Pin: ");
        fgets(sender_pin, sizeof(sender_pin), stdin);
        sender_pin[strcspn(sender_pin, "\n")] = '\0';

        if(check_pin(sender_filename,sender_pin)){
            break;
        } else {
            printError(402); //Invalid PIN
            continue;
        }
    }

    //Check if receiver's bank account number is valid
    while (true){
        printf("Receiver's Bank Account Number: ");
        fgets(receiver_accNum, sizeof(receiver_accNum), stdin);
        receiver_accNum[strcspn(receiver_accNum, "\n")] = '\0';

        sprintf(receiver_filename,"database/%s.txt",receiver_accNum);

        if(check_accNum(receiver_filename)){
            break;
        } else {
            printError(401); //Account not found
            continue;
        }
    }

    printf("CREDENTIAL VALID. PLEASE PROCEED\n");

    //Store sender and receiver current amount
    getAmount(receiver_filename);
    float receiver_amount = global_amount;
    getAmount(sender_filename);
    float sender_amount = global_amount;
    printf("Current Amount(RM): %.2f\n", sender_amount);

    //Check if amount to be transferred is valid
    while(true){
        printf("Amount to be transferred(RM): ");
        fgets(inputAmount, sizeof(inputAmount),stdin);
        inputAmount[strcspn(inputAmount, "\n")] = '\0';

        amountToRemit = strtof(inputAmount, &endptr);

        if(check_amountFormat(inputAmount) && check_digitNumber(inputAmount)){
            check_digitNumber(inputAmount);
            remitStatus = check_accType(sender_filename,receiver_filename);
            getAmount(sender_filename);
            if (global_amount < amountToRemit){
                printError(403);
                continue;
            }
            if(remitStatus == 1){ //Savings to Current
                remitFee = amountToRemit * savToCur;
                printf("Remittance Fee(RM): %.2f\n", remitFee);
                amountToRemit -= remitFee;
                printf("Transferring Amount(RM): %.2f\n\n", amountToRemit);
                printf("Please confirm the transaction.\n");
                printf("Enter 1 to accept or 0 to reject: ");
                fgets(remitAccept, sizeof(remitAccept), stdin);
                remitAccept[strcspn(remitAccept, "\n")] = '\0';
                if (onlyDigits(remitAccept)){ //If user input 0 or 1
                    if(strcmp(remitAccept, "0") == 0){
                        printError(406); //Transaction rejected.
                        continue;
                    } else if (strcmp(remitAccept,"1") == 0){
                        sender_amount -= amountToRemit;
                        receiver_amount += amountToRemit;
                        
                        updateAmount(sender_filename,sender_amount);
                        updateAmount(receiver_filename,receiver_amount);

                        printf("----------REMIT SUCCESSFUL----------\n\n");
                        break;
                    } else {
                        printError(405); //Invalid accept/reject input
                        continue;
                    }
                } else if (onlyAplhabets(remitAccept)){
                    toLowerCase(remitAccept);
                    if (strcmp(remitAccept,"reject") == 0) {
                        printError(406); //Transaction rejected.
                        continue;
                    } else if (strcmp(remitAccept,"accept") == 0){
                        sender_amount -= amountToRemit;
                        receiver_amount += amountToRemit;
                        
                        updateAmount(sender_filename,sender_amount);
                        updateAmount(receiver_filename,receiver_amount);

                        printf("----------REMIT SUCCESSFUL----------\n\n");
                        break;
                    } else {
                        printError(405); //Invalid accept/reject input
                        continue;
                    }
                } else {
                    printError(405); //Invalid accept/reject input 
                }
            } else if (remitStatus == 2){ //Current to Savings
                remitFee = amountToRemit * curToSav;
                printf("Remittance Fee(RM): %.2f\n", remitFee);
                amountToRemit -= remitFee;
                printf("Transferring Amount(RM): %.2f\n", amountToRemit);
                printf("Enter 0 to Reject, 1 to Accept: ");
                fgets(remitAccept, sizeof(remitAccept), stdin);
                remitAccept[strcspn(remitAccept, "\n")] = '\0';
                printf("%s\n", remitAccept);
                if (onlyDigits(remitAccept)){ //If user input 0 or 1
                    if(strcmp(remitAccept, "0") == 0){
                        printError(406); //Transaction rejected.
                        continue;
                    } else if (strcmp(remitAccept,"1") == 0){
                        sender_amount -= amountToRemit;
                        receiver_amount += amountToRemit;
                        
                        updateAmount(sender_filename,sender_amount);
                        updateAmount(receiver_filename,receiver_amount);

                        printf("----------REMIT SUCCESSFUL----------\n\n");
                        break;
                    } else {
                        printError(405); //Invalid accept/reject input
                        continue;
                    }
                } else if (onlyAplhabets(remitAccept)){
                    toLowerCase(remitAccept);
                    if (strcmp(remitAccept,"reject") == 0) {
                        printError(406); //Transaction rejected.
                        continue;
                    } else if (strcmp(remitAccept,"accept") == 0){
                        sender_amount -= amountToRemit;
                        receiver_amount += amountToRemit;
                        
                        updateAmount(sender_filename,sender_amount);
                        updateAmount(receiver_filename,receiver_amount);

                        printf("----------REMIT SUCCESSFUL----------\n\n");
                        break;
                    } else {
                        printError(405); //Invalid accept/reject input
                        continue;
                    }
                } else {
                    printError(405); //Invalid accept/reject input 
                }
            } else if (remitStatus == 3){
                break;
            } else if (remitStatus == 0){
                printf("ERROR\n");
                continue;
            }
        } else {
            printf("INVALID FORMAT. ONLY XX.XX IS ACCEPTED\n");
            continue;
        }
    }    
}

int main(){
    FILE *fp;
    srand(time(NULL));
    struct variables variable;

    char *actions[6];
    char input[256];
    int option = 0;
    char *end;
    
    //Body - Main Part
    while (true){
        int count = 1;
        start_menu();
        printf("Please type the number of actions(1-6) to proceed: ");

        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';
        option = strtol(input, &end, 10);

        if (*end == '\0'){
            switch(option){
                case 1: 
                    create_new_bank_acc();
                    break;
                case 2:
                    delete_bank_acc();
                    break;
                case 3: 
                    deposit();
                    break;
                case 4:
                    withdraw();
                    break;
                case 5:
                    remittance();
                    break;
                case 6:
                    printf("\n----------------------------------------\n");
                    printf("Program Exited");
                    return 0;
                default: 
                    printError(501); //Invalid selection
                    break;
            }
        } else {
            toLowerCase(input);
            int size = strlen(input);
            int i = 0;
            while(input[i] != '\0'){
                for(i=0;i<size;i++){
                    if (input[i] == ' ' && input[i+1] == '\0'){
                        input[i] = '\0';
                    }
                }
                i++;
            }

            if (strcmp(input, "create") == 0){
                create_new_bank_acc();
            } else if (strcmp(input, "delete") == 0 || strcmp(input,"delete bank account") == 0){
                delete_bank_acc();
            } else if (strcmp(input, "deposit") == 0 || strcmp(input, "deposit money") == 0){
                deposit();
            } else if (strcmp(input, "withdraw") == 0 || strcmp(input, "withdraw money") == 0){
                withdraw();
            } else if (strcmp(input, "remit") == 0 || strcmp(input,"remittance") == 0){
                remittance();
            } else if(strcmp(input,"exit") == 0){
                printf("\n----------CODE EXITED----------\n");
                return 0;
            } else {
                printError(501); //Invalid selection
            }
        }
    }
}