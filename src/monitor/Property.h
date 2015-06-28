#ifndef Property_h__
#define Property_h__

/* GLOBAL INCLUDES */
#include <functional>
#include <vector>
#include <iostream>
/* LOCAL INCLUDES */
#include "EventInterfaceHandler.h"
/* INCLUDES END */

/* FUNCTION TYPE DEFINITIONS */
using namespace std;

class Property{
protected:

  StateRegisterState* stateRegisterPtr;

public:
  vector<trilean> inputStates;
  vector<trilean> outputStates;

  Property* rootNode;
  Property* childrenNode;
  std::function < class Property*(class Property*) > constructChildrenNodeFunc;

  std::vector <std::function < trilean(class Property*) >> evalFunctions;

  Property();
  ~Property();

  Property* constructChildrenBlock();

  std::vector<std::function < trilean(class Property*) >> EvalFunctions() const;

  void EvalFunctions(std::vector<std::function < trilean(class Property*) >> func);

  void freeChildrenNode();

  std::vector<trilean> InputStates() const;

  //Getter-setters
  Property* RootNode() const;
  void RootNode(class Property* val);

  Property* ChildrenNode() const;
  void ChildrenNode(Property* val);

  trilean isEventFired(SR_regtype eventCode);

  static trilean Evaluate(Property* root);
  static trilean EvaluateROS(Property* root);
};
//--DECLARATIONS--
trilean EVAL_s0(Property* _prop);
trilean EVAL_s1a(Property* _prop);
Property* construct_START(Property* _rootNode);
Property* constructS1(Property* _rootNode);


//--EVALFUNCTIONS--


//--CONSTRUCTFUNCTIONS--

trilean EVAL_s0(Property* _prop)
{
  return
    AND_3(
    NAND_3(
    _prop->isEventFired(EVENT_A),
    AND_3(
    NOT_3(_prop->isEventFired(EVENT_B)),
    NAND_3(
    _prop->isEventFired(EVENT_C),
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
    NOT_3(_prop->isEventFired(EVENT_B)),
    NAND_3(_prop->isEventFired(EVENT_C), _prop->InputStates()[1])
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
#endif // Property_h__
