/*
 * Copyright (C) 2023
 *
 * Created by Pavel K
 * 
 * Пример преобразования структур. Правильные и неправильные способы преобразования структур.
 */
#include <stdio.h> // printf()

struct user
{
    char name[20];
    char last_name[20];
    float weight;
};

struct short_user
{
    char name[20];
    char last_name[20];
};

// Пример структуры которую нельзя преобразовать в структуру user и наоборот.
// Поскольку для преобразования одной структуры в другую следует сохранять порядок полей структуры
// В данном случае поля weight и last_name будут конвертированы неверно
struct error_convert_user
{
    char name[20];
    float weight;
    char last_name[20];
};

int main(void)
{
    struct user friend = 
    {
        "Ivan",
        "Durov",
        70.88
    };

    // Первый способ преобразования. Пример преобразования одной структуры в другую

    struct short_user *convert_friend = (struct short_user*) &friend;

    printf("struct user.         Name - %s Last name - %s Weight: %f\n", friend.name, friend.last_name, friend.weight);
    
    printf("struct short_user.   Name - %s Last name - %s\n", convert_friend->name, convert_friend->last_name);

    // Такое преобразование структуры неверно заполнит поля weight и last_name при этом ошибки компиляции не будет

    struct error_convert_user *e_user = (struct error_convert_user*) &friend;

    printf("error_convert_user.  Name - %s Last name - %s Weight: %f\n", e_user->name, e_user->last_name, e_user->weight);

    // Второй способ преобразования структуры

    struct short_user convert_friend2 = *(struct short_user*) &friend;

    printf("struct short_user 2. Name - %s Last name - %s\n", convert_friend2.name, convert_friend2.last_name);

    // Третий способ преобразования структуры строки в структуру.
    // В этом случае строка будет помещена в первое поле структуры

    char buffer[1500] = "Ya N";
    struct user *friend2;
    friend2 = (struct user*) buffer;

    printf("struct user 2.       Name - %s Last name - %s\n", friend2->name, friend2->last_name);

    return 0;
}