#ifndef MYCOM
#define MYCOM
#include "Req.h"

class mycomparison
{
public:
  bool operator() (const Req& lhs, const Req&rhs);
};
#endif


