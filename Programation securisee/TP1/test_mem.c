#include <stdio.h>

int main(int argc, char const *argv[])
{
	int a = 0;
	char b = 'l';
	double c = 5.5;

	printf("%s\n%s\n%s\n", &a, &b, &c);
	return 0;
}