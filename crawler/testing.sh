#!/bin/bash
#
# testing.sh: testing file for crawler.c
#
# Input:
# Output: test cases for crawler
#
# by Anand Mittal, CS50 Summer 2020


printf "\n\n----------------------------------------------------------------"

printf "Testing: incorrect arguments\n\n"
printf "Input: ./crawler 1 2 3 4\n"
./crawler 1 2 3 4


printf "\n\n----------------------------------------------------------------"

printf "Testing: out of depth range\n\n"
printf "Input: ./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html dir 12\n"
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html dir 12


printf "\n\n----------------------------------------------------------------"

printf "Testing: nonexistent directory\n\n"

printf "Input: ./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html sdfsdf 12\n"
./crawler http://old-www.cs.dartmouth.edu/~cs50/index.html sdfsdf 5

printf "\n\n----------------------------------------------------------------"


printf "Testing: invalid URL\n\n"

printf "Input: ./crawler http://www.wikipedia.org/ dir 3\n"
./crawler http://www.wikipedia.org/ dir 3


printf "\n\n----------------------------------------------------------------"

printf "Testing: cyclical URL\n\n"
printf "Input: ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html 3"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html dir 3
printf "\n"


if [ $? -eq 0 ]
then
    printf "Success\n" 
else
    printf "Fail\n"
fi

printf "\n\n----------------------------------------------------------------"

printf "Testing: cyclical URL with different seed\n\n"
printf "Input: ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html 3"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/A.html dir 3
printf "\n"

if [ $? -eq 0 ]
then
    printf "Success\n" 
else
    printf "Fail\n"
fi

printf "\n\n----------------------------------------------------------------"

printf "Testing: wikipedia playground\n\n"
printf "Input: ./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/index.html dir 2"
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/index.html dir 1
printf "\n"


if [ $? -eq 0 ]
then
    printf "Success\n" 
else
    printf "Fail\n"
fi
