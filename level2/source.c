#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char*	p(void)
{
	char str[76];
	unsigned int *address;

	fflush(stdout);
	gets(str);
	if (0xb0000000 == (*address & 0xb0000000))
	{
		printf("(%p)\n", address);
		exit(1);
	}
	puts(str);
	return(strdup(str));
}


int		main(void)
{
	p();
	return(0);
}