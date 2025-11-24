#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

bool onlyAplhabets(const char *str){
    int i = 0;

    while((str[i] != '\0' && str[i] != '\n')){
        if(str[i] == '-'){
            i++;
            continue;
        }

        //Check if it's not alphabet
        if (!isalpha(str[i])){
            return false;
        }

        i++;
    }

    //Return true if everything is alphabet and contains "-"
    return true;
}

int main(){
    char str[256] = "Anja-23";
    if(onlyAplhabets(str)){
        printf("It's accepted!");
    } else {
        printf("Rejected");
    }
 
    return 0;
}