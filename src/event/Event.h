#ifndef _EVENT_H_
#define _EVENT_H_

#include <string.h>
#include <sys/timerfd.h>

/* event callback function */
typedef void *(*EventCallback)(int fd, unsigned int events, void *arg);

#define EVENT_FLAG_EVENT 0
#define EVENT_FLAG_TIMER 1

struct TimerInfo
{
	struct itimerspec value;
	int num;
};

/**
 * event struct.
 */
struct Event
{
	unsigned int events;	 /* the event you want to monitor */
	int   fd;				 /* the fd want to monitor */
	EventCallback callback; /* the callback function */
	void *arg;			     /* the parameter of callback function */
	int flag;				 /* flag of event */

	union data {
		TimerInfo timer;
	};

	Event() {
		memset(this, 0, sizeof(this));
	}
};

#endif
