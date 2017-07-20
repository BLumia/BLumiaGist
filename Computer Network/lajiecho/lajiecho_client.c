#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <getopt.h>
#include <readline/readline.h>
#include "socketwrapper.h"

int isTCP = 1;
char hostAddr[61];
char* prgName;
int allowedRun = 0;

void process_arguments(int argc, char **argv) {
	int opt;
	
	while (~(opt = getopt(argc, argv, "s:S:tTuU"))) { 
		switch(opt) {
			case 't': case 'T':
				isTCP = 1;
				break;
			case 'u': case 'U':
				isTCP = 0;
				break;
			case 's': case 'S':
				strcpy(hostAddr,optarg);
				allowedRun = 1;
				break;
		}
    }
} 

void str_cli(FILE* fp, int socketfd) {
	char sendline[MAXLINE], recvline[MAXLINE];
	
	fputs("Client now running in TCP mode.\n", stdout);
	while (fgets(sendline, MAXLINE, fp) != NULL) {
		write(socketfd, sendline, strlen(sendline));
		if (fdgets(socketfd, recvline, MAXLINE) == 0) {
			printf("str_cli: srv terminated prematurely");
			exit(0);
		}
		fputs(recvline, stdout);
	}
}

void dg_cli(FILE *fp, int socketfd, const SA* psrvaddr, socklen_t srvlen) {
	int n;
	char sendline[MAXLINE], recvline[MAXLINE + 1];
	
	fputs("Client now running in UDP mode.\n", stdout);
	while (fgets(sendline, MAXLINE, fp) != NULL) {
		sendto(socketfd, sendline, strlen(sendline), 0, psrvaddr, srvlen);
		n = recvfrom(socketfd, recvline, MAXLINE, 0, NULL, NULL);
		recvline[n] = '\0';
		fputs(recvline, stdout);
	}
}

int main(int argc, char** argv) {
	
	int socketfd;
	struct sockaddr_in srvaddr;
	
	prgName = strrchr(argv[0], '/');
	if(prgName) prgName++;
	else prgName = argv[0];
	
	process_arguments(argc, argv);
	
	if (allowedRun == 0) {
		printf("usage: %s -s<ip_addr> [-u] [-t]");
		exit(0);
	}

	if (isTCP) {
		socketfd = Socket(AF_INET, SOCK_STREAM, 0);
	} else {
		socketfd = Socket(AF_INET, SOCK_DGRAM, 0);
	}

	bzero(&srvaddr, sizeof(srvaddr));
	
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, hostAddr, &srvaddr.sin_addr);
	
	if (isTCP) {
		connect(socketfd, (SA*) &srvaddr, sizeof(srvaddr));
		str_cli(stdin, socketfd);
	} else {
		dg_cli(stdin, socketfd, (SA*) &srvaddr, sizeof(srvaddr));
	}

	exit(0);
} 
