#ifndef _EVENT_H_
#define _EVENT_H_

/* event callback function */
typedef void *(*EventCallback)(int fd, void *arg);

/**
 * event struct.
 */
struct Event
{
	short event;			 /* the event you want to monitor */
	int   fd;				 /* the fd want to monitor */
	EventCallback callback; /* the callback function */
	void *arg;			     /* the parameter of callback function */
};

#endif
