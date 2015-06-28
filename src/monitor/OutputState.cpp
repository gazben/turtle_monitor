#include "OutputState.h"

bool trilean::operator!=(const trilean& rhs)
{
  return this->state != rhs.state;
}

trilean trilean::operator!()
{
  return trilean(
    (this->state == UNKNOWN) ? UNKNOWN : (this->state == FALSE) ? TRUE : FALSE);
}

trilean trilean::operator^(const trilean& rhs)
{
  return trilean((
    this->state == FALSE | rhs.state == FALSE) ? TRUE : (this->state == UNKNOWN | rhs.state == UNKNOWN) ? UNKNOWN : FALSE);
}

trilean trilean::operator|(const trilean& rhs)
{
  return trilean((
    this->state == TRUE | rhs.state == TRUE) ? TRUE : (this->state == UNKNOWN | rhs.state == UNKNOWN) ? UNKNOWN : FALSE);
}

bool trilean::operator==(const trilean& rhs)
{
  return this->state == rhs.state;
}

trilean& trilean::operator=(const trilean& rhs)
{
  this->state = rhs.state;
  return *this;
}

trilean trilean::operator&&(const trilean& rhs)
{
  return *this & rhs;
}

trilean trilean::operator&(const trilean& rhs)
{
  return trilean((
    this->state == FALSE | rhs.state == FALSE) ? FALSE : (this->state == UNKNOWN | rhs.state == UNKNOWN) ? UNKNOWN : TRUE);
}

trilean::operator OutputState() const
{
  return this->state;
}

std::string trilean::tostring(const trilean& obj)
{
  if (obj.state == TRUE)
    return "TRUE";
  else if (obj.state == FALSE)
    return "FALSE";
  else
    return "UNKNOWN";
}

bool trilean::isUnknown(OutputState a)
{
  return a == UNKNOWN;
}

trilean::trilean(OutputState _state /*= UNKNOWN*/) :state(_state)
{
}

trilean AND_3(trilean a, trilean b)
{
  return a & b;
}

trilean NAND_3(trilean a, trilean b)
{
  return !(a&b);
}

trilean OR_3(trilean a, trilean b)
{
  return a | b;
}

trilean XOR_3(trilean a, trilean b)
{
  return a^b;
}

trilean NOT_3(trilean a)
{
  return !a;
}

std::ostream& operator<<(std::ostream& os, const trilean& obj)
{
  os << trilean::tostring(obj);
  return os;
}