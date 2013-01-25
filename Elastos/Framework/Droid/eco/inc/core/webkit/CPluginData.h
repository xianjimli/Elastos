
#ifndef __CPLUGINDATA_H__
#define __CPLUGINDATA_H__

#include "_CPluginData.h"

CarClass(CPluginData)
{
public:
    CARAPI GetInputStream(
        /* [out] */ IInputStream ** ppStream);

    CARAPI GetContentLength(
        /* [out] */ Int64 * pLength);

    CARAPI GetHeaders(
        /* [out] */ IObjectStringMap ** ppHeaders);

    CARAPI GetStatusCode(
        /* [out] */ Int32 * pStatusCode);

    CARAPI constructor(
        /* [in] */ IInputStream * pStream,
        /* [in] */ Int64 length,
        /* [in] */ IObjectStringMap * pHeaders,
        /* [in] */ Int32 code);

private:
    // TODO: Add your private member variables here.
};

#endif // __CPLUGINDATA_H__
