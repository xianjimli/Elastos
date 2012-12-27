
#include "cmdef.h"
#include "CharArrayBuffer.h"
#include <elastos/AutoPtr.h>

CharArrayBuffer::CharArrayBuffer(
    /* [in] */ ArrayOf<Char32>* array)
    : CharBuffer(array->GetLength())
    , mBackingArray(array)
    , mOffset(0)
{}

CharArrayBuffer::CharArrayBuffer(
    /* [in] */ Int32 capacity)
    : CharBuffer(capacity)
    , mBackingArray(ArrayOf<Char32>::Alloc(capacity))
    , mOffset(0)
{}


CharArrayBuffer::CharArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Char32>* backingArray,
    /* [in] */ Int32 offset)
    : CharBuffer(capacity)
    , mBackingArray(backingArray)
    , mOffset(0)
{}

ECode CharArrayBuffer::GetChar(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    *value = (*mBackingArray)[mOffset + mPosition++];

    return NOERROR;
}

ECode CharArrayBuffer::GetCharEx(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    *value = (*mBackingArray)[mOffset + index];

    return NOERROR;
}

ECode CharArrayBuffer::GetCharsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Char32>* dst)
{
    VALIDATE_NOT_NULL(dst);

    Int32 length = dst->GetLength();
    if ((off < 0) || (len < 0) || (Int64) off + (Int64) len > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    Int32 remaining;
    Remaining(&remaining);
    if (len > remaining) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
//    System.arraycopy(backingArray, offset + position, dst, off, len);
    for(Int32 i = 0; i < len; ++i) {
        (*dst)[i + off] = (*mBackingArray)[mOffset + mPosition + i];
    }
    mPosition += len;
    return NOERROR;
}

ECode CharArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = FALSE;
    return NOERROR;
}

ECode CharArrayBuffer::GetOrder(
   /* [out] */ ByteOrder* byteOrder)
{
    return E_NOT_IMPLEMENTED;
}

ECode CharArrayBuffer::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);

    Int32 remaining;
    Remaining(&remaining);
    if (start < 0 || end < start || end > remaining) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    AutoPtr<ICharBuffer> result;
    Duplicate((ICharBuffer**)&result);
    assert(result);
    result->SetLimit(mPosition + end);
    result->SetPosition(mPosition + start);
    if (result->Probe(EIID_ICharSequence)) {
        *csq = (ICharSequence*)result->Probe(EIID_ICharSequence);
    }
    else {
        *csq = NULL;
    }

    return NOERROR;
}

ECode CharArrayBuffer::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    Int32 remaining;
    Remaining(&remaining);
    String result((char*)mBackingArray->GetPayload());
    *str = result.Substring(mOffset + mPosition, remaining);

    return NOERROR;
}
