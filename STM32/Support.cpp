//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//






extern "C" void __libc_init_array(void)
{
}





namespace __cxxabiv1 
{
    /* guard variables */
 
    /* The ABI requires a 64-bit type.  */
    __extension__ typedef int __guard __attribute__((mode(__DI__)));
 
    extern "C" int __cxa_guard_acquire (__guard *);
    extern "C" void __cxa_guard_release (__guard *);
    extern "C" void __cxa_guard_abort (__guard *);
 
    extern "C" int __cxa_guard_acquire (__guard *g) 
    {
        return !*(char *)(g);
    }
 
    extern "C" void __cxa_guard_release (__guard *g)
    {
        *(char *)g = 1;
    }
 
    extern "C" void __cxa_guard_abort (__guard *)
    {
 
    }
}




