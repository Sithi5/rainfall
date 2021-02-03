#include <stdio.h>
#include <stdlib.h>

char *c[78];

void	m(void)
{
	printf("%s - %d\n", c, time(0));
	return;
}

int		main(int ac, char **av)
{
	char *str1;
	void *str2;
	char *str3;
	FILE *file;

	str1 = (char *)malloc(8);
	*str1 = 1;
	str2 = (void *)malloc(8);
	str1[1] = str2;
	str3 = (char *)malloc(8);
	*str3 = 2;
	str2 = (void *)malloc(8);
	str3[1] = str2;
	strcpy(str1[1], av[1]);
	strcpy(str3[1], av[2]);
	file = fopen("/home/user/level8/.pass", "r");
	fgets(c, 68, file);
	puts("~~");
	return(0);
}
