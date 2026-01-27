#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define DIR_MAX_LENGTH 4096

int main(int argc, char* argv[]) {
	char dir[DIR_MAX_LENGTH];

	if (argc <= 1) {
		getcwd(dir, sizeof(dir));
	}
	else {
		if (strlen(argv[1]) > sizeof(dir)) {
			printf("Directory path provided is too long!\n");
			return 1;
		}

		if (!opendir(argv[1])) {
			printf("Directory does not exist!\n");
			return 1;
		}

		strcpy(dir, argv[1]); 
	}

	printf("%s\n", dir);

	return 0;
}
