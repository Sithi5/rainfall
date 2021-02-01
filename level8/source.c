#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char *service;
char *auth;


int main(int ac, char **av)
{
	char str[128];

	while(true) {
		printf("%p, %p \n", auth, service);
		if (fgets(str, 128, stdin) != 0)
			break ;
		if (strncmp(str, "auth ", 5) == 0)
		{
			auth = (char *)malloc(4);
			*auth = 0;
			if (strlen(str + 5) < 31){
				strcpy(auth, str + 5);
			}
		}
		else if (strncmp(str, "reset", 5) == 0)
		{
			free(auth);
		}
		else if (strncmp(str, "service", 7) == 0)
		{
			service = strdup(str + 7);
		}
		else if (strncmp(str, "login", 5) == 0)
		{
			if (*auth + 32 != 0)
				system("/bin/sh");
			else
				fwrite("Password:\n", 1, 10, stdout);
		}
	}
	return (0);
}
