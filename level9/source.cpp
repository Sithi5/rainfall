#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class N{
	public:

	int n;
	char str[108];

	N(int n){ this->n = n; }
	int operator+(N *param){ return (this->n + param->n); }
	int operator-(N *param){ return (this->n - param->n); }

	void setAnnotation(char *str){
		memcpy(this->str, str, strlen(str));
		return;
	}
};

int main(int ac, char **av)
{

	if (ac < 2) {
		exit(1);
	}
	N *class_1 = new N(5);
	N *class_2 = new N(6);
	class_1->setAnnotation(av[1]);
	class_2->operator+(class_1);
	return (0);
}