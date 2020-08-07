/*
*
*indexer.c - converts documents created by crawler into index structure 
*mapping words to documents
*
*usage: see README.md
*
*CS50
*
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../common/index.h"

bool indexBuilder(hashtable_t *indexer, char *dir);

int main(int argc, char *argv[]) {
    
    //check number of args
    if (argc != 3) {
        printf("Must have 2 arguments\n");
        return 1;
    }

    //make crawldir to add '/' to end of dir
    char *crawldir = malloc((strlen(argv[1]) + 2) * sizeof(char));

    //first add "/" to pageDir, if needed
    char lastChar = (argv[1])[strlen(argv[1]) - 1];
    if (lastChar != '/') {
        sprintf(crawldir, "%s%s", argv[1], "/");
    }
    else {
        sprintf(crawldir, "%s", argv[1]);
    }

    //check directory to read from
    if (!isCrawlerDirectory(crawldir)) {
        free(crawldir);
        return 1;
    }

    //check file to write to
    char *file = argv[2];
    FILE *fp = fopen(file, "w");
    if(fp == NULL) {
        printf("File not writeable\n");
        free(crawldir);
        return 1;
    }
    fclose(fp);

    //initialize table
    hashtable_t *indexer = hashtable_new(200);
    if (indexer == NULL) {
        printf("Couldn't initialize hashtable\n");
        free(crawldir);
        return 1;
    }

    if (!indexBuilder(indexer, crawldir)) {
        free(crawldir);
        //cast counters_delete
        hashtable_delete(indexer, (void(*)(void *))counters_delete);
        return 1;
    }

    if (!indexSaver(indexer,file)) {
        free(crawldir);
        hashtable_delete(indexer, (void(*)(void *))counters_delete);
        return 1;
    }

    //clean
    free(crawldir);
    hashtable_delete(indexer, (void(*)(void *))counters_delete);
    return 0;

}

bool
indexBuilder(hashtable_t *indexer, char *dir) {
    webpage_t *new;
    int id = 1; 

    while ((new = pageLoader(dir, id)) != NULL) {
        int pos = 0;
        char *curr;
        
        //iterate through words
        while ((curr = webpage_getNextWord(new, &pos)) != NULL) {
            if (strlen(curr) > 2) {
                normalizeWord(curr);
                //if not already in hashtable
                if(hashtable_find(indexer, curr) == NULL) {
                    //new counters struct
                    counters_t *newc = counters_new();
                    if (newc == NULL) {
                        printf("Failed to initialize new counters struct\n");
                        free(curr);
                        webpage_delete(new);
                        return false;
                    }
                    //insert new struct into table
                    hashtable_insert(indexer, curr, newc);
                }
                //increment count
                counters_add(hashtable_find(indexer, curr), id);
            }
            //clean
            free(curr);
        }
        id++;
        webpage_delete(new);               
    }
    return true;
}



