
#include "webkit/CPluginData.h"
ECode CPluginData::GetInputStream(
    /* [out] */ IInputStream ** stream)
{
    VALIDATE_NOT_NULL(stream);
    *stream = mStream;
    return NOERROR;
}

ECode CPluginData::GetContentLength(
    /* [out] */ Int64 * length)
{
    VALIDATE_NOT_NULL(length);
    *length = mContentLength;
    return NOERROR;
}

ECode CPluginData::GetHeaders(
    /* [out] */ IObjectStringMap ** headers)
{
    VALIDATE_NOT_NULL(headers);
    *headers = mHeaders;
    return NOERROR;
}

ECode CPluginData::GetStatusCode(
    /* [out] */ Int32 * statusCode)
{
    VALIDATE_NOT_NULL(statusCode);
    *statusCode = mStatusCode;
    return NOERROR;
}

ECode CPluginData::constructor(
    /* [in] */ IInputStream * stream,
    /* [in] */ Int64 length,
    /* [in] */ IObjectStringMap * headers,
    /* [in] */ Int32 code)
{
    mStream = stream;
    mContentLength = length;
    mHeaders = headers;
    mStatusCode = code;
    return NOERROR;
}

CPluginData::~CPluginData()
{
}

