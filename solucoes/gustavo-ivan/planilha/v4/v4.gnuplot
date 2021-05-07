set datafile separator ','
set ydata time
set timefmt "%s" # specify our time string format
set format y "%f" # otherwise it will show only MM:SS


set ylabel "Tempo" # label for the Y axis
set xlabel 'Quantidade de casos de teste' # label for the X axis

set title "Memorização com otimização"
set terminal svg
set output 'v4.svg'
plot 'versao4.2.csv' using 2:5 with lines lw 3 title "Saída normal",'versao4_Com.2.csv' using 2:5 with lines lw 3 title "Saída comentada",'versao4_Null_2.csv' using 2:5 with lines lw 3 title "Saída /dev/null"