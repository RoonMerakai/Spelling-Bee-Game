# Spelling Bee Game & Solver

A terminal-based generalized version of the NYT Spelling Bee game, 
written in C. Build a hive of letters, play the game yourself, or let 
the built-in solver find every valid word for you.

## What Is Spelling Bee?

Create as many valid words as possible using letters from a "hive." 
Every word must contain the required center letter and be at least 
4 letters long. Letters can be reused. Pangrams, words that use every 
letter in the hive, earn bonus points.

Example: hive = "abek", required letter = 'a'
Valid words: bake, beak, kabab, kebab...

## Features

- Interactive gameplay — enter words one at a time and track your score
- Random hive generator — guarantees a pangram exists in every hive
- Manual hive builder — set up a specific hive to challenge a friend
- Dictionary validation — every word checked against 120,000+ word dictionary
- Brute Force Solver — finds all valid words by scanning the full dictionary
- Optimized Solver — uses recursive binary search and hive letter dialing 
  for a faster, more efficient solution
- Pangram detection — flags pangrams and perfect pangrams in the word list
- Bingo detection — checks if valid words start with every hive letter
- Memory safe — built with dynamic memory management, verified with Valgrind

## How To Install & Run

**Requirements:** GCC, Unix/Linux terminal or Windows with MinGW

```bash
