set datafile separator ','
set ydata time
set timefmt "%s" # specify our time string format
set format y "%f" # otherwise it will show only MM:SS


set ylabel "Tempo" # label for the Y axis
set xlabel 'Quantidade de casos de teste' # label for the X axis

set title "Otimização com threads"
set key left top
set terminal svg
set output 'v5Thread.svg'
plot 'versao5_1t_2.csv' using 2:5 with lines lw 3 title '1 thread ' ,'versao5_2t_2.csv' using 2:5 with lines lw 3 title "2 threads",'versao5_4t_2.csv' using 2:5 with lines lw 3 title "4 threads" , 'versao5_6t_2.csv' using 2:5 with lines lw 3 title "6 threads",'versao5_8t_2.csv' using 2:5 with lines lw 3 title "8 threads",'versao5_12t_2.csv' using 2:5 with lines lw 3 title "12 threads"