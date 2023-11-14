#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <strings.h>

void printLine(char line[], int count, bool ifC, bool ifD, bool ifU){
	bool run;

	run = (!ifD && !ifU);

	if ((run) || (ifD && (count > 1))) {
		if (ifC) {
			printf("%7d ", count);
		}
		printf("%s", line);
	}
	if (ifU && (count == 1)){
		if (ifC) {
			printf("%7d ", count);
		}
		printf("%s", line);
	}

}

void uniqFile(FILE *fd, bool ifC, bool ifD, bool ifI, bool ifU){
	char last[BUFSIZ];
	char current[BUFSIZ];
	int strctr = 1;

	fgets(last, sizeof last, fd);

	if (ifI) {
		while(fgets(current, sizeof current, fd) != NULL) {
			if (strcasecmp(last, current) != 0) {
				printLine(last, strctr, ifC, ifD, ifU);
				strctr = 1;
				strcpy(last, current);
			} else {strctr += 1;}
		}
		printLine(last, strctr, ifC, ifD, ifU);
	}
	else {
		while(fgets(current, sizeof current, fd) != NULL){
			if (strcmp(last, current) != 0){
				printLine(last, strctr, ifC, ifD, ifU);
				strctr = 1;
				strcpy(last, current);
			} else {strctr += 1;}
		}
		printLine(last, strctr, ifC, ifD, ifU);
	}
}

int main(int argc, char *argv[]) {
	int opt;
	bool ifC, ifD, ifI, ifU;
	int totalF;
	int returnStatus = EXIT_FAILURE;
	
	ifC = ifD = ifI = ifU = false;
	opterr = 0;
	while ((opt = getopt(argc, argv, "cdiu")) != -1) {
		switch(opt) {
			case 'c': ifC = true; break;
			case 'd': ifD = true; break;
			case 'i': ifI = true; break;
			case 'u': ifU = true; break;
			default:
				  fprintf(stderr, "%s: illegal option '-%c\n", argv[0], optopt);
				  fprintf(stderr, "usage: %s [-cdiu] [FILE] ...\n", argv[0]);
				  goto cleanup;
		}
	}
	if (ifD && ifU){
		goto cleanup;
	}
	totalF = argc - optind;
	if (totalF == 0) {
		uniqFile(stdin, ifC, ifD, ifI, ifU);
		returnStatus = EXIT_SUCCESS;
		goto cleanup;
	}
	if ((optind + 1) == argc) {
		FILE *fd = fopen(argv[optind], "r");
		if (fd == NULL) {
			fprintf(stderr, "%s: File does not exist!\n", argv[optind]);
			goto cleanup;
		}
		uniqFile(fd, ifC, ifD, ifI, ifU);
		fclose(fd);
		returnStatus = EXIT_SUCCESS;
		goto cleanup;
	}
	if ((optind + 1) < argc) {
		while (argv[optind] != NULL){
			FILE *fd = fopen(argv[optind], "r");
			if (fd == NULL) {
				fprintf(stderr, "%s: File does not exist!\n", argv[optind]);	
				goto cleanup;
			}
			uniqFile(fd, ifC, ifD, ifI, ifU);
			fclose(fd);
			optind++;
			goto cleanup;
		}
	}
	cleanup:
		return returnStatus;
}

