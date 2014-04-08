



#ifndef __UTILITIES_H__
#define __UTILITIES_H__




#define NUMBER_OF_ELEMENTS(array)		(sizeof(array)/sizeof(array[0]))

#define CIRCULAR_INCREMENT(index, max)	(index=(index+1)%max)

#define DEBUGME							{while(true);}


#endif


