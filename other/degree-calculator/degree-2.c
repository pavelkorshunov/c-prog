#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	setlocale(LC_ALL, "RU");

	if(argc <= 1)
	{
		printf("Укажите параметр -c для градусов цельсия и -f для фаренгейт и введите число\n");
	}
	else
	{
		int c, degree, result;
		c = *++argv[1];
		degree = atoi(argv[2]);

		switch(c)
		{
			case 'c':
				result = (degree * 1.8) + 32;
				printf("%d градусов по цельсию равно %d по фаренгейту\n", degree, result);
				break;
			case 'f':
				result = (degree - 32) / 1.8;
				printf("%d градусов по фаренгейту равно %d по цельсию\n", degree, result);
				break;
			default:
				printf("Введенный вами параметр %c не поддерживается\n", c);
		}
	}
}