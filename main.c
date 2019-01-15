/*
 * This program is a custom C implementation
 * of the cp shell command. 
 *
 * Alex Bestoso
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_B_SIZE 1024

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("\nPropper use:\n[%s] [existing file] [new file]", argv[0]);
		return EINVAL;
	}

	struct stat st;
	int oldFD, newFD, status;
	char buf[MAX_B_SIZE];

	/*
	 * Attempt to open both old and new files
	 */
	
	if((stat(argv[1], &st)) == -1){
		printf("\n[ERR] : stat()\n");
		return EINVAL;
	}
       	size_t totalBits = st.st_size;	

	/*
	 * Handle oldFD
	 */
	oldFD = open(argv[1], O_RDONLY);
	if(oldFD == -1){
		printf("\n[ERR] : open()\n");
		return EBADF;
	}

	if(read(oldFD, buf, totalBits) != st.st_size){
		printf("\n[ERR] : read()");
		status = close(oldFD);
		printf("\nfd exit status : %d\n", status);
		return EIO;
	}else{
		status = close(oldFD);
		if(status == -1){
			printf("\n[ERR] : close()\n");
			return EBADF;
		}else{
			printf("\n[SUC] : Buffer filled");
		}
	}

	/*
	 * Handle newFD
	 */
	newFD = open(argv[2], O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);	
	if(newFD == -1){
		printf("\n[ERR] : open()\n");
                return EBADF;
	}

	if(write(newFD, buf, totalBits) != totalBits){
		printf("\n[ERR] : write()");
                status = close(oldFD);
                printf("\nfd exit status : %d\n", status);
                return EIO;
	}else{
		status = close(oldFD);
                if(status == -1){
                        printf("\n[ERR] : close()\n");
                        return EBADF;
                }else{
                        printf("\n[SUC] : Copy successful!");
                }
	}
	return 0;
}
