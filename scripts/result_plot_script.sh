#!/bin/bash
#
# Copyright (c) 2015, BMW Car IT GmbH
# Author: Michael Mühlbauer-Prassek
#
# This software is licensed under BSD 3-clause License
# (see http://spdx.org/licenses/BSD-3-Clause).
#
# This script plots the execution time using the result files.
# Files to be considered are selected by the "file identifier" 

#$1 -> name of file to be plotted (e.g. file.jpeg)
#$2 -> title of plot
#$3 -> left value of x range
#$4 -> right value of x range
#$5 -> left value of y range
#$6 -> right value of y range
#$7 -> file identifier

plot_parameter="
set title '$2' ;
set xrange [$3:$4] ;
set yrange [$5:$6] ;
set grid ;
set terminal jpeg size 1000,800 ;
set output '$1';";

plot_input="plot "

cd ./Results

input_files_string=$(ls | grep $7)
input_files_array=( $input_files_string )

count=1
for file in "${input_files_array[@]}"
  do
    lable="${file/.txt/""}"
    if [ $count -eq ${#array[@]} ]
    then
      concat="\"$file\" using 1:2 title \"#$lable\" w l ;"
    else
      concat="\"$file\" using 1:2 title \"#$lable\" w l ,"
    fi
    plot_input="$plot_input $concat"
    count=$((count + 1))
done

plot_input="$plot_parameter $plot_input"

echo $plot_input | gnuplot





