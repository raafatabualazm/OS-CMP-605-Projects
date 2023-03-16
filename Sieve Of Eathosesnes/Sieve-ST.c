#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

void SieveOfEratosthenes(unsigned long long n, bool* prime, unsigned long long start)
{
   

    for (unsigned long long p = start; p * p <= n; p++) {
        // If prime[p] is not changed, then it is a prime
        if (prime[p] == true) {
            // Update all multiples of p greater than or
            // equal to the square of it numbers which are
            // multiple of p and are less than p^2 are
            // already been marked.
            for (unsigned long long i = p * p; i <= n; i += p)
                prime[i] = false;
        }
    }
 
    // Print all prime numbers
    for (unsigned long long p = 2; p <= n; p++)
        if (prime[p])
            printf("%d ",p);
}

int main(int argc, char **argv)
{
	unsigned long long n = atoi(argv[1]);
	bool* prime = malloc(sizeof(bool)*(n+1));
	memset(prime, true, sizeof(bool)*(n+1));
	SieveOfEratosthenes(n, prime, 2);
	return 0;
}
