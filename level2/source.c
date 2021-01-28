#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void	p(void)
{
	char str[76];
	unsigned int address;

	fflush(stdout);
	gets(str);
	if (0xb0000000 == (address & 0xb0000000))
	{
		printf("(%p)\n", address);
		exit(1);
	}
	puts(str);
	strdup(str);
	return;
}


int		main(void)
{
	p();
	return(0);
}