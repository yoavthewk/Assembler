#!/bin/bash
for i in $(find . -type f -name "*.h")
do
	echo $i
	cat $i | grep "#include"
done
