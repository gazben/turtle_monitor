#ifndef EventHandler_h__
#define EventHandler_h__

/* GLOBAL INCLUDES */

/* LOCAL INCLUDES */
#include "StateRegister.h"
#include "OutputState.h"

/* INCLUDES END */

/* TEST FUNCTIONS */
class Eventhandler{
public:
	static void addEvent(SR_regtype event_code){
		StateRegister::stateRegister = StateRegister::stateRegister | event_code;
	}

	static void removeEvent(SR_regtype event_code){
		StateRegister::stateRegister = StateRegister::stateRegister ^ event_code;
	}

	static void clearEvents(){
		StateRegister::stateRegister = 0;
	}
};

#endif // EventHandler_h__
