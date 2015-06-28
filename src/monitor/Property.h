#ifndef Property_h__
#define Property_h__

/* GLOBAL INCLUDES */
#include <functional>
#include <vector>
#include <iostream>
#include <ros/ros.h>
/* LOCAL INCLUDES */
#include "EventInterfaceHandler.h"
/* INCLUDES END */

/* FUNCTION TYPE DEFINITIONS */
using namespace std;

class Property{
protected:
  StateRegisterState* stateRegisterPtr;
public:
  Property();
  ~Property();

  vector<trilean> inputStates;
  vector<trilean> outputStates;

  Property* rootNode;
  Property* childrenNode;
  std::function < class Property*(class Property*) > constructChildrenNodeFunc;
  std::vector <std::function < trilean(class Property*) >> evalFunctions;
  Property* constructChildrenBlock();

  void freeChildrenNode();

  std::vector<trilean> InputStates() const;

  //Getter-setters
  Property* RootNode() const;
  void RootNode(class Property* val);
  Property* ChildrenNode() const;
  void ChildrenNode(Property* val);

  trilean isEventFired(SR_regtype eventCode);
  static trilean Evaluate(Property* root);
};

trilean EVAL_s0(Property*);
trilean EVAL_s1a(Property*);
Property* construct_START(Property*);
Property* constructS1(Property*);
#endif // Property_h__
