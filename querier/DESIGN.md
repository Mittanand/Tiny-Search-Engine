# TSE Querier Design Spec

### User interface
Querier begins by prompting for a `pageDirectory` created by crawler and an `indexFilename` created by indexer. 

Thus, commandline arguments have the format `./querier pageDirectory indexFilename`. 

After these argument, the user is prompted for words to query, and documents matching the query are returned. The querying process ends once the user enters `ctrl + d`.

### Inputs and outputs
Input: the inputs are the commandline parameters and words to query as described above.

Output: Each query returns a set of documents showing what url contains the query as well as how often the query occurs on that url in sorted order.

### Functional Decomposition into Modules
We anticipate the following modules or functions:

1. *main*, which parses arguments and initializes other modules
2. *query*, which reads in queries from stdin, parses them, stores them in a counters structure, and prints in sorted order
3. *parseInp*, which takes a query and parses it according to the "and" and "or" operators
4. *makeCounters*, which creates a counters structure storing document id's and their respective scores
5. *ranking*, which takes the counter structure and prints document ids, urls, and scores in sorted order

And some helper modules that provide data structures:

1. *counters* storing document ids and scores
2. *hashtable* of words and their counters structure


### Pseudo code for logic/algorithmic flow
The querier will run as follows:

1. execute from commandline as shown in UI
2. validate parameters and initialize other modules
3. take in user queries
    a. validate the query
    b. parse the query
        i. store pointer to each word in array
        ii. validate words
    b. create a counters object based on parsed query
        i. union or intersect counters objects based on operators
        ii. return final counters structure with document ids and scores
    c. print appropriate documents in order
        i. sort scores
        ii. print scores

### Dataflow through modules
1. *main* parses parameters and passes them to hashtable
2. *query* reads inputs from user, parsing input and making counters structure
3. *parseInp* takes line of input and creates array of words
4. *makeCounters* processes operators to create final counters structure
5. *ranking* takes the final counters structure and sorts and outputs the documents and scores

### Major Data Structures
1. *counters* storing document ids and scores
2. *hashtable* of words and their counters structure
3. *set* of counters (indirectly used by hashtable)

### Testing plan
See `testing.sh` and `testing.out`