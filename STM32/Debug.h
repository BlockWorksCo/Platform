//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//




#ifndef __STM32DEBUG_H__
#define __STM32DEBUG_H__




#define STOP			{while(true);}

#define DEBUG_TEXT(msg)      


void SetupDebugPin();
void SetDebugPin();
void ClearDebugPin();
void SetDebugPin2();
void ClearDebugPin2();


#endif

