#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    extern int mult(int num1, int num2);
    extern int divide(int num1, int num2);
    extern int add(int num1, int num2);
    extern int sub(int num1, int num2);

    if (argc != 4){
        fprintf(stderr, "usage: %s num1 exression num2", argv[0]);
        exit(1);
    }

    if(strcmp(argv[2], "*") == 0){
        mult(atoi(argv[1]), atoi(argv[3]));
    }
    else if(strcmp(argv[2], "/") == 0){
        divide(atoi(argv[1]), atoi(argv[3]));
    }
    else if(strcmp(argv[2], "+") == 0){
        add(atoi(argv[1]), atoi(argv[3]));
    }
    else if(strcmp(argv[2], "-") == 0){
        sub(atoi(argv[1]), atoi(argv[3]));
    }
    else{
        fprintf(stderr, "usage: %s num1 exression num2", argv[0]);
        exit(1);
    }

    return 0;
}

int mult(int num1, int num2){
    int num = num1 * num2;

    printf("%d\n", num);
    return num;
}

int divide(int num1, int num2){
    if (num2 == 0){
        perror("Cannot divide by 0");
        exit(1);
    }

    int num = num1 / num2;

    printf("%d\n", num);
    return num;
}

int add(int num1, int num2){
    int num = num1 + num2;

    printf("%d\n", num);
    return num;
}

int sub(int num1, int num2){
    int num = num1 - num2;

    printf("%d\n", num);
    return num;
}