# Lab 4: SIMD Programming

## Objective
Focuses on Single Instruction, Multiple Data (SIMD) techniques to optimize code for modern processors. This lab explores vectorization, Intel Intrinsics (AVX2), and performance profiling.

## Contents
- **sources/**: C source code for SIMD implementations.
- **simd-programming/**: Working directory for SIMD tasks.
- **SIMD_Lab_README.md**: Detailed instructions for the SIMD lab.
- **SIMD_Quick_Reference.md**: Cheat sheet for AVX2 intrinsics.

## Key Concepts
- Data Parallelism.
- Intel AVX2 Intrinsics.
- Loop Vectorization.
- Performance Comparison (Scalar vs. Vectorized).

## How to Build
Use the provided `Makefile.template` (rename to `Makefile` or use as reference) to compile the source files. Typically:
```bash
gcc -mavx2 -O3 main.c -o simd_app
```

## Reference
See `SIMD_Quick_Reference.md` for a list of common intrinsics used in this lab.
