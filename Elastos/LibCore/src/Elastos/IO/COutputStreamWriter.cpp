#include "cmdef.h"
#include "COutputStreamWriter.h"

ECode COutputStreamWriter::Close()
{
    return OutputStreamWriter::Close();
}

ECode COutputStreamWriter::Flush()
{
    return OutputStreamWriter::Flush();
}

ECode COutputStreamWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    return OutputStreamWriter::Write(oneChar32);
}

ECode COutputStreamWriter::WriteBuffer(
    /* [in] */ const ArrayOf<Char8> &buffer)
{
    return OutputStreamWriter::WriteBuffer(buffer);
}

ECode COutputStreamWriter::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Char8> &buffer)
{
    return OutputStreamWriter::WriteBufferEx(offset, count, buffer);
}

ECode COutputStreamWriter::WriteString(
    /* [in] */ CString str)
{
    return OutputStreamWriter::WriteString(str);
}

ECode COutputStreamWriter::WriteStringEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ CString str)
{
    return OutputStreamWriter::WriteStringEx(offset, count, str);
}

ECode COutputStreamWriter::Append(
    /* [in] */ Char32 c)
{
    return OutputStreamWriter::Append(c);
}

ECode COutputStreamWriter::AppendEx(
    /* [in] */ ICharSequence *csq)
{
    VALIDATE_NOT_NULL(csq);

    return OutputStreamWriter::AppendEx(csq);
}

ECode COutputStreamWriter::AppendEx2(
    /* [in] */ ICharSequence *csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    VALIDATE_NOT_NULL(csq);

    return OutputStreamWriter::AppendEx2(csq, start, end);
}

ECode COutputStreamWriter::CheckError(
    /* [out] */ Boolean *hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return OutputStreamWriter::CheckError(hasError);
}

ECode COutputStreamWriter::GetEncoding(
    /* [out] */ String *encoding)
{
    VALIDATE_NOT_NULL(encoding);

    return OutputStreamWriter::GetEncoding(encoding);
}

ECode COutputStreamWriter::constructor(
    /* [in] */ IOutputStream *os)
{
    return OutputStreamWriter::Init(os);
}

ECode COutputStreamWriter::constructor(
    /* [in] */ IOutputStream *os,
    /* [in] */ const String &enc)
{
    return OutputStreamWriter::Init(os, enc);
}

Mutex* COutputStreamWriter::GetSelfLock()
{
    return &_m_syncLock;
}