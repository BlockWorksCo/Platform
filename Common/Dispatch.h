

#ifndef __DISPATCH_H__
#define __DISPATCH_H__






//
// EnumeratedDispatch
//

template <typename First, typename EnumerationType> void EnumeratedDispatch(const EnumerationType enumeration, const EnumerationType firstEnumeration, First first) 
{
    if(enumeration == firstEnumeration)
    {
        first();
    }
}

template <typename First, typename EnumerationType, typename... Rest> void EnumeratedDispatch(const EnumerationType enumeration, const EnumerationType firstEnumeration, First& first, Rest... rest) 
{
    if(enumeration == firstEnumeration)
    {
        first();
    }
    EnumeratedDispatch(enumeration, rest...);
}








#endif




