
#include <pcap.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <iostream>
#include <string>
#include "pkt_sniffer.h"

std::string currentCmd;
std::string currentHost;
int currentPort;
int ctl_flag;

// Function Prototypes
void pkt_callback(u_char *, const struct pcap_pkthdr *, const u_char *);


int start_sniffer(int num_packets, std::string filter, bool flag)
{
    char *nic_dev;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *nic_descr;
    struct bpf_program fp; // holds compiled program
    bpf_u_int32 maskp;     // subnet mask
    bpf_u_int32 netp;      // ip
    u_char *args = NULL;

    ctl_flag = flag;

    // find the first NIC that is up and sniff packets from it
    nic_dev = pcap_lookupdev(errbuf);
    if (nic_dev == NULL)
    {
        printf("%s\n", errbuf);
        exit(1);
    }

    // Use pcap to get the IP address and subnet mask of the device
    pcap_lookupnet(nic_dev, &netp, &maskp, errbuf);

    // open the device for packet capture & set the device in promiscuous mode
    nic_descr = pcap_open_live(nic_dev, BUFSIZ, 1, -1, errbuf);
    if (nic_descr == NULL)
    {
        printf("pcap_open_live(): %s\n", errbuf);
        exit(1);
    }

    // Compile the filter expression
    if (pcap_compile(nic_descr, &fp, filter.c_str(), 0, netp) == -1)
    {
        fprintf(stderr, "Error calling pcap_compile\n");
        char* err = pcap_geterr(nic_descr);
        std::cout << "filter:" << filter.c_str() << "error:" << err << std::endl;
        exit(1);
    }

    // Load the filter into the capture device
    if (pcap_setfilter(nic_descr, &fp) == -1)
    {
        fprintf(stderr, "Error setting filter\n");
        exit(1);
    }

    // Start the capture session
    pcap_loop(nic_descr, num_packets, pkt_callback, args);

    fprintf(stdout, "\nCapture Session Done\n");
    return 0;
}
