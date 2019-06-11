#include "ctr.h"

void encrypt_ctr(FILE *infile, FILE *outfile, uint32_t rounds,
                 uint32_t keys[]) {
    srand(keys[0]); // make the nonce based on supplied key
    uint64_t nonce = rand() % (1024);
    uint32_t left, right;
    size_t ret;
    uint64_t sblock;
    while (!feof(infile)) {
        // read data
        memset(&sblock, 0, sizeof(sblock));
        ret = fread(&sblock, 1, sizeof(sblock), infile);
        if (!ret)
            break;

        // block cipher the nonce
        left = (nonce >> 32) & 0xFFFFFFFF;
        right = nonce & 0xFFFFFFFF;
        sblock ^=
            encrypt(left, right, ROUNDS,
                    keys); // xor the block cipher output with the data block
        ret = fwrite(&sblock, 1, sizeof(sblock), outfile); // write data out
        nonce++;                                           // counter
    }
}

void decrypt_ctr(FILE *infile, FILE *outfile, uint32_t rounds,
                 uint32_t keys[]) {
    srand(keys[0]); // make the nonce based on supplied key
    uint64_t nonce = rand() % (1024);
    uint32_t left, right;
    size_t ret;
    uint64_t sblock;
    while (!feof(infile)) {
        // read data
        memset(&sblock, 0, sizeof(sblock));
        ret = fread(&sblock, 1, sizeof(sblock), infile);
        if (!ret)
            break;

        // block cipher the nonce
        left = (nonce >> 32) & 0xFFFFFFFF;
        right = nonce & 0xFFFFFFFF;
        sblock ^=
            encrypt(left, right, ROUNDS,
                    keys); // xor the block cipher output with the data block
        ret = fwrite(&sblock, 1, sizeof(sblock), outfile); // write data out
        nonce++;                                           // counter
    }
}