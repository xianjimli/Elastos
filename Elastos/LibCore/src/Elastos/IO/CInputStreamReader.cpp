#include "cmdef.h"
#include "CInputStreamReader.h"

ECode CInputStreamReader::constructor(
    /* [in] */ IInputStream *in)
{
    return InputStreamReader::Init(in);
}

ECode CInputStreamReader::constructor(
    /* [in] */ IInputStream *in,
    /* [in] */ const String &enc)
{
    return InputStreamReader::Init(in, enc);
}

ECode CInputStreamReader::constructor(
    /* [in] */ IInputStream* in,
    /* [in] */ ICharset* charset)
{
    return InputStreamReader::Init(in, charset);
}

ECode CInputStreamReader::Close()
{
    return InputStreamReader::Close();
}

ECode CInputStreamReader::GetEncoding(
    /* [out] */ String *encoding)
{
    VALIDATE_NOT_NULL(encoding);

    return InputStreamReader::GetEncoding(encoding);
}

ECode CInputStreamReader::Mark(
    /* [in] */ Int32 readLimit)
{
    return InputStreamReader::Mark(readLimit);
}

ECode CInputStreamReader::IsMarkSupported(
    /* [out] */ Boolean *supported)
{
    VALIDATE_NOT_NULL(supported);

    return InputStreamReader::IsMarkSupported(supported);
}

ECode CInputStreamReader::Read(
    /* [out] */ Int32 *value)
{
    VALIDATE_NOT_NULL(value);

    return InputStreamReader::Read(value);
}

ECode CInputStreamReader::ReadBuffer(
    /* [out] */ ArrayOf<Char8> *buffer,
    /* [out] */ Int32 *number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return InputStreamReader::ReadBuffer(buffer, number);
}

ECode CInputStreamReader::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Char8> *buffer,
    /* [out] */ Int32 *number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return InputStreamReader::ReadBufferEx(offset, count, buffer, number);
}

ECode CInputStreamReader::IsReady(
    /* [out] */ Boolean *ready)
{
    VALIDATE_NOT_NULL(ready);

    return InputStreamReader::IsReady(ready);
}

ECode CInputStreamReader::Reset()
{
    return InputStreamReader::Reset();
}

ECode CInputStreamReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64 *number)
{
    VALIDATE_NOT_NULL(number);

    return InputStreamReader::Skip(count, number);
}

Mutex* CInputStreamReader::GetSelfLock()
{
    return &_m_syncLock;
}