/*
 * crawler.c implements web crawler
 * 
 * usage: check IMPLEMENTATION.md
 *
 * CS50, Summer 2020
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"

//declaration
int crawler(hashtable_t *visited, bag_t *toCrawl, char *seedURL, char *pageDirectory, int maxDepth);

int main(int argc, char *argv[]) {
    
    if (argc != 4) {
        printf("Need 3 arguments\n");
        return 1;
    }

    char *seedURL = malloc(100*sizeof(char));
    int maxDepth;
    strcpy(seedURL, argv[1]);
    char *pageDirectory = argv[2];

    //validate seedURL
    if (!NormalizeURL(seedURL) || !IsInternalURL(seedURL)) {
        printf("seedURL is invalid\n");
        return 1;
    }

    //validate maxDepth
    if (sscanf(argv[3], "%d", &maxDepth) != 1) {
        printf("Max depth must be a number\n");
        return 1;
    }

    if (maxDepth < 0 || maxDepth > 10) {
        printf("Max depth must be between 0 and 10\n");
        return 1;
    }

    //validate and reformat directory with /
    char last = pageDirectory[strlen(pageDirectory) - 1];
    if (last != '/') {
        strncat(pageDirectory, "/", 2);
    }

    //.crawler is 8 + 1 end of char character = 9
    char *fpath = malloc((strlen(pageDirectory) + 9) * sizeof(char)); 
    if (fpath == NULL) {
        return 1;
    }

    //create file path in format directory/.crawler
    sprintf(fpath, "%s%s", pageDirectory, ".crawler");

    FILE* fp;
    if ((fp = fopen(fpath, "w")) == NULL) {
        printf("Invalid directory\n");
        free(fpath);
        return 1;
    }

    free(fpath);
    fclose(fp);

    //initialize structures
    bag_t *toCrawl = bag_new();
    if (toCrawl == NULL) {
        printf("Couldn't initialize bag\n");
        return 1;
    }

    hashtable_t *visited = hashtable_new(50);
    if (visited == NULL) {
        bag_delete(toCrawl, NULL);
        printf("Couldn't initialize hashtable\n");
        return 1;
    }

    return crawler(visited, toCrawl, seedURL, pageDirectory, maxDepth);
}

int 
crawler(hashtable_t *visited, bag_t *toCrawl, char *seedURL, char *pageDirectory, int maxDepth) {
    int id = 1;

    //initialize seed page
    webpage_t *seed = webpage_new(seedURL, 0, NULL);
    if (seed == NULL) {
        printf("Failed to make seed page\n");
        return 1;
    }

    //insert seed into bag and hashtable
    bag_insert(toCrawl, seed);
    hashtable_insert(visited, seedURL, "");

    //itearte through webpages to crawl
    webpage_t *curr;
    while ((curr = bag_extract(toCrawl)) != NULL) {
        if (pagefetcher(curr)) {  //fetch suceeds
            if (!pagesaver(curr, pageDirectory, id)) {
                //if save fails, clean and exit
                webpage_delete(curr);
                bag_delete(toCrawl, NULL);
                hashtable_delete(visited, NULL);
                printf("Failed to save page\n");
                return 1;
            }

            if (webpage_getDepth(curr) < maxDepth) {
                if (!pagescanner(curr, toCrawl, visited)) {
                    //if scan fails, clean and exit
                    webpage_delete(curr);
                    bag_delete(toCrawl, NULL);
                    hashtable_delete(visited, NULL);
                    printf("Failed to scan page\n");
                    return 1;
                }
            }

            //free page (key of bag and hashtable structs)
            webpage_delete(curr);
            //increment id
            id++;
        }
    }

    //clean
    hashtable_delete(visited, NULL); 
    bag_delete(toCrawl, NULL);
    return 0;
}