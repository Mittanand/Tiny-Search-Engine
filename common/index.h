/*
*
* index.h - header file for CS50 index.c module adding functionality to common library
*
* Provides word normalization
*
*
* CS50
*
*/

#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"


/**************** indexSaver ****************/
/*
*Creates index of words to documents and count
*Saves index struct to file
*/  
bool indexSaver(hashtable_t *indexer, char* file);


/**************** hthelp ****************/
/*prints key and calls function to print counters item with key and count
*used in hashtable_iterate
*/
void hthelp(void *arg, const char *key, void *item);


/**************** countershelp ****************/
/*prints key and item of counters element 
*passed into counters_iterate
*/
void countershelp(void *arg, const int key, int count);


/**************** indexLoader ****************/
/*
*Reads from hashtable to construct index struct
*Opposite of indexSaver
*/ 
bool indexLoader(hashtable_t *indexer, char* file);


#endif //__INDEX_H
