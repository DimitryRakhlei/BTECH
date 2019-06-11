#include "checkentropy.h"

void check_entropy()
{
    int fd;
    int c;
        fputs("Checking Entropy\n", stderr);
        if ((fd = open("/dev/random", O_RDONLY)) != -1) {
        if (ioctl(fd, RNDGETENTCNT, &c) == 0 && c < 160) {
            fputs("This system doesn't provide enough entropy to quickly generate high-quality random numbers.\n"
                "Installing the rng-utils/rng-tools, jitterentropy or haveged packages may help.\n"
                "On virtualized Linux environments, also consider using virtio-rng.\n"
                "The service will not start until enough entropy has been collected.\n", stderr);
        }
        (void) close(fd);
    }
}
