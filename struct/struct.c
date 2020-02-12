#include <stdio.h>

struct product
{
    char name[20];
    float price;
};

int main()
{
    struct product ball = 
    {
        "Ball",
        1.88
    };

    struct product *fake_ball = &ball;

    // Доступ к элементам структуры
    printf("Name - %s price: %f\n", ball.name, ball.price);
    // Доступ к элементам структуры типа указатель
    printf("Name - %s price: %f\n", fake_ball->name, fake_ball->price);

    return 0;
}