set xrange [500:2e9]
set logscale xy
set grid
set key right bottom
set xlabel "Number of generated bytes"
set ylabel "Bytes / second"

set terminal pdfcairo noenhanced
set output "benchmark.pdf"

set datafile separator ","

load "autogen_plotting.plot"

#plot \
#"bench_aes_encdec_result.csv" using (column(2)):(valid(4) ? column(4) : NaN) with linespoints title "AES", \
#"bench_rng_result.csv" using (column(2)):(valid(4) ? column(4) : NaN) with linespoints title "/dev/urandom"

# vim: expandtab ft=gnuplot :
