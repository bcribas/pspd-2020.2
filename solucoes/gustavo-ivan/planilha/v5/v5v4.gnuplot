set datafile separator ','
set ydata time
set timefmt "%s" # specify our time string format
set format y "%f" # otherwise it will show only MM:SS


set ylabel "Tempo" # label for the Y axis
set xlabel 'Quantidade de casos de teste' # label for the X axis

set title "Memorização com otimização com e sem threads"
set key left top
set terminal svg
set output 'v5v4.svg'
plot 'versao5_4t_2.csv' using 2:5 with lines lw 3 title "com threads",'../v4/versao4.2.csv' using 2:5 with lines lw 3 title "sem threads"