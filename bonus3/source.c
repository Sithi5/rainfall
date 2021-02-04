#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(int ac, char **av)
{
	FILE *fd;
	char str[24];
	char str2[66];
	int nb;
	
	fd = fopen("/home/user/end/.pass", "r");
	bzero(str, 33);
	if (fd == 0 || ac != 2)
		return(0xffffffff);
	fread(str, 1, 66, fd);
	nb = atoi(av[1]);
	str[nb] = 0;
	fread(str2, 1, 65, fd);
	fclose(fd);
	if (strcmp(str, av[1]) == 0)
		execl("/bin/sh", "sh", 0);
	else
		puts(str2);
	return(0);
}