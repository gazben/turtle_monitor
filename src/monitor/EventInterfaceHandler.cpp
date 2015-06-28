#include "EventInterfaceHandler.h"

EventInterfaceHandler* EventInterfaceHandler::instance = nullptr;

void EventInterfaceHandler::readEventsFromFile(std::string filename)
{
  EventInterfaceHandler::getinstance()->eventFile.open(filename);
}

SR_regtype EventInterfaceHandler::getNextEvent()
{
  EventInterfaceHandler* instance = EventInterfaceHandler::getinstance();

  if (instance->eventFile.is_open())
    instance->eventQueue.push_back(readNextLine());

  SR_regtype front = instance->eventQueue.front();
  StateRegisterState::stateRegister = front;
  instance->eventQueue.pop_front();
  return front;
}

EventInterfaceHandler::~EventInterfaceHandler()
{
  if (EventInterfaceHandler::getinstance()->eventFile.is_open())
    EventInterfaceHandler::getinstance()->eventFile.close();
}

SR_regtype EventInterfaceHandler::readNextLine()
{
  std::string line;
  SR_regtype tempStateReg;
  std::getline(EventInterfaceHandler::getinstance()->eventFile, line);

  for (char c : line){
    switch (c)
    {
    case 'a':
      tempStateReg |= EVENT_A;
      break;
    case 'b':
      tempStateReg |= EVENT_B;
      break;
    case 'c':
      tempStateReg |= EVENT_C;
      break;
    case 'd':
      tempStateReg |= EVENT_D;
      break;
    case '\n':
      break;
    default:
      printf("Invalid state found in testfile!\n");
      break;
    }
    return tempStateReg;
  }
}

void EventInterfaceHandler::insertEvent(SR_regtype params) {
  instance->eventQueue.push_back(params);
}

EventInterfaceHandler* EventInterfaceHandler::getinstance()
{
  if (instance == nullptr)
    instance = new EventInterfaceHandler();

  return instance;
}