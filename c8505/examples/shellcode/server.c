/* Simple echo server with a BUFFER OVERFLOW VULNERABILITY
 * 2011
 * Compile as: gcc -fno-stack-protector -z execstack -o server server.c
 * Also remember to: 
 * 	echo 0 > /proc/sys/kernel/randomize_va_space
 *	echo 0 > /proc/sys/kernel/exec-shield
 */
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
  	int sockfd;
  	int client;
  	struct sockaddr_in my_addr; 
  	struct sockaddr_in client_addr;
  	int client_size;

  	char buf[512];

  	if (argc != 2) 
	{
    		printf("Usage: %s [port]\n", argv[0]);
    		return -1;
  	}

  	sockfd = socket(PF_INET, SOCK_STREAM, 0);
  	if (sockfd <= 0) 
	{
    		perror("socket");
    		return -1;
  	}

  	memset(&my_addr, 0, sizeof(my_addr));
  	my_addr.sin_family = AF_INET;
  	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  	my_addr.sin_port = htons(atoi(argv[1]));

  	if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0) 
	{
    		perror("bind");
    		return -1;
  	}

  	if (listen(sockfd, 1) < 0) 
	{
    		perror("listen");
    		return -1;
  	}

  	client = 0;
	printf("buf ended up at %p\n", buf);
  	while (1) 
	{
    		client_size = sizeof(client_addr);

    		if (!client) 
		{
      			client = accept(sockfd, (struct sockaddr *) &client_addr, &client_size);
      			if (client < 0) 
			{
        			perror("client");
        			return -1;
      			}
      			else 
			{
        			printf("Connected to %s\n", inet_ntoa(client_addr.sin_addr));
      			}
    		}

    		client_size = recv(client, buf, 1024, 0);
    		if (client_size < 0) 
		{
      			perror("recv");
      			return -1;
    		}

    		if (client_size == 1) return 0;

    		if (send(client, buf, client_size, 0) < 0) 
		{
      			perror("send");
      			return -1;
    		}
	}
  	return 0;
}

