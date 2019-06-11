#include "../include/packet.h"

/* generic exit printing func
 * it prints a perror msg additionally
 * if err is 1
 */
void clean_exit(char *msg, int err) {
    if (err == 1)
        perror(msg);
    else
        fprintf(stderr, "%s \n", msg);

    exit(EXIT_FAILURE);
}

void print_usage(char t) {
    if (t == 1)
        fprintf(stdout, "Options: \n"
                        "-d: destination ip \n"
                        "-s: source ip \n"
                        "-p: destination port \n"
                        "-f: file name \n"
                        "-v: verbose mode \n"
                        "-h: help \n"
                        "\n");
    else
        fprintf(stdout, "Options: \n"
                        //"-d: destination ip \n"
                        "-s: host ip \n"
                        "-p: host port \n"
                        "-f: file name \n"
                        "-v: verbose mode \n"
                        "-h: help \n"
                        "\n");
    exit(EXIT_SUCCESS);
}

uint16_t checksum_comp(uint16_t *addr, int len) {
    register long sum = 0;
    int count = len;
    uint16_t temp;

    while (count > 1) {
        temp = htons(*addr++); // in this line:added -> htons
        sum += temp;
        count -= 2;
    }

    /*  Add left-over byte, if any */
    if (count > 0)
        sum += *(unsigned char *)addr;

    /*  Fold 32-bit sum to 16 bits */
    while (sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);

    uint16_t checksum = ~sum;
    return checksum;
}

int read_file(FILE *in) {
    char *buffer;
    fseek(in, 0, SEEK_END);
    long fsize = ftell(in);
    fseek(in, 0, SEEK_SET); // same as rewind(f);

    buffer = calloc(fsize + 1, sizeof(char));
    fread(buffer, fsize, 1, in);
    fclose(in);

    buffer[fsize] = 0;
    str = malloc(fsize + 1);
    strcpy(str, buffer);
    return fsize;
}

int encrypt_file(FILE *file) {
    int len = read_file(file);
    encrypted = malloc(len + 1);
    for (int i = 0; i < len; i++) {
        encrypted[i] = str[i] ^ ENC_STRING[i % 512];
    }
    printf("File Contents: %s\nEncrypted Contents: %s\nEncryption String: %s\n",
           str, encrypted, ENC_STRING);
    return len;
}

void decrypt_string(char *encrypted, int len) {
    for (int i = 0; i < len; i++) {
        printf("Decrypted: [%c]", encrypted[i] ^ ENC_STRING[i % 512]);
    }
}