#include <string.h>
#include <unistd.h>
#include <stdlib.h>


int main(int ac, char **av){
	int n;
	char str[40];
	
	n = atoi(av[1]);
	if (n < 10)
	{
		memcpy(str, av[2], n * 4);
		if (n == 1464814662)
			execl("/bin/sh", "sh", 0);
		return(0);
	}
	return(1);
}