#ifndef CBC_H
#define CBC_H
#include "encrypt.h"
#include <stdio.h>


void encrypt_cbc(FILE *infile, FILE *outfile, uint32_t rounds, uint32_t keys[]);
void decrypt_cbc(FILE *infile, FILE *outfile, uint32_t rounds, uint32_t keys[]);

#endif // CBC_H
