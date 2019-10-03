#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE * fp = fopen("test.txt", "rw");
    if(fp == NULL) {
        printf("Can not open file\n");
        exit(1);
    }

    fputs("Hello File", fp);

    char str[20];
    fgets(str, 18, fp);

    printf("%s\n", str);

    fclose(fp);
    
    return 0;
}