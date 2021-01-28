#include <stdio.h>

unsigned int m;

void	v(void)
{
	char str[520];
	fgets(str, 512, stdin);
	printf(str);
	if (m == 64){
		fwrite("Wait what?!\n", 1, 12, stdout);
		system("/bin/sh");
	}
	return;
}


int		main(void)
{
	v();
	return(0);
}