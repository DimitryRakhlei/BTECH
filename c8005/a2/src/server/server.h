#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <ev.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <omp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "../pool/src/threadpool.h"



#define SERVER_TCP_PORT 8005 // Default port
#define LISTENQ 5
#define MAXLINE 4096
#define RECORD_LEN 10
#define THREAD_CAP 8
#define QUEUE_CAP 1024

#define TRUE 1
#define FALSE 0

#define NEW_LOG 4
#define DATA_LOG 32
#define DONE_LOG 64

void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
void basic_server_handle_client(void *arg);
long calc_time(struct timeval *start, struct timeval *end);
void log_val(char* ip, int port, int opt, long sent, long time);



typedef struct {
    char *cli_ip;
    int cli_port;
    long transfered;
    struct timeval start_time;
    struct timeval end_time;
    char set;
} client_record;

#endif