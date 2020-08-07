/* 
 * index.c - functionality in common lib for indexer
 *
 * see index.h for more information.
 *
 * CS50 
 */

#include <stdio.h>
#include <stdlib.h>
#include "index.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"

/**************** indexSaver ****************/
bool
indexSaver(hashtable_t *indexer, char* file) {
    FILE *fp = fopen(file, "w");
    if(fp == NULL) {
        printf("Invalid file to save to\n");
        return false;
    }

    //iterate with helper function
    hashtable_iterate(indexer, fp, hthelp);
    fclose(fp);
    return true;
}

/**************** hthelp ****************/
void 
hthelp(void *arg, const char *key, void *item) {
    FILE *fp = arg;
    counters_t *ctrs = item;

    fprintf(fp, "%s ", key);
    counters_iterate(ctrs, fp, countershelp);
    fprintf(fp, "\n");
}

/**************** countershelp ****************/
void 
countershelp(void *arg, const int key, int count) {
    FILE *fp = arg;
    fprintf(fp, "%d %d ", key, count);
}

/**************** indexLoader ****************/
bool
indexLoader(hashtable_t *indexer, char* file) {
    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        printf("Invalid file to load from \n");
        return false;
    }

    char *curr;
    while ((curr = freadwordp(fp)) != NULL) {

        //new counters struct for every word in file
        counters_t *new = counters_new();
        if (new == NULL) {
            printf("Couldn't initialize counters struct\n");
            free(curr);
            return false;
        }
        //create new item in hashtable for word
        hashtable_insert(indexer, curr, new);

        int key;
        int count;
        //while key count pairs still exist for curr
        while (fscanf(fp, "%d %d ", &key, &count) == 2) {
            counters_set(hashtable_find(indexer, curr), key, count);
        }
        free(curr);
    }
    
    fclose(fp);
    return true;
}