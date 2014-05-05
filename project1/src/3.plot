set terminal png
set title "Depth for lists of length 10^3"
set output "3.png"
set trange [0:700]
plot "3.dat" using 1:2 with lines title "depth"
