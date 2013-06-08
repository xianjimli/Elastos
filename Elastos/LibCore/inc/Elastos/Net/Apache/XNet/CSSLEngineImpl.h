
#ifndef __CSSLENGINEIMPL_H__
#define __CSSLENGINEIMPL_H__

#include "_CSSLEngineImpl.h"

CarClass(CSSLEngineImpl)
{
public:
    CARAPI constructor(
        /* [in] */ ISSLParametersImpl * pSslParameters);

    CARAPI constructor(
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [in] */ ISSLParametersImpl * pSslParameters);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSSLENGINEIMPL_H__
