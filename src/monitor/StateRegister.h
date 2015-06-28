#ifndef StateRegister_h__
#define StateRegister_h__

/* GLOBAL INCLUDES */
#include <stdlib.h>

/* LOCAL INCLUDES */
#include "Events.h"
/* INCLUDES END */

class StateRegister{
private:
  static StateRegister * rootState;
  SR_regtype stateRegisterValue;
  StateRegister * rightNode;
  StateRegister * leftNode;

  static StateRegister * insertState(SR_regtype stateReg = stateRegister, StateRegister * root = rootState);

public:
  
  StateRegister();
  ~StateRegister();

  //Global stateRegister
  static SR_regtype stateRegister;
  static void clearEvents();

  static void freeState(StateRegister *root = rootState);

  static StateRegister * getStatePointer(SR_regtype StateRegisterCopy = stateRegister);

  friend class Property;
};

#endif // StateRegister_h__
