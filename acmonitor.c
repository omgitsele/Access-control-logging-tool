#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

struct entry {

	int uid; /* user id (positive integer) */
	int access_type; /* access type values [0-2] */
	int action_denied; /* is action denied values [0-1] */

	time_t date; /* file access date */
	time_t time; /* file access time */

	char *file; /* filename (string) */
	char *fingerprint; /* file fingerprint */

	int inv_accesses;
	
	/* add here other fields if necessary */
	/* ... */
	/* ... */

};

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}


void usage(void)
{
	printf(
	       "\n"
	       "usage:\n"
	       "\t./monitor \n"
		   "Options:\n"
		   "-m, Prints malicious users\n"
		   "-i <filename>, Prints table of users that modified "
		   "the file <filename> and the number of modifications\n"
		   "-h, Help message\n\n"
		   );

	exit(1);
}


void 
list_unauthorized_accesses(FILE *log)
{
	char *line = NULL;
	size_t len = 0;
    ssize_t read;
	int line_no = 0;
	int action_denied_flag = 0;
	char subbuff[5];

	int count = 0; //number of unique users in the table
    char *uid;
	int number1;
    int **users;
	int lineNo = 0;
	

	users = (int **)malloc(sizeof*users);
	users[count] = (int *)malloc(2*sizeof(int));
	

	while ((read = getline(&line, &len, log)) != -1) {

		char** tokens;
		tokens = str_split(line, ',');
		if (lineNo == 0)
		{
			lineNo++;
			if (tokens)
			{
				for (int i = 0; i<7; i++)
				{
					if (i == 0)
					{
						int uid1 = atoi(*(tokens+i));
						users = (int **) realloc(users, (count + 1) * sizeof(*users));
						users[0] = (int *)malloc(2*sizeof(int));
						users[0][0] = uid1;
						users[0][1] = 0;
					}
					if (i == 4)
					{
						if (**(tokens + i) == '1')
						{
							users[0][1] = 1;
						}
					}
					free(*(tokens + i));
					
				}
				free(tokens);			
			}
		}
		else
		{
			if (tokens)
			{
				int flag = -1;
				//iterate through all the fields
				for (int i = 0; i<7; i++)
				{
					
					//field 0 -> id
					if (i == 0)
					{
						char* uid1 = *(tokens+i);
						int intUid = atoi(uid1);

						//Search if user already exists in our table
						for (int j = 0; j < count+1; j++)
						{
							
							if (users[j][0] == intUid)
							{
								flag = j;
								
							}							
						}
						if (flag == -1)
						 //initialization of user
						{
								users = (int **) realloc(users, (count + 1) * sizeof(*users));
								users[count+1] = (int *)malloc((count+1)*sizeof(int));

								users[count+1][0] = intUid;
								users[count+1][1] = 0;
								count++;
						}
						//end of search
						
					}
					
					if (i == 4)
					{
						if (**(tokens + i) == '1')
						{
							if (flag !=-1)
							{
								users[flag][1] = users[flag][1] + 1;

							}
							else
							{
								users[count][1] = 1;

							}
						}
						
					}
					
					free(*(tokens + i));
				}
				printf("\n");
				free(tokens);
			}
		}

	}

	for (int i = 0; i < count+1; i++)
	{
		if (users[i][1] >=7)
		{
			printf("User with id: %d has malicious intents. (%d invalid accesses)\n", users[i][0], users[i][1]);
		}
		free(users[i]);
		
	}
	free(users);
	
	return;

}


void
list_file_modifications(FILE *log, char *file_to_scan)
{

	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */
	char *line = NULL;
	size_t len = 0;
    ssize_t read;
	int line_no = 0;

	int count = 0; //number of unique users in the table
	int number1;
	int lineNo = 0;
	char* thisFile;


	int **users;

	users = (int **)malloc(sizeof*users);
	users[count] = (int *)malloc(2*sizeof(int));
	int userNo = 0;
	char lastFinger[16];
	int timesFound = 0;
	

	while ((read = getline(&line, &len, log)) != -1) {
		char** tokens;
		tokens = str_split(line, ',');
		if (tokens)
		{
			int uid = atoi(*(tokens+0));
			thisFile = *(tokens+1);
			int accessType = atoi(*(tokens+4));
			int actionDeniedFlag = atoi(*(tokens+5));
			char* fingerPrint = *(tokens+6);
			int flag = -1;
			
			//if we found the file
			if (strcmp(file_to_scan, thisFile) == 0) 
			{
				//keep the last fingerprint
				if (timesFound == 0)
				{
					strcpy(lastFinger,fingerPrint);
					timesFound++;
				}
				//first user in table
				if (userNo == 0)
				{
					if(strcmp(lastFinger,fingerPrint)!=0 && accessType==2 && actionDeniedFlag != 1){
						
						users = (int **) realloc(users, (count + 1) * sizeof(*users));
						users[0] = (int *)malloc(2*sizeof(int));
						users[0][0] = uid;
						users[0][1] = 0;
						users[0][1] = 1;
						
						for (int i = 0; i < 7; i++)
						{
							free(*(tokens + i));
						}
						strcpy(lastFinger,fingerPrint);
						free(tokens);	
						userNo++;	
					}	
				}
				else
				{
					
					if(strcmp(lastFinger,fingerPrint)!=0 && accessType==2 && actionDeniedFlag != 1){
						strcpy(lastFinger,fingerPrint);
						//Search if user already exists in our table
						for (int j = 0; j < userNo; j++)
						{
							if (users[j][0] == uid)
							{
								flag = j;
								users[j][1] = users[j][1] + 1;
							}							
						}
						if (flag == -1)
						 //initialization of user
						{
							users = (int **) realloc(users, (userNo + 1) * sizeof(*users));
							users[userNo] = (int *)malloc((userNo+1)*sizeof(int));
							users[userNo][0] = uid;
							users[userNo][1] = 1;
							userNo++;
						}	
				
					
					}
					for (int i = 0; i < 7; i++)
					{
						free(*(tokens + i));
					}
					free(tokens);
				}
			}
			
			
			
		}
		

		
		


	
		
		
	}

	for (int i = 0; i < userNo; i++)
	{
		printf("User %d modified the file %d times\n", users[i][0], users[i][1]);
	}

	return;

}


int 
main(int argc, char *argv[])
{

	int ch;
	FILE *log;

	if (argc < 2)
		usage();

	log = fopen("./file_logging.log", "r");
	if (log == NULL) {
		printf("Error opening log file \"%s\"\n", "./log");
		return 1;
	}

	while ((ch = getopt(argc, argv, "hi:m")) != -1) {
		switch (ch) {		
		case 'i':
			list_file_modifications(log, optarg);
			break;
		case 'm':
			list_unauthorized_accesses(log);
			break;
		default:
			usage();
		}

	}


	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */


	fclose(log);
	argc -= optind;
	argv += optind;	
	
	return 0;
}
