//GitHub Repo Link:
//origin  https://github.com/pasadin-dot/Bank-Application-System.git

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <direct.h>
#include <time.h>

//FUNCTION DECLARATIONS
void printError(int errorID);
void printHeader(int headerID);
void start_menu();
void create_txt_file(char name[], char id[], char acc_type[], char pin[]);
void toLowerCase(char *str);
void getAmount(char filename[]);
void updateAmount(char filename[], float newAmount);
void getContent(char filename[], int targetLine);
void createTransactionLog();
void updateTransactionLog(int action);
bool checkNameFormat(const char *str);
bool onlyDigits(const char *str);
bool check_input(char *input, int size);
bool onlyAlphabets(const char *str);
bool check_digitNumber(char amount[]);
bool check_amountFormat(char strAmount[]);
bool check_accNum(char filename[]);
bool check_last4Digits(char filename[], char inputLast4[5]);
bool check_pin(char filename[], char inputPin[]);
bool isDirectoryEmpty(const char *path);
bool DBDirectoryExists();
bool logFileExists();
int check_accType(char send_filename[], char rec_filename[]);
void create_new_bank_acc();
void check_bank_acc();
void delete_bank_acc();
void deposit();
void withdraw();
void remittance();

//GLOBAL VARIABLES
float global_amount;
char backKeyWord[5] = "back";
char global_content[256];
char db_path[11] = "database";
char setupAccept[3];
char makeLogAccept[3];
char global_accNum[7];
char global_senderAccNum[7];
char global_recipientAccNum[7];
char global_oldBal[10];
char global_newBal[10];
char global_strAmount[10];

//STRUCT VARIABLES
struct variables {
    //Details - Create New Bank Acc
    char name[256];
    char id[14];
    char acc_type[8];
    char pin[6];

    //File Management
    char filename[20];

    //Delete, Deposit, Withdraw
    char accNum[8];
    char last4Digit[6];

    //Remittance
    char sender_accNum[8];
    char sender_pin[6];
    char sender_filename[20];
    char recipient_accNum[8];
    char recipient_filename[20];
    float sender_amount;
    float recipient_amount;

    //transaction.log
    char amount[10];
    char oldBal[10];
    char newBal[10];
};

// ==================== VOID FUNCTIONS ====================
void printError(int errorID) {
    printf("-----------------------------------------------------\n");
    switch (errorID) {
        // FILE / SYSTEM ERRORS
        case 101:
            printf("Error: Unable to open file\n\n");
            break;
        case 102:
            printf("Error: Unable to access directory\n\n");
            break;
        case 103:
            printf("Error: Unable to remove account. Please try again\n");
            printf("Leaving action...\n\n");
            break;
        case 104:
            printf("Error: Invalid input. Account not found\n\n");
            break;

        // INPUT LENGTH ERRORS
        case 201:
            printf("Error: Input exceeds 255 characters");
            break;
        case 202:
            printf("Error: ID number requires 12 numbers. Please enter a valid ID\n\n");
            break;
        case 203:
            printf("Error: Account type input exceeds maximum allowed length.Please enter 'Savings', 'Current', or the corresponding number.\n\n");
            break;
        case 204:
            printf("Error: ID number below 5 characters. Please enter a valid ID\n\n");
            break;
        case 205:
            printf("Error: Account number must be 6 digits. Please enter a valid account number\n\n");
            break;
        case 206:
            printf("Error: Only 4 digits are allowed\n\n");
            break;

        // INPUT FORMAT ERRORS
        case 301:
            printf("Error: Invalid name format. Only alphabets, dash(-), period(.) and slash(/) are allowed\n\n");
            break;
        case 302:
            printf("Error: Invalid ID number format. Only numeric characters are allowed\n\n");
            break;
        case 303:
            printf("Error: Invalid account type format. Please enter 'Savings', 'Current', or the corresponding number.\n\n");
            break;
        case 304:
            printf("Error: Invalid PIN format. Only numeric characters are allowed\n\n");
            break;
        case 305:
            printf("Error: Invalid account number format. Only numeric characters are allowed\n\n");
            break;
        case 306:
            printf("Error: Invalid format. Only float numbers in XXXXX.XX format is accepted\n\n");
            break;
        case 307:
            printf("Error: Invalid selection. Please enter a number between 1 and 6\n\n");
            break;
        case 308:
            printf("Error: Invalid input. Please select a valid option (accept[1]/reject[0])\n\n");
            break;
        case 309:
            printf("Error: Invalid input. Please select a valid option (setup[1]/exit[0])\n\n");
            break;

        // AUTH / VERIFICATION ERRORS
        case 401:
            printf("Error: Incorrect last 4 digit of id number. Please try again\n\n");
            break;
        case 402:
            printf("Error: Incorrect PIN. Please try again\n\n");
            break;

        // TRANSACTION LOGIC ERRORS
        case 501:
            printf("Error: No matching bank account was found. Please enter a valid account number\n\n");
            break;
        case 502:
            printf("Error: Amount exceeds allowed range. Only RM 0 - RM 50,000 per operation\n\n");
            break;
        case 503:
            printf("Error: Insufficient funds. The withdraw amount exceeds your available balance\n\n");
            break;
        case 504:
            printf("Error: Insufficient funds. The remittance amount exceeds your available balance\n\n");
            break;
        case 505:
            printf("Transaction rejected. Please re-enter\n\n");
            break;
        case 506:
            printf("Error: Sender's bank account number cannot be the same as the recipient's account number.\n\n");
            break;
        case 507:
            printf("Error: Amount number must be positive\n\n");
            break;
        case 508:
            printf("Deletion rejected\n");
            break;
        case 509:
            printf("Transaction exited\n");
            break;

        default:
            break;
    }
}

void printHeader(int headerID){
    printf("\n---------------");
    switch(headerID){
        case 11:
            printf("CREATE NEW BANK ACCOUNT");
            break;
        case 12:
            printf("CHECK ACCOUNT");
            break;
        case 13:
            printf("DELETE BANK ACCOUNT");
            break;
        case 14:
            printf("DEPOSIT MONEY");
            break;
        case 15: 
            printf("WITHDRAW MONEY");
            break;
        case 16:
            printf("REMITTANCE");
            break;
    }
    printf("---------------\n");
}

void start_menu(){
    bool invalid = false;

    char *actions[] = {
        "1. Create New Bank Account",
        "2. Check Bank Account",
        "3. Delete Bank Account",
        "4. Deposit Money",
        "5. Withdraw Money",
        "6. Remittance Money",
        "7. Exit"
    };

    //Make 'database' directory if not existed
    if (!DBDirectoryExists() || !logFileExists()){
        printf("\n========================================\n");
        printf("BANKING SYSTEM APPLICATION\n");
        printf("========================================\n");
        while(true){
            //Retrieve consent from user before creating 'database' folder
            printf("A 'database' folder and 'transaction.log' is required in your current path. Enter 1 to setup, 0 to exit: ");
            if(!check_input(setupAccept,sizeof(setupAccept))){ //Prevent users to enter massive amount of input
                printError(309); //Invalid setup input
                continue;
            }

            if (onlyDigits(setupAccept)){
                if (strcmp(setupAccept,"0") == 0){
                    return;
                } else if (strcmp(setupAccept,"1") == 0){
                    if (!DBDirectoryExists()){
                        int make_db = _mkdir(db_path);

                        if (make_db == 0){
                            printf("'database' directory created successfully.\n");
                        } else {
                            perror("_mkdir failed");
                        }
                    }

                    if (!logFileExists()){
                        createTransactionLog();
                        printf("'transaction.log' created successfully.\n");
                    }
                    printf("Program restarted.\n");
                    break;
                } else {
                    invalid = true;
                }
            } else {
                invalid = true;
            }

            if (invalid){
                printError(309); //Invalid setup input
                continue;
            }
        }
    }

    printf("\n========================================\n");
    printf("BANKING SYSTEM APPLICATION\n");
    printf("========================================\n");

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

    //Generate random bank account number
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

    strcpy(global_accNum, acc_num);
    
    updateTransactionLog(1);
}

void toLowerCase(char *str){
    while(*str){
        *str = tolower((unsigned char)*str);
        str++;
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
        content[strcspn(content, "\n")] = '\0';
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

void getContent (char filename[], int targetLine){
    int lineNum = 0;

    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL){
        return;
    }

    while (fgets(global_content,sizeof(global_content),fp)){
        lineNum++;
        if (lineNum == targetLine){
            break;
        }
    }

    global_content[strcspn(global_content, "\n")] = '\0';

    fclose(fp);
}

void createTransactionLog(){
    FILE *fp = fopen("transaction.log", "w");
    if (fp == NULL){
        printf("Unable to create 'transaction.log'\n");
    }

    //print header
    fprintf(fp, "%-25s | %-28s | %-12s | %-12s | %-12s | %-12s | %-12s\n",
            "Timestamp", "Action", "SenderAcc", "ReceiverAcc", "Amount(RM)", "OldBal(RM)", "NewBal(RM)");
    fprintf(fp, "------------------------------------------------------------------------------------------------------------------------------------\n");
    fclose(fp);
}

void updateTransactionLog(int action){
    struct variables variable;
    char *actionList[] = {
        "Create New Bank Account",
        "Check Bank Account",
        "Delete Bank Account",
        "Deposit Money",
        "Withdraw Money",
        "Remittance Money",
        "Exit"
    };
    char actionName[25];

    time_t now = time(NULL);
    char *timestamp = ctime(&now);
    timestamp[strcspn(timestamp, "\n")] = '\0';

    if (!logFileExists()){
        createTransactionLog();
    }

    if (action == 1){
        strcpy(actionName,actionList[0]); //Action = Create New Bank Account
        strcpy(variable.sender_accNum,global_accNum); //Sender Acc Num
        strcpy(variable.recipient_accNum, "NULL"); //Receiver Acc Num
        strcpy(variable.amount, "NULL"); //Amount
        strcpy(variable.oldBal, "0.00"); //OldBal
        strcpy(variable.newBal, "0.00"); //NewBal
    } else if (action == 2){
        strcpy(actionName, actionList[1]); //Action = Check Bank Account
        strcpy(variable.sender_accNum, global_accNum); 
        strcpy(variable.recipient_accNum, "NULL");
        strcpy(variable.amount, "NULL");
        strcpy(variable.oldBal, global_oldBal);
        strcpy(variable.newBal, global_newBal);
    } else if (action == 3){
        strcpy(actionName, actionList[2]); //Action = Delete Bank Account
        strcpy(variable.sender_accNum, global_accNum);
        strcpy(variable.recipient_accNum, "NULL");
        strcpy(variable.amount, "NULL");
        strcpy(variable.oldBal, "NULL");
        strcpy(variable.newBal, "NULL");
    } else if (action == 4){
        strcpy(actionName, actionList[3]); //Action = Deposit Money
        strcpy(variable.sender_accNum, global_accNum);
        strcpy(variable.recipient_accNum, "NULL");
        strcpy(variable.amount, global_strAmount);
        strcpy(variable.oldBal, global_oldBal);
        strcpy(variable.newBal, global_newBal);
    } else if (action == 5){
        strcpy(actionName, actionList[4]); //Action = Withdraw Money
        strcpy(variable.sender_accNum, global_accNum);
        strcpy(variable.recipient_accNum, "NULL");
        strcpy(variable.amount, global_strAmount);
        strcpy(variable.oldBal, global_oldBal);
        strcpy(variable.newBal, global_newBal);
    } else if (action == 6){
        strcpy(actionName, actionList[5]); //Action = Remittance
        strcpy(variable.sender_accNum, global_senderAccNum);
        strcpy(variable.recipient_accNum, global_recipientAccNum);
        strcpy(variable.amount, global_strAmount);
        strcpy(variable.oldBal, global_oldBal);
        strcpy(variable.newBal, global_newBal);
    }

    FILE *fp = fopen("transaction.log", "a");
    fprintf(fp, "%-25s | %-28s | %-12s | %-12s | %-12s | %-12s | %-12s\n",
        timestamp, actionName, variable.sender_accNum, variable.recipient_accNum, variable.amount, variable.oldBal, variable.newBal);
    fclose(fp);
}

// ==================== BOOL FUNCTIONS ====================
bool checkNameFormat(const char *str){
    //Reject empty string
    if (*str == '\0'){
        return false;
    }

    while((*str != '\0' && *str != '\n')){
        //Accepts dash(-), period(.) and slash(/) in name
        if(*str == '-' || *str == '.' || *str == '/' || *str == ' '){
            str++;
            continue;
        }

        //Check if it's not alphabet
        if (!isalpha((unsigned char)*str)){
            return false;
        }

        str++;
    }

    //Return true if everything is alphabet and contains special char stated
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

bool check_input(char *input, int size){
    if (fgets(input, size, stdin) != NULL){
        if (strchr(input, '\n') == NULL) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF); // flush
            return false;
        } else {
            input[strcspn(input, "\n")] = '\0';
            return true;
        }
    }
}

bool onlyAlphabets(const char *str){
    //Reject empty string
    if (*str == '\0'){
        return false;
    }

    while((*str != '\0')){
        //Check if it's not alphabet
        if (!isalpha((unsigned char)*str)){
            return false;
        }

        str++;
    }

    //Return true if everything is alphabet
    return true;
}

bool check_digitNumber(char amount[]){
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

bool isDirectoryEmpty(const char *path){
    DIR *d;
    struct dirent *dir;

    bool hasAccounts = false;

    d = opendir(path);

    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0){
            continue;
        }

        hasAccounts = true;
    }
    closedir(d);

    if(hasAccounts){
        return false;
    } else {
        return true;
    }
}

bool DBDirectoryExists(){
    DIR *d;
    struct dirent *dir;

    d = opendir(db_path);
    if (d == NULL){
        return false;
    }

    closedir(d);
    return true;
}

bool logFileExists(){
    FILE *fp = fopen("transaction.log", "r");
    if (fp == NULL){
        return false;
    }

    return true;
}

// ==================== INT FUNCTION ====================
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
    } else {
        return 0;
    }
}

// ==================== ACTION FUNCTIONS ====================
void create_new_bank_acc(){
    struct variables variable;
    char loweredAccType[20];

    int step = 1;

    bool valid = false;
    printHeader(11); //---------CREATE NEW BANK ACCOUNT------
    printf("Please insert your personal details below\n");

    while(true){
        if (step == 1){
            printf("Holder's Name: ");
            if(!check_input(variable.name, sizeof(variable.name))){ //Prevent users to enter massive amount of input
                printError(201); //Input exceeds 255 characters
                continue;
            }

            if (strcmp(variable.name,backKeyWord)==0){
                return;
            } else {
                if (checkNameFormat(variable.name)) {
                    step++;
                    continue;
                } else {
                    printError(301); //Invalid name format
                    continue;
                }
            }
        } else if (step == 2){
            printf("Holder's Identification Number: ");
            if(!check_input(variable.id, sizeof(variable.id))){ //Prevent users to enter massive amount of input
                printError(202); //ID number != 12
                continue;
            }

            if (strcmp(variable.id,backKeyWord) == 0){
                step--;
                continue;
            }

            if (strlen(variable.id) != 12){
                printError(202); //ID number != 12
                continue;
            }

            if(onlyDigits(variable.id)){
                step++;
                continue;
            } else {
                printError(302); //Invalid ID format
                continue;
            }
        } else if (step == 3){
            printf("Type of Account(Savings[1] OR Current[2]): ");
            if(!check_input(variable.acc_type,sizeof(variable.acc_type))){ //Prevent users to enter massive amount of input
                printError(203); //Input exceeds 8 characters
                continue;
            }
            
            if (strcmp(variable.acc_type,backKeyWord)==0){
                step--;
                continue;
            } else {
                if(onlyAlphabets(variable.acc_type)){
                    strcpy(loweredAccType,variable.acc_type);
                    toLowerCase(loweredAccType);
                    if (strcmp(loweredAccType,"savings") == 0 || strcmp(loweredAccType,"current") == 0){
                        valid = true;
                    }
                } else if(onlyDigits(variable.acc_type)){
                    if(strcmp(variable.acc_type,"1")==0){
                        strcpy(variable.acc_type,"Savings");
                        valid = true;
                    } else if (strcmp(variable.acc_type,"2")==0){
                        strcpy(variable.acc_type,"Current");
                        valid = true;
                    }
                }

                if (valid){
                    step++;
                } else {
                    printError(303);
                }

                continue;
            }
        } else if (step == 4){
            printf("Pin: ");
            if(!check_input(variable.pin, sizeof(variable.pin))){ //Prevent users to enter massive amount of input
                printError(206); //Input exceeds 4 characters
                continue;
            } 

            if (strcmp(variable.pin,backKeyWord) == 0){
                step--;
                continue;
            }

            if(strlen(variable.pin) != 4){
                printError(206); //Input exceeds 4 characters
                continue;
            }

            if(!onlyDigits(variable.pin)){
                printError(304); //Invalid pin format
                continue;
            } else {
                break; //Credentials valid. Exit loop
            }
        } else {
            printf("System crashed. Restarting the program...\n");
            return; //Force exit void function
        }
    }  
    
    create_txt_file(variable.name,variable.id,variable.acc_type,variable.pin);

    printf("\n----------BANK ACCOUNT CREATED----------\n");
    printf("Bank Account Number: %s\n", global_accNum);

}

void check_bank_acc(){
    DIR *d;
    struct dirent *dir;
    struct variables variable;

    char *info[] = {
        "Bank Account Number:",
        "Holder's Name:",
        "Holder's Identification Number:",
        "Account Type:",
        "Pin:",
        "Account Balance: RM",
    };

    int step = 1;
    int fileIndex = 1;
    int amountLine = 6;

    char exitInput[3];

    bool last4Correct = false;

    printHeader(12); //---------CHECK BANK ACCOUNT------
    d = opendir(db_path);
    if (d == NULL){
        printError(102); //Unable to access directory
        return;
    }

    //List current bank accounts
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;

        printf("%d. %s\n", fileIndex, dir->d_name);
        fileIndex++;
    }
    closedir(d);

    if(isDirectoryEmpty(db_path)){
        printf("No bank accounts found. Unable to proceed.\n");
        printf("Please create a bank account first.\n");
        return;
    }

    printf("\n");

    while(true){
        if (step == 1){
            printf("Bank Account Number: ");
            if(!check_input(variable.accNum,sizeof(variable.accNum))){ //Prevent users to enter massive amount of input
                printError(205); //Input exceeds 6 characters
                continue;
            }

            if(strcmp(variable.accNum,backKeyWord)==0){
                return;
            }

            if (strlen(variable.accNum) != 6){
                printError(205); //Acc num must be 6 digit number
                continue;
            }

            if(onlyDigits(variable.accNum)){
                sprintf(variable.filename, "database/%s.txt", variable.accNum);

                if (check_accNum(variable.filename)){
                    step++;
                    continue;
                } else {
                    printError(501); //No matching bank account number
                    continue;
                }
            } else {
                printError(305); //Invalid account number format
            }
        } else if (step == 2){
            printf("Last 4-digit of ID Number: ");
            if(!check_input(variable.last4Digit,sizeof(variable.last4Digit))){ //Prevent users to enter massive amount of input
                printError(206); //Input exceeds 4 characters
                continue;
            }

            if(strcmp(variable.last4Digit,backKeyWord) == 0){
                step--;
                continue;
            }

            if (strlen(variable.last4Digit) != 4){
                printError(206); //Input exceeds 4 characters
                continue;
            }

            if(check_last4Digits(variable.filename,variable.last4Digit)){
                last4Correct = true;
                break;
            } else {
                printError(401); //Incorrect last 4 digit
                continue;
            }
        }
    }

    if (last4Correct){
        printf("-----------------------------------------------------\n");
        printf("CREDENTIALS VALID\n\n");

        int num_info = sizeof(info)/sizeof(info[0]);

        printf("Bank Account Details: \n");
        for (int i=0;i<num_info;i++){
            getContent(variable.filename,i+1);
            if(i+1 == amountLine){
                strcpy(variable.amount,global_content);
            }

            printf("%s %s\n",info[i],global_content);

        }
    }

    strcpy(global_accNum,variable.accNum);
    strcpy(global_oldBal,variable.amount);
    strcpy(global_newBal,variable.amount);

    updateTransactionLog(2);
}

void delete_bank_acc(){
    DIR *d;
    struct dirent *dir;
    struct variables variable;

    char deleteAccept[8];
    char loweredDeleteAccept[7];

    bool last4Correct = false;
    bool pinCorrect = false;
    bool invalid = false;

    int step = 1;
    int fileIndex = 1;
    int amountLine = 6;
    int lineNum = 1;

    printHeader(13);  //---------DELETE BANK ACCOUNT------
    d = opendir(db_path); 
    if (d == NULL) {
        printError(102); //Unable to access directory
        return;
    }

    //List current bank accounts
    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;

        printf("%d. %s\n", fileIndex, dir->d_name);
        fileIndex++;
    }
    closedir(d);

    if(isDirectoryEmpty(db_path)){
        printf("No bank accounts found. Unable to proceed with deletion\n");
        printf("Please create a bank account first.\n");
        return;
    }

    printf("\n");

    while(true){
        if(step == 1){
            printf("Bank Account Number: ");
            if(!check_input(variable.accNum,sizeof(variable.accNum))){ //Prevent users to enter massive amount of input
                printError(205); //Input exceeds 6 characters
                continue;
            }

            if(strcmp(variable.accNum,backKeyWord)==0){
                return;
            }

            if (strlen(variable.accNum) != 6){
                printError(205); //Acc num must be 6 digit number
                continue;
            }

            if(onlyDigits(variable.accNum)){
                sprintf(variable.filename, "database/%s.txt", variable.accNum);

                if (check_accNum(variable.filename)){
                    step++;
                    continue;
                } else {
                    printError(501); //No matching bank account number
                    continue;
                }
            } else {
                printError(305); //Invalid account number format
            }
        } else if (step == 2){
            printf("Last 4-digit of Holder's ID Number: ");
            if(!check_input(variable.last4Digit,sizeof(variable.last4Digit))){ //Prevent users to enter massive amount of input
                printError(206); //Input exceeds 4 characters
                continue;
            }

            if(strcmp(variable.last4Digit,backKeyWord) == 0){
                step--;
                continue;
            }

            if (strlen(variable.last4Digit) != 4){
                printError(206); //Input exceeds 4 characters
                continue;
            }

            if(check_last4Digits(variable.filename,variable.last4Digit)){
                last4Correct = true;
                step++;
                continue;
            } else {
                printError(401); //Incorrect last 4 digit
                continue;
            }
        } else if (step == 3){
            printf("Pin: ");
            if(!check_input(variable.pin, sizeof(variable.pin))){ //Prevent users to enter massive amount of input
                printError(206); //Input exceeds 4 characters
                continue;
            } 

            if (strcmp(variable.pin,backKeyWord) == 0){
                step--;
                continue;
            }

            if (strlen(variable.pin) != 4){
                printError(206); //Input exceeds 4 characters
                continue;
            }

            
            if (check_pin(variable.filename,variable.pin)){
                pinCorrect = true;
                step++;
                break;
            } else {
                printError(402); //Incorrect pin
                continue;
            }
        } else {
            printf("System crashed. Restarting program...\n");
            return; //Force exit void function
        }
    }

    if (last4Correct && pinCorrect){
        while(true){
            printf("Confirm? (Accept[1], Reject[0]): ");
            if(!check_input(deleteAccept,sizeof(deleteAccept))){ //Prevent users to enter massive amount of input
                printError(308); //Input exceeds 6 characters
                continue;
            }

            if (onlyAlphabets(deleteAccept) || onlyDigits(deleteAccept)){
                if (onlyAlphabets(deleteAccept)){
                    strcpy(loweredDeleteAccept,deleteAccept);
                    toLowerCase(loweredDeleteAccept);
                    strcpy(deleteAccept,loweredDeleteAccept);
                }

                if (strcmp(deleteAccept,"reject") == 0 || strcmp(deleteAccept,"0") == 0){
                    printError(508); //Deletion rejected
                    return; //Force exit delete function
                } else if (strcmp(deleteAccept,"accept") == 0 || strcmp(deleteAccept,"1") == 0){
                    if (remove(variable.filename) == 0){
                        printf("----------ACCOUNT DELETED----------\n");

                        strcpy(global_accNum, variable.accNum);
                        updateTransactionLog(3);
                        return;
                    } else if (remove(variable.filename) != 0) {
                        perror("Remove failed");
                    }
                } else {
                    invalid = true;
                }
            } else {
                invalid = true;
            }

            if (invalid){
                printError(308); //Invalid accept format
                continue;
            }
        }
    } else {
        printError(103); //Unable to remove account. Leaving action..
        return;
    }
}

void deposit(){
    struct variables variable;

    char inputAmount[9];
    float amountToDeposit = 0.00;

    char depositAccept[8];
    char loweredDepositAccept[256];
    char local_oldBal[10];

    char *endptr;

    int step = 1;
    bool acceptValid = true;

    printHeader(14); //---------DEPOSIT MONEY------

    if(isDirectoryEmpty(db_path)){
        printf("No bank accounts found. Unable to proceed.\n");
        printf("Please create a bank account first.\n");
        return;
    }

    while(true){
        if (step == 1){
            printf("Bank Account Number: ");
            if(!check_input(variable.accNum,sizeof(variable.accNum))){ //Prevent users to enter massive amount of input
                printError(205); //Input exceeds 6 characters
                continue;
            }

            if (strcmp(variable.accNum,backKeyWord) == 0){
                return;
            }

            if (strlen(variable.accNum) != 6){
                printError(205); //Acc num must be 6 digit number
                continue;
            }

            if(!onlyDigits(variable.accNum)){
                printError(305);
                continue;
            }

            sprintf(variable.filename,"database/%s.txt",variable.accNum);

            FILE *fp;
            fp = fopen(variable.filename, "r");

            if (fp == NULL){
                printError(104); //Account not found
                continue;
            } else {
                step++;
                continue;
            }

            fclose(fp);
        } else if (step == 2){
            //Get pin from user
            printf("Pin: ");
            if(!check_input(variable.pin, sizeof(variable.pin))){ //Prevent users to enter massive amount of input
                printError(206); //Input exceeds 4 characters
                continue;
            } 

            if(strcmp(variable.pin,backKeyWord) == 0){
                step--;
                continue;
            }

            if (strlen(variable.pin) != 4){
                printError(206); //Input exceeds 4 characters
                continue;
            }
 
            //Check if pin is valid
            if(check_pin(variable.filename,variable.pin)){
                printf("-----------------------------------------------------\n");
                printf("CREDENTIALS VALID. PLEASE PROCEED\n\n");
                step++;
                getAmount(variable.filename);
                sprintf(global_oldBal, "%.2f", global_amount);
                continue;
            } else {
                printError(402); //Incorrect Pin
                continue;
            }
        } else if (step == 3){
            printf("Account Balance: RM %.2f\n",global_amount);
            printf("Enter Amount To Deposit: RM ");
            if(!check_input(inputAmount,sizeof(inputAmount))){ //Prevent users to enter massive amount of input
                printError(306); //Input exceeds 8 characters
                continue;
            }

            amountToDeposit = strtof(inputAmount,&endptr);

            if (check_amountFormat(inputAmount) && check_digitNumber(inputAmount)){
                if (amountToDeposit < 0 || amountToDeposit > 50000){
                    printError(502); //Amount exceed allowed range
                    continue;
                } else {
                    printf("Confirm? (Accept[1], Reject[0], Exit[2]): ");
                    if(!check_input(depositAccept,sizeof(depositAccept))){ //Prevent users to enter massive amount of input
                        printError(308); //Input exceeds 8 characters
                        continue;
                    }

                    if(onlyDigits(depositAccept) || onlyAlphabets(depositAccept)){
                        if (onlyAlphabets(depositAccept)){
                            strcpy(loweredDepositAccept,depositAccept);
                            toLowerCase(loweredDepositAccept);
                            strcpy(depositAccept,loweredDepositAccept);
                        }

                        if (strcmp(depositAccept,"2") == 0 || strcmp(depositAccept,"exit") == 0){
                            printError(509);
                            return;
                        } else if(strcmp(depositAccept,"0") == 0 || strcmp(depositAccept,"reject") == 0){
                            printError(505); //Transaction rejected
                            continue;
                        } else if (strcmp(depositAccept,"1") == 0 || strcmp(depositAccept,"accept") == 0){
                            global_amount += amountToDeposit;
                            updateAmount(variable.filename,global_amount);
                            sprintf(global_newBal, "%.2f", global_amount);
                            
                            printf("----------DEPOSIT SUCCESSFUL----------\n");
                            getAmount(variable.filename);
                            printf("Updated Amount: RM %.2f\n", global_amount);
                            break;
                        } else {
                            acceptValid = false;
                        }
                    } else {
                        acceptValid = false;
                    }
                }
            } else {
                printError(306); //Invalid amount format.
                continue;
            }
        }

        if (!acceptValid){
            printError(308); //Invalid accept format.
            continue;
        }
    } 

    strcpy(global_accNum, variable.accNum);
    strcpy(global_strAmount, inputAmount);

    updateTransactionLog(4);
}

void withdraw(){
    char inputAmount[9];
    float amountToWithdraw = 0.00;

    char withdrawAccept[8];
    char loweredWithdrawAccept[7];

    char *endptr;

    int step = 1;
    bool acceptValid = true;

    struct variables variable;

    printHeader(15); //---------WITHDRAW MONEY------

    if(isDirectoryEmpty(db_path)){
        printf("No bank accounts found. Unable to proceed.\n");
        printf("Please create a bank account first.\n");
        return;
    }

    while(true){
        if (step == 1){
            printf("Bank Account Number: ");
            if(!check_input(variable.accNum,sizeof(variable.accNum))){ //Prevent users to enter massive amount of input
                printError(205); //Input exceeds 6 characters
                continue;
            }

            if (strlen(variable.accNum) != 6){
                printError(205); //Acc num must be 6 digit number
                continue;
            }

            if (strcmp(variable.accNum,backKeyWord) == 0){
                return;
            } else {
                if(!onlyDigits(variable.accNum)){
                    printError(305);
                    continue;
                }

                sprintf(variable.filename,"database/%s.txt",variable.accNum);

                FILE *fp;
                fp = fopen(variable.filename, "r");

                if (fp == NULL){
                    printError(104); //Account not found
                    continue;
                } else {
                    step++;
                    continue;
                }

                fclose(fp);
            }
        } else if (step == 2){
            //Get pin from user
            printf("Pin: ");
            if(!check_input(variable.pin, sizeof(variable.pin))){ //Prevent users to enter massive amount of input
                printError(206); //Input exceeds 4 characters
                continue;
            } 

            if (strlen(variable.pin) != 4){
                printError(206); //Input exceeds 4 characters
                continue;
            }
 

            if(strcmp(variable.pin,backKeyWord) == 0){
                step--;
                continue;
            } else {
                //Check if pin is valid
                if(check_pin(variable.filename,variable.pin)){
                    printf("-----------------------------------------------------\n");
                    printf("CREDENTIALS VALID. PLEASE PROCEED\n\n");
                    step++;
                    getAmount(variable.filename);
                    sprintf(global_oldBal, "%.2f", global_amount);
                    continue;
                } else {
                    printError(402); //Incorrect Pin
                    continue;
                }
            }
        } else if (step == 3){
            printf("Account Balance: RM %.2f\n",global_amount);
            printf("Enter Amount To Withdraw: RM ");
            if(!check_input(inputAmount,sizeof(inputAmount))){ //Prevent users to enter massive amount of input
                printError(306); //Input exceeds 8 characters
                continue;
            }

            amountToWithdraw = strtof(inputAmount,&endptr);

            if (check_amountFormat(inputAmount) && check_digitNumber(inputAmount)){
                if (amountToWithdraw < 0){
                    printError(507); //Amount must be positive
                    continue;
                }
                if (amountToWithdraw > global_amount){
                    printError(503);
                    continue;
                }

                printf("Confirm? (Accept[1], Reject[0], Exit[2]): ");
                if(!check_input(withdrawAccept,sizeof(withdrawAccept))){ //Prevent users to enter massive amount of input
                    printError(308); //Input exceeds 8 characters
                    continue;
                }

                if(onlyDigits(withdrawAccept) || onlyAlphabets(withdrawAccept)){
                    if (onlyAlphabets(withdrawAccept)){
                        strcpy(loweredWithdrawAccept,withdrawAccept);
                        toLowerCase(loweredWithdrawAccept);
                        strcpy(withdrawAccept,loweredWithdrawAccept);
                    }

                    if (strcmp(withdrawAccept,"2") == 0 || strcmp(withdrawAccept, "exit") == 0){
                        printError(509);
                        return;
                    } else if(strcmp(withdrawAccept,"0") == 0 || strcmp(withdrawAccept,"reject") == 0){
                        printError(505); //Transaction rejected
                        continue;
                    } else if (strcmp(withdrawAccept,"1") == 0 || strcmp(loweredWithdrawAccept,"accept") == 0){
                        global_amount -= amountToWithdraw;
                        updateAmount(variable.filename,global_amount);
                        sprintf(global_newBal, "%.2f", global_amount);
                        
                        printf("----------DEPOSIT SUCCESSFUL----------\n");
                        getAmount(variable.filename);
                        printf("Updated Amount: RM %.2f\n", global_amount);
                        break;
                    } else {
                        acceptValid = false;
                    }
                } else {
                    acceptValid = false;
                }
            } else {
                printError(306); //Invalid amount format.
                continue;
            }
        }

        if (!acceptValid){
            printError(308); //Invalid accept format.
            continue;
        }
    }

    strcpy(global_accNum, variable.accNum);
    strcpy(global_strAmount, inputAmount);

    updateTransactionLog(5);
}

void remittance(){
    struct variables variable;

    char inputAmount[9];
    float amountToRemit = 0.00;
    char *endptr;

    int remitStatus;
    float savToCur = 0.02;
    float curToSav = 0.03;
    float remitFee;

    char remitAccept[7];

    bool rejected = false;
    bool invalid = false;

    int step = 1;

    printHeader(16); //---------REMITTANCE------

    if(isDirectoryEmpty(db_path)){
        printf("No bank accounts found. Unable to proceed.\n");
        printf("Please create a bank account first.\n");
        return;
    }

    printf("REMITTANCE FEE CHARGES:\n");
    printf("Savings to Current: 2%%\n");
    printf("Current to Savings: 3%%\n\n");

    
    while (true){
        if (step == 1){
            //Check if sender's bank account number is valid
            printf("Sender Bank Account Number: ");
            if(!check_input(variable.sender_accNum,sizeof(variable.sender_accNum))){ //Prevent users to enter massive amount of input
                printError(205); //Input exceeds 6 characters
                continue;
            }

            if (strcmp(variable.sender_accNum,backKeyWord) == 0){
                return;
            }
            sprintf(variable.sender_filename,"database/%s.txt", variable.sender_accNum);

            if (check_accNum(variable.sender_filename)){
                step++;
                continue;
            } else {
                printError(104); //Account not found
                continue;
            }
        } else if (step == 2){
            //Check if pin is valid
            printf("Pin: ");
            if(!check_input(variable.sender_pin,sizeof(variable.sender_pin))){ //Prevent users to enter massive amount of input
                printError(206); //Input exceeds 4 characters
                continue;
            }

            if (strcmp(variable.sender_pin,backKeyWord) == 0){
                step--;
                continue;
            }
            if (!onlyDigits(variable.sender_pin)){
                printError(304); //Invalid PIN format
                continue;
            }

            if(check_pin(variable.sender_filename,variable.sender_pin)){
                step++;
                continue;
            } else {
                printError(402); //Invalid PIN
                continue;
            }
        } else if (step == 3){
            //Check if recipient's bank account number is valid
            printf("Recipient Bank Account Number: ");
            if(!check_input(variable.recipient_accNum,sizeof(variable.recipient_accNum))){ //Prevent users to enter massive amount of input
                printError(205); //Input exceeds 6 characters
                continue;
            }

            if (strcmp(variable.recipient_accNum,backKeyWord) == 0){
                step--;
                continue;
            }

            sprintf(variable.recipient_filename,"database/%s.txt",variable.recipient_accNum);

            if(check_accNum(variable.recipient_filename)){
                if(strcmp(variable.recipient_accNum, variable.sender_accNum) == 0){ //Check if sender_accNum = recipient_accNum
                    printError(506); //Sender bank account != Recipient bank account
                    continue;
                }
            } else {
                printError(104); //Account not found
                continue;
            }

            printf("-----------------------------------------------------\n");
            printf("CREDENTIALS VALID. PLEASE PROCEED\n\n");

            //Store sender and recipient current amount
            getAmount(variable.sender_filename);
            variable.sender_amount = global_amount;
            sprintf(global_oldBal, "%.2f", global_amount);
            getAmount(variable.recipient_filename);
            variable.recipient_amount = global_amount;
            printf("Account Balance: RM %.2f\n", variable.sender_amount);
            step++;
            continue;
        } else if(step == 4){
            //Check if amount to be transferred is valid
            printf("Amount to be transferred: RM ");
            if(!check_input(inputAmount,sizeof(inputAmount))){ //Prevent users to enter massive amount of input
                printError(306); //Input exceeds 8 characters
                continue;
            }

            //Change string to float
            amountToRemit = strtof(inputAmount, &endptr);

            if(check_amountFormat(inputAmount) && check_digitNumber(inputAmount)){
                check_digitNumber(inputAmount);
                remitStatus = check_accType(variable.sender_filename,variable.recipient_filename);
                getAmount(variable.sender_filename);
                if (global_amount < amountToRemit){
                    printError(504); //Remittance amount exceeds the current amount
                    continue;
                }
                step++;
                continue;
            } else {
                printError(306); //Invalid amount format
                continue;
            }
        } else if (step == 5){
            if(remitStatus == 1){ //Savings to Current
                remitFee = amountToRemit * savToCur;
                printf("Remittance Fee: RM %.2f\n", remitFee);
                amountToRemit -= remitFee;
                printf("Transferring Amount: RM %.2f\n\n", amountToRemit);
                printf("Confirm? (Accept[1], Reject[0], Exit[2]): ");
                if(!check_input(remitAccept,sizeof(remitAccept))){ //Prevent users to enter massive amount of input
                    printError(308); //Input exceeds 6 characters
                    continue;
                }

                if (onlyDigits(remitAccept)){ //If user input 0 or 1
                    if(strcmp(remitAccept, "0") == 0){
                        rejected = true; //Transaction rejected
                    } else if (strcmp(remitAccept,"1") == 0){
                        variable.sender_amount -= amountToRemit;
                        variable.recipient_amount += amountToRemit;
                        
                        updateAmount(variable.sender_filename,variable.sender_amount);
                        updateAmount(variable.recipient_filename,variable.recipient_amount);

                        printf("----------REMIT SUCCESSFUL----------\n");
                        break;
                    } else if(strcmp(remitAccept,"2") == 0){
                        printError(509);
                        return;
                    } else {
                        invalid = true; //Invalid accept format
                    }
                } else if (onlyAlphabets(remitAccept)){
                    toLowerCase(remitAccept);
                    if (strcmp(remitAccept,"reject") == 0) {
                        rejected = true; //Transaction rejected
                    } else if (strcmp(remitAccept,"accept") == 0){
                        variable.sender_amount -= amountToRemit;
                        variable.recipient_amount += amountToRemit;
                        
                        updateAmount(variable.sender_filename,variable.sender_amount);
                        updateAmount(variable.recipient_filename,variable.recipient_amount);

                        printf("----------REMIT SUCCESSFUL----------\n");
                        break;
                    } else if(strcmp(remitAccept,"exit") == 0){
                        printError(509);
                        return;
                    } else {
                        invalid = true; //Invalid accept format
                    }
                } else {
                    invalid = true; //Invalid accept format
                }
            } else if (remitStatus == 2){ //Current to Savings
                remitFee = amountToRemit * curToSav;
                printf("Remittance Fee: RM %.2f\n", remitFee);
                amountToRemit -= remitFee;
                printf("Transferring Amount: RM %.2f\n", amountToRemit);
                printf("Confirm? (Accept[1], Reject[0]): ");
                if(!check_input(remitAccept,sizeof(remitAccept))){ //Prevent users to enter massive amount of input
                    printError(306); //Input exceeds 6 characters
                    continue;
                }

                if (onlyDigits(remitAccept)){ //If user input 0 or 1
                    if(strcmp(remitAccept, "0") == 0){
                        rejected = true; //Transaction rejected
                    } else if (strcmp(remitAccept,"1") == 0){
                        variable.sender_amount -= amountToRemit;
                        variable.recipient_amount += amountToRemit;
                        
                        updateAmount(variable.sender_filename,variable.sender_amount);
                        updateAmount(variable.recipient_filename,variable.recipient_amount);

                        printf("----------REMIT SUCCESSFUL----------\n");
                        break;
                    } else {
                        invalid = true; //Invalid accept format
                    }
                } else if (onlyAlphabets(remitAccept)){
                    toLowerCase(remitAccept);
                    if (strcmp(remitAccept,"reject") == 0) {
                        rejected = true; //Transaction rejected
                    } else if (strcmp(remitAccept,"accept") == 0){
                        variable.sender_amount -= amountToRemit;
                        variable.recipient_amount += amountToRemit;
                        
                        updateAmount(variable.sender_filename,variable.sender_amount);
                        updateAmount(variable.recipient_filename,variable.recipient_amount);

                        printf("----------REMIT SUCCESSFUL----------\n");
                        break;
                    } else {
                        invalid = true; //Invalid accept format
                    }
                } else {
                    invalid = true;  //Invalid accept format
                }

                if (rejected){
                    printError(505); //Transaction rejected
                    step--;
                    continue;
                } 

                if (invalid){
                    printError(308); //Invalid accept format
                    continue;
                }
            } else if (remitStatus == 0){
                printf("-----------------------------------------------------\n");
                printf("System crashed. Restarting the program...\n");
                return; //Force exit void function
            }
        } else {
            printf("-----------------------------------------------------\n");
            printf("System crashed. Restarting the program...\n");
            return; //Force exit void function
        }
    }

    printf("Account Balance(%s): RM %.2f\n", variable.sender_accNum, variable.sender_amount);
    printf("Account Balance(%s): RM %.2f\n", variable.recipient_accNum, variable.recipient_amount);

    strcpy(global_senderAccNum, variable.sender_accNum);
    strcpy(global_recipientAccNum, variable.recipient_accNum);
    sprintf(global_strAmount, "%.2f", amountToRemit);
    sprintf(global_newBal, "%.2f", variable.sender_amount);

    updateTransactionLog(6);

}

int main(){
    srand(time(NULL));
    struct variables variable;

    char input[256];
    int option = 0;
    char *end;
    
    //Body - Main Part
    while (true){
        start_menu();

        //Exit code if user does not accept to create 'database' folder
        if (strcmp(setupAccept,"0") == 0){
            printf("----------------------------------------\n");
            printf("Program Exited");
            return 0;
        }

        printf("Please type the number of actions (1-6) to proceed: ");
        if(!check_input(input, sizeof(input))){ //Prevent users to enter massive amount of input
            printError(201); //Input exceeds 255 characters
            continue;
        }

        option = strtol(input, &end, 10);

        if (*end == '\0'){
            switch(option){
                case 1: 
                    create_new_bank_acc();
                    break;
                case 2:
                    check_bank_acc();
                    break;
                case 3:
                    delete_bank_acc();
                    break;
                case 4: 
                    deposit();
                    break;
                case 5:
                    withdraw();
                    break;
                case 6:
                    remittance();
                    break;
                case 7:
                    printf("\n----------------------------------------\n");
                    printf("Program Exited");
                    return 0;
                default: 
                    printError(307); //Invalid selection
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
            } else if (strcmp(input, "check") == 0 || strcmp(input,"check bank account") == 0){
                check_bank_acc();
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
                printError(307); //Invalid selection
            }
        }
    }
}