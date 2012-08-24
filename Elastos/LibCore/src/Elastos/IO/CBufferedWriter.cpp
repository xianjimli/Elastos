#include "cmdef.h"
#include "CBufferedWriter.h"

ECode CBufferedWriter::Close()
{
    return BufferedWriter::Close();
}

ECode CBufferedWriter::Flush()
{
    return BufferedWriter::Flush();
}

ECode CBufferedWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    return BufferedWriter::Write(oneChar32);
}

ECode CBufferedWriter::WriteBuffer(
    /* [in] */ const ArrayOf<Char8>& buffer)
{
    return BufferedWriter::WriteBuffer(buffer);
}

ECode CBufferedWriter::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Char8>& buffer)
{
    return BufferedWriter::WriteBufferEx(offset, count, buffer);
}

ECode CBufferedWriter::WriteString(
    /* [in] */ CString str)
{
    return BufferedWriter::WriteString(str);
}

ECode CBufferedWriter::WriteStringEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ CString str)
{
    return BufferedWriter::WriteStringEx(offset, count, str);
}

ECode CBufferedWriter::Append(
    /* [in] */ Char32 c)
{
    return BufferedWriter::Append(c);
}

ECode CBufferedWriter::AppendEx(
    /* [in] */ ICharSequence* csq)
{
    return BufferedWriter::AppendEx(csq);
}

ECode CBufferedWriter::AppendEx2(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return BufferedWriter::AppendEx2(csq, start, end);
}

ECode CBufferedWriter::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return BufferedWriter::CheckError(hasError);
}

ECode CBufferedWriter::NewLine()
{
    return BufferedWriter::NewLine();
}

ECode CBufferedWriter::constructor(
    /* [in] */ IWriter* wout)
{
    return BufferedWriter::Init(wout);
}

ECode CBufferedWriter::constructor(
    /* [in] */ IWriter* wout,
    /* [in] */ Int32 size)
{
    return BufferedWriter::Init(wout, size);
}

Mutex* CBufferedWriter::GetSelfLock()
{
    return &_m_syncLock;
}
