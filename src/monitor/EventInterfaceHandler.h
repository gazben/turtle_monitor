#ifndef EventInterfaceHandler_h__
#define EventInterfaceHandler_h__

/*
This file will contain the handling of Events, occurred in the system we want to observe.
If an event happens in the system, it will trigger a function here.
*/

/* GLOBAL INCLUDES */
#include <deque>
#include <string>
/* LOCAL INCLUDES */
#include "EventHandler.h"
/* INCLUDES END */

class EventInterfaceHandler{
private:
  static EventInterfaceHandler* instance;
protected:
  std::deque<SR_regtype> eventQueue;
public:
  static EventInterfaceHandler* getinstance();
  SR_regtype getNextEvent();
  void insertEvent(SR_regtype params);
};

#endif // EventInterfaceHandler_h__
