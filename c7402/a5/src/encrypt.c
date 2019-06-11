#include "encrypt.h"

uint32_t f(uint32_t block, uint32_t key) {
    return block ^ key;
}

uint64_t encrypt(uint32_t left, uint32_t right, uint32_t rounds, uint32_t keys[]) {
    uint32_t i, left1, right1;
    for(i = 0;i < rounds;i++) {
        left1 = f(left,keys[i]) ^ right;
        right1 = left;
        if(i == (rounds-1)) {
            left = right1;
            right = left1;
        } else {
            left = left1;
            right = right1;
        }
    }
    return (uint64_t)left<<32 | right;
}

uint64_t decrypt(uint32_t left, uint32_t right, uint32_t rounds, uint32_t keys[]) {
    uint32_t i, left1, right1;
    for(i = 0;i < rounds;i++) {
        left1 = f(left,keys[rounds-i-1]) ^ right;
        right1 = left;
        if(i == (rounds-1)) {
            left = right1;
            right = left1;
        } else {
            left = left1;
            right = right1;
        }
    }
    return (uint64_t)left<<32 | right;
}