#include <stdio.h>
#include <stdlib.h>

int main()
{
    char s[100];

    fgets(s, 99, stdin);
    if (atoi(s) == 12)
        printf("do something\n");
    printf("in any case, do something else\n");
    return(0);
}