#if defined(__linux__)
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/random.h>
#endif

#if defined(__linux__) && defined(RNDGETENTCNT)

void check_entropy();
 
#endif