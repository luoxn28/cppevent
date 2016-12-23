#ifndef _EVENT_H_
#define _EVENT_H_

/* event callback function */
typedef void *(*EventCallback)(int fd, void *arg);

/**
 * event struct.
 */
struct Event
{
	short event;
	int   fd;
	EventCallback callback;
	void *arg;
};

#endif
