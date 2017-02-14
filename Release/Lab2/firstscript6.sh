#!/bin/bash
# Could you conver it to comput the factorial?
read -p "Please enter a number: " num
ctr=1 sum=1
while [ $ctr -le $num ]; do
    sum=$((sum*ctr))
    echo $ctr
    let ctr++
done
echo "The sum of $num numbers is $sum."
