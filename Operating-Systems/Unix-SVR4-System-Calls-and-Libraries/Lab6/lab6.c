#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        perror("Invalid arguments.");
        return 0;
    }

    int File = open(argv[1], O_RDONLY);
    if (File == -1) {
        perror("This file can't be opened.\n");
        return 0;
    }

    int trm = open("/dev/tty", O_RDONLY | O_NDELAY);
    if (trm == -1) {
        perror("/dev/tty can't be opened.");
        return 0;
    }

	char symbol;
	int line_length[100] = {0};
	int start_line[100] = {0};
	int line_number = 1;
	int number_symbols = 0;

	while (read(File, &symbol, 1)) {
		number_symbols++;
		if (symbol == '\n') {
			line_length[line_number] = number_symbols;
			line_number++;
			start_line[line_number] = lseek(File, 0, SEEK_CUR);
			number_symbols = 0;
		}
	}

	char str[10];
    while(1) {
        printf("You have 5 seconds to enter a line number.\n");
        sleep(5);
        int n = read(trm, str, 10);
        if (n == 0) {
            lseek(File, 0, SEEK_SET);
            for (int i = 0; i < line_number; ++i) {
                lseek(File, start_line[i], SEEK_SET);
                char* line = (char*)malloc((line_length[i] + 1) * sizeof(char));
                read(File, line, line_length[i]);
                printf("%s", line);
                free(line);
            }
            break;
        }

        if (str[0] == '0')
            break;

        int number = atoi(str);
        if (number <= 0 || (number > line_number - 1)) {
            fprintf(stderr, "Invalid line number. Repeat please. \n");
            continue;
        }
        lseek(File, start_line[number], SEEK_SET);
        char* line = (char*)malloc((line_length[number] + 1) * sizeof(char));
        read(File, line, line_length[number]);
        printf("%s", line);
        free(line);
    }
    close (File);
    return 0;
}

