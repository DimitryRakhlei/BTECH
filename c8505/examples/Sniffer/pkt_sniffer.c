/*---------------------------------------------------------------------------------------------
--	SOURCE FILE:	pkt_sniffer.c -   A simple but complete packet capture
--					program that will capture and parse datagrams
--
--	FUNCTIONS:		libpcap - packet filtering library based on the BSD packet
--					filter (BPF)
--
--	DATE:			April 23, 2006
--
--	REVISIONS:		(Date and nic_description)
--
--				March 29, 2011
--				Fixed memory leak - no more malloc
--
--				April 26, 2011
--				Fixed the pcap_open_live function issues
--				Use the pcap_lookupnet function before using pcap_open_live
--
--				April 10, 2014
--				Added TCP header processing in proc_hdrs.c
--
--
--	DESIGNERS:		Based on the code by Martin Casado
--					Code was also taken from tcpdump source, namely from the
following files:
--					print-ether.c
--					print-ip.c
--					ip.h
--					Modified & redesigned: Aman Abdulla: 2006, 2014, 2016
--
--	PROGRAMMER:		Aman Abdulla
--
--	NOTES:
--	The program will selectively capture a specified number packets using a
specified filter
--	The program will parse the headers and print out selected fields of
interest.
---
--	Compile:
--		Use the Makefile provided
--	Run:
--		./pkt_sniffer 5 "udp and port 53"
--
-------------------------------------------------------------------------------------------------*/

#include "pkt_sniffer.h"
#include <netinet/ip.h>
#include <pcap.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Function Prototypes
void pkt_callback(u_char *, const struct pcap_pkthdr *, const u_char *);

int main(int argc, char **argv) {
    char *nic_dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *nic_descr;
    struct bpf_program fp; // holds compiled program
    bpf_u_int32 maskp;     // subnet mask
    bpf_u_int32 netp;      // ip
    u_char *args = NULL;

    // Options must be passed in as a string
    if (argc < 2) {
        fprintf(stdout, "Usage: %s <Number of Packets> \"<Filter String>\"\n",
                argv[0]);
        return 0;
    }

    pcap_if_t *interface_list;
    pcap_if_t *if_list_ptr;
    int result;
    int i;

    // Get a list of interfaces
    result = pcap_findalldevs(&interface_list, errbuf);
    if (result == -1) {
        fprintf(stderr, "%s\n", errbuf);
        exit(1);
    }

    if_list_ptr = interface_list;
    i = 0;
    while (if_list_ptr) {
        if (if_list_ptr->description) {
            //if (strcmp("wlp3s0", if_list_ptr->name) == 0) {
            //printf("%d. %s (%s)\n", i, if_list_ptr->name,
            //       if_list_ptr->description);
			//printf("%s : %s\n", if_list_ptr->name, if_list_ptr->description);
            
			//printf("%s\n", nic_dev);
            //break;
            //}
        } else {
            printf("%d. %s\n", i, if_list_ptr->name);
            nic_dev = malloc(strlen(if_list_ptr->name));
            strcpy(nic_dev, if_list_ptr->name);
            break;
        }
        if_list_ptr = if_list_ptr->next;
        i += 1;
    }
    // free the data structures
    pcap_freealldevs(interface_list);

    // Use pcap to get the IP address and subnet mask of the device
    pcap_lookupnet(nic_dev, &netp, &maskp, errbuf);

    // open the device for packet capture & set the device in promiscuous mode
    nic_descr = pcap_open_live(nic_dev, BUFSIZ, 1, -1, errbuf);
    if (nic_descr == NULL) {
        printf("pcap_open_live(): %s\n", errbuf);
        exit(1);
    }

    if (argc > 2) {
        // Compile the filter expression
        if (pcap_compile(nic_descr, &fp, argv[2], 0, netp) == -1) {
            fprintf(stderr, "Error calling pcap_compile\n");
            exit(1);
        }

        // Load the filter into the capture device
        if (pcap_setfilter(nic_descr, &fp) == -1) {
            fprintf(stderr, "Error setting filter\n");
            exit(1);
        }
    }

    // Start the capture session
    pcap_loop(nic_descr, atoi(argv[1]), pkt_callback, args);

    fprintf(stdout, "\nCapture Session Done\n");
    return 0;
}
