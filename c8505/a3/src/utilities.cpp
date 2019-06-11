#include "utilities.h"

const std::string RESPONSE = "[RESPONSE]";
const std::string COMMAND  = "[COMMAND]";
const std::string IDENTIFY = "[he5had38Kqs]";

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}


void send(const std::string host, int port, const std::string data)
{
    using namespace boost::asio;
    using boost::asio::ip::udp;
    using boost::asio::ip::address;

    io_service io_service;
    udp::socket socket(io_service);
    udp::endpoint remote_endpoint;

    socket.open(udp::v4());

    remote_endpoint = udp::endpoint(address::from_string(host), port);

    boost::system::error_code err;
    socket.send_to(buffer(data.c_str(), data.length()), remote_endpoint, 0, err);

    socket.close();

    //echo "This is my data" > /dev/udp/127.0.0.1/3000
    // std::stringstream cmd;
    // cmd << "echo " << '"' << data << '"' << " > /dev/udp/" << host << "/" << port; 

    // std::cout << cmd.str().c_str() << std::endl;
        

    // system(cmd.str().c_str());
}
