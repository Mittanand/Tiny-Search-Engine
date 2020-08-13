# CS50 - TSE
## Anand Mittal
### Indexer
`indexer` takes documents created by the `crawler` containing the url, depth, and html, and creates a file showing different words and their frequency from each webpage.

 `indextest` loads the indexer produced file and saves the result to another file
 
 ### Usage
 `indexer` takes in two arguments: the crawler produced directory and the pathname of a file to write indexer resuts
  Thus, we have `./indexer pageDirectory indexFilename
 
 `indextest` similarily takes in the file created by indexer and an output file as its two arguments
 
 `./indextest oldIndexFilename newIndexFilename`
 
 ### Compilation
 Run `make` to compile. To run `testing.sh`, run `make test`. 
