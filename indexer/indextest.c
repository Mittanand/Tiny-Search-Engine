/*
*
*indextest.c - reverses indexer process by taking output and 
*converting to index structure in new file
*
*usage: see README.md
*
*CS50
*
*/

#include <stdio.h>
#include <stdlib.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "../common/index.h"

int main(int argc, char *argv[]) {
    //check number of args
    if (argc != 3) {
        printf("Must have 2 arguments\n");
        return 1;
    }

    char *file1 = argv[1];
    char *file2 = argv[2];

    //check if file1 valid
    FILE* fp = fopen(file1, "r");
    if (fp  == NULL) {
        printf("File is not readable\n");
        return 1;
    }
    fclose(fp);

    hashtable_t *indexer = hashtable_new(200);
    if (indexer == NULL) {
        printf("Failed to initialize hashtable\n");
        return 1;
    }

    if(!indexLoader(indexer,file1)) {
        hashtable_delete(indexer, (void(*)(void *))counters_delete);  
        return 1;  
    }

     if(!indexSaver(indexer, file2)) {
        hashtable_delete(indexer, (void(*)(void *))counters_delete);  
        return 1;  
    }

    //clean
    hashtable_delete(indexer, (void(*)(void *))counters_delete);
    return 0;  
}