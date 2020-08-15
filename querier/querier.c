/* 
 * querier.c - reads index file produced by Indexer and answers search queries
 * 
 * see USAGE.md for more information
 *
 * CS50 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../libcs50/file.h"

typedef struct score {
    int id;
    int val;
} score_t;

typedef struct scoreArray {
    score_t (*scores)[];
    int occ;
} scoreArray_t;

bool query(hashtable_t *indexer, char *dir);
int cntNorm(char *inp);
bool parseInp(int cnt, char *inp, char *parsed[]);
counters_t *makeCounters(hashtable_t *indexer, int cnt, char *parsed[]);
void intersC(void *arg, const int key, int count);
void unionC(void *arg, const int key, int count);
void copyC(void *arg, const int key, int count);
bool ranking(counters_t *comp, char *dir);
void docCount(void *arg, const int key, int count);
scoreArray_t *scoreArray_new(int *cnt);
void makeArray(void *arg, const int key, int count);
score_t *score_new(const int id, int count);
void scoreInsert(scoreArray_t *scoreArray, score_t *score);
void scoreArrayDelete(scoreArray_t *scoreArray);
void sort(score_t (*scores)[], int *cnt);
bool printScore(score_t score, char *dir);


int main(int argc, char *argv[]) {
    
    //validate arguments
    if (argc != 3) {
        printf("Invalid number of args\n");
        return 1;
    }

    //validate pageDir and add '/' if necessary
    char *dir = malloc((2 + strlen(argv[1])) * sizeof(char));
    char last = (argv[1])[strlen(argv[1]) - 1];
    if(last != '/') {
        sprintf(dir, "%s%c", argv[1], '/');
    }
    else {
        sprintf(dir, "%s", argv[1]);
    }

    if (!isCrawlerDirectory(dir)) {
        printf("Invalid pageDirectory");
        free(dir);
        return 1;
    }

    //validate index file
    char *indexFile = argv[2];
    FILE *fp = fopen(indexFile, "r");
    if (fp == NULL) {
        printf("Invald file\n");
        free(dir);
        return 1;
    }

    //hashtable size based on lines in file
    hashtable_t *indexer = hashtable_new(2*lines_in_file(fp));
    fclose(fp);
    if (indexer == NULL) {
        printf("Couldn't initialize hashtable\n");
        free(dir);
        return 1;
    }

    //load to indexer from indexFile
    if (!indexLoader(indexer, indexFile)) {
        printf("Failed to load to indexer\n");
        free(dir);
        hashtable_delete(indexer, (void(*)(void *))counters_delete);
        return 1;
    }

    //pass indexer and dir and query
    if(!query(indexer, dir)) {
        printf("Failed to query\n");
        free(dir);
        hashtable_delete(indexer, (void(*)(void *))counters_delete);
        return 1;
    }

    //cleaning
    free(dir);
    hashtable_delete(indexer, (void(*)(void *))counters_delete);
    return 0;
}

/******************* query *******************/
/*
*Prompts user for queries, validates and parses
*Ranks pages and outputs pages and scores in sorted order
*/
bool query(hashtable_t *indexer, char *dir) {
    printf("Query? ");
    char *inp;
    //read from stdin
    while ((inp = freadlinep(stdin)) != NULL) {
        int wordcnt = cntNorm(inp);
        
        //if non-letter
        if (wordcnt == -1) {
            printf("Query contains non-letter\n");
        }

        //if blank query
        else if(wordcnt == 0) {
            printf("Query: \n");
        }

        else {
            //parse, make counters struct, and sort and display
            char *parsed[wordcnt];
            if(parseInp(wordcnt, inp, parsed)) {
                counters_t *comp = makeCounters(indexer, wordcnt, parsed);

                if (!ranking(comp, dir)) {
                    return false;
                }
                counters_delete(comp);
            }
        }

        //clean
        free(inp);
        printf("Query? ");
    }
    return true;
}  

/******************* cntNorm *******************/
/*
*Counts words in inp
*Normalizes characters in inp
*/
int cntNorm(char *inp) {
    int cnt = 0;

    //iterate through chars
    for(char *c = inp; *c != '\0'; c++) {
        //if non-letter
        if(!isalpha(*c) && !isspace(*c)) {
            return -1;
        }
        
        //normalize
        if (isalpha(*c)) {
            if (isupper(*c)) {
                *c = tolower(*c);
            }

            //go to end of word and normalize
            while (isalpha(*(c+1))) {
                if (isupper(*(c+1))) {
                    *(c+1) = tolower(*(c+1));
                }
                c++;
            }
            //increment word count
            cnt++;
        }
    }
    return cnt;
}

/******************* parseInp *******************/
/*
*Iterates through inp and parses each word and validates query
*/
bool parseInp(int cnt, char *inp, char *parsed[]) {
    int idx = 0;

    for (char *c = inp; *c != '\0'; c++) {
        if (isalpha(*c)) { //new word
            parsed[idx] = c;
            idx++;
            
            //go to end of word
            while (isalpha(*(c+1))) {   
                c++;
            }

            //signify end of word
            *(c+1) = '\0';  
            c++;
        }
    }

    //check beginning and end of query
    if ((strcmp(parsed[0], "and") == 0) || (strcmp(parsed[0], "or") == 0) || (strcmp(parsed[cnt - 1], "and") == 0) || (strcmp(parsed[cnt - 1], "or") == 0)) {
        printf("Cannot begin or end with \"and\" or \"or\"\n");
        return false;
    }

    //check for adjacent "and" and "or"
    for (int i = 1; i <= cnt - 3; i++) {
        if (((strcmp(parsed[i], "and") == 0) || (strcmp(parsed[i], "or") == 0)) && ((strcmp(parsed[i+1], "and") == 0) || (strcmp(parsed[i+1], "or") == 0))) {
            printf("Cannot have adjacent \"and\" and \"or\"\n");
            return false;
        }
    }

    printf("Query:");
    for (int i = 0; i < cnt; i++) {
        printf(" %s", parsed[i]);
    }
    printf("\n");

    return true;
}

/******************* makeCounters *******************/
/*
*Creates counters struct based on operators and query
*Uses intersect and union functions to merge and combine counters structs
*/
counters_t *makeCounters(hashtable_t *indexer, int cnt, char *parsed[]) {
    counters_t *toUnion;
    counters_t *toIntersect[2];

    toIntersect[0] = counters_new();
    counters_iterate(hashtable_find(indexer, parsed[0]), toIntersect[0], copyC);

    //boolean val for "or" with second word seen
    bool seen = false;

    for (int i = 1; i < cnt; i++) {
        //no "and" seen
        if (strcmp(parsed[i], "and") != 0) {
            //"or" seen    
            if (strcmp(parsed[i], "or") == 0) { 
                //already have seen second word after or, so unionHelp not empty
                if (seen) { 
                    //union and clean
                    counters_iterate(toIntersect[0], toUnion, unionC);
                    counters_delete(toIntersect[0]);
                }
                //haven't seen second word after or
                else {
                    toUnion = toIntersect[0];
                    seen = true;
                }
                toIntersect[0] = counters_new();
                counters_iterate(hashtable_find(indexer, parsed[i+1]), toIntersect[0], copyC);
            }
            //no "or" seen
            else {
                toIntersect[1] = hashtable_find(indexer, parsed[i]);
                counters_iterate(toIntersect[0], toIntersect, intersC);
            }
        }
    }

    if (seen) {
        counters_iterate(toIntersect[0], toUnion, unionC);
        counters_delete(toIntersect[0]);
    }
    else {
        toUnion = toIntersect[0];
    }

    //clean
    counters_t *comp = counters_new();
    counters_iterate(toUnion, comp, copyC);
    counters_delete(toUnion);
    return comp;
    
}

/******************* intersC *******************/
/*
*creates intersection of counters struct with min count
*/
void intersC(void *arg, const int key, int count) {
    counters_t **both = arg;

    //update to smaller count
    if (counters_get(both[1], key) < count) {
        counters_set(both[0], key, counters_get(both[1], key));
    }
}

/******************* unionC *******************/
/*
*creates union of counters struct with combined count
*/
void unionC(void *arg, const int key, int count) {
    counters_t *curr = arg;

    //sum counts
    counters_set(curr, key, count+counters_get(curr, key));
}

/******************* copyC *******************/
/*
*copies key and count into arg
*/
void copyC(void *arg, const int key, int count) {
    counters_t *copy = arg;

    if(count != 0) {
        counters_set(copy, key, count);
    }
}

/******************* ranking *******************/
/*
*stores scores and ids in array, sorts, and prints results
*/
bool ranking(counters_t *comp, char *dir) {
    int *cnt = malloc(sizeof(int));
    *cnt = 0;

    //count documents
    counters_iterate(comp, cnt, docCount);

    //if documents match query
    if(*cnt != 0) {
        printf("Matches %d documents (ranked):\n", *cnt);

        //array of scores and ids
        scoreArray_t *scoreArray = scoreArray_new(cnt);
        if(scoreArray == NULL) {
            return false;
        }

        //create array and sort
        counters_iterate(comp, scoreArray, makeArray);
        sort(scoreArray->scores, cnt);

        score_t (*scores)[] = scoreArray->scores;

        //display scores
        for(int i = 0; i < *cnt; i++) {
            score_t s = (*scores)[i];
            if(!printScore(s, dir)) {
                return false;
            }
        }
        //clean
        scoreArrayDelete(scoreArray);
    }
    else {
        printf("No documents \n");
    }
    free(cnt);
    return true;
}

/******************* docCount *******************/
/*
*count docs in counters struct
*/
void docCount(void *arg, const int key, int count) {
    int *cnt = arg;
    (*cnt)++;
}

/******************* scoreArray_new *******************/
/*
*malloc new scoreArray struct
*/
scoreArray_t *scoreArray_new(int *cnt) {
    scoreArray_t *scoreArray = malloc(sizeof(scoreArray_t));
    if (scoreArray == NULL) {
        printf("Failed to make scoreArray\n");
        return NULL;
    }

    scoreArray->scores = calloc(*cnt, sizeof(score_t));
    scoreArray->occ = 0;

    return scoreArray;
}

/******************* makeArray *******************/
/*
*initialize scoreArray
*/
void makeArray(void *arg, const int key, int count) {
    scoreArray_t *scoreArray = arg;

    score_t *score = score_new(key, count);
    scoreInsert(scoreArray, score);

    free(score);
}

/******************* score_new *******************/
/*
*mallco and initialize score struct
*/
score_t *score_new(const int id, int count) {
    score_t *score = malloc(sizeof(score_t));
    if(score == NULL) {
        printf("Couldn't make score struct\n");
        return NULL;
    }
    score->id = id;
    score->val = count;

    return score;
}

/******************* scoreInsert *******************/
/*
*insert score into scoreArray
*/
void scoreInsert(scoreArray_t *scoreArray, score_t *score) {

    //point to scores struct in scoreArray and insert score
    score_t (*scores)[] = scoreArray->scores;
    (*scores)[scoreArray->occ] = *score;

    (scoreArray->occ)++;
}

/******************* scoreArrayDelete *******************/
/*
*clean
*/
void scoreArrayDelete(scoreArray_t *scoreArray) {
    free(scoreArray->scores);
    free(scoreArray);
}

/******************* sort *******************/
/*
*insertion sort algorithm in descending order
*/
void sort(score_t (*scores)[], int *cnt) {
    score_t curr;

    for(int i = 1; i < *cnt; i++) {
        curr = (*scores)[i];
        int j = i - 1;

        while(j >= 0 && ((*scores)[j].val < curr.val)) {
            (*scores)[j+1] = (*scores)[j];
            j = j - 1;
        }
        (*scores)[j+1] = curr;
    }
}

/******************* printScore *******************/
/*
*Display score, doc id, and url
*/
bool printScore(score_t score, char *dir) {
    char *fpath = malloc((strlen(dir) + 9) * sizeof(char)); 
    if (fpath == NULL) {
        printf("Failed to allocate space for fpath");
        return false;
    }
    if (sprintf(fpath, "%s%d", dir, score.id) < 0) {
        printf("Failed to write to fpath \n");
        free(fpath);
        return false;
    }

    FILE *fp = fopen(fpath, "r");
    if (fp == NULL) {
        printf("Couldn't read file\n");
        free(fpath);
        return false;
    }

    char *url = freadlinep(fp);
    printf("score   %d doc   %4d: %s\n", score.val, score.id, url);

    fclose(fp);
    free(fpath);
    free(url);
    return true;
}



