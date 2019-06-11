/**------------------------------------------------------------------------------
Program: server.c
Date: March 1, 2018
Designer: Dimitry Rakhlei
Programmer: Dimitry Rakhlei
Function: Runs a server which echoes back commands. Takes in arguments which specify 
          the type of server to run. Options are [ epoll, select and basic ]
Notes:
The server takes 2 arguments. Type of server and buffer size. Tested using buffer
size of 4096.
  -------------------------------------------------------------------------------*/


#include "server.h"


int total_clients = 0;
pthread_mutex_t total_mutex;
pthread_mutex_t log_mutex;
int server_fd;
int BUFLEN = 1024;
client_record records[RECORD_LEN];
FILE *logfile;

void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents) {
<<<<<<< HEAD
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_sd;
	struct ev_io *w_client = (struct ev_io *)malloc(sizeof(struct ev_io));

	if (EV_ERROR & revents) {
		perror("got invalid event");

	} else {
		client_sd =
		  accept(watcher->fd, (struct sockaddr *)&client_addr, &client_len);

		if (client_sd < 0) {
			perror("accept error");
		} else {
			pthread_mutex_lock(&total_mutex);
			total_clients++;
			printf("Successfully connected with client\n");
			printf("%d clients connected.\n", total_clients);
			pthread_mutex_unlock(&total_mutex);
			client_record record;
			record.cli_ip = inet_ntoa(client_addr.sin_addr);
			record.cli_port = ntohs(client_addr.sin_port);
			record.set = 1;
			for (int i = 0; i < RECORD_LEN; i++) {
				if (records[i].set == 0) {
					records[i] = record;
					printf("Added Record %d\n", i);
					break;
				}
			}

			gettimeofday(&record.start_time, 0);
			ev_io_init(w_client, read_cb, client_sd, EV_READ);
			ev_io_start(loop, w_client);
		}
	}
}

void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents) {
	char buffer[BUFLEN];
	ssize_t read;

	if (EV_ERROR & revents) {
		perror("got invalid event");
		return;
	}

	read = recv(watcher->fd, buffer, BUFLEN, 0);

	if (read < 0) {
		perror("read error");
		return;
	}

	if (read == 0) {
		ev_io_stop(loop, watcher);
		// struct sockaddr_in addr;
		// socklen_t size = sizeof(struct sockaddr);
		// getpeername(watcher->fd, (struct sockaddr *)&addr, &size);
		// char *ip = inet_ntoa(addr.sin_addr);
		// int port = ntohs(addr.sin_port);
		// for (int i = 0; i < RECORD_LEN; i++) {
		//     if (records[i].cli_port == port && records[i].cli_ip == ip) {
		//         gettimeofday(&records[i].end_time, 0);
		//     }
		// }
		free(watcher);
		pthread_mutex_lock(&total_mutex);
		fprintf(stderr, "connection done: %d clients left: ", --total_clients);
		perror("");
		pthread_mutex_unlock(&total_mutex);
		return;
	} else {
		struct sockaddr_in addr;
		socklen_t size = sizeof(struct sockaddr);
		getpeername(watcher->fd, (struct sockaddr *)&addr, &size);
		char *ip = inet_ntoa(addr.sin_addr);
		int port = ntohs(addr.sin_port);
		// printf("message:%s\n", buffer);
		for (int i = 0; i < RECORD_LEN; i++) {
			if (records[i].cli_port == port && records[i].cli_ip == ip) {
				records[i].transfered += read;
				printf("%s:%d-> %lu B\n", ip, port, records[i].transfered);
			}
		}
	}

	send(watcher->fd, buffer, read, 0);
	bzero(buffer, read);
}

void basic_server_handle_client(void *arg) {
	int *client_fd = (int *)arg;
	char buf[BUFLEN];
	char *bp;
	int n, bytes_to_read = 0;
	perror("inside");
	while (TRUE) {
		bp = buf;
		bytes_to_read = BUFLEN;
		while ((n = recv(*client_fd, bp, bytes_to_read, 0)) < BUFLEN) {
			if (n <= 0) {
				perror("Socket Got 0");
				break;
			}
			bp += n;
			bytes_to_read -= n;
		}
		if (n <= 0) {
			break;
		}

		printf("sending:%ld, total clients: %d\n", strlen(buf), total_clients);
		send(*client_fd, buf, BUFLEN, 0);
		memset(buf, 0, BUFLEN);
	}

	pthread_mutex_lock(&total_mutex);
	total_clients--;
	printf("Client Disconnected, Total Clients: %d\n", total_clients);
	pthread_mutex_unlock(&total_mutex);
	close(*client_fd);
=======

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sd;
    struct ev_io *w_client = (struct ev_io *)malloc(sizeof(struct ev_io));

    if (EV_ERROR & revents) {
        perror("got invalid event");

    } else {
        client_sd =
            accept(watcher->fd, (struct sockaddr *)&client_addr, &client_len);

        if (client_sd < 0) {
            perror("accept error");
        } else {

            pthread_mutex_lock(&total_mutex);
            total_clients++;
            printf("Connected, Total Clients: %d\n", total_clients);
            pthread_mutex_unlock(&total_mutex);

            ev_io_init(w_client, read_cb, client_sd, EV_READ);
            ev_io_start(loop, w_client);

            getpeername(watcher->fd, (struct sockaddr *)&client_addr,
                        &client_len);
            char *ip = inet_ntoa(client_addr.sin_addr);
            int port = ntohs(client_addr.sin_port);
            struct timeval curtime;
            gettimeofday(&curtime, 0);

            pthread_mutex_lock(&log_mutex);
            log_val(ip, port, NEW_LOG, 0,
                    curtime.tv_sec * 1000000 + curtime.tv_usec);
            pthread_mutex_unlock(&log_mutex);
        }
    }
}

void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents) {
    char buffer[BUFLEN];
    ssize_t read;

    if (EV_ERROR & revents) {
        perror("got invalid event");
        return;
    }

    read = recv(watcher->fd, buffer, BUFLEN, 0);

    if (read < 0) {
        perror("read error");
        return;
    }

    if (read == 0) {
        ev_io_stop(loop, watcher);
        struct sockaddr_in addr;
        socklen_t size = sizeof(struct sockaddr);
        getpeername(watcher->fd, (struct sockaddr *)&addr, &size);
        char *ip = inet_ntoa(addr.sin_addr);
        int port = ntohs(addr.sin_port);
        struct timeval curtime;
        gettimeofday(&curtime, 0);

        pthread_mutex_lock(&log_mutex);
        log_val(ip, port, DONE_LOG, 0,
                curtime.tv_sec * 1000000 + curtime.tv_usec);
        pthread_mutex_unlock(&log_mutex);
        // pthread_mutex_lock(&records_mutex);
        // for (int i = 0; i < RECORD_LEN; i++) {
        //     if (records[i].cli_port == port && records[i].cli_ip == ip) {
        //         gettimeofday(&records[i].end_time, 0);
        //         break;
        //     }
        // }
        // pthread_mutex_unlock(&records_mutex);

        free(watcher);

        pthread_mutex_lock(&total_mutex);
        fprintf(stderr,
                "connection done, clients left: %d : ", --total_clients);
        perror("");
        pthread_mutex_unlock(&total_mutex);
        // if (total_clients == 0) {

        //     pthread_mutex_lock(&records_mutex);
        //     client_record *records_cpy = calloc(RECORD_LEN,
        //     sizeof(client_record)); memcpy(records_cpy, records, RECORD_LEN *
        //     sizeof(client_record)); perror("memcpy");
        //     pthread_mutex_unlock(&records_mutex);

        //     for (int i = 0; i < RECORD_LEN; i++) {
        //         fprintf(stderr,
        //                 "client %s:%d spent %ld usec and sent %ld bytes \n",
        //                 records_cpy[i].cli_ip, records_cpy[i].cli_port,
        //                 calc_time(&records_cpy[i].end_time,
        //                 &records_cpy[i].start_time),
        //                 records_cpy[i].transfered);
        //     }

        // }

        return;
    } else {
        struct sockaddr_in addr;
        socklen_t size = sizeof(struct sockaddr);
        getpeername(watcher->fd, (struct sockaddr *)&addr, &size);
        char *ip = inet_ntoa(addr.sin_addr);
        int port = ntohs(addr.sin_port);
        size_t sent = send(watcher->fd, buffer, read, 0);
        bzero(buffer, read);
        if (sent > 0) {
            pthread_mutex_lock(&log_mutex);
            log_val(ip, port, DATA_LOG, sent, 0);
            pthread_mutex_unlock(&log_mutex);
        }
    }
}

void basic_server_handle_client(void *arg) {
    int *client_fd = (int *)arg;
    char buf[BUFLEN];
    char *bp;
    int n, bytes_to_read = 0;
    struct timeval curtime;
    struct sockaddr_in addr;
    socklen_t size = sizeof(struct sockaddr);
    getpeername(*client_fd, (struct sockaddr *)&addr, &size);
    char *ip = inet_ntoa(addr.sin_addr);
    int port = ntohs(addr.sin_port);

    gettimeofday(&curtime, 0);
    pthread_mutex_lock(&log_mutex);
    log_val(ip, port, NEW_LOG, 0, curtime.tv_sec * 1000000 + curtime.tv_usec);
    pthread_mutex_unlock(&log_mutex);

    while (TRUE) {
        bp = buf;
        bytes_to_read = BUFLEN;
        while ((n = recv(*client_fd, bp, bytes_to_read, 0)) < BUFLEN) {
            if (n <= 0) {
                perror("Socket Got 0");
                break;
            }
            bp += n;
            bytes_to_read -= n;
        }
        if (n <= 0) {
            break;
        }

        printf("sending:%ld, total clients: %d\n", strlen(buf), total_clients);
        int sent = send(*client_fd, buf, BUFLEN, 0);
        if (sent > 0) {
            pthread_mutex_lock(&log_mutex);
            log_val(ip, port, DATA_LOG, sent, 0);
            pthread_mutex_unlock(&log_mutex);
        }
        memset(buf, 0, BUFLEN);
    }

    gettimeofday(&curtime, 0);
    pthread_mutex_lock(&log_mutex);
    log_val(ip, port, DONE_LOG, 0, curtime.tv_sec * 1000000 + curtime.tv_usec);
    pthread_mutex_unlock(&log_mutex);

    pthread_mutex_lock(&total_mutex);
    total_clients--;
    printf("Client Disconnected, Total Clients: %d\n", total_clients);
    pthread_mutex_unlock(&total_mutex);
    close(*client_fd);
>>>>>>> f536d0c6581c21f5492b13c2d3d49e22ba7457ed
}

long calc_time(struct timeval *end, struct timeval *start) {
    return (((*end).tv_sec * 1000000 + (*end).tv_usec) -
            ((*start).tv_sec * 1000000 + (*start).tv_usec));
}

void log_val(char *ip, int port, int opt, long sent, long time) {
    if (opt != DONE_LOG && opt != NEW_LOG && opt != DATA_LOG)
        return;

    switch (opt) {
    case NEW_LOG:
        // printf("%s:%d:%s:%ld:%ld\n", ip, port, "start", sent, time);
        fprintf(logfile, "%s:%d:%s:%ld:%ld\n", ip, port, "start", sent, time);
        break;
    case DATA_LOG:
        // printf("%s:%d:%s:%ld:%ld\n", ip, port, "data", sent, time);
        fprintf(logfile, "%s:%d:%s:%ld:%ld\n", ip, port, "data", sent, time);
        break;
    case DONE_LOG:
        // printf("%s:%d:%s:%ld:%ld\n", ip, port, "end", sent, time);
        fprintf(logfile, "%s:%d:%s:%ld:%ld\n", ip, port, "end", sent, time);
        break;
    default:
        break;
    }
    fflush(logfile);
}

int main(int argc, char **argv) {
<<<<<<< HEAD
	char server_type = -1;
	struct ev_loop *loop;
	struct sockaddr_in addr, client;
	struct ev_io accept_io;
	socklen_t client_len;
	int new_sd, n;
	threadpool_t *pool;
	pthread_mutex_init(&total_mutex, NULL);

	memset(&records, 0, RECORD_LEN * sizeof(client_record));

	FILE *logfile = fopen("./logs/server.log", "w+");

	if (logfile == 0) {
		perror("log failed");
		exit(-1);
	}

	// deal with inputs
	if (argc < 3) {
		perror("not enough arguments, need ./program [server_type] [buflen]");
		return 1;
	}

	n = BUFLEN;
	if (sscanf(argv[2], "%d", &n) < 0) {
		perror("sscanf failed");
		exit(1);
	}
	BUFLEN = n;
	printf("Buflen Set To= %d\n", BUFLEN);
	char buf[BUFLEN];

	if (strcmp(argv[1], "epoll") == 0) {
		server_type = 0;
		if (!(loop = ev_default_loop(EVBACKEND_EPOLL))) {
			perror("could not open epoll loop");
			return -1;
		}
	} else if (strcmp(argv[1], "select") == 0) {
		server_type = 1;
		if (!(loop = ev_default_loop(EVBACKEND_SELECT))) {
			perror("could not open select loop");
			return -1;
		}
	} else if (strcmp(argv[1], "basic") == 0) {
		server_type = 2;
		pool = threadpool_create(THREAD_CAP, QUEUE_CAP, 0);
	} else {
		perror(
		  "unrecognized type of server\n\tacceptable types: [epoll, "
		  "select, poll]");
		return 1;
	}

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error");
		return -1;
	}

	if (server_type < 2) {
		int flags = fcntl(server_fd, F_GETFL, 0);
		assert(flags != -1);
		if (fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0) {
			perror("fcntl(O_NONBLOCK) failed");
			return -1;
		}
	}

	int optval = BUFLEN;
	if (setsockopt(server_fd, SOL_SOCKET, SO_RCVLOWAT, &optval, sizeof(optval))
	    < 0) {
		perror("setsockopt(SO_REUSEPORT) failed");
		return -1;
	}

	optval = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval))
	    < 0) {
		perror("setsockopt(SO_REUSEPORT) failed");
		return -1;
	}

	memset(&addr, 0, sizeof(struct sockaddr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_TCP_PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
		perror("bind error");
		return -1;
	}

	if (listen(server_fd, LISTENQ) < 0) {
		perror("listen error");
		return -1;
	}

	if (server_type < 2) {
		ev_io_init(&accept_io, accept_cb, server_fd, EV_READ);
		ev_io_start(loop, &accept_io);
		ev_loop(loop, 0);
	} else if (server_type == 2) {
		printf("Starting Basic Server\n");

		while (TRUE) {
			client_len = sizeof(client);
			memset(buf, 0, BUFLEN);
			if ((new_sd =
			       accept(server_fd, (struct sockaddr *)&client, &client_len))
			    == -1) {
				perror("Could Not Accept");
				continue;
			}
			printf("New Client:  %s:%d, Total Clients: %d\n",
			       inet_ntoa(client.sin_addr), ntohs(client.sin_port),
			       ++total_clients);

			threadpool_add(pool, &basic_server_handle_client, (void *)&new_sd,
			               0);
			// basic_server_handle_client(new_sd);
		}
		close(server_fd);
	}

	// thpool_destroy(thpool);
	return 0;
=======
    char server_type = -1;
    struct ev_loop *loop;
    struct sockaddr_in addr, client;
    struct ev_io accept_io;
    socklen_t client_len;
    int new_sd, n;
    threadpool_t *pool;
    pthread_mutex_init(&total_mutex, NULL);
    pthread_mutex_init(&log_mutex, NULL);

    memset(&records, 0, RECORD_LEN * sizeof(client_record));

    srand(time(NULL));
    rand();
    char logname[256];
    sprintf(logname, "./logs/server-%d.log", rand());
    printf("%s\n", logname);
    logfile = fopen(logname, "w+");

    if (logfile == 0) {
        perror("log failed");
        exit(-1);
    }

    // deal with inputs
    if (argc < 3) {
        perror("not enough arguments, need ./program [server_type] [buflen]");
        return 1;
    }

    n = BUFLEN;
    if (sscanf(argv[2], "%d", &n) < 0) {
        perror("sscanf failed");
        exit(1);
    }
    BUFLEN = n;
    printf("Buflen Set To= %d\n", BUFLEN);
    char buf[BUFLEN];

    if (strcmp(argv[1], "epoll") == 0) {
        server_type = 0;
        if (!(loop = ev_default_loop(EVBACKEND_EPOLL))) {
            perror("could not open epoll loop");
            return -1;
        }
    } else if (strcmp(argv[1], "select") == 0) {
        server_type = 1;
        if (!(loop = ev_default_loop(EVBACKEND_SELECT))) {
            perror("could not open select loop");
            return -1;
        }
    } else if (strcmp(argv[1], "basic") == 0) {
        server_type = 2;
        pool = threadpool_create(THREAD_CAP, QUEUE_CAP, 0);
    } else {
        perror("unrecognized type of server\n\tacceptable types: [epoll, "
               "select, poll]");
        return 1;
    }

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        return -1;
    }

    if (server_type < 2) {
        int flags = fcntl(server_fd, F_GETFL, 0);
        assert(flags != -1);
        if (fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0) {
            perror("fcntl(O_NONBLOCK) failed");
            return -1;
        }
    }

    int optval = BUFLEN;
    if (setsockopt(server_fd, SOL_SOCKET, SO_RCVLOWAT, &optval,
                   sizeof(optval)) < 0) {
        perror("setsockopt(SO_REUSEPORT) failed");
        return -1;
    }

    optval = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &optval,
                   sizeof(optval)) < 0) {
        perror("setsockopt(SO_REUSEPORT) failed");
        return -1;
    }

    memset(&addr, 0, sizeof(struct sockaddr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_TCP_PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        perror("bind error");
        return -1;
    }

    if (listen(server_fd, LISTENQ) < 0) {
        perror("listen error");
        return -1;
    }

    if (server_type < 2) {
        ev_io_init(&accept_io, accept_cb, server_fd, EV_READ);
        ev_io_start(loop, &accept_io);
        ev_loop(loop, 0);
    } else if (server_type == 2) {
        printf("Starting Basic Server\n");

        while (TRUE) {
            client_len = sizeof(client);
            memset(buf, 0, BUFLEN);
            if ((new_sd = accept(server_fd, (struct sockaddr *)&client,
                                 &client_len)) == -1) {
                perror("Could Not Accept");
                continue;
            }
            printf("New Client:  %s:%d, Total Clients: %d\n",
                   inet_ntoa(client.sin_addr), ntohs(client.sin_port),
                   ++total_clients);

            threadpool_add(pool, &basic_server_handle_client, (void *)&new_sd,
                           0);
            // basic_server_handle_client(new_sd);
        }
        close(server_fd);
    }

    // thpool_destroy(thpool);
    return 0;
>>>>>>> f536d0c6581c21f5492b13c2d3d49e22ba7457ed
}
