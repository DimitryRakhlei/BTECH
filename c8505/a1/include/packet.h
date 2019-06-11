#include <arpa/inet.h>
#include <errno.h>
#include <linux/ip.h>  // linux ip header struct
#include <linux/tcp.h> // linux tcp header struct
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/* Encryption String */
#define ENC_STRING                                                             \
    "BBPAFIqjxQIGzyssjiYGHzwStfd3hbq3p3upa9YUGzlBnXkjTTzMGJ92GXXol25HvDzR96g0" \
    "uFoA4QVxSOtIQVmnMutfGzSAgSlVyFOLfm6uLYOhWSCnI4ByRBYklvfmgCIpgsSVxoJtoYWB" \
    "11m453wDxuFNasGi2ELcVYCdnQpbzLbytenDUVz2pF4BD0wS5MmtWItyk8ZnIYXAjdZOd1U8" \
    "7rZGBfxQYlwh7aIy4W82twr38YgC15HYoUFdJu3iApKZNPT1Kh7bae9pRD00Jb5MzFfWq59L" \
    "bkg0fTUQYLetKLCVQTxwVwOUDQCt3PPNoSwXWc8NsMubHg3FCRMMrI0uluZtG5vERHFq6MSq" \
    "ZS9cvdaBDfCfZlm2JzHmgOSgeoKNEamxe3OZr72w14GhvANMGOzihMm7BzunbkZ2nTqDti49" \
    "oKdtLi27DHHSFTV0nbbLe0t0pJboifDNrBhAGpELzwLaLXhZg5lzakog4SpsHK2TgOWoFOez" \
    "WCdz9Tt"

/* DECLARATIONS */
#define TRUE 1
#define FALSE 0

/* FUNCTION PROTOTYPES */
void print_usage(char t);
void raw_packet_init(void);
uint16_t checksum_comp(uint16_t *, int);
void clean_exit(char *, int);
int encrypt_file(FILE *file);
int read_file(FILE *in);
void decrypt_string(char *encrypted, int len);

/* option stuct - idea taken by nmap */
typedef struct options {
    char src[16]; /*  16 = 4*3 digits + 3 dots + 1'\0' */
    char dst[16];
    unsigned long int port;
    char *file;
    unsigned short int mode;
    unsigned short int verbose;
    char opt_ex; /* bitwise boolean - if an option exists
                     corresponding bit will be 1 or else 0 -the setting
                     takes place inside the getopt loop */
#ifdef DEBUG
    unsigned short int debug;
#endif
} options;

/* this struct contains both the ip header
 * and tcp header - if we reversed the
 * order of the headers would it work ???
 */
typedef struct raw_pack {
    struct iphdr ip;
    struct tcphdr tcp;
} raw_pack;

/* pseudo header used for checksuming */
/* this header never reaches the wire */
typedef struct pseudo_hdr {
    u_int32_t src;
    u_int32_t dst;
    __u_char mbz;
    __u_char proto;
    u_int16_t len;
} pseudo_hdr;

/* GLOBAL VARIABLES */

options o;
char *str;
char *encrypted;