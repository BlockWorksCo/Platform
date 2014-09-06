


#ifndef __MAP_H__
#define __MAP_H__



//
//
//
template <typename IndexType, typename KeyType, uint32_t offset>
class OffsetHash
{
public:
    IndexType operator()(KeyType key)
    {
        IndexType     valueIndex  = key-offset;

        return valueIndex;
    }
};



//
//
//
template < typename A, typename B, typename HashFunctorType >
class Map
{

public:

    Map(A* _a[], HashFunctorType& _hashFunctor) :
        hashFunctor(_hashFunctor),
        a(_a)
    {

    }


    A& Lookup(B key)
    {
        B   valueIndex  = hashFunctor(key);
        A*  value       = a[valueIndex];
        
        return *value;
    }

private:

    HashFunctorType&    hashFunctor;
    A**                 a;

};



#endif


