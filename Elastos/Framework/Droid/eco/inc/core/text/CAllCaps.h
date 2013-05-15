
#ifndef __CALLCAPS_H__
#define __CALLCAPS_H__

#include "_CAllCaps.h"

CarClass(CAllCaps)
{
public:
    CARAPI Filter(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ISpanned* dest,
        /* [in] */ Int32 dstart,
        /* [in] */ Int32 dend,
        /* [out] */ ICharSequence** cs);
};

#endif //__CALLCAPS_H__