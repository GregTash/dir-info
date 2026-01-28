#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define DIR_MAX_LENGTH 4096

int main(int argc, char* argv[]) {
	char location[DIR_MAX_LENGTH];
	struct dirent* ent;
	DIR* dir;
	char* fType = NULL;

	unsigned long lineCount = 0;

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

		if (!opendir(argv[1])) {
			printf("Directory %s does not exist!\n", argv[1]);
			return 1;
		}

		strcpy(location, argv[1]); 
	}

	if (!(dir = opendir(location))) {
		perror("Could not open directory!\n");
		return 1;
	}

	fType = argv[argc-1];

	//Analyse the directory
	while ((ent = readdir(dir))) {
		int len = strlen(ent->d_name);
		char* fileEnd = &ent->d_name[len - strlen(fType)];
		
		if (!strcmp(fileEnd, fType)) {
			char* fileToOpen = malloc(sizeof(location) + strlen(ent->d_name) + 1);
			strcpy(fileToOpen, location);
			strcat(fileToOpen, "/");
			strcat(fileToOpen, ent->d_name);

			FILE* file = fopen(fileToOpen, "r");

			if (!file) {
				printf("Could not open file: %s\n", ent->d_name);
				return 1;
			}

			while (!feof(file)) {
				int c = fgetc(file);

				if (c == '\n') lineCount++;
			}

			fclose(file);
		}
	}

	printf("%lu\n", lineCount);

	return 0;
}
