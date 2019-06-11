#include "cbc.h"
#include "ctr.h"
#include "ecb.h"
#include <getopt.h>

#define INT_SIZE sizeof(unsigned int) // Size of int in bytes
#define INT_BITS ((INT_SIZE * 8) - 1) // Size of int in bits - 1

/**
 * Function to rotate bits of a number to left.
 *
 * @num         Number to rotate.
 * @rotation    Number of times to rotate left.
 */
unsigned int rotateLeft(unsigned int num, unsigned int rotation) {
    unsigned int DROPPED_MSB;
    // The effective rotation
    rotation %= INT_BITS;
    // Loop till rotation becomes 0
    while (rotation--) {
        // Get MSB of num before it gets dropped
        DROPPED_MSB = (num >> INT_BITS) & 1;
        // Left rotate num by 1 and
        // Set its dropped MSB as new LSB
        num = (num << 1) | DROPPED_MSB;
    }
    return num;
}

void usage(char *av) {
    printf("[*] Usage:\n\t%s -e|d -m [ecb|cbc|ctr] -i <infile> -o <outfile> -k "
           "<4 char key>",
           av);
}

union int_chars {
    uint32_t a;
    char b[4];
};

int main(int argc, char *argv[]) {
    uint32_t keys[ROUNDS];

    int do_encrypt = 0;
    int do_decrypt = 0;
    int do_ecb = 0;
    int do_cbc = 0;
    int do_ctr = 0;
    int key_set = 0;

    union int_chars key_gen;
    FILE *infile, *outfile;

    if (argc == 1) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    int c;
    while ((c = getopt(argc, argv, "k:i:o:edhm:")) != -1) {
        switch (c) {
        case 'k':
            if (optarg) {
                fprintf(stderr, "\n[Generating Keys]\n");
                fprintf(stderr, "__________\n\n");
                strncpy(key_gen.b, optarg, 4);
                for (int i = 0; i < ROUNDS; i++) {
                    keys[i] = rotateLeft(key_gen.a, i);
                    printf("0x%08x\n", keys[i]);
                }
                fprintf(stderr, "__________\n\n\n");
                key_set = 1;
            } else {
                exit(0);
            }
            break;
        case 'i':
            if (optarg) {
                infile = fopen(optarg, "r");
                if (!infile) {
                    perror("fopen");
                    return EXIT_FAILURE;
                } else {
                    fprintf(stderr, "[*] Input path set to: %s\n", optarg);
                }
            }
            break;
        case 'o':
            if (optarg) {
                outfile = fopen(optarg, "w");
                if (!outfile) {
                    perror("fopen");
                    return EXIT_FAILURE;
                } else {
                    fprintf(stderr, "[*] Output path set to: %s\n", optarg);
                }
            }
            break;
        case 'e':
            do_encrypt = 1;
            break;
        case 'd':
            do_decrypt = 1;
            break;
        case 'h':
            usage(argv[0]);
            exit(EXIT_FAILURE);
            break;
        case 'm':
            if (!strcmp(optarg, "ecb")) {
                do_ecb = 1;
            } else if (!strcmp(optarg, "cbc")) {
                do_cbc = 1;
            } else if (!strcmp(optarg, "ctr")) {
                do_ctr = 1;
            } else {
                fprintf(stderr, "Invalid mode '%s'\n", optarg);
            }
            break;
        }
    }

    // Error checking
    if (do_encrypt && do_decrypt) {
        printf("You can't encrypt and decrypt the data at the same time!\n");
        exit(EXIT_FAILURE);
    } else if (!do_encrypt && !do_decrypt) {
        printf("You have to either choose to encryt or decrypt!\n");
        exit(EXIT_FAILURE);
    }
    if (!infile || !outfile) {
        printf(
            "You have to use -o and -i to pass input and output arguments.\n");
        exit(EXIT_FAILURE);
    }
    if (!key_set) {
        printf("You need to provide a key with -k and the key should be of 8 "
               "bytes in length!\n");
        exit(EXIT_FAILURE);
    }
    if ((do_cbc + do_ctr + do_ecb) > 1) {
        printf("You Cannot do multiple operations at the same time!\n");
        exit(EXIT_FAILURE);
    } else if ((do_cbc + do_ctr + do_ecb) == 0) {
        printf("You have to set ecb, cbc or ctr under the -m param!\n");
        exit(EXIT_FAILURE);
    }

    // Encrypt & Decrypt
    if (do_encrypt) {
        printf("[*] encrypting with: ");
        if (do_ecb) {
            printf("ecb \n");
            encrypt_ecb(infile, outfile, ROUNDS, keys);
        } else if (do_cbc) {
            printf("cbc \n");
            encrypt_cbc(infile, outfile, ROUNDS, keys);
        } else if (do_ctr) {
            printf("ctr\n");
            encrypt_ctr(infile, outfile, ROUNDS, keys);
        }
    } else if (do_decrypt) {
        printf("[*] decrypting with: ");
        if (do_ecb) {
            printf("ecb \n");
            decrypt_ecb(infile, outfile, ROUNDS, keys);
        } else if (do_cbc) {
            printf("cbc \n");
            decrypt_cbc(infile, outfile, ROUNDS, keys);
        } else if (do_ctr) {
            printf("ctr\n");
            decrypt_ctr(infile, outfile, ROUNDS, keys);
        }
    }

    fclose(infile);
    fclose(outfile);
    fprintf(stderr, "[*] Done!\n");
    return 0;
}
