#include "../include/packet.h"

void raw_packet_init(void) {

    char datagram[sizeof(raw_pack) +
                  sizeof(pseudo_hdr)]; /* buffer for the headers */
    raw_pack *raw =
        (struct raw_pack *)datagram; /* point the raw_pack to the datagram */
    pseudo_hdr *phdr;                /* pseudo header pointer */
    FILE *file;                      /* file pointer */
    int ch;                          /* buffer storing input from file */
    int sockfd;                      /* raw socket descriptor */
    unsigned int dst,
        src; /* integers used for filling in the addresses with inet_pton() */
    struct sockaddr_in sin; /* struct used for the raw socket info */

    if ((file = fopen(o.file, "wb+")) == NULL) {
        fprintf(stderr, "file %s cannot be opened for writing\n", o.file);
        exit(EXIT_FAILURE);
    }

    memset(datagram, 0, sizeof(datagram)); /* bzero the datagram */

    if (inet_pton(AF_INET, o.src, (unsigned int *)&src) < 0)
        clean_exit("invalid dest addr", 0);

    /* make a raw socket */
    if ((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) < 0)
        clean_exit("cannot open socket", 1);

    encrypted = calloc(8191, sizeof(char));
    int index;
    printf("Waiting for messages\n");
    while (1) {
        int err = read(sockfd, (struct raw_pack *)&datagram, sizeof(raw_pack));
        char ch;
        char *remote_addr;
        if (o.port == ntohs(raw->tcp.dest)) {
            // sig fault on some machines
            inet_ntop(AF_INET, &raw->ip.saddr, remote_addr, INET_ADDRSTRLEN);
            ch = raw->tcp.seq;
            if (ch == '\n') {
                printf("Got: \"\\n\"");
                if ((ch ^ ENC_STRING[index % 512]) == '\n') {
                    printf(" Decrypted->[\\n] from [%s : %d]\n", "\\n",
                           remote_addr, o.port);
                    index++;
                } else {
                    printf(" Decrypted->[%c] from [%s : %d]\n",
                           ch ^ ENC_STRING[index++ % 512], remote_addr, o.port);
                }
            } else if ((ch ^ ENC_STRING[index % 512]) == '\n') {
                printf("Got: \"%c\" Decrypted->[\\n] from [%s : %d]\n", "\\n",
                       remote_addr, o.port);
                index++;
            } else
                printf("Got: \"%c\" Decrypted->[%c] from [%s : %d]\n", ch,
                       ch ^ ENC_STRING[index++ % 512], remote_addr, o.port);
            // fputc((char)raw->tcp.seq, file);
            // fflush(file);
            // printf("%c\n", ch ^ ENC_STRING[index++ % 512]);
        }
    }
    fclose(file);
}

/****************** MAIN PROGRAM **************************/
int main(int argc, char **argv) {

    if (argc == 1) {
        print_usage(0);
        exit(EXIT_SUCCESS);
    }

    /* option parsing */
    int opt;
    while ((opt = getopt(argc, argv, "d:s:p:f:vhD")) != -1) {
        switch (opt) {
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
            print_usage(0);
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

    if ((o.opt_ex & 0x0E) != 0x0E)
        clean_exit("need to provide all -d -p -f arguments", 0);

    /* check if u r r00t */
    if (getuid() && geteuid())
        clean_exit("need to be root", 0);

    /* create the raw packet and send it */
    raw_packet_init();

    return (EXIT_SUCCESS);
}