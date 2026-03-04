#include <omp.h>
#include <stdio.h>
#include <math.h>

// This function checks if a number is prime
int unknown_func(int n){
    if(n < 2) return 0;
    for(int i=2;i<=sqrt(n);i++){
        if(n%i==0) return 0;
    }
    return 1; // It's prime!
}

int main(){
    int N = 200000;
    
    // --- SERIAL VERSION ---
    int count_serial = 0;
    double start_serial = omp_get_wtime();
    
    for(int i=2; i<=N; i++){
        if(unknown_func(i)) {
            count_serial++;
        }
    }
    double end_serial = omp_get_wtime();
    printf("--- Serial Version ---\n");
    printf("Answer = %d\n", count_serial);
    printf("Time taken = %f seconds\n\n", end_serial - start_serial);

    // --- PARALLEL VERSION ---
    int count_parallel = 0;
    double start_parallel = omp_get_wtime();

    #pragma omp parallel for reduction(+:count_parallel) schedule(dynamic)
    for(int i=2; i<=N; i++){
        if(unknown_func(i)) {
            count_parallel++;
        }
    }

    double end_parallel = omp_get_wtime();
    printf("--- Parallel Version ---\n");
    printf("Answer = %d\n", count_parallel);
    printf("Time taken = %f seconds\n\n", end_parallel - start_parallel);
    
    printf("Speedup: %f x\n", (end_serial - start_serial) / (end_parallel - start_parallel));

    return 0;
}
