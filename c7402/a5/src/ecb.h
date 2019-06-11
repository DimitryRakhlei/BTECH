#ifndef ECB_H
#define ECB_H
#include "encrypt.h"
#include <stdio.h>

void encrypt_ecb(FILE *infile, FILE *outfile, uint32_t rounds, uint32_t keys[]);
void decrypt_ecb(FILE *infile, FILE *outfile, uint32_t rounds, uint32_t keys[]);

#endif // ECB_H
