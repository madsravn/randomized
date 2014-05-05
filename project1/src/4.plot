set terminal png
set output "4.png"
set title "Depth for lists of length 10^4"
set trange [0:700]
plot "4.dat" using 1:2 with lines title "depth"
