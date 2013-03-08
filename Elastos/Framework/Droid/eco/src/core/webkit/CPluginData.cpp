
#include "webkit/CPluginData.h"
ECode CPluginData::GetInputStream(
    /* [out] */ IInputStream ** ppStream)
{
    *ppStream = mStream;
    mStream -> AddRef();
    return NOERROR;
}

ECode CPluginData::GetContentLength(
    /* [out] */ Int64 * pLength)
{
    *pLength = mContentLength;
    return NOERROR;
}

ECode CPluginData::GetHeaders(
    /* [out] */ IObjectStringMap ** ppHeaders)
{
    *ppHeaders = mHeaders;
    mHeaders -> AddRef();
    return NOERROR;
}

ECode CPluginData::GetStatusCode(
    /* [out] */ Int32 * pStatusCode)
{
    *pStatusCode = mStatusCode;
    return NOERROR;
}

ECode CPluginData::constructor(
    /* [in] */ IInputStream * pStream,
    /* [in] */ Int64 length,
    /* [in] */ IObjectStringMap * pHeaders,
    /* [in] */ Int32 code)
{
    mStream = pStream;
    mStream -> AddRef();
    mContentLength = length;
    mHeaders = pHeaders;
    mHeaders ->AddRef();
    mStatusCode = code;
    return NOERROR;
}

CPluginData::~CPluginData()
{
    mStream -> Release();
    mHeaders -> Release();
}

