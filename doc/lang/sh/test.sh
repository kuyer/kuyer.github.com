#!/bin/bash
read -p "Enter two number: " n1 n2
if test $n1 -eq $n2
then
    echo "the two number is equal."
else
    echo "the two number is not equal."
fi
