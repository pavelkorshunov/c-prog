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

    printf("Name - %s price: %f\n", ball.name, ball.price);

    return 0;
}