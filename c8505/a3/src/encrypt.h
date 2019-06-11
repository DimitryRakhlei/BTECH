#ifndef ENCRYPT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string>

#include <sodium.h>


#define ENCRYPT_H
#endif

#define MAX_MSG_SIZE 1400
#define PKEY "22422647d7dd87f0541ad298551b22528d9089aabe5ce9f7e26b6ceb86926d39"
#define SKEY "20e1d9805559c47e34882d2ee13f793771397a68854e1f4cac06a23dc467d7ea"

typedef struct {
	char* name;
	uint8_t public_key[crypto_box_PUBLICKEYBYTES];
	uint8_t secret_key[crypto_box_SECRETKEYBYTES];
} Machine;



int start_encrypt(std::string in, char *out);
int start_decrypt(char* data, int len);
void randombytes(uint8_t buffer[], unsigned long long size);
char* to_hex( char hex[], const uint8_t bin[], size_t length );
int is_zero( const uint8_t *data, int len );
int encrypt(uint8_t encrypted[], const uint8_t pk[], const uint8_t sk[], const uint8_t nonce[], const uint8_t plain[], int length);
int decrypt(uint8_t plain[], const uint8_t pk[], const uint8_t sk[], const uint8_t nonce[], const uint8_t encrypted[], int length);
Machine *new_machine(char* name);
void print_machine(Machine *machine);
