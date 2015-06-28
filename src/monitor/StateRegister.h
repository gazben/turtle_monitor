#ifndef StateRegister_h__
#define StateRegister_h__

/* GLOBAL INCLUDES */
#include <stdlib.h>

/* LOCAL INCLUDES */
#include "Events.h"
/* INCLUDES END */

class StateRegisterState{
private:

  static StateRegisterState* rootState;

  SR_regtype stateRegisterValue;

  StateRegisterState* rightNode;
  StateRegisterState* leftNode;

  static StateRegisterState* insertState(SR_regtype stateReg = stateRegister, StateRegisterState* root = rootState);

public:
  
  StateRegisterState();
  ~StateRegisterState();

  //Global stateRegister
  static SR_regtype stateRegister;

  static void freeState(StateRegisterState *root = rootState);

  static StateRegisterState* getStatePointer(SR_regtype StateRegisterCopy = stateRegister);

  friend class Eventhandler;
};

#endif // StateRegister_h__
