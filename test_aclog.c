#include <stdio.h>
#include <string.h>

int main() 
{
	int i;
	size_t bytes;
	FILE *file;
	char filenames[10][7] = {"file_0", "file_1", 
			"file_2", "file_3", "file_4",
			"file_5", "file_6", "file_7", 		
			"file_8", "file_9"};


	/* example source code */

	for (i = 0; i < 10; i++) {

		file = fopen(filenames[i], "w+");
		if (file == NULL) 
			printf("fopen error\n");
		else {
			printf("Writing %s\n",filenames[i]);
			bytes = fwrite(filenames[i], strlen(filenames[i]), 1, file);
			fclose(file);
		}

	}

	file = fopen("test1.txt", "r");
	bytes = fwrite("test1", 5, 1, file);
	fclose(file);
	file = fopen("test2.txt", "r");
	bytes = fwrite("test2", 5, 1, file);
	fclose(file);
	file = fopen("test3.txt", "r");
	bytes = fwrite("test3", 5, 1, file);
	fclose(file);
	file = fopen("test4.txt", "r");
	bytes = fwrite("test4", 5, 1, file);
	fclose(file);
	file = fopen("test5.txt", "r");
	bytes = fwrite("test5", 5, 1, file);
	fclose(file);

	
	



}
