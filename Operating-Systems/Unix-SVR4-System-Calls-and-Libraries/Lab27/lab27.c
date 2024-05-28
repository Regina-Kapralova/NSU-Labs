#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Please try again and don't forget to enter file name.\n");
        return 0;
    }

    FILE *textfile, *fpout;
    if((textfile = fopen(argv[1], "r")) == (FILE*)NULL) {
        printf("Please try again and to enter correct file name.\n");
        return 0;
    }

    fpout = popen("wc -l", "w");

    char line[BUFSIZ];
    while(fgets(line, BUFSIZ, textfile) != (char*)NULL)
        if(line[0] == '\n')
            fputs(line, fpout);

    fclose(textfile);
    pclose(fpout);
    return 0;
}