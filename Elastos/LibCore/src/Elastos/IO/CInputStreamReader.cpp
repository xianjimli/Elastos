
#include "CInputStreamReader.h"

ECode CInputStreamReader::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ String enc)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInputStreamReader::Close()
{
    return E_NOT_IMPLEMENTED;
}

ECode CInputStreamReader::Mark(
    /* [in] */ Int32 readLimit)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInputStreamReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInputStreamReader::Read(
    /* [out] */ Byte* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInputStreamReader::ReadBuffer(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInputStreamReader::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInputStreamReader::IsReady(
    /* [out] */ Boolean* ready)
{
    return E_NOT_IMPLEMENTED;
}

ECode CInputStreamReader::Reset()
{
    return E_NOT_IMPLEMENTED;
}

ECode CInputStreamReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* skipped)
{
    return E_NOT_IMPLEMENTED;
}
