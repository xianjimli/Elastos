#include "cmdef.h"
#include "CCharArrayWriter.h"

ECode CCharArrayWriter::Close()
{
    return CharArrayWriter::Close();
}

ECode CCharArrayWriter::Flush()
{
    return CharArrayWriter::Flush();
}

ECode CCharArrayWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    return CharArrayWriter::Write(oneChar32);
}

ECode CCharArrayWriter::WriteBuffer(
    /* [in] */ const ArrayOf<Char8>& buffer)
{
    return CharArrayWriter::WriteBuffer(buffer);
}

ECode CCharArrayWriter::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Char8>& buffer)
{
    return CharArrayWriter::WriteBufferEx(offset, count, buffer);
}

ECode CCharArrayWriter::WriteString(
    /* [in] */ CString str)
{
    return CharArrayWriter::WriteString(str);
}

ECode CCharArrayWriter::WriteStringEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ CString str)
{
    return CharArrayWriter::WriteStringEx(offset, count, str);
}

ECode CCharArrayWriter::Append(
    /* [in] */ Char32 c)
{
    return CharArrayWriter::Append(c);
}

ECode CCharArrayWriter::AppendEx(
    /* [in] */ ICharSequence* csq)
{
    return CharArrayWriter::AppendEx(csq);
}

ECode CCharArrayWriter::AppendEx2(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return CharArrayWriter::AppendEx2(csq, start, end);
}

ECode CCharArrayWriter::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return CharArrayWriter::CheckError(hasError);
}

ECode CCharArrayWriter::Reset()
{
    return CharArrayWriter::Reset();
}

ECode CCharArrayWriter::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    return CharArrayWriter::GetSize(size);
}

ECode CCharArrayWriter::ToCharArray(
    /* [out, callee] */ ArrayOf<Char8>** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    return CharArrayWriter::ToCharArray(buffer);
}

ECode CCharArrayWriter::WriteTo(
    /* [in] */ IWriter* ot)
{
    return CharArrayWriter::WriteTo(ot);
}

ECode CCharArrayWriter::constructor()
{
    return CharArrayWriter::Init();
}

ECode CCharArrayWriter::constructor(
    /* [in] */ Int32 initialSize)
{
    return CharArrayWriter::Init(initialSize);
}

Mutex* CCharArrayWriter::GetSelfLock()
{
    return &_m_syncLock;
}
