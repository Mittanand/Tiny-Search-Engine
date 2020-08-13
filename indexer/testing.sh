#!/bin/bash
#
# testing.sh: testing file for indexer.c and indextest.c
#
# Input:
# Output: test cases for indexer
#
# by Anand Mittal, CS50 Summer 2020


printf "\n\n----------------------------------------------------------------"

printf "Indexer Testing: incorrect number arguments\n\n"
printf "Input: ./indexer 1 2 3\n"
./indexer 1 2 3


printf "\n\n----------------------------------------------------------------"

printf "Indexer Testing: bad arguments\n\n"
printf "Input: ./indexer ../crawler/fakedir test\n"
./indexer ../crawler/fakedir test


printf "\n\n----------------------------------------------------------------"

printf "Indexer Testing: non crawler produced directory\n\n"

printf "Input: ./indexer ../crawler/notcrawler test\n"
./indexer ../crawler/notcrawler test

printf "\n\n----------------------------------------------------------------"


printf "Indextest Testing: Invalid number of arguments \n\n"
printf "Input: ./indextest 1 2 3\n"
./indextest 1 2 3


printf "\n\n----------------------------------------------------------------"


printf "Indextest Testing: bad arguments\n\n"
printf "Input: ./indextest fakefile testout\n"
./indextest fakefile testout



printf "\n\n----------------------------------------------------------------"

printf "Testing: Correct functioning \n\n"
printf "Input: ./indexer ../crawler/dir test\n"
./indexer ../crawler/dir test
printf "Input: ./indextest test testout\n"
./indextest test testout
printf "Sorting test and testout\n"
sort test > testsort
sort testout > testoutsort
printf "Comparing sorted files...\n"
diff testsort testoutsort
