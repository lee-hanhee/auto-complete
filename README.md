# Autocomplete Project 

## Overview
This project implements a high-performance autocomplete feature using C. The primary objective is to retrieve the most relevant terms based on a query string from a large dataset, considering both lexicographic ordering and associated importance weights. The project follows the specifications outlined for the ESC 190 H1F course at the University of Toronto.

## Features
1. **Data Loading and Sorting**: The terms are read from a file, dynamically allocated in memory, and sorted lexicographically using the `qsort` function.

2. **Binary Search for Matches**:
   - **Lowest Match Search**: Finds the first occurrence of a term matching the query prefix using binary search, ensuring O(log(n)) complexity.
   - **Highest Match Search**: Identifies the last occurrence of a matching term using binary search, maintaining O(log(n)) efficiency.

3. **Result Extraction and Sorting**:
   - Extracts terms within the matching range.
   - Sorts extracted terms by descending weight to prioritize relevance.

4. **Autocomplete Results**: Returns the top matching terms based on query input, ensuring the output is sorted by descending weight for maximum relevance.

## Core Functions
- `void read_in_terms(term **terms, int *pnterms, char *filename);`  
  Reads and dynamically allocates memory for terms from the file and sorts them lexicographically.

- `int lowest_match(term *terms, int nterms, char *substr);`  
  Finds the index of the first lexicographical match for the query prefix using binary search.

- `int highest_match(term *terms, int nterms, char *substr);`  
  Finds the index of the last lexicographical match for the query prefix using binary search.

- `void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr);`  
  Extracts relevant terms, sorts them by descending weight, and stores results dynamically.

## Development Guidelines
- **Language**: C (C99 standard)
- **Compiler Flags**: `gcc -Wall -std=99 autocomplete.c main.c -o autocomplete`
- **Dependencies**: Standard C libraries (`stdio.h`, `stdlib.h`, `string.h`)
