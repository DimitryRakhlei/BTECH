/*------------------------------------------------------------------------------------------
-- SOURCE FILE: rfclient.c - A basic Bluetooth client application that will connect to  
-- a server and send it a string. 
--
-- PROGRAM: rfclient
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
-- The program uses the his program uses the RFCOMM socket communication calls to open an active socket 
-- and uses it to connect to a server listening on port 0x1001. Once a connection has been established, 
-- the client sends string and exits. The user may specify a hardware address or the client
-- will use a default address. 
-- The application first checks for at least one active adapter before opening the listening socket.
-- Compile the applications as follows:
	
	gcc -Wall -o rfclient rfclient.c -lbluetooth
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

#define ADDR_SIZE   	19
#define BUFSIZE 	20
#define MSG		"Hello Server!" 
#define DEFAULT_ADDR	"00:1B:41:01:61:C7"

// Function Prototypes
int CheckAdapter (void);

int main (int argc, char **argv)
{
    struct sockaddr_rc addr = { 0 };
    int sd, status;
    char str[BUFSIZ] = MSG;
    char dest[ADDR_SIZE] = DEFAULT_ADDR;
    
    // Check for at least one active adapter
    CheckAdapter();
    
    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s <bt_addr>\n", argv[0]);
	fprintf(stderr, "Using default address: %s\n", dest);
    }
    else
    {
	  strncpy(dest, argv[1], ADDR_SIZE);
	  fprintf(stderr, "Using address: %s\n", dest);
    }
    str[BUFSIZ-1] = '\0';
  
    // Create a connection socket    
    if ((sd = socket (AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) < 0) 
    {
                fprintf(stderr, "Could not create socket\n");
                return 0;
    }

    // set the connection parameters (device to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    if ((status = connect (sd, (struct sockaddr *)&addr, sizeof(addr)) < 0))
    {
          fprintf(stderr, "Could not connect!\n");
          return 0;
    }

    // send a message
    if( status == 0 ) 
	status = write (sd, str, sizeof (str));
    
    if( status < 0 ) 
	perror("Error:");
    
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