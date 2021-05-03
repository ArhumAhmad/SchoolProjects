#include <stdio.h>
#include <stdlib.h>

struct item {
    int key;
    int data;
    struct item *next;
};

struct item *head = NULL;

int main()
{
    extern void insert(int key, int data), delete(int key), printall();
    extern int search(int key);

    insert(38, 3);
    insert(20, 2);
    insert(5, 0);
    insert(22, 6);
    insert(46, 18);

    printf("With all five items:\n");
    printall();

/*

    printf("After delete(22):\n");
    delete(22);
    printall();
    printf("After delete(5):\n");
    delete(5);
    printall();
    printf("delete(30) shouldn't do anything:\n");
    delete(30);
    printall();

*/

    return(0);
}


void insert(int key, int data)
{
    struct item *node, **ptr;

    if((node = malloc(sizeof(struct item))) == NULL){
        perror("Ran out of memory for nodes!");
        exit(1);
    }
    node->data = data;
    node->key = key;

    ptr = &head;

    if(head == NULL || head->key >= key){
        ptr = NULL;
    }else{
        while((*ptr)->next != NULL && (*ptr)->key < key){
            ptr = &(*ptr)->next;
        }
    }

    if(ptr == NULL){
        node->next = head;
        head = node;
    }else{
        node->next = (*ptr)->next;
        (*ptr)->next = node;
    }
}



int search(int key) /* returns -1 if not found */
{
    struct item *p;
    for (p = head; p && p->key < key; p = p->next)
        ;
    if (p && p->key == key)
        return(p->data);
    else
        return(-1);
}


void printall()
{
    struct item *p;
    for (p = head; p; p = p->next)
        printf("%d: %d\n", p->key, p->data);
    printf("[end]\n");
}
