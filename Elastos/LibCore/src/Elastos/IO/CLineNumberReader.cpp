#include "cmdef.h"
#include "CLineNumberReader.h"

ECode CLineNumberReader::Close()
{
    return LineNumberReader::Close();
}

ECode CLineNumberReader::Mark(
    /* [in] */ Int32 readLimit)
{
    return LineNumberReader::Mark(readLimit);
}

ECode CLineNumberReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported != NULL);

    return LineNumberReader::IsMarkSupported(supported);
}

ECode CLineNumberReader::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value != NULL);

    return LineNumberReader::Read(value);
}

ECode CLineNumberReader::ReadBuffer(
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer != NULL);
    VALIDATE_NOT_NULL(number != NULL);

    return LineNumberReader::ReadBuffer(buffer, number);
}

ECode CLineNumberReader::ReadBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ ArrayOf<Char8>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer != NULL);
    VALIDATE_NOT_NULL(number != NULL);

    return LineNumberReader::ReadBufferEx(offset, count, buffer, number);
}

ECode CLineNumberReader::IsReady(
    /* [out] */ Boolean* ready)
{
    VALIDATE_NOT_NULL(ready != NULL);

    return LineNumberReader::IsReady(ready);
}

ECode CLineNumberReader::Reset()
{
    return LineNumberReader::Reset();
}

ECode CLineNumberReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number != NULL);

    return LineNumberReader::Skip(count, number);
}

ECode CLineNumberReader::ReadLine(
    /* [out] */ String * contents)
{
    VALIDATE_NOT_NULL(contents != NULL);

    return LineNumberReader::ReadLine(contents);
}

ECode CLineNumberReader::GetLineNumber(
    /* [out] */ Int32* lineNumber)
{
    VALIDATE_NOT_NULL(lineNumber != NULL);

    return LineNumberReader::GetLineNumber(lineNumber);
}

ECode CLineNumberReader::SetLineNumber(
    /* [in] */ Int32 lineNumber)
{
    return LineNumberReader::SetLineNumber(lineNumber);
}

ECode CLineNumberReader::constructor(
    /* [in] */ IReader* ir)
{
    return LineNumberReader::Init(ir);
}

ECode CLineNumberReader::constructor(
    /* [in] */ IReader* ir,
    /* [in] */ Int32 size)
{
    return LineNumberReader::Init(ir, size);
}

Mutex* CLineNumberReader::GetSelfLock()
{
    return &_m_syncLock;
}

