
#ifndef __CLENGTHFILTER_H__
#define __CLENGTHFILTER_H__

#include "_CLengthFilter.h"

CarClass(CLengthFilter)
{
public:
    CLengthFilter();

    CARAPI Filter(
        /* [in] */ ICharSequence* source,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ISpanned* dest,
        /* [in] */ Int32 dstart,
        /* [in] */ Int32 dend,
        /* [out] */ ICharSequence** cs);

    CARAPI constructor(
        /* [in] */ Int32 max);

private:
    Int32 mMax;
};

#endif //__CLENGTHFILTER_H__