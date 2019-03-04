# Gnuplot/SqlPlotTools scripts for d_ary_heap_speedtest.cpp

# IMPORT-DATA stats d_ary_heap_speedtest_results.txt

set terminal pdf size 18.6cm,12cm linewidth 2.0 noenhanced
set output "d_ary_heap_speedtest_results.pdf"

set pointsize 0.7
set style line 1 linecolor 1 dashtype 2
set style line 2 linecolor 1 dashtype 1
set style line 3 linecolor 2 dashtype 2
set style line 4 linecolor 2 dashtype 1
set style line 5 linecolor 3 dashtype 2
set style line 6 linecolor 3 dashtype 1
set style line 7 linecolor 4 dashtype 2
set style line 8 linecolor 4 dashtype 1
set style line 9 linecolor 5 dashtype 2
set style line 10 linecolor 5 dashtype 1
set style line 11 lc rgb "#f0b000" dashtype 2
set style line 12 lc rgb "#f0b000" dashtype 1
set style line 13 linecolor 8 dashtype 2
set style line 14 linecolor 8 dashtype 1
set style line 15 linecolor 7 dashtype 4 pointtype 2
set style increment user

set grid xtics ytics

set key top left reverse Left

set title 'tlx::DAryHeap vs std::priority_queue -- Push Fill Performance Test'
set xlabel 'Item Count [log_2(n)]'
set ylabel 'Time per Item [Nanoseconds / Item]'

## MULTIPLOT(container)
## SELECT LOG(2, items) AS x, MEDIAN(time / items * 1e9) AS y, MULTIPLOT
## FROM stats
## WHERE op = 'heap_fill'
## GROUP BY MULTIPLOT,slots,x ORDER BY slots,container,x
plot \
    'd_ary_heap_speedtest-data.txt' index 0 title "container=tlx::DAryAIntHeap<2>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 1 title "container=tlx::DAryHeap<2>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 2 title "container=tlx::DAryAIntHeap<3>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 3 title "container=tlx::DAryHeap<3>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 4 title "container=tlx::DAryAIntHeap<4>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 5 title "container=tlx::DAryHeap<4>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 6 title "container=tlx::DAryAIntHeap<6>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 7 title "container=tlx::DAryHeap<6>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 8 title "container=tlx::DAryAIntHeap<8>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 9 title "container=tlx::DAryHeap<8>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 10 title "container=tlx::DAryAIntHeap<16>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 11 title "container=tlx::DAryHeap<16>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 12 title "container=tlx::DAryAIntHeap<32>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 13 title "container=tlx::DAryHeap<32>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 14 title "container=std::priority_queue" with linespoints

set title 'tlx::DAryHeap vs std::priority_queue -- Push Fill Pop-All Performance Test'
set xlabel 'Item Count [log_2(n)]'
set ylabel 'Time per Item [Nanoseconds / Item]'

## MULTIPLOT(container)
## SELECT LOG(2, items) AS x, MEDIAN(time / items * 1e9) AS y, MULTIPLOT
## FROM stats
## WHERE op = 'heap_fill_popall'
## GROUP BY MULTIPLOT,slots,x ORDER BY slots,container,x
plot \
    'd_ary_heap_speedtest-data.txt' index 15 title "container=tlx::DAryAIntHeap<2>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 16 title "container=tlx::DAryHeap<2>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 17 title "container=tlx::DAryAIntHeap<3>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 18 title "container=tlx::DAryHeap<3>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 19 title "container=tlx::DAryAIntHeap<4>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 20 title "container=tlx::DAryHeap<4>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 21 title "container=tlx::DAryAIntHeap<6>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 22 title "container=tlx::DAryHeap<6>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 23 title "container=tlx::DAryAIntHeap<8>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 24 title "container=tlx::DAryHeap<8>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 25 title "container=tlx::DAryAIntHeap<16>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 26 title "container=tlx::DAryHeap<16>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 27 title "container=tlx::DAryAIntHeap<32>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 28 title "container=tlx::DAryHeap<32>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 29 title "container=std::priority_queue" with linespoints

set title 'tlx::DAryHeap vs std::priority_queue -- Fill and Pop-Push Cycle Performance Test'
set xlabel 'Item Count [log_2(n)]'
set ylabel 'Time per Item [Nanoseconds / Item]'

## MULTIPLOT(container)
## SELECT LOG(2, items) AS x, MEDIAN(time / items * 1e9) AS y, MULTIPLOT
## FROM stats
## WHERE op = 'heap_fill_cycle'
## GROUP BY MULTIPLOT,slots,x ORDER BY slots,container,x
plot \
    'd_ary_heap_speedtest-data.txt' index 30 title "container=tlx::DAryAIntHeap<2>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 31 title "container=tlx::DAryHeap<2>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 32 title "container=tlx::DAryAIntHeap<3>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 33 title "container=tlx::DAryHeap<3>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 34 title "container=tlx::DAryAIntHeap<4>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 35 title "container=tlx::DAryHeap<4>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 36 title "container=tlx::DAryAIntHeap<6>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 37 title "container=tlx::DAryHeap<6>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 38 title "container=tlx::DAryAIntHeap<8>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 39 title "container=tlx::DAryHeap<8>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 40 title "container=tlx::DAryAIntHeap<16>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 41 title "container=tlx::DAryHeap<16>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 42 title "container=tlx::DAryAIntHeap<32>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 43 title "container=tlx::DAryHeap<32>" with linespoints, \
    'd_ary_heap_speedtest-data.txt' index 44 title "container=std::priority_queue" with linespoints

