#include "simpleencrypt.h"



std::string xor_data(std::string message, int len)
{   
    std::string encrypted = std::string();
    for (int i = 0; i < len; i++ ) 
    {
        encrypted += (char)((message[i] ^ key[i % 64]));
    }
    //printf("%d\n", encrypted.length());
    return encrypted;
}