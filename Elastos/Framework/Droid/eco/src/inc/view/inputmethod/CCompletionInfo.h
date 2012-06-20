
#ifndef __CCOMPLETIONINFO_H__
#define __CCOMPLETIONINFO_H__

#include "_CCompletionInfo.h"

CarClass(CCompletionInfo)
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);
};

#endif //__CCOMPLETIONINFO_H__