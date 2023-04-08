#include "static_allocation.h"
#include <stdio.h>

#define LINE    "-------------------------------------------------------------------------\n"
#define HEADING(message)    printf("\nAction  : %s \n", message);\
                            printf(LINE)

int main(){

    // String allocation
    HEADING("Allocating a string ");

    char *hello = static_malloc(sizeof("hello"));

    printf("La taille de la chaine est  : %zu \n", sizeof("hello"));
    printf("L'adresse de la chaine est  : %p \n", (void*)(hello - SIZE_OF_HEADING) );

    for(size_t i = 0; i < sizeof("hello"); i++)
        hello[i] = "hello"[i];
    
    printf("La chaine contient  : %s\n", hello);


    // Allocate an array of integers 
    HEADING("Allocating an array of int");

    int *one2four = static_malloc(4 * sizeof(int));

    printf("La taille du tableau d'entier  : %zu\n", 4U);
    printf("L'adresse du tableau d'entier  : %p \n", (void*)((char*)one2four - SIZE_OF_HEADING));

    for(size_t i = 0; i < 4; i++)
        one2four[i] = i+1;
    
    for(size_t i = 0; i < 4; i++)
        printf("Tableau[%d]  = %d\n", i, one2four[i]);
    

    // Allocate a float 
    HEADING("Allocating a float");

    float * five = static_malloc(sizeof(float));

    *five = 5.;

    printf("L'adresse du variable float est  : %p \n", (void*)((char*)five - SIZE_OF_HEADING));
    printf("La variable float contient  : %f\n", *five);


    //free  int[4] : one2four
    HEADING("free the previous int array");

    static_free(one2four);


    //Allocate a String
    HEADING("Allocating a string that must be stored on the previous freed space");

    char *hello_world = static_malloc(sizeof("hello world"));

    printf("La taille de la chaine est  : %zu \n", sizeof(hello_world));
    printf("L'adresse de la chaine est  : %p \n", (void*)(hello_world - SIZE_OF_HEADING));

    for(size_t i = 0; i < sizeof("hello world"); i++)
        hello_world[i] = "hello world"[i];
    
    printf("La chaine contient  : %s\n", hello_world);





}