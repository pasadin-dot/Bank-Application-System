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
    char name[30];
    char id[12];
    char acc_type[20];
    char pin[5];

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
        printf("Error! File cannot be opened!\n");
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

        if (onlyAplhabets(variable.name)) {
            break;
        } else {
            printf("INVALID FORMAT. ONLY ALPHABETS AND '-' ARE ACCEPTED.\n");
            continue;
        }
    }
    
    while (true){
        printf("Identification Number: ");
        fgets(variable.id, sizeof(variable.id), stdin);
        variable.id[strcspn(variable.id, "\n")] = '\0';

        if(onlyDigits(variable.id)){
            break;
        } else {
            printf("INVALID FORMAT. ONLY NUMBERS ARE ACCEPTED.\n");
        }
    }
    
    while(true){
        printf("Type of Account(1-Savings OR 2-Current): ");
        fgets(variable.acc_type, sizeof(variable.acc_type), stdin);
        variable.acc_type[strcspn(variable.acc_type, "\n")] = '\0';

        if(onlyAplhabets(variable.acc_type)){
            strcpy(loweredAccType,variable.acc_type);
            toLowerCase(loweredAccType);
            if (loweredAccType == "savings" || loweredAccType == "current"){
                break;
            }
        } else {
            if (strcmp(variable.acc_type,"1")){
                strcpy(variable.acc_type,"Savings");
                break;
            } else if(strcmp(variable.acc_type,"2")){
                strcpy(variable.acc_type,"Current");
                break;
            } else {
                printf("INVALID FORMAT\n");
                continue;
            }
        }
    }
    

    while(numPin < 4){
        printf("4-digit Pin: ");
        fgets(variable.pin, sizeof(variable.pin), stdin);
        variable.pin[strcspn(variable.pin, "\n")] = '\0';

        if(!onlyDigits){
            printf("INVALID FORMAT. ONLY NUMBERS ARE ACCEPTED");
            continue;
        }

        numPin = strlen(variable.pin);
        if (numPin < 4) {
            printf("4 Digit Pin is required. Please retype.\n\n");
            continue;
        } else {
            break;
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
        return true;
    }
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
        //Stop at Pin Line (5)
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

void print_digitNumber(char amount[]){
    int len = strlen(amount);
    int digitNumber;

    for(int i=0;i<len;i++){
        if (amount[i] == '.'){
            int dotNum = i + 1;
            digitNumber = len - dotNum;
            
            break;
        }
    }

    printf("%d\n",digitNumber);
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
            printf("%d\n",digitNumber);
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
            printf("NO CURRENT ACCOUNT. TRY AGAIN\n");
            continue;
        } else {
            //Get pin from user
            printf("Pin: ");
            fgets(pin, sizeof(pin), stdin);
            pin[strcspn(pin, "\n")] = '\0'; 

            //Check if pin is valid
            if(check_pin(filename,pin)){
                getAmount(filename);
                
                fclose(fp);
                while(true){
                    printf("Current Amount (RM): %.2f\n",global_amount);
                    printf("Enter Amount To Deposit (RM): ");
                    fgets(inputAmount, sizeof(inputAmount), stdin);
                    inputAmount[strcspn(inputAmount, "\n")] = '\0';

                    amountToDeposit = strtof(inputAmount,&endptr);

                    if (check_amountFormat(inputAmount) && check_digitNumber(inputAmount)){
                        if (amountToDeposit < 0 || amountToDeposit > 50000){
                            printf("EXCEEDED AMOUNT. ONLY RM 0 - RM50,000 IS ALLOWED PER OPERATION\n");
                            continue;
                        } else {
                            global_amount += amountToDeposit;
                            updateAmount(filename,global_amount);

                            printf("DEPOSIT SUCCESSFUL!\n");
                            return;
                        }
                    } else {
                        printf("INVALID FORMAT. ONLY XX.XX IS ACCEPTED\n");
                        continue;  
                    }     
                }
            } else {
                printf("WRONG PIN. TRY AGAIN\n");
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
            printf("NO CURRENT ACCOUNT. TRY AGAIN\n");
            continue;
        } else {
            //Get pin from user
            printf("Pin: ");
            fgets(pin, sizeof(pin), stdin);
            pin[strcspn(pin, "\n")] = '\0';

            //Check if pin is valid
            if(check_pin(filename,pin)){
                getAmount(filename);
                
                fclose(fp);
                while(true){
                    printf("Current Amount (RM): %.2f\n",global_amount);
                    printf("Enter Amount To Withdraw (RM): ");
                    fgets(inputAmount, sizeof(inputAmount), stdin);
                    inputAmount[strcspn(inputAmount, "\n")] = '\0';
                    printf("Amount to withdraw(RM): %s\n",inputAmount);
                    amountToWithdraw = strtof(inputAmount,NULL);

                    if (check_amountFormat(inputAmount) && check_digitNumber(inputAmount)){
                        if(amountToWithdraw < 0){
                            printf("INVALID FORMAT. ONLY POSITIVE NUMBERS CAN BE ACCEPTED\n");
                            continue;
                        } else {
                            global_amount -= amountToWithdraw;
                            updateAmount(filename,global_amount);

                            printf("WITHDRAW SUCCESSFUL!\n");
                            return;
                        }
                    } else {
                        printf("INVALID FORMAT. ONLY XX.XX IS ACCEPTED\n");
                        continue;
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

int check_accType(char send_filename[], char rec_filename[]){
    FILE *send_fp = fopen(send_filename,"r");
    FILE *rec_fp = fopen(rec_filename, "r");
    if (send_fp == NULL || rec_fp == NULL){
        printf("FILE NOT OPENED\n");
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
            printf("Invalid input. Please try again\n\n");
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
            printf("Wrong Pin. Please try again\n");
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
            printf("Invalid input. Please try again\n\n");
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
            if(remitStatus == 1){ //Savings to Current
                remitFee = amountToRemit * savToCur;
                printf("Remittance Fee(RM): %.2f\n", remitFee);
                amountToRemit -= remitFee;
                printf("Transferring Amount(RM): %.2f\n", amountToRemit);
                printf("Enter 0 to Reject, 1 to Accept: ");
                fgets(remitAccept, sizeof(remitAccept), stdin);
                remitAccept[strcspn(remitAccept, "\n")] = '\0';
                if(strcmp(remitAccept, "0") == 0){
                    printf("PLEASE RE-ENTER1\n");
                    continue;
                } else if (strcmp(remitAccept,"1") == 0){
                    sender_amount -= amountToRemit;
                    receiver_amount += amountToRemit;
                    
                    updateAmount(sender_filename,sender_amount);
                    updateAmount(receiver_filename,receiver_amount);

                    printf("REMIT SUCCESSFUL\n");
                    break;
                } else {
                    printf("PLEASE RE-ENTER2\n");
                    continue;
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
                if(strcmp(remitAccept, "0") == 0){
                    printf("PLEASE RE-ENTER3\n");
                    continue;
                } else if (strcmp(remitAccept,"1") == 0){
                    sender_amount -= amountToRemit;
                    receiver_amount += amountToRemit;
                    
                    updateAmount(sender_filename,sender_amount);
                    updateAmount(receiver_filename,receiver_amount);

                    printf("REMIT SUCCESSFUL\n");
                    break;
                } else {
                    printf("PLEASE RE-ENTER4\n");
                    continue;
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
    char input[20];
    int option = 0;
    char *end;
    bool keepGoing = true;
    
    //Body - Main Part
    while (keepGoing){
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
                printf("ERROR. PLEASE RETYPE\n");
            }
        }
    }
}