/*------------------------------------------------------------------------------------------
-- SOURCE FILE: devscan.c - A simple application that will scan for Bluetooth
-- devices in close proximity to the transceiver.
--
-- PROGRAM: devscan
--
-- FUNCTIONS:
-- 
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
-- The program uses the HCI calls to establish a connection to the Bluetooth adapter 
-- microcontroller and issues various commands to scan and interrogate devices.
-- The names and addresses of the devices are then printed out. 
--
-- Compile the applications as follows:
	
	gcc -Wall -o devscan devscan.c -lbluetooth
--
---------------------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#define ADDRLEN	19
#define NAMELEN	248
#define INQLEN	8	// this value translates to an scan duration of (1.28*INQLEN) secs
#define NUMRESP	255

int main(int argc, char **argv)
{
    inquiry_info *ii = NULL;
    int max_rsp, num_rsp;
    int dev_id, hci_sock, len, flags;
    int i;
    char addr[ADDRLEN] = { 0 };
    char name[NAMELEN] = { 0 };

    if ((dev_id = hci_get_route (NULL)) < 0)
    {
	fprintf (stderr, "No Bluetooth Device:Error %d: %s\n", errno, strerror (errno));
	exit (1);
    }
    
    // connect to the microcontroller in the adapter
    if ((hci_sock = hci_open_dev (dev_id)) < 0)
    {
	fprintf (stderr, "Cannot connect to the adapter:Error %d: %s\n", errno, strerror (errno));
	exit (1);
    }
    
    len  = INQLEN;
    max_rsp = NUMRESP;
    flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));
    
    // Query the detected devices for information
    num_rsp = hci_inquiry (dev_id, len, max_rsp, NULL, &ii, flags);
    if( num_rsp < 0 ) 
      perror("hci_inquiry");

    // Print out the device names and addresses
    for (i = 0; i < num_rsp; i++) 
    {
        ba2str(&(ii+i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if (hci_read_remote_name (hci_sock, &(ii+i)->bdaddr, sizeof (name), name, 0) < 0)
	    strcpy(name, "[unknown]");
        printf ("%s  %s\n", addr, name);
    }

    free (ii);
    close (hci_sock);
    return 0;
}
