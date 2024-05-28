#include <stdio.h>

int main(){

    FILE* fptr;
    fptr = popen("./toupper", "w");

    char* message = { "It'S a woNdeRfUl daY!\n" };
    fputs(message, fptr);

    pclose(fptr);
    return 0;
}
