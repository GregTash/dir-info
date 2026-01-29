#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>

#define DIR_MAX_LENGTH 4096
char location[DIR_MAX_LENGTH];
char* fType = NULL;

unsigned long lineCount = 0;

int analyseDir(char* location, bool recursive) {
	struct dirent* ent;
	DIR* dir;

	if (!(dir = opendir(location))) {
		perror("Could not open directory!\n");
		return 1;
	}

	while ((ent = readdir(dir))) {
		int len = strlen(ent->d_name);
		char* fileEnd = &ent->d_name[len - strlen(fType)];
		
		char* fileToOpen = malloc(strlen(location) + strlen(ent->d_name) + 2);
		strcpy(fileToOpen, location);
		strcat(fileToOpen, "/");
		strcat(fileToOpen, ent->d_name);

		struct stat pathStat;
		stat(fileToOpen, &pathStat);
		int isFile = S_ISREG(pathStat.st_mode);
		if (!isFile && fileToOpen[strlen(fileToOpen)-1] != '.' && recursive) {
			analyseDir(fileToOpen, true);

			free(fileToOpen);
			continue;
		}

		//If the end of the file name matches the file type then do stuff
		if (!strcmp(fileEnd, fType)) {
			FILE* file = fopen(fileToOpen, "r");

			if (!file) {
				printf("Could not open file: %s\n", ent->d_name);
				return 0;
			}

			//Read through the file and analyse info
			while (!feof(file)) {
				int c = fgetc(file);

				if (c == '\n') lineCount++;
			}

			fclose(file);
		}

		free(fileToOpen);
	}

	closedir(dir);
	return 1;
}

int main(int argc, char* argv[]) {
	DIR* dir;
	//Get the directory location
	if (argc < 1) {
		printf("No file type specified!\n");
		return 1;
	}
	else if (argc == 1) {
		if (!getcwd(location, sizeof(location))) {
			printf("Could not open directory!\n");
			return 1;
		}
	}
	else {
		if (strlen(argv[1]) > sizeof(location)) {
			printf("Directory path provided is too long!\n");
			return 1;
		}

		if (!(dir = opendir(argv[1]))) {
			printf("Directory %s does not exist!\n", argv[1]);
			return 1;
		}
		closedir(dir);

		strcpy(location, argv[1]); 
	}


	fType = argv[argc-1];

	//Analyse the directory
	if (!analyseDir(location, true)) {
		printf("Analysing directory failed!\n");
		return 1;
	}

	printf("Lines: %lu\n", lineCount);

	return 0;
}
