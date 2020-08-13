# Indexer Implementation Spec

### Indexer Pseudocode
````
1. Validate arguments in main
  a) check for 2 arguments
  b) ensure directory is crawler produced
2. Initialize hashtable struct representing index
3. Run indexBuilder
  a) Iterate through pages in pageDirectory
    i) Iterate through words in each page
    ii) if word is new, add to table
    iii) increment count of word
    iv) Free word
  b) increment id and free page after each iteration
4. Run indexSaver
  a) Call hashtable_iterator to write hashtable to file in format word id count id count ...
````

### Indextest Pseudocode
````
1. Validate arguments in main
2. Ensure first argument file is readable 
3. Initialize hashtable index structure
4. Call indexLoader on file
  a) while words in file
    i) Create new counters struct
    ii) Insert into hashtable
    iii) Read in key id and count and add to counters struct
6. Call indexSaver to save result
````

### Interfaces
`word` allows for normalization of words

`index` has loader and saver functionality of index hashtable structure to and from file

`pagedir` has pageloading functionality to create webpage from file
