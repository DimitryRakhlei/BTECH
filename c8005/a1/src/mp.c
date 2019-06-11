/*
 *
 =====================================================================================
 *
 *       Filename:  mp.c
 *
 *    Description:  A program which uses 4 processes to decompose 4 individual
 numbers into prime multiples. Works in parallel.
 *
 *        Version:  1.0
 *        Created:  01/22/2018 01:03:15 AM
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_FACTORS 1024

// Globals
mpz_t dest[MAX_FACTORS]; // must be large enough to hold all the factors!

int main(int argc, char **argv) {
    mpz_t n;
    int i = 0, l = 0;
    int childpid = -1;
    clock_t start, end;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int timelog = 0;

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

    int type = 0;
    for (type = 1; type < 5; type++)
        if ((childpid = fork()) <= 0)
            break;

    start = clock();

    if (type == 1) {
        mpz_init_set_str(n, argv[type], 10);
        l = decompose(n, dest);

    } else if (type == 2) {
        mpz_init_set_str(n, argv[type], 10);
        l = decompose(n, dest);

    } else if (type == 3) {
        mpz_init_set_str(n, argv[type], 10);
        l = decompose(n, dest);

    } else if (type == 4) {
        mpz_init_set_str(n, argv[type], 10);
        l = decompose(n, dest);
    } else {
        return EXIT_FAILURE;
    }
    end = clock();
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    if (timelog == 0) {
        printf("\nThe Proc[%d] took %f seconds to execute.\n", type,
               time_spent);

        for (i = 0; i < l; i++) {
            gmp_printf("%s%Zd", i ? " * " : "", dest[i]);
            mpz_clear(dest[i]);
        }
        printf("\n");
    }else {
		printf("%f\n", time_spent);
	}
    return EXIT_SUCCESS;
}
