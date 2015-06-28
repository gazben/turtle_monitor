#include "EventInterfaceHandler.h"

EventInterfaceHandler* EventInterfaceHandler::instance = nullptr;

SR_regtype EventInterfaceHandler::getNextEvent()
{
  EventInterfaceHandler* instance = EventInterfaceHandler::getinstance();

  SR_regtype front = instance->eventQueue.front();
  StateRegisterState::stateRegister = front;
  instance->eventQueue.pop_front();
  return front;
}

void EventInterfaceHandler::insertEvent(SR_regtype params)
{
  instance->eventQueue.push_back(params);
}

EventInterfaceHandler* EventInterfaceHandler::getinstance()
{
  if (instance == nullptr)
    instance = new EventInterfaceHandler();

  return instance;
}
