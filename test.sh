#!/bin/bash

# Trigger all your test cases with this script
export ASAN_OPTIONS=verify_asan_link_order=0
for file in testcase/*.c
do
	file_name=${file##*/}
	gcc -fsanitize=address -Wall -Werror -std=gnu11 -g testcase/${file_name%.*}.c virtual_alloc.c -o ${file_name%.*} -lm
	./${file_name%.*}  > result
	if cmp -s result testcase/${file_name%.*}.out;
	then
		#echo -e "${file_name%.*} : \033[32m [PASS] \033[0m"
		echo -e "\033[32m [PASS] \033[0m : ${file_name%.*}" 
	else
		#echo -e "${file_name%.*} : \033[31m [FAIL] \033[0m"
		echo -e "\033[31m [FAIL] \033[0m : ${file_name%.*}" 
	fi
	rm -f result
	rm -f ${file_name%.*}
done 
