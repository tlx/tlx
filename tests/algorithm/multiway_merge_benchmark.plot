# Gnuplot/SqlPlotTools scripts for algorithm_multiway_merge_benchmark.cpp

# IMPORT-DATA stats stats.txt

set terminal pdf size 28cm,18cm linewidth 2.0 noenhanced
set output "algorithm_multiway_merge_benchmark_results.pdf"

set pointsize 0.7
set style line 6 lc rgb "#f0b000"
set style line 15 lc rgb "#f0b000"
set style line 24 lc rgb "#f0b000"
set style line 33 lc rgb "#f0b000"
set style line 42 lc rgb "#f0b000"
set style line 51 lc rgb "#f0b000"
set style line 60 lc rgb "#f0b000"
set style increment user

set grid xtics ytics

set key top left

set title 'Speed of Sequential Multiway Merge'
set xlabel 'number of 2 MiB sequences'
set ylabel 'running time per item (nanoseconds / item)'

## MULTIPLOT(method,item_size)
## SELECT seq_count AS x, AVG(time / total_size * 1e9) AS y, MULTIPLOT
## FROM stats WHERE method LIKE 'seq_%' AND method NOT LIKE '%_bb' AND seq_items * item_size = 2097152
## GROUP BY MULTIPLOT,x ORDER BY MULTIPLOT,x
plot \
    'algorithm_multiway_merge_benchmark-data.txt' index 0 title "method=seq_gnu_mwm,item_size=8" with linespoints, \
    'algorithm_multiway_merge_benchmark-data.txt' index 1 title "method=seq_mwm_lt,item_size=8" with linespoints, \
    'algorithm_multiway_merge_benchmark-data.txt' index 2 title "method=seq_mwm_lt_combined,item_size=8" with linespoints, \
    'algorithm_multiway_merge_benchmark-data.txt' index 3 title "method=seq_mwm_lt_stable,item_size=8" with linespoints

set title 'Speed of Parallel Multiway Merge'

## MULTIPLOT(method,item_size)
## SELECT seq_count AS x, AVG(time / total_size * 1e9) AS y, MULTIPLOT
## FROM stats WHERE method LIKE 'para_%' AND seq_items * item_size = 2097152
## GROUP BY MULTIPLOT,x ORDER BY MULTIPLOT,x
plot \
    'algorithm_multiway_merge_benchmark-data.txt' index 4 title "method=para_gnu_mwm_exact,item_size=8" with linespoints, \
    'algorithm_multiway_merge_benchmark-data.txt' index 5 title "method=para_gnu_mwm_sampling,item_size=8" with linespoints, \
    'algorithm_multiway_merge_benchmark-data.txt' index 6 title "method=para_mwm_exact_lt,item_size=8" with linespoints, \
    'algorithm_multiway_merge_benchmark-data.txt' index 7 title "method=para_mwm_exact_lt_stable,item_size=8" with linespoints, \
    'algorithm_multiway_merge_benchmark-data.txt' index 8 title "method=para_mwm_sampling_lt,item_size=8" with linespoints, \
    'algorithm_multiway_merge_benchmark-data.txt' index 9 title "method=para_mwm_sampling_lt_stable,item_size=8" with linespoints

set title 'Speed of Sequential/Parallel Multiway Merge vs Array Size'
set xlabel 'input size in bytes (log_2(n))'

## MULTIPLOT(method,item_size)
## SELECT LOG(2, total_bytes) AS x, AVG(time / total_size * 1e9) AS y, MULTIPLOT
## FROM stats WHERE method IN ('seq_mwm_lt_combined', 'para_mwm_exact_lt')
## GROUP BY MULTIPLOT,x ORDER BY MULTIPLOT,x
plot \
    'algorithm_multiway_merge_benchmark-data.txt' index 10 title "method=para_mwm_exact_lt,item_size=8" with linespoints, \
    'algorithm_multiway_merge_benchmark-data.txt' index 11 title "method=para_mwm_exact_lt,item_size=36" with linespoints, \
    'algorithm_multiway_merge_benchmark-data.txt' index 12 title "method=seq_mwm_lt_combined,item_size=8" with linespoints, \
    'algorithm_multiway_merge_benchmark-data.txt' index 13 title "method=seq_mwm_lt_combined,item_size=36" with linespoints
