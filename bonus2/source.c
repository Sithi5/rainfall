int language;

void greetuser(char *str){
	char s[88];

	if (language == 0){
		strcpy(s, "Hello ");
	}
	else if (language == 1)
	{
		strcpy(s, "Hyvää päivää ");
	}
	else if (language == 2)
	{
		strcpy(s, "Goedemiddag! ");
	}
	strcat(s, str);
	puts(s);
	return;
	}

int main(int ac, char **av)
{
	char *var;
	char str[80];
	char *result;

	if (ac == 3)
	{
		bzero(str, 19);
		strncpy(str, av[1], 40);
		strncpy(*(str + 40), av[2], 32);
		var = getenv("LANG");
		if (var)
		{
			if (memcmp(var, "fi", 2))
				language = 1;
			else if (memcmp(var, "nl", 2))
				language = 2;
		}
		strncpy(result, str, 19);
		greetuser(result);
		return(0);
	}
	return(1);
}