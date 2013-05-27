
#include "cmdef.h"
#include "CharBuffer.h"
#include "BufferFactory.h"
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>
#include <elastos/Math.h>

// {43410042-EC51-485f-9C70-1DDBC11B759D}
extern "C" const InterfaceID EIID_CharBuffer =
    { 0x43410042, 0xec51, 0x485f, { 0x9c, 0x70, 0x1d, 0xdb, 0xc1, 0x1b, 0x75, 0x9d } };

ECode CharBuffer::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ ICharBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    if (capacity < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }
    return BufferFactory::NewCharBufferEx2(capacity, buf);
}

ECode CharBuffer::Wrap(
    /* [in] */ ArrayOf<Char32>* array,
    /* [out] */ ICharBuffer** buf)
{
    return WrapEx(array, 0, array->GetLength(), buf);
}

ECode CharBuffer::WrapEx(
    /* [in] */ ArrayOf<Char32>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 len,
    /* [out] */ ICharBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    Int32 length = array->GetLength();
    if ((start < 0) || (len < 0) || (Int64) start + (Int64) len > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    AutoPtr<ICharBuffer> charBuf;
    BufferFactory::NewCharBuffer(array, (ICharBuffer**)&charBuf);
    CharBuffer* bufCls = (CharBuffer*)charBuf->Probe(EIID_CharBuffer);
    bufCls->mPosition = start;
    bufCls->mLimit = start + len;

    *buf = charBuf;
    return NOERROR;
}

ECode CharBuffer::WrapEx2(
    /* [in] */ ICharSequence* chseq,
    /* [out] */ ICharBuffer** buf)
{
    return BufferFactory::NewCharBufferEx(chseq, buf);
}

ECode CharBuffer::WrapEx3(
    /* [in] */ ICharSequence* chseq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharBuffer** buf)
{
    if (chseq == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new NullPointerException();
    }
    Int32 len;
    chseq->GetLength(&len);
    if (start < 0 || end < start || end > len) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    AutoPtr<ICharBuffer> result;
    BufferFactory::NewCharBufferEx(chseq, (ICharBuffer**)&result);
    CharBuffer* bufCls = (CharBuffer*)result->Probe(EIID_CharBuffer);
    bufCls->mPosition = start;
    bufCls->mLimit = end;

    *buf = result;
    result->AddRef();
    return NOERROR;
}

CharBuffer::CharBuffer(
    /* [in] */ Int32 capacity)
    : Buffer(capacity)
{
    mElementSizeShift = 2;
}

ECode CharBuffer::Array(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    return ProtectedArray(array);
}

ECode CharBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return ProtectedArrayOffset(offset);
}

ECode CharBuffer::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 remaining;
    Remaining(&remaining);

    if (index < 0 || index >= remaining) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    return GetCharEx(mPosition + index, value);
}

ECode CharBuffer::CompareTo(
    /* [in] */ ICharBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    VALIDATE_NOT_NULL(otherBuffer);

    Int32 remaining;
    Int32 otherRemaining;
    Remaining(&remaining);
    otherBuffer->Remaining(&otherRemaining);

    Int32 compareRemaining = (remaining < otherRemaining) ?
                              remaining : otherRemaining;
    Int32 thisPos = mPosition;
    Int32 otherPos;
    otherBuffer->GetPosition(&otherPos);

    Char32 thisByte;
    Char32 otherByte;
    while (compareRemaining > 0) {
        GetCharEx(thisPos, &thisByte);
        otherBuffer->GetCharEx(otherPos, &otherByte);
        if (thisByte != otherByte) {
            *result = thisByte < otherByte ? -1 : 1;
            return NOERROR;
        }

        thisPos++;
        otherPos++;
        compareRemaining--;
    }

    *result = remaining - otherRemaining;
    return NOERROR;
}

ECode CharBuffer::GetChars(
    /* [out] */ ArrayOf<Char32>* dst)
{
    return GetCharsEx(0, dst->GetLength(), dst);
}

ECode CharBuffer::GetCharsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Char32>* dst)
{
    VALIDATE_NOT_NULL(dst);

    Int32 length = dst->GetLength();
    if ((off < 0) || (len < 0) || ((Int64) off + (Int64) len > length)) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 remaining;
    Remaining(&remaining);
    if (len > remaining) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    for (Int32 i = off; i < off + len; i++) {
        GetChar(&(*dst)[i]);
    }

    return NOERROR;
}

ECode CharBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return ProtectedHasArray(hasArray);
}

ECode CharBuffer::GetLength(
    /* [out] */ Int32* number)
{
    return Remaining(number);
}

ECode CharBuffer::PutChars(
    /* [in] */ const ArrayOf<Char32>& src)
{
    return PutCharsEx(0, src.GetLength(), src);
}

ECode CharBuffer::PutCharsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Char32>& src)
{
    Int32 length;

    length = src.GetLength();
    if ((off < 0) || (len < 0) ||
        (Int64) off + (Int64) len > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 remaining;
    Remaining(&remaining);
    if (len > remaining) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }
    for (Int32 i = off; i < off + len; i++) {
        PutChar(src[i]);
    }

    return NOERROR;
}

ECode CharBuffer::PutCharBuffer(
    /* [in] */ ICharBuffer* src)
{
    if (src == (ICharBuffer*)this->Probe(EIID_ICharBuffer)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 srcRemaining, remaining;
    src->Remaining(&srcRemaining);
    Remaining(&remaining);
    if (srcRemaining > remaining) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }

    ArrayOf<Char32>* contents = ArrayOf<Char32>::Alloc(srcRemaining);
    src->GetChars(contents);
    PutChars(*contents);
    ArrayOf<Char32>::Free(contents);

    return NOERROR;
}

ECode CharBuffer::PutString(
    /* [in] */ const String& str)
{
    return PutStringEx(str, 0, str.GetLength());
}

ECode CharBuffer::PutStringEx(
    /* [in] */ const String& str,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    Int32 length = str.GetLength();
    if (start < 0 || end < start || end > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    Int32 remaining;
    Remaining(&remaining);
    if (end - start > remaining) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    for (Int32 i = start; i < end; i++) {
        PutChar(str[i]);
    }

    return NOERROR;
}

ECode CharBuffer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    StringBuffer result;
    Char32 c;
    for (Int32 i = mPosition; i < mLimit; i++) {
        GetCharEx(i, &c);
        result += c;
    }
    *str = String(result);

    return NOERROR;
}

ECode CharBuffer::AppendChar(
    /* [in] */ Char32 c)
{
    return PutChar(c);
}

ECode CharBuffer::AppendChars(
    /* [in] */ ICharSequence* csq)
{
    if (csq != NULL) {
        String str;
        csq->ToString(&str);
        return PutString(str);
    }
    return PutString(String("null"));
}

ECode CharBuffer::AppendCharsEx(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (csq == NULL) {
        //csq = "null";
        return NOERROR;
    }
    AutoPtr<ICharSequence> cs;
    csq->SubSequence(start, end, (ICharSequence**)&cs);
    Int32 len;
    cs->GetLength(&len);
    if (len > 0) {
        String str;
        cs->ToString(&str);
        return PutString(str);
    }
    return NOERROR;
}

ECode CharBuffer::Read(
    /* [in] */ ICharBuffer* target,
    /* [out] */ Int32* number)
{
    Int32 remaining;
    Remaining(&remaining);
    if (target == (ICharBuffer*)this->Probe(EIID_ICharBuffer)) {
        if (remaining == 0) {
            *number = -1;
            return NOERROR;
        }

        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 targetRemaining;
    target->Remaining(&targetRemaining);
    if (remaining == 0) {
        *number = mLimit > 0 && targetRemaining == 0 ? 0 : -1;
        return NOERROR;
    }

    remaining = Math::Min(targetRemaining, remaining);
    if (remaining > 0) {
        ArrayOf<Char32>* chars = ArrayOf<Char32>::Alloc(remaining);
        GetChars(chars);
        target->PutChars(*chars);
        ArrayOf<Char32>::Free(chars);
    }

    *number = remaining;
    return NOERROR;
}
