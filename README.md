# Spelling Bee Game & Solver

A terminal-based generalized version of the NYT Spelling Bee, written 
in C. Build a hive of letters, play the game yourself, or let the 
built-in solver find every valid word automatically.

## What Is Spelling Bee?

Create valid words using letters from a hive. Every word must contain 
the required center letter and be at least 4 letters long. Pangrams, 
words that use every hive letter, earn bonus points.

## Features

- Interactive gameplay with live score tracking
- Random hive generator that guarantees a pangram exists
- 120,000+ word dictionary validation at runtime
- Brute Force Solver — scans full dictionary for valid words
- Optimized Solver — recursive binary search with hive letter dialing
- Pangram and bingo detection
- Memory safe, verified with Valgrind

## How To Run

```bash
make build
./spellingBee.exe -r 7 -p   # random hive, play mode ON
./spellingBee.exe -r 7 -o   # random hive, optimized solver
```

## Built With

- Language: C (C99)
- Algorithms: Binary Search, Recursive Backtracking
- Data Structures: Dynamic arrays of C-strings
- Tools: GCC, Make, Valgrind, GDB

## Author

Aaron Merakai — UIC CS 211
[GitHub](https://github.com/RoonMerakai)


## Demo
[Watch Demo Here] https://drive.google.com/open?id=1Hw01YdmNC3gUxeJuf-8zs99ebp5MppVm&usp=drive_copy




