#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SIZE_MESSAGE	256
#define END_SESSION     "exit\n"

int main(int argc, char *argv[]){
	int pipeOutput;

	char msg[SIZE_MESSAGE];
    char end[SIZE_MESSAGE]= END_SESSION;

    if( argc != 2){
        return EXIT_FAILURE;
        }

	if((pipeOutput = open (argv[1], O_RDONLY)) == -1) {
		perror("Error to open the output of named pipe");
        getchar();
		exit(EXIT_FAILURE);
	    }

    while(1){
	    read(pipeOutput, msg, SIZE_MESSAGE);
        printf("%s", msg);

        if(strcmp(msg, end) == 0){
            sleep(1);
            return EXIT_SUCCESS;
            }
        }

	return EXIT_FAILURE;
    }
