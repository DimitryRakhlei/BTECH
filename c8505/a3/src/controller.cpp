#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include "utilities.h"
#include "libpcap/pkt_sniffer.h"

bool controller_running;

int controller(const std::string host, int port){

    controller_running = true;

    std::thread t1(receiver, port);
    t1.detach();

    while(controller_running){

        std::string cmd;
        std::cout << "Please enter a command to run at the destination: " << std::endl;
        std::getline (std::cin, cmd);
        std::cout << "Sent cmd: " << cmd  << std::endl;

        cmd.insert (0, COMMAND);
        cmd.insert (0, IDENTIFY);
        send(host, port, cmd);

        sleep(2);
    }
    return 1;
}

int receiver(int port){

    std::stringstream filter;
    filter << "udp and port " << port; 

    int ctl_flag = 1;
    start_sniffer(-1, filter.str(), ctl_flag);
}