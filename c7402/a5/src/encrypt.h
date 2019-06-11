#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdint.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

static int ROUNDS = 8;

uint32_t f(uint32_t block, uint32_t key);
uint64_t encrypt(uint32_t left, uint32_t right, uint32_t rounds, uint32_t keys[]);
uint64_t decrypt(uint32_t left, uint32_t right, uint32_t rounds, uint32_t keys[]);


#endif // ENCRYPT_H
