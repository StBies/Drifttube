#gnuplot script

set terminal wxt
set title 'drift time spectrum'
set xlabel 'time [ns]'
set ylabel '# entries'
set grid
plot './scripts/plots/data/out.dat' u 1:2 notitle