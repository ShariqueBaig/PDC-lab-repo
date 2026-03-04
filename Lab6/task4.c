#include <omp.h>
#include <stdio.h>

int main() {
    int counter = 0;

    printf("Starting Task 4...\n");

    /* 
       DIRECTIONS: Uncomment ONLY ONE of the options below to see how it affects the counter.
       To test a different one, comment the current one back out and uncomment the new one.
    */

    // --- OPTION 1: BROKEN (Race Condition) --- 
    /*
    #pragma omp parallel for
    for(int i=0; i<10000; i++) {
        counter++; 
    }
    */

    // --- OPTION 2: FIXED with CRITICAL ---
    /*
    #pragma omp parallel for
    for(int i=0; i<10000; i++) {
        #pragma omp critical
        {
            counter++;
        }
    }
    */

    // --- OPTION 3: FIXED with ATOMIC ---
    /*
    #pragma omp parallel for
    for(int i=0; i<10000; i++) {
        #pragma omp atomic
        counter++;
    }
    */

    // --- OPTION 4: FIXED with REDUCTION (Fastest/Recommended) ---
    #pragma omp parallel for reduction(+:counter)
    for(int i=0; i<10000; i++) {
        counter++;
    }

    printf("Final Counter Value: %d (Expected: 10000)\n", counter);

    return 0;
}
