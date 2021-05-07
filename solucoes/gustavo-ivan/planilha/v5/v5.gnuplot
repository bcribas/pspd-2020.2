set datafile separator ','
set ydata time
set timefmt "%s" # specify our time string format
set format y "%f" # otherwise it will show only MM:SS


set ylabel "Tempo" # label for the Y axis
set xlabel 'Quantidade de casos de teste' # label for the X axis

set title "Memorização com otimização e threads"
set key left top
set terminal svg
set output 'v5.svg'
plot 'versao5_4t_2.csv' using 2:5 with lines lw 3 title "Saída normal",'versao5_Com_2.csv' using 2:5 with lines lw 3 title "Saída comentada",'v5_Null_2.csv' using 2:5 with lines lw 3 title "Saída /dev/null"