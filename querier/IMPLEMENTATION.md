# Querier Implementation Spec

### Querier Pseudocode
main
````
1. validate arguments
    a. Check for 2 arguments
    b. Ensure pageDir is crawler made
    c. Ensure indexFilename is readable
2. Initialize hashtable indexer storing indexFilename data from indexLoader
3. Call query on hashtable indexer and pageDir
4. Clean
````

querier
````
1. take input from user until ctrl+d
    a. validate input with wordcnt
    b. parse input with parseInp
    c. Used parsed input to create counters_t comp
    d. Pass comp to ranking to display documents sorted by score
````

parseInput
````
1. Iterate through characters in query
    a. Ensure characters are letters
    b. Store pointer to start of word
    c. Iterate through word
    d. Insert end of string char '\0' to end of word
2. Validate start and end of query (not "and" or "or")
3. Ensure no adjacent operators
4. Print query
````

makeCounters
````
1. Initialize structures holding counters for words
2. Store first word counters object
3. Iterate through parsed input
    a. For "or" operators union unless first "or"
    b. Store curr counters object in Intersect structure
    c. Intersect if no operators (like and)
4. If "or" in query, union counters
5. Otherwise, return counters struct in Intersect
````

ranking
````
1. Count documents in final counters struct
2. Make array of scores and doc ids
3. Sort array by score
4. Print scores
5. Clean 
````

sort
````
1. Iterate through scores array
    a. For each index, iterate through values from beginning to index
        i. Move smaller values to end portion
        ii. Decrement and continue
````


### Interfaces
`pagedir` allows for directory validation

`index` allows for loading indexFile to hashtable

`file.h` allows for reading lines from file

### Testing Plan
See `testing.sh` and `testing.out`
