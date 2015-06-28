#include "Property.h"

trilean Property::isEventFired(SR_regtype eventCode)
{
  return (stateRegisterPtr->stateRegister & eventCode) ? FALSE : TRUE;
}

trilean Property::Evaluate(Property* root)
{
  Property* currentNode = root;
  trilean result = UNKNOWN;

  bool isChanged = 0;

  //while (result == UNKNOWN){
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

  //ROS_INFO_STREAM( (trilean::tostring(result)).c_str() );
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
                      _prop->isEventFired(EVENT_LEFT),
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
          NAND_3(_prop->isEventFired(EVENT_LEFT), _prop->InputStates()[1])
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
