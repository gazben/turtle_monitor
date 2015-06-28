#ifndef EventInterfaceHandler_h__
#define EventInterfaceHandler_h__

/*
This file will contain the handling of Events, occurred in the system we want to observe.
If an event happens in the system, it will trigger a function here.
*/

/* GLOBAL INCLUDES */
#include <fstream>
#include <deque>
#include <string>
/* LOCAL INCLUDES */
#include "EventHandler.h"
/* INCLUDES END */

class EventInterfaceHandler{
private:
  static EventInterfaceHandler* instance;
  SR_regtype readNextLine();

protected:
  std::ifstream eventFile;
  std::deque<SR_regtype> eventQueue;

public:
  static EventInterfaceHandler* getinstance();

  ~EventInterfaceHandler();
  SR_regtype getNextEvent();

  void readEventsFromFile(std::string filename);
  void insertEvent(SR_regtype params);
};

#endif // EventInterfaceHandler_h__
