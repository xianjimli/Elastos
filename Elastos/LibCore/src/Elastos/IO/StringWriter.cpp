#include "cmdef.h"
#include "StringWriter.h"
#include <elastos/Character.h>

using namespace Elastos::Core;

StringWriter::StringWriter()
{
}

StringWriter::~StringWriter()
{
    StringBuf::Free(mBuf);
}

ECode StringWriter::Init()
{
    mBuf = StringBuf::Alloc(16);
    if (mBuf == NULL) return E_OUT_OF_MEMORY_ERROR;

    return NOERROR;
}

ECode StringWriter::Init(
    /* [in] */ Int32 initialSize)
{
    if (initialSize < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mBuf = StringBuf::Alloc(initialSize);
    if (mBuf == NULL) return E_OUT_OF_MEMORY_ERROR;

    return NOERROR;
}

ECode StringWriter::Close()
{
    return NOERROR;
}

ECode StringWriter::Flush()
{
    return NOERROR;
}

//ECode StringWriter::GetBuffer(
//    /* [out] */ StringBuff* buf)
//{
//    assert(buf != NULL);

//    buf = mBuf;
//    return NOERROR;
//}

ECode StringWriter::ToString(
    /* [out] */ String* str)
{
    return E_NOT_IMPLEMENTED;
}

ECode StringWriter::Write(
    /* [in] */ Int32 oneChar8)
{
//    mBuf.Append((Char8)oneChar8);
    return NOERROR;
}

ECode StringWriter::WriteBufferEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Char8>& buffer)
{
    // avoid int overflow
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // removed redundant check, added null check, used (offset | count) < 0
    // instead of (offset < 0) || (count < 0) to safe one operation
    if ((offset | count) < 0 || count > buffer.GetLength() - offset) {
//      throw new IndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    if (count == 0) {
        return NOERROR;
    }
//    mBuf.Append(buffer, offset, count);

    return NOERROR;
}

ECode StringWriter::WriteString(
    /* [in] */ CString &str)
{
//    mBuf.Append(str);
    return NOERROR;
}

ECode StringWriter::WriteStringEx(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ CString &str)
{
//    mBuf.Append(str, offset, count);
    return NOERROR;
}

ECode StringWriter::Append(
    /* [in] */ Char8 c)
{
    Write(c);

    return NOERROR;
}

ECode StringWriter::AppendEx(
    /* [in] */ ICharSequence* csq)
{
    if (csq == NULL) {
        //WriteString("NULL");
    } else {
//        String tmp;
//        csq->ToString(&tmp);
//        WriteString(tmp);
    }
    return NOERROR;
}

ECode StringWriter::AppendEx2(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (NULL == csq) {
        csq = NULL;
    }
    AutoPtr<ICharSequence> subcsq;
    CString output;
    csq->SubSequence(start, end, (ICharSequence**)&subcsq);
//    subcsq->ToString(&output);

    return WriteStringEx(0, output.GetLength(), output);
}