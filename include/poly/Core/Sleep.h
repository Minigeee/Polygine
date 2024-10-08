#ifndef POLY_SLEEP_H
#define POLY_SLEEP_H

#include <poly/Core/Time.h>

namespace poly
{

///////////////////////////////////////////////////////////
/// \brief Make current sleep for the time specified
///
/// \param time A Time to sleep for
///
///////////////////////////////////////////////////////////
void sleep(Time time);

///////////////////////////////////////////////////////////
/// \brief Make current sleep for the time specified
///
/// \param seconds The time in seconds to sleep for
///
///////////////////////////////////////////////////////////
void sleep(float seconds);

///////////////////////////////////////////////////////////
/// \brief Make current sleep for the time specified
///
/// \param seconds The time in seconds to sleep for
///
///////////////////////////////////////////////////////////
void sleep(double seconds);

///////////////////////////////////////////////////////////
/// \brief Make current sleep for the time specified
///
/// \param milliseconds The time in milliseconds to sleep for
///
///////////////////////////////////////////////////////////
void sleep(Int32 milliseconds);

///////////////////////////////////////////////////////////
/// \brief Make current sleep for the time specified
///
/// \param milliseconds The time in milliseconds to sleep for
///
///////////////////////////////////////////////////////////
void sleep(Uint32 milliseconds);

}

#endif