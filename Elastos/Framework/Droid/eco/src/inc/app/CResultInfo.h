
#ifndef __CRESULTINFO_H__
#define __CRESULTINFO_H__

#include "_CResultInfo.h"
#include "content/CIntent.h"
#include <elastos/AutoPtr.h>

CarClass(CResultInfo)
{
public:
    CARAPI constructor(
        /* [in] */ const String& resultWho,
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data);
    
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

public:
    String mResultWho;
    Int32 mRequestCode;
    Int32 mResultCode;
    AutoPtr<IIntent> mData;        
};

#endif // __CRESULTINFO_H__
