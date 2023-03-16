#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define min(x,y) y ^ ((x ^ y) & -(x < y))

typedef struct mark_args {
    unsigned long long start;
    unsigned long long end;
    unsigned long long n;
    bool* lst;

} mark_args;

void* Mark(void* args)
{
    mark_args* mark_argus = (mark_args*)args;
    for (unsigned long long p = 2; p*p <= mark_argus->clear; p++) {
        if (mark_argus->lst[p] == true) {
            unsigned long long startMultiple = (mark_argus->start / p) * p;
            if (startMultiple < p*p) startMultiple = p*p;
            for (unsigned long long i = startMultiple; i <= mark_argus->end; i += p) {
                    mark_argus->lst[i] = false;
            }
        }
    }

}

void* SieveOfEratosthenes(unsigned long long n,  bool* lst, unsigned long long th_cnt, pthread_t* th_pool)
{
    
    unsigned long long blockSize = (n + th_cnt - 1) / th_cnt;
   
   
    for (unsigned long long t = 0; t < th_cnt; t++) {
        mark_args* args = malloc(sizeof(mark_args));
        args->start = t * blockSize +2;
        args->end = min((t + 1) * blockSize, n);
        args->n = n;
        args->lst = lst;
        pthread_create(th_pool + t, NULL, Mark, (void*)args);
    }
    for (unsigned long long t = 0; t < th_cnt; t++) {
        pthread_join(th_pool[t], NULL);

    }
 
    
    
    
    return NULL;
}

int main(int argc, char **argv)
{
    unsigned long long n = atoi(argv[1]);
    unsigned long long th_cnt = atoi(argv[2]);
    
	bool* lst = (bool*)malloc(sizeof(bool)*(n+1));
    memset(lst, true, sizeof(bool)*(n+1));

    unsigned long long* primes  = (unsigned long long*)malloc(sizeof(unsigned long long)*(n));
    pthread_t* th_pool = (pthread_t*)malloc(sizeof(pthread_t)*(th_cnt));

    SieveOfEratosthenes(n, lst, th_cnt, th_pool);

    // Print all prime numbers
    for (unsigned long long p = 2; p <= n; p++)
        if (lst[p])
            printf("%d ",p);
	return 0;
}
