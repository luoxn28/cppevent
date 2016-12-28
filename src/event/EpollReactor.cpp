#include "EpollReactor.h"


int EpollReactor::addEvent(const Event &event)
{
	int errCode = 0;
	epoll_event epollev;

	epollev.data.fd = event.fd;
	epollev.events  = event.events;
	errCode = epoll_ctl(this->epollFd, EPOLL_CTL_ADD, event.fd, &epollev);
	if (errCode < 0) {
		std::cerr << "epoll_ctl error" << std::endl;
		return errCode;
	}

	this->events[event.fd] = event;
	return errCode;
}

int EpollReactor::delEvent(const Event &event)
{
	int errCode = 0;
	epoll_event epollev;

	epollev.data.fd = event.fd;
	epollev.events  = event.events;
	errCode = epoll_ctl(this->epollFd, EPOLL_CTL_DEL, event.fd, &epollev);
	if (errCode < 0) {
		std::cerr << "epoll_ctl error" << std::endl;
		return errCode;
	}

	this->events.erase(event.fd);
	return errCode;
}

int EpollReactor::delEvent(int fd)
{
	int errCode = 0;
	
	if (this->events.count(fd)) {
		errCode = epoll_ctl(this->epollFd, EPOLL_CTL_DEL, fd, NULL);
		this->events.erase(fd);
	}

	return errCode;
}

int EpollReactor::dispatcher()
{
	struct epoll_event epollevs[32];

	int nevents = epoll_wait(this->epollFd, epollevs, 32, -1);
	if (nevents < 0) {
		std::cerr << "epoll_wait error" << std::endl;
		return nevents;
	}

	for (int i = 0; i < nevents; i++) {
		Event event = this->events[epollevs[i].data.fd];

		if (event.callback) {
			event.callback(event.fd, event.events, event.arg);
		}
	}

	return 0;
}

