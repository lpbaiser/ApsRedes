/* 
 * File:   fileRead.c
 * Author: Leonardo Baiser <lpbaiser@gmail.com>
 *
 * Created on December 4, 2015, 2:40 PM
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char** readFileByteStuffing(char* path) {
    FILE* file;

    if ((file = fopen(path, "r")) == NULL) {
        printf("Erro ao abrir o arquivo");
        return NULL;
    }

    char** bytes;
    bytes = (char**) calloc(10, sizeof (char*));
    char c;
    int i = 0;

    while ((c = fgetc(file)) != EOF) {
        printf("char: %s", &c);
        bytes[i] = (char*) malloc(sizeof (char));
        strcpy(&bytes[0][i], &c);
        //        bytes[i] = c;
        i++;
    }

    //    printf("tam i: %d -", i);
    //    printf("%s \n", &bytes[0][1]);
    //    printf("%s", bytes[0]);
    //    printf("%s", bytes[1]);
    //    printf("%s", bytes[2]);



    fclose(file);
    return bytes;
}
