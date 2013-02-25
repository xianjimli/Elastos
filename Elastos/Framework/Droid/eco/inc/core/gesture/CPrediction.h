
#ifndef __CPREDICTION_H__
#define __CPREDICTION_H__

#include "_CPrediction.h"
#include "ext/frameworkext.h"
using namespace Elastos;
using namespace Elastos::Core;

CarClass(CPrediction)
{
public:
    Double mScore;

    CARAPI constructor(
            /* [in] */ const String& label,
            /* [in] */ Double predictionScore);

    CARAPI GetScore(
            /* [out] */ Double *score);

    CARAPI SetScore(
            /* [in] */ Double score);

    /*@Override*/
    CARAPI ToString(
            /* [out] */ String* ret);

private:
    String mName;
};

#endif // __CPREDICTION_H__
