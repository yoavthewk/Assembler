#!/bin/bash
for i in $(find . -type f -name "*.c")
do
	echo $i
	cat $i | grep "#include"
done
