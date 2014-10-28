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
#include <sys/ioctl.h>

int main(void) {
	int fd = open("/dev/scull0", O_RDWR);
	int count;
	char buf[] = "le chat courait dans la prairie";
	char buf2[30] = {'\0'};
	unsigned long quantum;

	if (fd == -1)
		perror("opening scull0");

	count = write(fd, buf, sizeof(buf));
	printf("%d bytes wrote\n", count);

	lseek(fd, -32, SEEK_CUR);
	count = read(fd, buf2, sizeof(buf2));
	buf2[29] = '\0';
	printf("%d bytes read\nbuf2 =\"%s\"\n", count, buf2);
	count = read(fd, buf2, sizeof(buf2));
	buf2[29] = '\0';
	printf("%d bytes read\nbuf2 =\"%s\"\n", count, buf2);
	count = read(fd, buf2, sizeof(buf2) - 8);
	buf2[sizeof(buf2) - 8 - 1] = '\0';
	printf("%d bytes read\nbuf2 =\"%s\"\n", count, buf2);
	count = read(fd, buf2, sizeof(buf2));
	buf2[sizeof(buf2) -1] = '\0';
	printf("%d bytes read\nbuf2 =\"%s\"\n", count, buf2);

	ioctl(fd, 65024);
	perror("ioctl");

	ioctl(fd, 2147810817, &quantum);
	perror("ioctl");
	printf("quantum = %lu\n", quantum);

	close(fd);
	return EXIT_SUCCESS;
}
