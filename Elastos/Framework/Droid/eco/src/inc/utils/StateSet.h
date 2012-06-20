
#ifndef __STATESET_H__
#define __STATESET_H__

#include "ext/frameworkext.h"

class StateSet
{
public:
    static Boolean IsWildCard(
        /* [in] */ ArrayOf<Int32>* stateSetOrSpec);

    static Boolean StateSetMatches(
        /* [in] */ ArrayOf<Int32>* stateSpec, 
        /* [in] */ ArrayOf<Int32>* stateSet);

    static Boolean StateSetMatches(
        /* [in] */ ArrayOf<Int32>* stateSpec, 
        /* [in] */ Int32 state);

    static ArrayOf<Int32>* TrimStateSet(
        /* [in] */ ArrayOf<Int32>* states, 
        /* [in] */ Int32 newSize);

    //static String dump(int[] states);

public:
    static const ArrayOf<Int32>* WILD_CARD;
};

#endif //__STATESET_H__
