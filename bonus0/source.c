#include <string.h>
#include <unistd.h>
#include <stdio.h>

void p(char *str, char *s)
{
	char buffer[4104];
	char *tmp;

	puts(s);
	read(0, buffer, 4096);
	*strchr(buffer, '\n') = '\0';
	strncpy(str, buffer, 20);
	return ;
}


void pp(char *str)
{
	char str1[20];
	char str2[20];
	p(str1, " - ");
	p(str2, " - ");
	strcpy(str, str1);
	str[strlen(str)] = ' ';
	strcat(str, str2);
	return;
}


int main()
{
	char str[30];

	pp(str);
	puts(str);
	return (0);
}
