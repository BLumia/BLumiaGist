#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "socketwrapper.h"

int isTCP = 1;

void process_arguments(int argc, char **argv) {
	int opt;
	
	while (~(opt = getopt(argc, argv, "tTuU"))) { 
		switch(opt) {
			case 't': case 'T':
				isTCP = 1;
				break;
			case 'u': case 'U':
				isTCP = 0;
				break;
		}
    }
} 

void str_echo(int socketfd) {
	ssize_t n;
	char buf[MAXLINE];

again:
	while ( (n = read(socketfd, buf, MAXLINE)) > 0) {
		write(socketfd, buf, n);
	}

	if (n < 0 && errno == EINTR) goto again;
	else if (n < 0) {
		perror("str_echo: read error");
	}

}

void dg_echo(int socketfd, SA* pcliaddr, socklen_t clilen) {
	int n;
	socklen_t len;
	char msg[MAXLINE+1];
	
	for(;;) {
		len = clilen;
		n = recvfrom(socketfd, msg, MAXLINE, 0, pcliaddr, &len);
		sendto(socketfd, msg, n, 0, pcliaddr, len);
	}
}

int main(int argc, char** argv) {

	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, srvaddr;
	
	process_arguments(argc, argv);
	
	if (isTCP) {
		listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	} else {
		listenfd = Socket(AF_INET, SOCK_DGRAM, 0);
	}
	
	bzero(&srvaddr, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons(SERV_PORT);
	
	bind(listenfd, (SA*) &srvaddr, sizeof(srvaddr));
	
	if (isTCP) {
		fputs("Server now running in TCP mode.\n", stdout);
		listen(listenfd, LISTENQ);

		for(;;) {
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (SA*) &cliaddr, &clilen);

			if ((childpid = fork()) == 0) {
				close(listenfd);
				str_echo(connfd);
				exit(0);
			}
			close(connfd);
		}
	} else {
		fputs("Server now running in UDP mode.\n", stdout);
		dg_echo(listenfd, (SA *) &srvaddr, sizeof(cliaddr));
	}
}
