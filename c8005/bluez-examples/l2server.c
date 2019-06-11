/*------------------------------------------------------------------------------------------
-- SOURCE FILE: l2server.c - A basic Bluetooth server application that will listen for 
-- connections from a Bluetooth client.
--
-- PROGRAM: l2server
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
-- The program uses the L2CAP communication calls to open a listen socket and binds it 
-- to port 0x1001. Once a connection has been established, the server will read and print
-- the received data from the client.  
-- The application first checks for at least one active adapter before opening the listening socket.
-- Compile the applications as follows:
	
	gcc -Wall -o l2server l2server.c -lbluetooth
--
---------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 #include <unistd.h>
 #include <errno.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#define BUFSIZE	1024

// Function Prototypes
int CheckAdapter (void);

int main (int argc, char **argv)
{
    struct sockaddr_l2 loc_addr = { 0 }, rem_addr = { 0 };
    char buf[BUFSIZE] = { 0 };
    int sd, client_sd, bytes_read, backlog = 1;
    socklen_t opt = sizeof(rem_addr);

    // Check for at least one active adapter
    CheckAdapter();
    
    // Create the listening socket
    if ((sd = socket (AF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP)) < 0) 
    {
                fprintf(stderr, "Could not create socket\n");
                return 0;
    }
    
    // bind socket to port 0x1001 of the first available 
    // bluetooth adapter
    loc_addr.l2_family = AF_BLUETOOTH;
    loc_addr.l2_bdaddr = *BDADDR_ANY;
    loc_addr.l2_psm = htobs(0x1001);

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
   
    
    ba2str( &rem_addr.l2_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);

    memset(buf, 0, sizeof(buf));

    // read data from the client
    bytes_read = read (client_sd, buf, sizeof(buf));
    if( bytes_read > 0 ) {
        printf("received [%s]\n", buf);
    }

    // close connection
    close(client_sd);
    close(sd);
    exit (0);
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