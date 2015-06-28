#include "Property.h"

trilean Property::isEventFired(SR_regtype eventCode)
{
  return (stateRegisterPtr->stateRegister & eventCode) ? FALSE : TRUE;
}


trilean Property::Evaluate(Property* root)
{
  Property* currentNode = root;
  trilean result = UNKNOWN;

  EventInterfaceHandler::getinstance()->getNextEvent();

  bool isChanged = 0;

  while (result == UNKNOWN){
    isChanged = false;

    for (int i = 0; i < currentNode->outputStates.size(); i++){
      trilean tempOutputResult = currentNode->evalFunctions[i](currentNode);
      if (tempOutputResult != currentNode->outputStates[i]){
        isChanged = true;
        currentNode->outputStates[i] = tempOutputResult;
        break;
      }
    }

    //Output of the descendant node changed. We can go up in the stack.
    if (isChanged){
      //Free the current node.
      if (currentNode->rootNode != nullptr){
        currentNode = currentNode->rootNode;

        //give the output to the input
        if (currentNode->inputStates.size() != currentNode->childrenNode->outputStates.size()){
          throw std::runtime_error("Invalid eval function size!");
        }

        //COPY right now, optimise later!
        for (int i = 0; i < currentNode->inputStates.size(); i++){
          currentNode->inputStates[i] = currentNode->childrenNode->outputStates[i];
        }

        delete currentNode->childrenNode;
      }
      else{
        //GOAL REACHED
        result = currentNode->outputStates[0];
        root->freeChildrenNode();
      }
    }
    //No change happened we go deeper
    else{
      currentNode->constructChildrenBlock();
      currentNode = currentNode->childrenNode;
    }
  }

  return result;
}

trilean Property::EvaluateROS(Property* root)
{
  Property* currentNode = root;
  trilean result = UNKNOWN;

  EventInterfaceHandler::getinstance()->getNextEvent();

  bool isChanged = 0;

  //while (result == UNKNOWN){
    isChanged = false;

    for (int i = 0; i < currentNode->outputStates.size(); i++){
      trilean tempOutputResult = currentNode->evalFunctions[i](currentNode);
      if (tempOutputResult != currentNode->outputStates[i]){
        isChanged = true;
        currentNode->outputStates[i] = tempOutputResult;
        break;
      }
    }

    //Output of the descendant node changed. We can go up in the stack.
    if (isChanged){
      //Free the current node.
      if (currentNode->rootNode != nullptr){
        currentNode = currentNode->rootNode;

        //give the output to the input
        if (currentNode->inputStates.size() != currentNode->childrenNode->outputStates.size()){
          throw std::runtime_error("Invalid eval function size!");
        }

        //COPY right now, optimise later!
        for (int i = 0; i < currentNode->inputStates.size(); i++){
          currentNode->inputStates[i] = currentNode->childrenNode->outputStates[i];
        }

        delete currentNode->childrenNode;
      }
      else{
        //GOAL REACHED
        result = currentNode->outputStates[0];
        root->freeChildrenNode();
      }
    }
    //No change happened we go deeper
    else{
      currentNode->constructChildrenBlock();
      currentNode = currentNode->childrenNode;
    }
  //}

  cout<<result<<"\n";
  return result;
}

void Property::ChildrenNode(Property* val)
{
  childrenNode = val;
}

Property* Property::ChildrenNode() const
{
  return childrenNode;
}

void Property::RootNode(class Property* val)
{
  rootNode = val;
}

Property* Property::RootNode() const
{
  return rootNode;
}

std::vector<trilean> Property::InputStates() const
{
  return inputStates;
}

void Property::freeChildrenNode()
{
  delete childrenNode;

  if (rootNode != nullptr)
    rootNode->childrenNode = nullptr;
}

void Property::EvalFunctions(std::vector<std::function < trilean(class Property*) >> func)
{
  if (func.size() != evalFunctions.size()){
    throw std::runtime_error("Invalid eval function size!");
  }
  evalFunctions = func;
}

Property* Property::constructChildrenBlock()
{
  Property* childrenBlockTemp = new Property();
  childrenBlockTemp->rootNode = this;
  this->childrenNode = constructChildrenNodeFunc(childrenBlockTemp);;

  return childrenBlockTemp;
}

Property::~Property()
{
  delete childrenNode;

  if (rootNode != nullptr)
    rootNode->childrenNode = nullptr;
}

Property::Property() :childrenNode(nullptr),
rootNode(nullptr),
stateRegisterPtr(nullptr),
constructChildrenNodeFunc(nullptr)
{
  stateRegisterPtr = StateRegisterState::getStatePointer();
}