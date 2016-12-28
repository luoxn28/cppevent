#include <iostream>

#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <sys/epoll.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <fcntl.h>  
#include <string.h> 


#include "../event/IReactor.h"
#include "../event/EpollReactor.h"


using namespace std;

EpollReactor gReactor;
bool gRunning = true;

void *stdinCallback(int fd, short events, void *arg)
{
	string input;

	cin >> input;
	if (input == "quit") {
		gRunning = false;
	}
	else {
		cout << input << endl;
	}
}

void *connCallback(int fd, short events, void *arg)
{
	if (events & EPOLLIN) {
		char buff[512];
		
		int len = recv(fd, buff, sizeof(buff) - 1, 0);
		if (len > 0) {
			buff[len] = '\0';
			cout << "recv: " << buff << endl;
		}
		else if (len == 0) {
			cout << fd << ": client close" << endl;
			gReactor.delEvent(fd);
		}
	}
}

void *listenCallback(int fd, short events, void *arg)
{
	if (events & EPOLLIN) {
		int conn = accept(fd, NULL, NULL);
		if (conn > 0) {
			Event event;

			event.fd = conn;
			event.events = EPOLLIN;
			event.callback = connCallback;

			gReactor.addEvent(event);
		}
	}
	
	return NULL;
}

int main(int argc, char **argv)
{
	cout << "hello cppevent" << endl;

	Event event;
	int listenfd, connfd;
	struct sockaddr_in servAddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	memset(&servAddr, sizeof(servAddr), 0);
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(8080);
	servAddr.sin_addr.s_addr = INADDR_ANY;
	bind(listenfd, (struct sockaddr *)&servAddr, sizeof(servAddr));

	listen(listenfd, 5);

	memset(&event, sizeof(event), 0);
	event.fd = listenfd;
	event.callback = listenCallback;
	event.events = EPOLLIN;
	gReactor.addEvent(event);

	memset(&event, sizeof(event), 0);
	event.fd = STDIN_FILENO;
	event.callback = stdinCallback;
	event.events = EPOLLIN;
	gReactor.addEvent(event);

	while (gRunning) {
		gReactor.dispatcher();
	}

	return 0;
}

