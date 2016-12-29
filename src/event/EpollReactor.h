#ifndef _EPOLLREACTOR_H_
#define _EPOLLREACTOR_H_

#include <iostream>
#include <map>

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
	std::map<int, Event> events;
	
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
		if (0 != this->epollFd) {
			close(this->epollFd);
			this->epollFd = 0;

			for (std::map<int, Event>::iterator iter = this->events.begin();
				 iter != this->events.end(); iter++) {
				 if (iter->first > 0) {
				 	close(iter->first);
				 }
			}
			
		}
	}
	
	virtual int addEvent(const Event &event);
	virtual int delEvent(const Event &event);
	virtual int delEvent(int fd);
	virtual int dispatcher();

	int getEpollFd() {
		return this->epollFd;
	}

	int getEpollSize() {
		return this->epollSize;
	}

	int getEventsSize() {
		return this->events.size();
	}

private:
	void init() {
		this->epollFd = 0;
		this->epollSize = 0;
	}
};

#endif
