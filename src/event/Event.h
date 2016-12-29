#ifndef _EVENT_H_
#define _EVENT_H_

#include <string.h>

/* event callback function */
typedef void *(*EventCallback)(int fd, unsigned int events, void *arg);

/**
 * event struct.
 */
struct Event
{
	unsigned int events;			 /* the event you want to monitor */
	int   fd;				 /* the fd want to monitor */
	EventCallback callback; /* the callback function */
	void *arg;			     /* the parameter of callback function */

	Event() {
		memset(this, 0, sizeof(this));
	}
};

#endif
