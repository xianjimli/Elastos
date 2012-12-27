
#include "cmdef.h"
#include "ReadWriteCharArrayBuffer.h"
#include "ReadOnlyCharArrayBuffer.h"

PInterface ReadWriteCharArrayBuffer::Probe(
        /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(ICharBuffer*)this;
    }
    else if (riid == EIID_ICharBuffer) {
        return (ICharBuffer*)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_ICharSequence) {
        return (ICharSequence*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_CharBuffer) {
        return reinterpret_cast<PInterface>((CharBuffer*)this);
    }

    return NULL;
}


UInt32 ReadWriteCharArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadWriteCharArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadWriteCharArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ReadWriteCharArrayBuffer* ReadWriteCharArrayBuffer::Copy(
    /* [in] */ CharArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 capacity;
    other->Capacity(&capacity);
    ReadWriteCharArrayBuffer* buf = new ReadWriteCharArrayBuffer(capacity,
            other->mBackingArray, other->mOffset);
    other->GetLimit(&buf->mLimit);
    other->GetPosition(&buf->mPosition);
    buf->mMark = markOfOther;
    return buf;
}

ReadWriteCharArrayBuffer::ReadWriteCharArrayBuffer(
    /* [in] */ ArrayOf<Char32>* array)
    : CharArrayBuffer(array)
{}

ReadWriteCharArrayBuffer::ReadWriteCharArrayBuffer(
    /* [in] */ Int32 capacity)
    : CharArrayBuffer(capacity)
{}

ReadWriteCharArrayBuffer::ReadWriteCharArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Char32>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : CharArrayBuffer(capacity, backingArray, arrayOffset)
{}

ECode ReadWriteCharArrayBuffer::Array(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    return CharArrayBuffer::Array(array);
}

ECode ReadWriteCharArrayBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return CharArrayBuffer::ArrayOffset(offset);
}

ECode ReadWriteCharArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyCharArrayBuffer* buf = ReadOnlyCharArrayBuffer::Copy(
            (CharArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_ICharBuffer) != NULL) {
        *buffer = (ICharBuffer*)buf->Probe(EIID_ICharBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::Compact()
{
//    System.arraycopy(backingArray, position + offset, backingArray, offset,
//            remaining());
    Int32 remaining;
    Remaining(&remaining);
    for(Int32 i = 0; i < remaining; ++i) {
        (*mBackingArray)[i + mOffset] = (*mBackingArray)[mOffset + mPosition + i];
    }
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = Buffer_UNSET_MARK;
    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::CompareTo(
    /* [in] */ ICharBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return CharArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadWriteCharArrayBuffer::Duplicate(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadWriteCharArrayBuffer* buf = Copy((CharArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_ICharBuffer) != NULL) {
        *buffer = (ICharBuffer*)buf->Probe(EIID_ICharBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::GetChar(
    /* [out] */ Char32* value)
{
    return CharArrayBuffer::GetChar(value);
}

ECode ReadWriteCharArrayBuffer::GetCharEx(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return CharArrayBuffer::GetCharEx(index, value);
}

ECode ReadWriteCharArrayBuffer::GetChars(
    /* [out] */ ArrayOf<Char32>* dst)
{
    return CharArrayBuffer::GetChars(dst);
}

ECode ReadWriteCharArrayBuffer::GetCharsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Char32>* dst)
{
    return CharArrayBuffer::GetCharsEx(off, len, dst);
}

ECode ReadWriteCharArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return CharArrayBuffer::GetOrder(order);
}

CARAPI ReadWriteCharArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = mBackingArray;
    return NOERROR;
}

CARAPI ReadWriteCharArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = mOffset;
    return NOERROR;
}

CARAPI ReadWriteCharArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::PutChar(
    /* [in] */ Char32 c)
{
    if (mPosition == mLimit) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
    (*mBackingArray)[mOffset + mPosition++] = c;
    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::PutCharEx(
    /* [in] */ Int32 index,
    /* [in] */ Char32 c)
{
    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    (*mBackingArray)[mOffset + index] = c;
    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::PutChars(
    /* [in] */ const ArrayOf<Char32>& src)
{
    return CharArrayBuffer::PutChars(src);
}

ECode ReadWriteCharArrayBuffer::PutCharsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Char32>& src)
{
    Int32 length = src.GetLength();
    if (off < 0 || len < 0 || (Int64)len + (Int64)off > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    Int32 remaining;
    Remaining(&remaining);
    if (len > remaining) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }
//    System.arraycopy(src, off, backingArray, offset + position, len);
    for(Int32 i = 0; i < len; ++i) {
        (*mBackingArray)[i + mOffset + mPosition] = src[off + i];
    }
    mPosition += len;
    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::PutCharBuffer(
    /* [in] */ ICharBuffer* src)
{
    return CharArrayBuffer::PutCharBuffer(src);
}

ECode ReadWriteCharArrayBuffer::PutString(
    /* [in] */ const String& str)
{
    return CharArrayBuffer::PutString(str);
}

ECode ReadWriteCharArrayBuffer::PutStringEx(
    /* [in] */ const String& str,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return CharArrayBuffer::PutStringEx(str, start, end);
}

ECode ReadWriteCharArrayBuffer::Slice(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining;
    Remaining(&remaining);
    ReadWriteCharArrayBuffer* buf = new ReadWriteCharArrayBuffer(remaining, mBackingArray,
            mOffset + mPosition);
    if (buf->Probe(EIID_ICharBuffer) != NULL) {
        *buffer = (ICharBuffer*)buf->Probe(EIID_ICharBuffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::AppendChar(
    /* [in] */ Char32 c)
{
    return CharArrayBuffer::AppendChar(c);
}

ECode ReadWriteCharArrayBuffer::AppendChars(
    /* [in] */ ICharSequence* csq)
{
    return CharArrayBuffer::AppendChars(csq);
}

ECode ReadWriteCharArrayBuffer::AppendCharsEx(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return CharArrayBuffer::AppendCharsEx(csq, start, end);
}

ECode ReadWriteCharArrayBuffer::Read(
    /* [in] */ ICharBuffer* target,
    /* [out] */ Int32* number)
{
    return CharArrayBuffer::Read(target, number);
}

ECode ReadWriteCharArrayBuffer::Capacity(
    /* [out] */ Int32* cap)
{
    return CharArrayBuffer::Capacity(cap);
}

ECode ReadWriteCharArrayBuffer::Clear()
{
    return CharArrayBuffer::Clear();
}

ECode ReadWriteCharArrayBuffer::Flip()
{
    return CharArrayBuffer::Flip();
}

ECode ReadWriteCharArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return CharArrayBuffer::HasArray(hasArray);
}

ECode ReadWriteCharArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return CharArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadWriteCharArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return CharArrayBuffer::IsDirect(isDirect);
}

ECode ReadWriteCharArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = FALSE;
    return NOERROR;
}

ECode ReadWriteCharArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return CharArrayBuffer::GetLimit(limit);
}

ECode ReadWriteCharArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return CharArrayBuffer::SetLimit(newLimit);
}

ECode ReadWriteCharArrayBuffer::Mark()
{
    return CharArrayBuffer::Mark();
}

ECode ReadWriteCharArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return CharArrayBuffer::GetPosition(pos);
}

ECode ReadWriteCharArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return CharArrayBuffer::SetPosition(newPosition);
}

ECode ReadWriteCharArrayBuffer::Remaining(
    /* [out] */ Int32* remaining)
{
    return CharArrayBuffer::Remaining(remaining);
}

ECode ReadWriteCharArrayBuffer::Reset()
{
    return CharArrayBuffer::Reset();
}

ECode ReadWriteCharArrayBuffer::Rewind()
{
    return CharArrayBuffer::Rewind();
}

ECode ReadWriteCharArrayBuffer::GetLength(
    /* [out] */ Int32* number)
{
    return CharArrayBuffer::GetLength(number);
}

ECode ReadWriteCharArrayBuffer::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    return CharArrayBuffer::GetCharAt(index, c);
}

ECode ReadWriteCharArrayBuffer::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    return CharArrayBuffer::SubSequence(start, end, csq);
}

ECode ReadWriteCharArrayBuffer::ToString(
    /* [out] */ String* str)
{
    return CharArrayBuffer::ToString(str);
}

