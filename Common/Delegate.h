


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

