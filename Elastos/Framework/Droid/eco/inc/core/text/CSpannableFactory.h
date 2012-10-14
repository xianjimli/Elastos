
#ifndef __CSPANNABLEFACTORY_H__
#define __CSPANNABLEFACTORY_H__

#include "_CSpannableFactory.h"

CarClass(CSpannableFactory)
{
public:
    CARAPI NewSpannable(
        /* [in] */ ICharSequence* source,
        /* [out] */ ISpannable** spannable);
};

#endif //__CSPANNABLEFACTORY_H__