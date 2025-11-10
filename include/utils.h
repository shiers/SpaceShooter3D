#pragma once

#if !defined(__UTILS_H__)
#define __UTILS_H__

// Library Includes
#include <sstream>

// Local Includes
#include "defines.h"

// Types

// Constants

// Prototypes

inline WChar16 *
StringToWchar_t(std::string _string)
{
	Int32 iStringSize = static_cast<Int32>(_string.size() + 1);
	WChar16 *wideChar = new WChar16[iStringSize];
	for (Int32 i = 0; i < iStringSize; ++i)
	{
		wideChar[i] = _string[i];
	}
	wideChar[iStringSize - 1] = '\0';
	return (wideChar);
}

template<typename T>
std::string ToString(const T &_value)
{
	std::ostringstream theStream; //strstream
	theStream << _value << std::ends;
	return (theStream.str());
}

#endif // __UTILS_H__



