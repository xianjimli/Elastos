
#ifndef __CExtractedText_h__
#define __CExtractedText_h__

#include "_CExtractedText.h"

CarClass(CExtractedText)
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);
};

#endif //__CExtractedText_h__