#include <omp.h>
#include <stdio.h>

int main() {
    #pragma omp parallel
    {
        printf("Hello before barrier from thread %d\n", omp_get_thread_num());
        
        #pragma omp barrier
        
        printf("Hello after barrier from thread %d\n", omp_get_thread_num());
    }
    return 0;
}
