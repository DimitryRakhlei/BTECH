#include "ecb.h"

void encrypt_ecb(FILE *infile, FILE *outfile, uint32_t rounds, uint32_t keys[]) {
    uint32_t left, right;
    size_t ret;
    uint64_t sblock;
    while(!feof(infile)) {
        memset(&sblock,0,sizeof(sblock));
        ret = fread(&sblock,1,sizeof(sblock),infile);
        if(!ret) break;
        left = (sblock>>32) & 0xFFFFFFFF;
        right = sblock & 0xFFFFFFFF;
        sblock = encrypt(left,right,ROUNDS,keys);
        ret = fwrite(&sblock,1,sizeof(sblock),outfile);
    }
}

void decrypt_ecb(FILE *infile, FILE *outfile, uint32_t rounds, uint32_t keys[]) {
    uint32_t left, right;
    size_t ret;
    uint64_t sblock;
    while(!feof(infile)) {
        memset(&sblock,0,sizeof(sblock));
        ret = fread(&sblock,1,sizeof(sblock),infile);
        if(!ret) break;
        left = (sblock>>32) & 0xFFFFFFFF;
        right = sblock & 0xFFFFFFFF;
        sblock = decrypt(left,right,ROUNDS,keys);
        ret = fwrite(&sblock,1,sizeof(sblock),outfile);
    }
}
