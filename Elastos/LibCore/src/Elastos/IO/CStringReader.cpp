#include "cmdef.h"
#include "CStringReader.h"

ECode CStringReader::Close()
{
    return StringReader::Close();
}

ECode CStringReader::Mark(
    /* [in] */ Int32 readLimit)
{
    return StringReader::Mark(readLimit);
}

ECode CStringReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);

    return StringReader::IsMarkSupported(supported);
}

ECode CStringReader::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    return StringReader::Read(value);
}

ECode CStringReader::ReadBuffer(
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return StringReader::ReadBuffer(buffer, number);
}

ECode CStringReader::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    return StringReader::ReadBufferEx(offset, count, buffer, number);
}

ECode CStringReader::IsReady(
    /* [out] */ Boolean* ready)
{
    VALIDATE_NOT_NULL(ready);

    return StringReader::IsReady(ready);
}

ECode CStringReader::Reset()
{
    return StringReader::Reset();
}

ECode CStringReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    return StringReader::Skip(count, number);
}

ECode CStringReader::constructor(
    /* [in] */ String str)
{
    return StringReader::Init(str);
}

Mutex* CStringReader::GetSelfLock()
{
    return &_m_syncLock;
}

