# Parallel Programming Lab: OpenMP in Practice

## Overview

In this lab, you will learn how to design and implement shared-memory parallel programs using OpenMP.  
Rather than just running examples, you will:

- understand how OpenMP constructs map to parallel design ideas  
- reason about correctness (race conditions, synchronization, work distribution)  
- design a small parallel solution yourself  

This lab emphasizes *thinking like an HPC programmer*, not just using directives.

---

## Learning Objectives

By the end of this lab you should be able to:

1. Use core OpenMP directives (`parallel`, `for`, `sections`, `reduction`)
2. Choose an appropriate work-sharing strategy
3. Identify and fix race conditions
4. Use synchronization constructs appropriately
5. Design a simple parallel algorithm using OpenMP

---

## Setup

You may compile locally **or** use Docker.

### Option A — Local compilation

```bash
gcc -fopenmp program.c -o program && ./program
````

### Option B — Docker

```bash
docker run -it -v %cd%:/tmp gcc:latest
```

Inside container:

```bash
gcc -fopenmp program.c -o program && ./program
```

---

# Part 1 — Understanding the OpenMP Execution Model

### Task 1: Thread Creation

Compile and run:

```c
#include <omp.h>
#include <stdio.h>

int main() {
    #pragma omp parallel
    {
        printf("Hello from thread %d\n", omp_get_thread_num());
    }
}
```

### Questions (write answers in your submission)

1. How many threads ran the program?
2. Change the thread count using:

   ```c
   omp_set_num_threads(4);
   ```

   What happens?
3. Explain the fork-join model in your own words.

---

# Part 2 — Parallel Loop Design

### Task 2: Work Sharing with `for`

```c
#include <omp.h>
#include <stdio.h>

#define N 20

int main() {
    int a[N], b[N], c[N];

    for(int i=0;i<N;i++){
        a[i]=i;
        b[i]=i*2;
    }

    #pragma omp parallel for
    for(int i=0;i<N;i++){
        c[i]=a[i]+b[i];
        printf("Thread %d handled index %d\n", omp_get_thread_num(), i);
    }
}
```

### Experiment

1. Try schedules: `static`, `dynamic`, `guided`, and `runtime`
2. Observe how work is distributed.


* When would dynamic scheduling be better than static?
* Why might scheduling matter in HPC workloads?

---

# Part 3 — Parallel Tasks vs Parallel Loops

### Task 3: Sections

```c
#pragma omp parallel sections
{
    #pragma omp section
    printf("Task A executed by %d\n", omp_get_thread_num());

    #pragma omp section
    printf("Task B executed by %d\n", omp_get_thread_num());
}
```

### Questions

* When should you use `sections` instead of `for`?
* Give a real example of two independent tasks in scientific computing.

---

# Part 4 — Race Conditions and Fixes

### Task 4: Broken Counter

```c
int counter = 0;

#pragma omp parallel for
for(int i=0;i<10000;i++){
    counter++;
}
```

Run the code multiple times.

* Does the result change?

### Step 1 - Fix using `critical`
```c
#pragma omp critical
counter++;
```

### Step 2 — Fix using `atomic`

```c
#pragma omp atomic
counter++;
```

### Step 3 — Fix using `reduction`

```c
#pragma omp parallel for reduction(+:counter)
```

### Reflection Questions

1. Why is `reduction` often faster than `critical`?
2. When would `atomic` be insufficient?

---

# Part 5 — Synchronization Concepts

### Task 5: Barrier Experiment

Insert a barrier between two prints:

```c
#pragma omp barrier
```

### Questions

* What happens if the barrier is removed?
* Why are barriers dangerous for performance?

---

# Part 6 — Mini Design Challenge

## Problem: Parallel the program

You will parallelize this sequential program:

```c
#include <stdio.h>
#include <math.h>

int unknown_func(int n){
    if(n < 2) return 0;
    for(int i=2;i<=sqrt(n);i++){
        if(n%i==0) return 0;
    }
    return 1;
}

int main(){
    int N = 200000;
    int count = 0;

    for(int i=2;i<=N;i++){
        if(unknown_func(i))
            count++;
    }

    printf("Answer = %d\n", count);
}
```

### Requirements

1. Parallelize the loop
2. Ensure correctness
3. Avoid race conditions
4. Use OpenMP constructs appropriately

### Deliverables

Submit:

* Your final code
* A short explanation (5–8 sentences):

  * Which directive(s) you used
  * Why you chose them
  * What mistakes you initially made
  * What task is being performed using the code?

---

# Final Reflection

Answer briefly:

1. When does parallelism *not* improve performance?
2. Which OpenMP construct felt most useful?
3. What design decision matters more than syntax in parallel programming?