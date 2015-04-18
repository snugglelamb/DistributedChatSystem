#include "mycomparison.h"

bool mycomparison::operator() (const Req&lhs, const Req&rhs){

	    return (lhs.comparator<rhs.comparator);// type  0 supass type 1


}
