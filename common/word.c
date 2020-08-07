/* 
 * word.c - CS50 common library functionality
 *
 * see word.h for more information.
 *
 * CS50 
 */

#include <stdlib.h>
#include <ctype.h>
#include "word.h"


/**************** normalizeWord ****************/
char *
normalizeWord(char *word) {
    char *temp;
    if (word != NULL) {
        //iterate through characters until reach end of word
        for (temp = word; *temp != '\0'; temp++) {
            *temp = tolower(*temp);
        }
    }
    return word;
}
