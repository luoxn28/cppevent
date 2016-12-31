#include "EpollReactor.h"


int EpollReactor::addEvent(Event *pevent)
{
	int errCode = 0;

	if (pevent->flag == EVENT_FLAG_TIMER) {
		if (pevent->fd == 0) {
			pevent->fd = timerfd_create(CLOCK_REALTIME, 0);
			if (pevent->fd < 0) {
				std::cerr << "timerfd_create error" << std::endl;
				return (errCode = 1);
			}

			timerfd_settime(pevent->fd, 0, &(pevent->data.timer.value), NULL);
		}
	}

	epoll_event epollev;
	epollev.data.fd = pevent->fd;
	epollev.events  = pevent->events;
	errCode = epoll_ctl(this->epollFd, EPOLL_CTL_ADD, pevent->fd, &epollev);
	if (errCode < 0) {
		std::cerr << "epoll_ctl error" << std::endl;
		return errCode;
	}

	this->events[pevent->fd] = *pevent;
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
		Event *pevent = &(this->events[epollevs[i].data.fd]);

		if (pevent->callback) {
			switch (pevent->flag) {
				case EVENT_FLAG_EVENT:
					pevent->callback(pevent->fd, pevent->events, pevent->arg);
					break;

				case EVENT_FLAG_TIMER:
					if (pevent->data.timer.num == -1) {
						pevent->callback(pevent->fd, pevent->events, pevent->arg);
					}
					else if (pevent->data.timer.num > 0) {
						pevent->callback(pevent->fd, pevent->events, pevent->arg);
						pevent->data.timer.num--;
					}
					else if (pevent->data.timer.num == 0) {
						delEvent(pevent->fd);
					}
					break;
				
				default:
					std::cerr << "dispatcher default" << std::endl;
					break;
			}
		}
	}

	return 0;
}

