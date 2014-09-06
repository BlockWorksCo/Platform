


#ifndef __STDOUTLOG_H__
#define __STDOUTLOG_H__


#include <stdarg.h>



template <uint32_t MaxLineSize>
class StdoutLog
{
public:

	//
	// Simple text output.
	//
	static void puts(const char* text)
	{
		::printf("%s\n",text);
	}

	//
	//
	//
	static void printf(const char* format, ...)
	{
		va_list 	args;
		static char	buffer[MaxLineSize];

		va_start(args, format);
		memset(buffer, 0, sizeof(buffer));
		vsprintf(&buffer[0], format, args);
		va_end(args);

		puts(buffer);
	}

};




#endif



