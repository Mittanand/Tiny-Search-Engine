# Crawler Implementation Spec
## Anand Mittal

### Crawler Pseudocode
````
1. Verify correct number and type of arguments
2. Create hashtable struct of visited URLs and bag struct of pages to crawl
3. Make webpage for seedURL starting at 0 depth
4. Add seedpage to hashtable and bag structures
5. Iterate through pages in bag and do following
  a) Fetch page html
  b) Save page in pageDirectory with specified id
  c) If maxDepth not exceeded, scan page for URLs
  d) If URL valid and unvisited, create new webpage
  e) Add new webpage to bag
  f) Free webpage and increment id
  g) Repeat
  f) In either pagesaver or pagescanner fail, abort non-zero
6. Clean up and free bag and hashtable structures

 ````
 
 ### Structures
 `hashtable_t` stores visited nodes
 
 `bag_t` stores webpages to crawl
 
 `webpage` stores URL, depth, and html
 
 ## Function Descriptions and Prototypes
 See header `*.h` files
