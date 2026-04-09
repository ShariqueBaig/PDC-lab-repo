# Lab 6: OpenMP Programming

## Objective
This lab introduces shared-memory parallel programming using the OpenMP API in C. It covers work-sharing constructs, synchronization, and performance scaling.

## Tasks
- **task1.c** to **task6.c**: Various OpenMP exercises including parallel loops, reductions, and sections.

## Key Concepts
- OpenMP Pragmas (`#pragma omp parallel for`).
- Number of Threads management (`omp_set_num_threads`).
- Data Sharing Clauses (`private`, `shared`).
- Reduction operations.

## How to Build and Run

### Requirements
- A C compiler with OpenMP support (e.g., GCC).

### Compilation
```bash
gcc -fopenmp task1.c -o task1
```

### Execution
```bash
./task1
```

## Documentation
- **Lab 6 - Open MP.md**: Detailed task descriptions and expected outputs.
- **Answers.pdf**: Completed lab report and discussion.
