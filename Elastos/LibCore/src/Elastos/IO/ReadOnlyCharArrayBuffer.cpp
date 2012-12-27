
#include "cmdef.h"
#include "ReadOnlyCharArrayBuffer.h"

PInterface ReadOnlyCharArrayBuffer::Probe(
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


UInt32 ReadOnlyCharArrayBuffer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ReadOnlyCharArrayBuffer::Release()
{
    return ElRefBase::Release();
}

ECode ReadOnlyCharArrayBuffer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ReadOnlyCharArrayBuffer* ReadOnlyCharArrayBuffer::Copy(
    /* [in] */ CharArrayBuffer* other,
    /* [in] */ Int32 markOfOther)
{
    Int32 capacity;
    other->Capacity(&capacity);
    ReadOnlyCharArrayBuffer* buf = new ReadOnlyCharArrayBuffer(capacity,
            other->mBackingArray, other->mOffset);
    other->GetLimit(&buf->mLimit);
    other->GetPosition(&buf->mPosition);
    buf->mMark = markOfOther;
    return buf;
}

ReadOnlyCharArrayBuffer::ReadOnlyCharArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Char32>* backingArray,
    /* [in] */ Int32 arrayOffset)
    : CharArrayBuffer(capacity, backingArray, arrayOffset)
{}

ECode ReadOnlyCharArrayBuffer::Array(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    return CharArrayBuffer::Array(array);
}

ECode ReadOnlyCharArrayBuffer::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return CharArrayBuffer::ArrayOffset(offset);
}

ECode ReadOnlyCharArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    return Duplicate(buffer);
}

ECode ReadOnlyCharArrayBuffer::Compact()
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyCharArrayBuffer::CompareTo(
    /* [in] */ ICharBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return CharArrayBuffer::CompareTo(otherBuffer, result);
}

ECode ReadOnlyCharArrayBuffer::Duplicate(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    ReadOnlyCharArrayBuffer* buf = Copy((CharArrayBuffer*)this, mMark);
    if (buf->Probe(EIID_ICharBuffer) != NULL) {
        *buffer = (ICharBuffer*)buf->Probe(EIID_ICharBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode ReadOnlyCharArrayBuffer::GetChar(
    /* [out] */ Char32* value)
{
    return CharArrayBuffer::GetChar(value);
}

ECode ReadOnlyCharArrayBuffer::GetCharEx(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return CharArrayBuffer::GetCharEx(index, value);
}

ECode ReadOnlyCharArrayBuffer::GetChars(
    /* [out] */ ArrayOf<Char32>* dst)
{
    return CharArrayBuffer::GetChars(dst);
}

ECode ReadOnlyCharArrayBuffer::GetCharsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Char32>* dst)
{
    return CharArrayBuffer::GetCharsEx(off, len, dst);
}

ECode ReadOnlyCharArrayBuffer::GetOrder(
    /* [out] */ ByteOrder* order)
{
    return CharArrayBuffer::GetOrder(order);
}

CARAPI ReadOnlyCharArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

CARAPI ReadOnlyCharArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

CARAPI ReadOnlyCharArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode ReadOnlyCharArrayBuffer::PutChar(
    /* [in] */ Char32 c)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyCharArrayBuffer::PutCharEx(
    /* [in] */ Int32 index,
    /* [in] */ Char32 c)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyCharArrayBuffer::PutChars(
    /* [in] */ const ArrayOf<Char32>& src)
{
    return CharArrayBuffer::PutChars(src);
}

ECode ReadOnlyCharArrayBuffer::PutCharsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Char32>& src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyCharArrayBuffer::PutCharBuffer(
    /* [in] */ ICharBuffer* src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode ReadOnlyCharArrayBuffer::PutString(
    /* [in] */ const String& str)
{
    return CharArrayBuffer::PutString(str);
}

ECode ReadOnlyCharArrayBuffer::PutStringEx(
    /* [in] */ const String& str,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return CharArrayBuffer::PutStringEx(str, start, end);
}

ECode ReadOnlyCharArrayBuffer::Slice(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);
    Int32 remaining;
    Remaining(&remaining);
    ReadOnlyCharArrayBuffer* buf = new ReadOnlyCharArrayBuffer(remaining, mBackingArray,
            mOffset + mPosition);
    if (buf->Probe(EIID_ICharBuffer) != NULL) {
        *buffer = (ICharBuffer*)buf->Probe(EIID_ICharBuffer);
    }
    else {
        *buffer = NULL;
    }
    return NOERROR;
}

ECode ReadOnlyCharArrayBuffer::AppendChar(
    /* [in] */ Char32 c)
{
    return CharArrayBuffer::AppendChar(c);
}

ECode ReadOnlyCharArrayBuffer::AppendChars(
    /* [in] */ ICharSequence* csq)
{
    return CharArrayBuffer::AppendChars(csq);
}

ECode ReadOnlyCharArrayBuffer::AppendCharsEx(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return CharArrayBuffer::AppendCharsEx(csq, start, end);
}

ECode ReadOnlyCharArrayBuffer::Read(
    /* [in] */ ICharBuffer* target,
    /* [out] */ Int32* number)
{
    return CharArrayBuffer::Read(target, number);
}

ECode ReadOnlyCharArrayBuffer::Capacity(
    /* [out] */ Int32* cap)
{
    return CharArrayBuffer::Capacity(cap);
}

ECode ReadOnlyCharArrayBuffer::Clear()
{
    return CharArrayBuffer::Clear();
}

ECode ReadOnlyCharArrayBuffer::Flip()
{
    return CharArrayBuffer::Flip();
}

ECode ReadOnlyCharArrayBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return CharArrayBuffer::HasArray(hasArray);
}

ECode ReadOnlyCharArrayBuffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return CharArrayBuffer::HasRemaining(hasRemaining);
}

ECode ReadOnlyCharArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    return CharArrayBuffer::IsDirect(isDirect);
}

ECode ReadOnlyCharArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

ECode ReadOnlyCharArrayBuffer::GetLimit(
    /* [out] */ Int32* limit)
{
    return CharArrayBuffer::GetLimit(limit);
}

ECode ReadOnlyCharArrayBuffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return CharArrayBuffer::SetLimit(newLimit);
}

ECode ReadOnlyCharArrayBuffer::Mark()
{
    return CharArrayBuffer::Mark();
}

ECode ReadOnlyCharArrayBuffer::GetPosition(
    /* [out] */ Int32* pos)
{
    return CharArrayBuffer::GetPosition(pos);
}

ECode ReadOnlyCharArrayBuffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return CharArrayBuffer::SetPosition(newPosition);
}

ECode ReadOnlyCharArrayBuffer::Remaining(
    /* [out] */ Int32* remaining)
{
    return CharArrayBuffer::Remaining(remaining);
}

ECode ReadOnlyCharArrayBuffer::Reset()
{
    return CharArrayBuffer::Reset();
}

ECode ReadOnlyCharArrayBuffer::Rewind()
{
    return CharArrayBuffer::Rewind();
}

ECode ReadOnlyCharArrayBuffer::GetLength(
    /* [out] */ Int32* number)
{
    return CharArrayBuffer::GetLength(number);
}

ECode ReadOnlyCharArrayBuffer::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    return CharArrayBuffer::GetCharAt(index, c);
}

ECode ReadOnlyCharArrayBuffer::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    return CharArrayBuffer::SubSequence(start, end, csq);
}

ECode ReadOnlyCharArrayBuffer::ToString(
    /* [out] */ String* str)
{
    return CharArrayBuffer::ToString(str);
}

