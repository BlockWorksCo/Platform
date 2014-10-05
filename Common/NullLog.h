//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//




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



