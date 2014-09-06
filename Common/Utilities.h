


#ifndef __UTILITIES_H__
#define __UTILITIES_H__





#define NUMBER_OF_ELEMENTS(array)       (sizeof(array)/sizeof(array[0]))

#define DEBUGME                         while(true)

#define PANIC(code)						panicCode = code;

extern uint32_t 	panicCode;



#endif



