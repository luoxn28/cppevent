#ifndef _IREACTOR_H_
#define _IREACTOR_H_

/* public include */

/* private include */
#include "Event.h"

/**
 * the interface of reactor.
 */
class IReactor
{
public:
	virtual int addEvent(const Event &event) = 0;
	virtual int delEvent(const Event &event) = 0;
	virtual int dispatcher() = 0;

	virtual ~IReactor() { }
};

#endif
