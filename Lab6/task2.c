#include <omp.h>
#include <stdio.h>

#define N 20

int main() {
    int a[N], b[N], c[N];

    for(int i=0;i<N;i++){
        a[i]=i;
        b[i]=i*2;
    }

    // Try changing 'schedule' to static, dynamic, guided, or runtime
    #pragma omp parallel for schedule(runtime)
    for(int i=0;i<N;i++){
        c[i]=a[i]+b[i];
        printf("Thread %d handled index %d\n", omp_get_thread_num(), i);
    }
    
    return 0;
}
