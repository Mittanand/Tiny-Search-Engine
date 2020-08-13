# TSE Indexer Design Spec

### User Interface
The user inputs arguments into the command-line to produce output files from `indexer` and `indextest`. See README.md for more usage examples.

### Inputs and Outputs
Input: user specifies crawler directory and output file for `indexer`, and `indexer` produced file and output file for `indextest`. 

Output: The output document contains one word per line and one line per word, showing the word, the document id it was found on, and its frequency.

### Functional Decomposition into Modules
We have the following modules and functions

1. *main*, which validates command-line arguments and calls the functions to produce the output documents
2. *isCrawlerDirectory*, which determines if the given path is a valid crawler-produced directory
3. *pageLoader*, which returns the webpage associated with a file and
4. *indexBuilder*, which builds the index hashtable structure by reading from documents in the crawler directory
5. *indexSaver*, which converts the hashtable index structure to a file

### Pseudocode

The `indexer` runs as follows:

1. execute from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules
3. While there are documents to load in the crawler directory
  a) call *pageLoader* to obtain the webpage for the document
  b) add the word to the hashtable if new
  c) increment the count of the word
4. For every word in the hashtable, print the word, doc ID's in which it was found, and frequency per document


### Dataflow through modules

1. *main* parses parameters and passes them to the *indexBuilder*.
 2. *indexBuilder* takes webpages using *pageLoader* and creates the index hashtable
 3. *webage_getNextWord* allows us to iterate through the webpage and obtain each word to add to the hashtable
 4. *indexSaver* is able to convert the index structure into the formatted output file specified above

### Major data structures
1. *hashtable*: each key is a word and the items are counters
2. *counters*: contain integer key and count pairs for documents and frequency of word in respective document

### Testing Plan
See `testing.out`
