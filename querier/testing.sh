#!/bin/bash
#
# testing.sh: testing file for querier.c
#
# Input:
# Output: test cases for querier
#
# by Anand Mittal, CS50 Summer 2020


printf "\n\n--------------------------------------------------------"
printf "Testing: Incorrect number arguments\n"
printf "Input: ./querier 1 2 3\n"
./querier 1 2 3


printf "\n\n--------------------------------------------------------"
printf "Testing: bad arguments\n"
printf "Input: ./querier fakedir fakefile\n"
./querier fakedir fakefile

printf "\n\n--------------------------------------------------------"
printf "Testing: bad query\n"
printf "Input: ./querier ../crawler/dir ../indexer/test\n"
echo "and bad query" | ./querier ../crawler/dir ../indexer/test

printf "\n\n--------------------------------------------------------"
printf "Testing: bad query\n"
printf "Input: ./querier ../crawler/dir ../indexer/test\n"
echo "50 bad query" | ./querier ../crawler/dir ../indexer/test

printf "\n\n--------------------------------------------------------"
printf "Testing: bad query\n"
printf "Input: ./querier ../crawler/dir ../indexer/test\n"
echo "the bad query or" | ./querier ../crawler/dir ../indexer/test

printf "\n\n--------------------------------------------------------"
printf "Testing: good query\n"
printf "Input: ./querier ../crawler/dir ../indexer/test\n"
echo "the" | ./querier ../crawler/dir ../indexer/test

printf "\n\n--------------------------------------------------------"
printf "Testing: good query\n"
printf "Input: ./querier ../data/letters ../data/index.letters\n"
echo "tse or for breadth or the first" | ./querier ../data/letters ../data/index.letters

printf "\n\n--------------------------------------------------------"
printf "Testing: good query\n"
printf "Input: ./querier ../data/letters ../data/index.letters\n"
echo "tse this or biology" | ./querier ../data/letters ../data/index.letters

printf "\n\n--------------------------------------------------------"
printf "Testing: good query\n"
printf "Input: ./querier ../data/letters ../data/index.letters\n"
echo "tse or biology or this and this and for" | ./querier ../data/letters ../data/index.letters
printf "\n"






