#!/bin/bash
rm -f out.log
cat ./sid | while read line
do
	for ((i=3;i<10;i=i+1))
	do
		Time_in=$(printf "2013-%02d" $i)
		echo "./for_my_occi $Time_in $line hisdata forcast"
		./for_my_occi $Time_in $line hisdata forcast >> out.log
#echo $Time_in
	done
done
