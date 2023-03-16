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
    unsigned long long  idx;
    bool* lst;
    unsigned long long* primes;

} mark_args;

void* Mark(void* args)
{
    mark_args* mark_argus = (mark_args*)args;
    for (unsigned long long p = 0; p <= mark_argus->idx -1; p++) {
        unsigned long long startMultiple = (mark_argus->start / mark_argus->primes[p]) * mark_argus->primes[p];
        for (unsigned long long i = startMultiple; i <= mark_argus->end; i += mark_argus->primes[p]) {
                mark_argus->lst[i] = false;
        }
    }

}

void* SieveOfEratosthenes(unsigned long long n,  bool* lst, unsigned long long th_cnt, pthread_t* th_pool, unsigned long long* primes)
{
    unsigned long long offset;
    unsigned long long idx = 0;
    // find primes less than sqrt(n)
    for (unsigned long long p = 2; p * p <= n; p++) {
        // If prime[p] is not changed, then it is a prime
        if (lst[p] == true) {
            // Update all multiples of p greater than or
            // equal to the square of it numbers which are
            // multiple of p and are less than p^2 are
            // already been marked.
            primes[idx++] = p;
            for (unsigned long long i = p * p; i*i <= n; i += p) {
                lst[i] = false;
                offset = i+1;
                
            }
        }
        
    }
    unsigned long long work = n - offset;
    
    unsigned long long blockSize = (work + th_cnt - 1) / th_cnt;
   
   
    for (unsigned long long t = 0; t < th_cnt; t++) {
        mark_args* args = malloc(sizeof(mark_args));
        args->start = t * blockSize + offset +1;
        args->end = min((t + 1) * blockSize + offset, n);
        args->n = n;
        args->lst = lst;
        args->primes = primes;
        args->idx = idx;
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

    SieveOfEratosthenes(n, lst, th_cnt, th_pool, primes);

    // Print all prime numbers
    for (unsigned long long p = 2; p <= n; p++)
        if (lst[p])
            printf("%d ",p);
	return 0;
}
