#include "Property.h"

//Static field init
unsigned int Property::currentMaxID = 0;
unsigned int Property::level = 0;
const unsigned int Property::maxDepth = 2; //will be generated
Property* Property::currentBlock = nullptr;

trilean Property::isEventFired(SR_regtype eventCode)
{
  return (stateRegisterPtr->stateRegister & eventCode) ? FALSE : TRUE;
}

trilean Property::Evaluate()
{
  if(currentBlock == nullptr)
    currentBlock = this;

  if(currentBlock->stateRegisterPtr == nullptr){
    currentBlock->stateRegisterPtr = StateRegister::getStatePointer();
    currentBlock->stateRegisterPtr = StateRegister::getStatePointer();
  }


  //Print the current block out
  ROS_INFO_STREAM("BEFORE");
  printBlock(currentBlock);

  trilean result = UNKNOWN;
  bool isChanged = false;
  for (int i = 0; i < currentBlock->outputStates.size(); i++)
  {
    trilean tempOutputResult = currentBlock->evalFunctions[i](currentBlock);
    if (tempOutputResult != currentBlock->outputStates[i])
    {
      isChanged = true;
      currentBlock->outputStates[i] = tempOutputResult;
    }
  }

  //Output of the descendant node changed. We can go up in the stack.
  if (isChanged){
    ROS_INFO_STREAM("CHANGE");
    //Free the current node.
    if (currentBlock->rootNode != nullptr){
      ROS_INFO_STREAM("GOING UP");
      ROS_INFO_STREAM("--");
      ROS_INFO_STREAM("AFTER");
      printBlock(currentBlock);

      currentBlock = currentBlock->rootNode;
      if (currentBlock->inputStates.size() != currentBlock->childrenNode->outputStates.size()){
        ROS_ERROR_STREAM("Invalid eval function size!");
      }

      //give the output to the upper node
      //COPY right now, optimise later!
      for (int i = 0; i < currentBlock->inputStates.size(); i++){
        currentBlock->inputStates[i] = currentBlock->childrenNode->outputStates[i];
      }
      currentBlock->freeChildrenNode();
/*
      if(level == 2){
        level--;
        printBlock(currentBlock);
        currentBlock->Evaluate();
        printBlock(currentBlock);
      }
*/
      //currentBlock->Evaluate();

      level--;
    }
    else{
      //GOAL REACHED
      ROS_INFO_STREAM("GOAL REACHED");
      result = currentBlock->outputStates[0];
      currentBlock->freeChildrenNode();
    }
  }
  else{
    //No change happened we go deeper
    level++;
    ROS_INFO_STREAM("GOING DEEPER");
    ROS_INFO_STREAM("--");
    ROS_INFO_STREAM("AFTER");
    printBlock(currentBlock);

    currentBlock->constructChildrenBlock();
    currentBlock = currentBlock->childrenNode;
  }
  ROS_INFO_STREAM("Result: " + trilean::tostring(result) );

  return result;
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
  childrenNode = constructChildrenNodeFunc(childrenBlockTemp);
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
  ID = currentMaxID;
  currentMaxID++;
  ROS_INFO_STREAM("BLOCK CREATED | ID " + std::to_string(ID) );
  //if we reach the button, we have to initialize the inputs to false
  if(level == maxDepth){
    inputStates.resize(2);
    for (auto& entry : inputStates) {
      entry = trilean(OutputState::FALSE);
    }
  }
}


trilean EVAL_s0(Property* _prop)
{
  return
      NAND_3(
          NAND_3(
              _prop->isEventFired(EVENT_UP),
              AND_3(
                  NOT_3(_prop->isEventFired(EVENT_RIGHT)),
                  NAND_3(
                      _prop->isEventFired(EVENT_DOWN),
                      _prop->inputStates[0])
              )
          ),
          NAND_3(TRUE, _prop->inputStates[1])
      );
}

trilean EVAL_s1a(Property* _prop)
{
  return
      NAND_3(
          NOT_3(_prop->isEventFired(EVENT_RIGHT)),
          NAND_3(_prop->isEventFired(EVENT_DOWN), _prop->inputStates[1])
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
  _rootNode->evalFunctions.resize(2);
  _rootNode->evalFunctions[0] = EVAL_s1a;
  _rootNode->evalFunctions[1] = EVAL_s0;
  _rootNode->constructChildrenNodeFunc = constructS1;
  _rootNode->outputStates.resize(2);
  _rootNode->inputStates.resize(2);
  return _rootNode;
}

void Property::printBlock(Property *block) {
  //Print the current block out
  std::string tempOut;
  for (auto& entry : block->outputStates) {
    tempOut += (entry == OutputState::FALSE)? "F" : (entry == OutputState::TRUE)? "T" : "U";
    tempOut += " ";
  }
  std::string tempIn;
  ROS_INFO_STREAM("Out: " + tempOut);
  ROS_INFO_STREAM( "ID: " + std::to_string(block->ID) + " level: " + std::to_string(level)
                   + " Statereg: " + std::to_string(block->stateRegisterPtr->stateRegisterValue));
  for (trilean& entry : block->inputStates) {
    tempIn += (entry == OutputState::FALSE)? "F" : (entry == OutputState::TRUE)? "T" : "U";
    tempIn += " ";
  }
  ROS_INFO_STREAM("In: " + tempIn);
}
