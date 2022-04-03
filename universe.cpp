#include "universe.h"
#include <math.h>

universe& universe::operator*=(double val) {
	volume *= val;
	scale = pow(volume, 1 / 3);
	return *this;
}
