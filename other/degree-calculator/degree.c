#include <stdio.h>

int main()
{
	// Перевод из градусов цельсия в фаренгейт
	int degreec, resultc;

	printf("Enter the degree ");

	scanf("%d", &degreec);

	resultc = (degreec * 1.8) + 32;
	printf("Degrees Fahrenheit %d\n", resultc);

	// Перевод из фаренгейта в цельсия
	float degreef, resultf;

	printf("Enter the degree F ");

	scanf("%f", &degreef);
	resultf = (degreef - 32) / 1.8;

	printf("Degrees Celsius %f\n", resultf);
}