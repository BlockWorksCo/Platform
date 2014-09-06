


#ifndef __NULLLOG_H__
#define __NULLLOG_H__

#include <stdarg.h>



template <int id>
class NullLog
{
public:

	//
	// Simple text output.
	//
	static void puts(const char* text)
	{
	}

	//
	//
	//
	static void printf(const char* format, ...)
	{
	}

};




#endif



