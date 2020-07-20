#ifndef POLY_SLEEP_H
#define POLY_SLEEP_H

#include <poly/Core/Time.h>

namespace poly
{

void sleep(Time time);
void sleep(float seconds);
void sleep(double seconds);
void sleep(Int32 milliseconds);
void sleep(Uint32 milliseconds);

}

#endif