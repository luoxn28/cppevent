#ifndef _EPOLLREACTOR_H_
#define _EPOLLREACTOR_H_

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>

#include "Event.h"
#include "IReactor.h"

class EpollReactor : public IReactor
{
private:
	int epollFd;
	int epollSize;
	
public:
 	EpollReactor() : EpollReactor(32) { }
	EpollReactor(int epollSize) {
		init();
		this->epollFd = epoll_create(epollSize);
		if (this->epollFd < 0) {
			std::cerr << "epoll_create error" << std::endl;
			exit(-1);
		}

		this->epollSize = epollSize;
	}
	virtual ~EpollReactor() {
		if (0 != epollFd) {
			close(epollFd);
			epollFd = 0;
		}
	}
	
	virtual int addEvent(const Event &event);
	virtual int delEvent(const Event &event);
	virtual int dispatcher();

	int getEpollFd() {
		return this->epollFd;
	}

private:
	void init() {
		epollFd = 0;
		epollSize = 0;
	}
};

#endif
