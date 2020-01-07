#!/usr/bin/env bash

for ((i=0;i<10;i++));do
{
    sleep 3;
    python3.6 ./test_python.py 
} &
done

