#include <omp.h>
#include <stdio.h>

int main() {
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            printf("Task A executed by %d\n", omp_get_thread_num());
        }

        #pragma omp section
        {
            printf("Task B executed by %d\n", omp_get_thread_num());
        }
    }
    return 0;
}
