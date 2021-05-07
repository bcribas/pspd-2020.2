set datafile separator ','
set ydata time
#set timefmt "%s:%s:%s" # specify our time string format
#set format y "%.10f" # otherwise it will show only MM:SS
set timefmt "%s"
set format y "%.3S"

set ylabel "Tempo" # label for the Y axis
set xlabel 'Quantidade de casos' # label for the X axis

set title "Leitura"
set key bot right
set terminal svg
set output 'leitura.svg'
plot './v1/versao1_Lei_2.csv' using 2:5 with lines lw 3 title 'leitura naive' ,'./v4/versao4_Lei_2.csv' using 2:5 with lines lw 3 title "Leitura memorização com otimização"