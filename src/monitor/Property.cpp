#include "Property.h"

//Static field init
unsigned int Property::currentMaxID = 0;
unsigned int Property::level = 0;
const unsigned int Property::maxDepth = 3;

trilean Property::isEventFired(SR_regtype eventCode)
{
  return (stateRegisterPtr->stateRegister & eventCode) ? FALSE : TRUE;
}

trilean Property::Evaluate()
{
  static Property* currentNode = this;
  trilean result = UNKNOWN;

  bool isChanged = 0;

  EventInterfaceHandler::getinstance()->getNextEvent();
  isChanged = false;

  for (int i = 0; i < currentNode->outputStates.size(); i++)
  {
    trilean tempOutputResult = currentNode->evalFunctions[i](currentNode);
    if (tempOutputResult != currentNode->outputStates[i])
    {
      isChanged = true;
      currentNode->outputStates[i] = tempOutputResult;
      break;
    }
  }

  //Output of the descendant node changed. We can go up in the stack.
  if (isChanged){
    //Free the current node.
    if (currentNode->rootNode != nullptr){
      level--;
      currentNode = currentNode->rootNode;

      if (currentNode->inputStates.size() != currentNode->childrenNode->outputStates.size()){
        ROS_ERROR_STREAM("Invalid eval function size!");
      }

      //give the output to the upper node
      //COPY right now, optimise later!
      for (int i = 0; i < currentNode->inputStates.size(); i++){
        currentNode->inputStates[i] = currentNode->childrenNode->outputStates[i];
      }

      delete currentNode->childrenNode;
    }
    else{
      //GOAL REACHED
      result = currentNode->outputStates[0];
      currentNode->freeChildrenNode();
    }
  }
  //No change happened we go deeper
  else{
    level++;
    currentNode->constructChildrenBlock();
    currentNode = currentNode->childrenNode;
  }

  //Print the current block out
  std::string tempOut;
  for (trilean& entry : outputStates) {
    tempOut += (entry == OutputState::FALSE)? "F" : (entry == OutputState::TRUE)? "T" : "U";
    tempOut += " ";
  }
  ROS_INFO_STREAM("Out: " + tempOut);
  ROS_INFO_STREAM( "ID: " + std::to_string(ID) + " level: " + std::to_string(level) );
  std::string tempIn;
  for (trilean& entry : inputStates) {
    tempIn += (entry == OutputState::FALSE)? "F" : (entry == OutputState::TRUE)? "T" : "U";
    tempIn += " ";
  }
  ROS_INFO_STREAM("In: " + tempIn);
  ROS_INFO_STREAM("Result: " + trilean::tostring(result) );

  return result;
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

Property* Property::constructChildrenBlock()
{
  Property* childrenBlockTemp = new Property();
  childrenBlockTemp->rootNode = this;
  this->childrenNode = constructChildrenNodeFunc(childrenBlockTemp);

  return childrenBlockTemp;
}

Property::~Property()
{
  delete childrenNode;

  if (rootNode != nullptr)
    rootNode->childrenNode = nullptr;
}

Property::Property()
    :childrenNode(nullptr),
    rootNode(nullptr),
    stateRegisterPtr(nullptr),
    constructChildrenNodeFunc(nullptr)
{
  stateRegisterPtr = StateRegisterState::getStatePointer();
  ID = currentMaxID;
  currentMaxID++;
  //if we reach the button, we have to initialize the inputs to false
  if(level == maxDepth){
    for (trilean& entry : inputStates) {
       entry = OutputState::FALSE;
    }
  }
}


trilean EVAL_s0(Property* _prop)
{
  return
      AND_3(
          NAND_3(
              _prop->isEventFired(EVENT_UP),
              AND_3(
                  NOT_3(_prop->isEventFired(EVENT_DOWN)),
                  NAND_3(
                      _prop->isEventFired(EVENT_RIGHT),
                      _prop->InputStates()[0])
              )
          ),
          NAND_3(TRUE, _prop->InputStates()[1])
      );
}

trilean EVAL_s1a(Property* _prop)
{
  return
      NAND_3(
          NOT_3(_prop->isEventFired(EVENT_DOWN)),
          NAND_3(_prop->isEventFired(EVENT_RIGHT), _prop->InputStates()[1])
      );
}

Property* construct_START(Property* _rootNode)
{
  _rootNode->evalFunctions.push_back(EVAL_s0);
  _rootNode->constructChildrenNodeFunc = constructS1;
  _rootNode->outputStates.resize(1);
  _rootNode->inputStates.resize(2);
  return _rootNode;
}

Property* constructS1(Property* _rootNode)
{
  _rootNode->evalFunctions.push_back(EVAL_s1a);
  _rootNode->evalFunctions.push_back(EVAL_s0);
  _rootNode->constructChildrenNodeFunc = constructS1;
  _rootNode->outputStates.resize(2);
  _rootNode->inputStates.resize(2);
  return _rootNode;
}
