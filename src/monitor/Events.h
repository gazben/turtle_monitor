#ifndef Events_h__
#define Events_h__

typedef unsigned long long int SR_regtype;
const SR_regtype EVENT_UP = 0x1;
const SR_regtype EVENT_DOWN = 0x2;
const SR_regtype EVENT_LEFT = 0x4;
const SR_regtype EVENT_RIGHT = 0x8;

#endif // Events_h__