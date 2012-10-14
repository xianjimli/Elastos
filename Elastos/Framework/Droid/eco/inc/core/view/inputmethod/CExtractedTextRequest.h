
#ifndef __CEXTRACTEDTEXTREQUEST_H__
#define __CEXTRACTEDTEXTREQUEST_H__

#include "_CExtractedTextRequest.h"

CarClass(CExtractedTextRequest)
{
public:
    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);
};

#endif //__CEXTRACTEDTEXTREQUEST_H__