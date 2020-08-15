# CS50 Summer 2020, TSE Querier
## Anand Mittal

### Querier
The `querier` takes in a crawler produced directory and index file to display documents based on queried words and phrases.

### Usage
Querier takes the following arguments in the commandline `./querier pageDirectory indexFilename`

Later, querier prompts users for words to search in the indexed webpages found with crawler.

See `DESIGN.md` for more details

### Assumptions
None beyond `REQUIREMENTS.md`.

### Compilation
To compile, run `make`.
To run `testing.sh`, run `make test`.

To run with valgrind, run `make valgrind`.
