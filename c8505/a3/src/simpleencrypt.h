#ifndef SIMPLEENCRYPT_H
#include <string>
#include <stdio.h>
#include <cstring>
#include <sodium.h>
#define key "22422647d7dd87f0541ad298551b22528d9089aabe5ce9f7e26b6ceb86926d3"
#define SIMPLEENCRYPT_H
#endif

std::string xor_data(std::string message, int len);