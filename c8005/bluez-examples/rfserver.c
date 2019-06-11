/*------------------------------------------------------------------------------------------
-- SOURCE FILE: rfserver.c - A basic Bluetooth server application that will listen for 
-- connections from a Bluetooth client.
--
-- PROGRAM: rfserver
--
-- FUNCTIONS:
-- int CheckAdapter (void);
--
-- DATE: March 20, 2010
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Aman Abdulla
--
-- PROGRAMMER: Aman Abdulla
--
-- NOTES:
-- This program uses the RFCOMM socket communication calls to open a listen socket and binds it 
-- to port 1. Once a connection has been established, the server will read and print
-- the received data from the client.  
-- The application first checks for at least one active adapter before opening the listening socket.
-- Compile the applications as follows:
	
	gcc -Wall -o rserver rfserver.c -lbluetooth
--
---------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#define BUFSIZE	1024

// Function Prototypes
int CheckAdapter (void);

int main(int argc, char **argv)
{
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[BUFSIZE] = { 0 };
    int sd, client_sd, bytes_read, backlog = 1;
    socklen_t opt = sizeof(rem_addr);

    // Check for at least one active adapter
    CheckAdapter();
    
    // Create the listening socket
    if ((sd = socket (AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) < 0) 
    {
                fprintf(stderr, "Could not create socket\n");
                return 0;
    }

    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    

    if ((bind (sd, (struct sockaddr *)&loc_addr, sizeof(loc_addr))) != 0) 
    {
	fprintf(stderr, "Could not bind socket\n");
        return 0;
    }

    
    // Listen for connections
    if ((listen(sd, backlog)) != 0) 
    {
	fprintf(stderr, "listen failed!\n");
        return 0;
    }

    
    // accept a connection
    if ((client_sd = accept(sd, (struct sockaddr *)&rem_addr, &opt)) < 0) 
    {
	fprintf(stderr, "accept failed!\n");
        return 0;
    }

    
    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "Accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    // read data from the client
    bytes_read = read(client_sd, buf, sizeof(buf));
    if( bytes_read > 0 ) {
        printf("Received [%s]\n", buf);
    }

    // close connection
    close(client_sd);
    close(sd);
    return 0;
}

/*------------------------------------------------------------------------------------------------------
- FUNCTION: CheckAdapter
--
-- DATE: March 20, 2010
--
-- REVISIONS: (Date and Description)
--
-- DESIGNER: Aman Abdulla
--
-- PROGRAMMER: Aman Abdulla
--
-- INTERFACE: int CheckAdapter (void)
--
-- RETURNS: 0 if adapter was found
--	    1 and exits if no adapter was found
--
-- NOTES:
-- Call this function to check for at least one active Bluetooth adapter. Basically this function 
-- uses the hci_get_route HCI call to check for an adapter in the system interface list.
------------------------------------------------------------------------------------------------------*/
int CheckAdapter (void)
{
    int dev_id;
    
    if ((dev_id = hci_get_route (NULL)) < 0)
    {
	fprintf (stderr, "No Bluetooth Device:Error %d: %s\n", errno, strerror (errno));
	exit (1);
    }
    return 0;
}
