#include <stdio.h>
#include <stdlib.h>

void	n(void)
{
	system("/bin/cat /home/user/level7/.pass");
	return;
}

void		m(void)
{
	puts("Nope");
	return;
}


int		main(int ac , char **av)
{
	char *str;
	void (*f)(void);

	str = (char *)malloc(64);
	f = (void *)malloc(4);
	f = &m;
	strcpy(str, av[1]);
	f();
	return(0);
}