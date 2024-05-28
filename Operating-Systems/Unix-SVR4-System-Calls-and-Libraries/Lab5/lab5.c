#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>

int main(int argc, char* argv[]) {

    if (argc < 2) {
		perror("Invalid arguments.");
		return 0;
	}

	int File = open(argv[1], O_RDONLY);
	if (File == -1) {
		perror("This file can't be opened.");
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
	while (printf("Enter line number (or 0 to finish): ") && scanf("%s", str) ) {

		if (str[0] == '0')
			break;

        int number = atol(str);
		if ((number <= 0) || (number > line_number - 1)) {
			printf("Invalid line number. Repeat please. \n");
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
