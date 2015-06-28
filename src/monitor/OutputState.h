#ifndef OutputState_h__
#define OutputState_h__

#include <string>

typedef enum OutputState{
  TRUE, FALSE, UNKNOWN
}OutputState;

class trilean {
protected:
  OutputState state;
public:

  trilean(OutputState _state = UNKNOWN);

  static bool isUnknown(OutputState a);
  static std::string tostring(const trilean & obj);
  explicit operator OutputState() const;
  trilean operator&(const trilean & rhs);
  trilean operator&&(const trilean & rhs);
  trilean & operator=(const trilean & rhs);
  bool operator==(const trilean & rhs);
  trilean operator|(const trilean & rhs);
  trilean operator^(const trilean & rhs);
  trilean operator!();
  bool operator!=(const trilean & rhs);
};

std::ostream& operator<<(std::ostream& os, const trilean & obj);

/* 3 state logic functions */
trilean AND_3(trilean a, trilean b);
trilean NAND_3(trilean a, trilean b);
trilean OR_3(trilean a, trilean b);
trilean XOR_3(trilean a, trilean b);
trilean NOT_3(trilean a);

#endif // OutputState_h__