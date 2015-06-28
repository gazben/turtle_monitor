#include "StateRegister.h"

/*Static init*/
SR_regtype StateRegisterState::stateRegister = 0;
StateRegisterState* StateRegisterState::rootState = nullptr;

StateRegisterState* StateRegisterState::getStatePointer(SR_regtype StateRegisterCopy /*= stateRegister*/)
{
  if (rootState == nullptr){
    rootState = insertState();
    return rootState;
  }

  StateRegisterState* temp = rootState;

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

void StateRegisterState::freeState(StateRegisterState *root /*= rootState*/)
{
  if (rootState == nullptr)
    return;

  delete root->leftNode;
  delete root->rightNode;
  delete root;
}

StateRegisterState::~StateRegisterState()
{
}

StateRegisterState::StateRegisterState()
{
  leftNode = nullptr;
  rightNode = nullptr;
  stateRegisterValue = stateRegister;
}

StateRegisterState* StateRegisterState::insertState(SR_regtype stateReg /*= stateRegister*/, StateRegisterState* root /*= rootState*/)
{
  if (root == nullptr) {
    StateRegisterState *temp = new StateRegisterState();
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