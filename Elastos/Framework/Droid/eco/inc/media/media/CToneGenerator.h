
#ifndef __CTONEGENERATOR_H__
#define __CTONEGENERATOR_H__

#include "_CToneGenerator.h"

#include "media/ToneGenerator.h"

CarClass(CToneGenerator), public ToneGenerator
{
public:
    CARAPI StartTone(
        /* [in] */ Int32 toneType,
        /* [out] */ Boolean * pRes);

    CARAPI constructor(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 volume);

private:
    // TODO: Add your private member variables here.
};

#endif // __CTONEGENERATOR_H__
