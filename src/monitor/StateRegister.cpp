#include "StateRegister.h"

/*Static init*/
SR_regtype StateRegister::stateRegister = 0;
StateRegister *StateRegister::rootState = nullptr;

StateRegister *StateRegister::getStatePointer(SR_regtype StateRegisterCopy /*= stateRegister*/)
{
  if (rootState == nullptr){
    rootState = insertState();
    return rootState;
  }

  StateRegister * temp = rootState;

  while (temp != nullptr && temp->stateRegisterValue != StateRegisterCopy) {
    if (StateRegisterCopy < temp->stateRegisterValue){
      if (temp->leftNode == nullptr){
        temp->leftNode = insertState(StateRegisterCopy);
        return temp->leftNode;
      }
      temp = temp->leftNode;
    }
    else{
      if (temp->rightNode == nullptr){
        temp->rightNode = insertState(StateRegisterCopy);
        return temp->rightNode;
      }
      temp = temp->rightNode;
    }
  }
  return temp;
}

void StateRegister::freeState(StateRegister *root /*= rootState*/)
{
  if (rootState == nullptr)
    return;

  delete root->leftNode;
  delete root->rightNode;
  delete root;
}

StateRegister::~StateRegister()
{
}

StateRegister::StateRegister()
{
  leftNode = nullptr;
  rightNode = nullptr;
  stateRegisterValue = stateRegister;
}

StateRegister *StateRegister::insertState(SR_regtype stateReg /*= stateRegister*/, StateRegister * root /*= rootState*/)
{
  if (root == nullptr) {
    StateRegister *temp = new StateRegister();
    temp->leftNode = temp->rightNode = nullptr;
    temp->stateRegisterValue = stateReg;
    return temp;
  }
  else if (stateReg < root->stateRegisterValue){
    root->leftNode = insertState(stateReg, root->leftNode);
    return root->leftNode;
  }
  else if (stateReg > root->stateRegisterValue){
    root->rightNode = insertState(stateReg, root->rightNode);
    return root->rightNode;
  }
  else
    return root;
}

void StateRegister::clearEvents() {
  stateRegister = 0;
}
