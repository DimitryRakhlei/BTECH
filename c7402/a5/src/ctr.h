#ifndef CTR_H
#define CTR_H

#include "encrypt.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void encrypt_ctr(FILE *infile, FILE *outfile, uint32_t rounds, uint32_t keys[]);
void decrypt_ctr(FILE *infile, FILE *outfile, uint32_t rounds, uint32_t keys[]);


#endif // CTR_H
