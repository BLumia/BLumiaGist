#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include "socketwrapper.h"

int isTCP = 1;
int epollfd;

struct echo_event {
	int fd;
	uint32_t event;
	char data[MAXLINE];
	int length;
	int offset;
};

void Epoll_ctl(int epollfd, int op, int sfd, uint32_t evt, void* data) {
	struct epoll_event event;
	event.events = evt;
	event.data.ptr = data;
	if (epoll_ctl(epollfd, op, sfd, &event) == -1) {
		perror("epoll_ctl");
		exit(1);
	}
}

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

void* handle(void* ptr) {
	struct echo_event* echo_evt = ptr;
	
	if (echo_evt->event == EPOLLIN) {
		int n = read(echo_evt->fd, echo_evt->data, MAXLINE);
		
		if (n == 0) { // close it
			close(echo_evt->fd);
			free(echo_evt);
		} else if (n == -1) {
			perror("read");
			close(echo_evt->fd);
			free(echo_evt);
		} else { // response it
			echo_evt->length = n;
			// add write evt
			Epoll_ctl(epollfd, EPOLL_CTL_ADD, echo_evt->fd, EPOLLOUT, echo_evt);
		}
	} else if (echo_evt->event == EPOLLOUT) {
		int n = write(echo_evt->fd, (echo_evt->data) + (echo_evt->offset), echo_evt->length);
		if (n == -1 && errno == EINTR || echo_evt->length > n) {
			// still need send(write) again
			Epoll_ctl(epollfd, EPOLL_CTL_ADD, echo_evt->fd, EPOLLOUT, echo_evt);
			// now we dont care about if sent half of the str, so no offset thing.
		} else if (n == -1) { // error occered, close and free
			perror("read");
			close(echo_evt->fd);
			free(echo_evt);
		} else {
			// writen done, continue read
			Epoll_ctl(epollfd, EPOLL_CTL_ADD, echo_evt->fd, EPOLLIN, echo_evt);
		}
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
		listenfd = Socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	} else {
		listenfd = Socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, 0);
	}
	
	bzero(&srvaddr, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons(SERV_PORT);
	
	bind(listenfd, (SA*) &srvaddr, sizeof(srvaddr));
	
	if (isTCP) {
		fputs("Server now running in TCP mode.\n", stdout);
		listen(listenfd, LISTENQ);

		if ((epollfd = epoll_create1(0)) == -1) {
			perror("epoll");
			exit(0);
		}
		
		Epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, EPOLLIN, &listenfd);
		
		clilen = sizeof(cliaddr);
		
		struct epoll_event *evt_list = calloc(64, sizeof(struct epoll_event));
		for(;;) {
			int n = epoll_wait(epollfd, evt_list, 64, -1);
			if (n == -1) {
				perror("epoll_wait");
				exit(1);
			}
			
			for(int i = 0; i < n; i++) {
				if (evt_list[i].data.ptr == &listenfd) {
					if (evt_list[i].events & EPOLLHUP || 
						evt_list[i].events & EPOLLERR) {
							fputs("epoll main loop, EPOLLHUP or EPOLLERR", stdout);
							close(listenfd);
							exit(1);
					}
					connfd = Accept(listenfd, (SA*) &cliaddr, &clilen);
					// accepted successful
					set_nonblocking(connfd);
					struct echo_event* echo_evt = calloc(1, sizeof(struct echo_event));
					echo_evt->fd = connfd;
					Epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, EPOLLIN, echo_evt);
				} else {
					if(evt_list[i].events & EPOLLHUP || evt_list[i].events & EPOLLERR) {
						struct echo_event* echo_evt = (struct echo_event*) evt_list[i].data.ptr;
						perror("epoll main loop (not listenfd), EPOLLHUP or EPOLLERR");
						close(echo_evt->fd);
						free(echo_evt);
					} else if(EPOLLIN == evt_list[i].events) { // Delete read evt
						struct echo_event* echo_evt = (struct echo_event*) evt_list[i].data.ptr;
						echo_evt->event = EPOLLIN;
						Epoll_ctl(epollfd, EPOLL_CTL_DEL, echo_evt->fd, 0, 0);
						handle(echo_evt);
					} else if(EPOLLOUT == evt_list[i].events) { // Delete write evt
						struct echo_event* echo_evt = (struct echo_event*) evt_list[i].data.ptr;
						echo_evt->event = EPOLLOUT; 
						Epoll_ctl(epollfd, EPOLL_CTL_DEL, echo_evt->fd, 0, 0);
						handle(echo_evt);
					}
				}
			} 
		}
		
		free(evt_list);
		exit(0);
		
	} else {
		fputs("Server now running in UDP mode.\n", stdout);
		dg_echo(listenfd, (SA *) &cliaddr, sizeof(cliaddr));
	}
}
