#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class A{
	private:
	unsigned int _n;
	char *_str;

	public:
	A(unsigned int n){
		this->_n = n;
	}

	void setAnnotation(char *str){
		memcpy(_str, str, strlen(str));
	}

	int operator+(A *param)
	{
		return this->_n + param->_n;
	}
};

int main(int ac, char **av)
{

	if (ac < 2) {
		exit(1);
	}
	A *class_1 = new A(5);
	A *class_2 = new A(6);
	class_1->setAnnotation(av[1]);
	class_2->operator+(class_1);
	return (0);
}