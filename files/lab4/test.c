#include <stdio.h>
#include <string.h>
#include <unistd.h>

int PKCS7Padding(unsigned char *str)
{
    int remain, i;
    int len = strlen(str);
    printf("len = %d\n", len);
    remain = 16 - len % 16;
    printf("remain = %d\n", remain);
    for (i = 0; i < remain; i++)
    {
        str[len + i] = 'a';
        printf("str[len+i] = %d\n", str[len + i]);
    }
    str[len + i] = '\0';

    return len + remain;
}

int main(void)
{
    char str[10];
    for (int i = 0; i < 10; i++)
    {
        str[i] = 97 + i;
    }
    str[10] = '\0';
    int a = PKCS7Padding((unsigned char *)str);
    printf("%d\n", a);
    printf("%s", str);
}