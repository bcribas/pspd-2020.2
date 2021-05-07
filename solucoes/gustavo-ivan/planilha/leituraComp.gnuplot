set datafile separator ','
set ydata time
set timefmt "%s" # specify our time string format



set ylabel "Tempo" # label for the Y axis
set xlabel 'Quantidade de casos' # label for the X axis

set title "Comparação Leitura"
set key bot right
set terminal svg
set output 'leituraComp.svg'
plot './v1/versao1_Lei_2.csv' using 2:5 with lines lw 3 title 'leitura naive' ,'./v4/versao4_Lei_2.csv' using 2:5 with lines lw 3 title "Leitura memorização com otimização",'./v1/versao1.2.csv' using 2:5 with lines lw 3 title "Naive",'./v4/versao4.2.csv' using 2:5 with lines lw 3 title "Memorização com otimização"