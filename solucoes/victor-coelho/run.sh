#!/bin/bash
echo "arquivo, consumers, real-1, real-2, mean" > $1.csv
for I in {1..12}
do
		allResults=(0 0)
		for i in {0..1}
		do
			allResults[i]=$({ time python3 $2 $I < $1 $@ ; } |& grep real | sed -E 's/[^0-9\.]+//g' | tr -d '\n' | (cat && echo " * 1000") | bc)
		done
		echo "$1, $I, ${allResults[0]}, ${allResults[1]}, $(((allResults[0] + allResults[1])/2))" >> $1.csv
done

