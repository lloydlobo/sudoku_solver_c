#!/bin/bash

while true; do
    inotifywait -e modify sudoku_4x4.py
    python3 sudoku_4x4.py
done
