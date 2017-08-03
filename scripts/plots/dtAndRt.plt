#gnuplot script

#plot in a window with ID 1
set terminal wxt 1
set style line 1 linecolor rgb "blue"
set title 'drift time spectrum'
set xlabel 'time [ns]'
set ylabel '# entries'
set grid
plot './scripts/plots/data/out.dat' u 1:2 notitle with line linestyle 1

#plot in a seperate window with ID 2
set terminal wxt 2
set title 'rt relation'
#set xlabel 'time [ns]'
set ylabel 'radius [mm]'
set grid
plot './scripts/plots/data/out.dat' u 1:3 notitle w l ls 1