/*
 *
 =====================================================================================
 *
 *       Filename:  mt.c
 *
 *    Description:  Application which employs 4 worker threads to decompose 4
 different prime numbers in parallel.
 *
 *        Version:  1.0
 *        Created:  01/22/2018 01:50:19 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dimitry Rakhlei
 *   Organization:
 *
 *
 =====================================================================================
 */
#include "primedecompose.h"
#include <fcntl.h>
#include <gmp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_FACTORS 1024

// Globals
// mpz_t dest[MAX_FACTORS]; // must be large enough to hold all the factors!
pthread_mutex_t output_lock = PTHREAD_MUTEX_INITIALIZER;
int timelog = 0;

typedef struct {
    mpz_t n;
    mpz_t dest[MAX_FACTORS];
    int thr_num;
} ThreadData;

void *thread_func(void *dataptr) {
    ThreadData *td = (ThreadData *)dataptr;
    clock_t start, end;
    start = clock();

    int l = decompose(td->n, td->dest);

    pthread_mutex_lock(&output_lock);

    end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    if (timelog == 0) {
        printf("\nThe Thread[%d] took %f seconds to execute.\n", td->thr_num,
               time_spent);
        for (int i = 0; i < l; i++) {
            gmp_printf("%s%Zd", i ? " * " : "", td->dest[i]);
            mpz_clear(td->dest[i]);
        }
    } else {
		printf("%f\n", time_spent);
	}
    pthread_mutex_unlock(&output_lock);

    return NULL;
}

int main(int argc, char **argv) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    pthread_t thread1, thread2, thread3, thread4;
    ThreadData *td1, *td2, *td3, *td4;
    if (argc == 6) {
        if (strcmp(argv[5], "t") == 0) {
            timelog = 1;
        }
    } else if (argc != 5) {
        puts("Usage: ./pdec <number to be factored>");
        return EXIT_SUCCESS;
    }
    if (timelog == 0) {
        printf("\n\nStarting Test: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900,
               tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        fflush(stdout);
    }

    td1 = malloc(sizeof(ThreadData));
    td2 = malloc(sizeof(ThreadData));
    td3 = malloc(sizeof(ThreadData));
    td4 = malloc(sizeof(ThreadData));

    mpz_init_set_str(td1->n, argv[1], 10);
    mpz_init_set_str(td2->n, argv[2], 10);
    mpz_init_set_str(td3->n, argv[3], 10);
    mpz_init_set_str(td4->n, argv[4], 10);

    td1->thr_num = 1;
    td2->thr_num = 2;
    td3->thr_num = 3;
    td4->thr_num = 4;

    pthread_create(&thread1, NULL, thread_func, (void *)td1);
    pthread_create(&thread2, NULL, thread_func, (void *)td2);
    pthread_create(&thread3, NULL, thread_func, (void *)td3);
    pthread_create(&thread4, NULL, thread_func, (void *)td4);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);

    free(td1);
    free(td2);
    free(td3);
    free(td4);

    return EXIT_SUCCESS;
}
