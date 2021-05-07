set datafile separator ','
set ydata time
set timefmt "%s" # specify our time string format
set format y "%f" # otherwise it will show only MM:SS


set ylabel "Tempo" # label for the Y axis
set xlabel 'Quantidade de casos' # label for the X axis

set title "Naive com threads"
set key left top
set terminal svg
set output 'v2Thread.svg'
plot 'versao2_1t.2.csv' using 2:5 with lines lw 3 title '1 thread ' ,'versao2_2t.2.csv' using 2:5 with lines lw 3 title "2 threads",'versao2_4t.2.csv' using 2:5 with lines lw 3 title "4 threads",'versao2_6t.2.csv' using 2:5 with lines lw 3 title "6 threads",'versao2_8t.2.csv' using 2:5 with lines lw 3 title "8 threads",'versao2_12t.2.csv' using 2:5 with lines lw 3 title "12 threads"