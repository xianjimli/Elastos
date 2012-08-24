#include "cmdef.h"
#include "CStringWriter.h"

ECode CStringWriter::Close()
{
    return StringWriter::Close();
}

ECode CStringWriter::Flush()
{
    return StringWriter::Flush();
}

ECode CStringWriter::Write(
    /* [in] */ Int32 oneChar32)
{
    return StringWriter::Write(oneChar32);
}

ECode CStringWriter::WriteBuffer(
    /* [in] */ const ArrayOf<Char8> &buffer)
{
    return StringWriter::WriteBuffer(buffer);
}

ECode CStringWriter::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Char8> &buffer)
{
    return StringWriter::WriteBufferEx(offset, count, buffer);
}

ECode CStringWriter::WriteString(
    /* [in] */ CString str)
{
    return StringWriter::WriteString(str);
}

ECode CStringWriter::WriteStringEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ CString str)
{
    return StringWriter::WriteStringEx(offset, count, str);
}

ECode CStringWriter::Append(
    /* [in] */ Char32 c)
{
    return StringWriter::Append(c);
}

ECode CStringWriter::AppendEx(
    /* [in] */ ICharSequence *csq)
{
    VALIDATE_NOT_NULL(csq);

    return StringWriter::AppendEx(csq);
}

ECode CStringWriter::AppendEx2(
    /* [in] */ ICharSequence *csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    VALIDATE_NOT_NULL(csq);

    return StringWriter::AppendEx2(csq, start, end);
}

ECode CStringWriter::CheckError(
    /* [out] */ Boolean *hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return StringWriter::CheckError(hasError);
}

ECode CStringWriter::constructor()
{
    return StringWriter::Init();
}

ECode CStringWriter::constructor(
    /* [in] */ Int32 initialSize)
{
    return StringWriter::Init(initialSize);
}

Mutex* CStringWriter::GetSelfLock()
{
    return &_m_syncLock;
}



