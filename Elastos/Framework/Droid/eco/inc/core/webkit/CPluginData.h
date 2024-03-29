
#ifndef __CPLUGINDATA_H__
#define __CPLUGINDATA_H__

#include "_CPluginData.h" 
#include "ext/frameworkext.h"
//#include <elastos/Map.h>
//#include <elastos/Vector.h>

using namespace Elastos;

CarClass(CPluginData)
{
public:
    CARAPI GetInputStream(
        /* [out] */ IInputStream ** stream);

    CARAPI GetContentLength(
        /* [out] */ Int64 * length);

    CARAPI GetHeaders(
        /* [out] */ IObjectStringMap ** headers);

    CARAPI GetStatusCode(
        /* [out] */ Int32 * statusCode);

    CARAPI constructor(
        /* [in] */ IInputStream * stream,
        /* [in] */ Int64 length,
        /* [in] */ IObjectStringMap * headers,
        /* [in] */ Int32 code);

    ~CPluginData();

private:    
    /**
     * The content stream.
     */
    IInputStream * mStream;
    /**
     * The content length.
     */
    Int64 mContentLength;
    /**
     * The associated HTTP response headers stored as a map of
     * lowercase header name to [ unmodified header name, header value].
     * TODO: This design was always a hack. Remove (involves updating
     * the Gears C++ side).
     */
    //Map<String, Vector<String> > mHeaders;
    IObjectStringMap * mHeaders;

    /**
     * The associated HTTP response code.
     */
    Int32 mStatusCode;

};

#endif // __CPLUGINDATA_H__
