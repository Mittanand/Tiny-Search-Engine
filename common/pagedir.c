/* 
 * pagedir.c - CS50 common library functionality
 *
 * see pagedir.h for more information.
 *
 * CS50 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

/**************** pagefetcher ****************/
bool 
pagefetcher(webpage_t *page) {
    return webpage_fetch(page);
}


/**************** pagescanner ****************/
bool 
pagescanner(webpage_t *page, bag_t *toCrawl, hashtable_t *visited) {
    char *res;
    int pos = 0;

    //iterate through URLs on page
    while((res = webpage_getNextURL(page, &pos)) != NULL) {
        //check if valid and not visited
        if (NormalizeURL(res) && IsInternalURL(res) && hashtable_insert(visited, res, "")) {
            //create new page
            webpage_t *next = webpage_new(res, webpage_getDepth(page)+1, NULL);
            if (next == NULL) {
                printf("failed to make new page\n");
                return false;
            }
            //insert in bag for crawling
            bag_insert(toCrawl, next);
        }
    }
    return true;
}

/**************** pagesaver ****************/
bool
pagesaver(webpage_t *page, char *pageDirectory, int id) {
    char *fpath = malloc((strlen(pageDirectory)+11)*sizeof(char));   //accounting for large int id's
    //check if file path is valid and writable
    if (fpath == NULL) {
        fprintf(stderr, "Couldn't allocate space for file pathname\n");
        return false;
    }

    //create path of format pageDirectory/id
    if (sprintf(fpath, "%s%d", pageDirectory, id) < 0) {
        fprintf(stderr, "Failed to write to fpath\n");
        return false;
    }

    FILE *fp;
    if ((fp = fopen(fpath, "w")) == NULL) {
        fprintf(stderr, "Couldn't open file");
        free(fpath);
        return false;
    }

    //save page info in file named id
    fprintf(fp, "%s\n%d\n%s\n", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
    fclose(fp);
    free(fpath);
    return true;
}

bool 
isCrawlerDirectory(char *dir) {
    //check if ./crawler is in dir, meaning it is crawler produce
    char *fpath = malloc((strlen(dir) + 9) * sizeof(char)); 
    if (fpath == NULL) {
        return false;
    }
    //create path
    sprintf(fpath, "%s%s", dir, ".crawler");

    //see if file is valid
    FILE* fp;
    if ((fp = fopen(fpath, "w")) == NULL) {
        printf("Invalid directory\n");
        free(fpath);
        return false;
    }
    free(fpath);
    fclose(fp);
    return true;
}

webpage_t*
pageLoader(char *dir, int id) {
    char *fpath = malloc((strlen(dir)+11)*sizeof(char));   //accounting for large int id's
    //check if file path is valid and writable
    if (fpath == NULL) {
        fprintf(stderr, "Couldn't allocate space for file pathname\n");
        return NULL;
    }

    //create path of format dir/id
    if (sprintf(fpath, "%s%d", dir, id) < 0) {
        fprintf(stderr, "Failed to write to fpath\n");
        free(fpath);
        return NULL;
    }
    FILE *fp = fopen(fpath, "r");
    if (fp == NULL) {
        free(fpath);
        return NULL;
    }

    //initialize items held in webpage struct
    char *url = freadlinep(fp);
    char *depth = freadlinep(fp);
    char *html = freadfilep(fp);
    int maxDepth;
    //if fail to scan to maxDepth int
    if(sscanf(depth, "%d", &maxDepth) == 0) {
        fprintf(stderr, "Failed to get maxDepth\n");
        free(url);
        free(depth);
        free(html);
        return NULL;
    }

    //if webpage cannot be initialized
    webpage_t *loaded = webpage_new(url, maxDepth, html);
    if (loaded == NULL) {
        fprintf(stderr, "Failed to get maxDepth\n");
        free(url);
        free(depth);
        free(html);
        return NULL;
    }

    //cleaning
    free(depth);
    free(fpath);
    fclose(fp);

    return loaded;

}