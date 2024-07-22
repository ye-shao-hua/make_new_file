#!/bin/bash
i=1
while((i<=48))
do
	touch BEM_INPUT_${i}_362113.txt
	i=`expr $i + 1` 
done
