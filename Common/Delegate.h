//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, September 2014.
//




#ifndef __DELEGATE_H__
#define __DELEGATE_H__




#define DELEGATE_TYPE(targetClass,targetMethod, typeName)       \
struct typeName                                                 \
{                                                               \
    typeName(targetClass& _instance) : instance(_instance) {}   \
    void operator()() {instance.targetMethod();}                \
    targetClass&  instance;                                     \
}



#endif

