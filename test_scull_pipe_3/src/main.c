/*
 ============================================================================
 Name        : test_scull_module.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char * argv[]) {
	int fd = open("/dev/sleepy", O_RDWR);

	if (fd == -1)
		perror("opening sleepy");

	if (argv[1][0] == '0') {
		read(fd, NULL, 0);
		printf("end reading : process %i\n", getpid());
	}
	else if (argv[1][0] == '1') {
		write(fd, NULL, 0);
	}

	close(fd);
	return EXIT_SUCCESS;
}
