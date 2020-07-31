/* 
 * pagedir.c - CS50 'counters' module
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
