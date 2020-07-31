/*
*
* pagedir.h - header file for CS50 pagedir module
*
* Provides functionality for crawler.c
*
* See implementation.md for more information
*
* CS50
*
*/

#ifndef __PAGEDIR_H
#define __PAGEDIR_H


#include <stdio.h>
#include <stdlib.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"

/**************** functions ****************/


/**************** pagefetcher ****************/
/*
*Fetches page from provided webpage using url
*Stores html into webpage struct
*/
bool pagefetcher(webpage_t *page);



/**************** pagescanner ****************/
/*
*Extracts and processes each URL from a page, given page, hashtable, and bag struct
*Adds valid page to hashtable
*Adds valid page to bag for later crawling
*If any url invalid, then skipped
*/
bool pagescanner(webpage_t *page, bag_t *toCrawl, hashtable_t *visited);


/**************** pagesaver ****************/
/*
*Outputs page to appropriate file id
*File contents as specified in implementation
*/
bool pagesaver(webpage_t *page, char *pageDirectory, int id);




#endif //__PAGEDIR_H
