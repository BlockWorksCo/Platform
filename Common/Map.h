


#ifndef __MAP_H__
#define __MAP_H__



//
//
//
template <typename IndexType, typename KeyType>
class SimpleHash
{
public:
    IndexType operator()(KeyType key)
    {
        IndexType     valueIndex  = key;

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

    Map(A _a[], HashFunctorType& _hashFunctor) :
        hashFunctor(_hashFunctor)
    {

    }


    A& Lookup(B key)
    {

    }

private:

    HashFunctorType&    hashFunctor;

};



#endif


