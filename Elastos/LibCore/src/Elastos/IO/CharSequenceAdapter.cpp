
#include "cmdef.h"
#include "CharSequenceAdapter.h"

CharSequenceAdapter* CharSequenceAdapter::Copy(
    /* [in] */ CharSequenceAdapter* other)
{
    if (NULL == other) {
        return NULL;
    }

    Int32 len;
    other->mSequence->GetLength(&len);

    CharSequenceAdapter* buf = new CharSequenceAdapter(len, other->mSequence);
    if (NULL == buf) {
        return NULL;
    }

    buf->mLimit     = other->mLimit;
    buf->mPosition  = other->mPosition;
    buf->mMark      = other->mMark;

    return buf;
}

CharSequenceAdapter::CharSequenceAdapter(
    /* [in] */ Int32 capacity,
    /* [in] */ ICharSequence* chseq)
    : CharBuffer(capacity)
    , mSequence(chseq)
{
    assert(NULL != mSequence);
}

PInterface CharSequenceAdapter::Probe(
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


UInt32 CharSequenceAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CharSequenceAdapter::Release()
{
    return ElRefBase::Release();
}

ECode CharSequenceAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CharSequenceAdapter::Array(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    return CharBuffer::Array(array);
}

ECode CharSequenceAdapter::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return CharBuffer::ArrayOffset(offset);
}

ECode CharSequenceAdapter::AsReadOnlyBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    return Duplicate(buffer);
}

ECode CharSequenceAdapter::Compact()
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode CharSequenceAdapter::CompareTo(
    /* [in] */ ICharBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return CharBuffer::CompareTo(otherBuffer, result);
}

ECode CharSequenceAdapter::Duplicate(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    CharSequenceAdapter* buf = Copy(this);
    /*
    if (buf->Probe(EIID_ICharBuffer) != NULL) {
        *buffer = (ICharBuffer*)buf->Probe(EIID_ICharBuffer);
    }
    else {
        *buffer = NULL;
    }
    */
    if (NULL == buf) {
        *buffer = NULL;
        return NOERROR;
    }

    *buffer = (ICharBuffer*)buf;

    return NOERROR;
}

ECode CharSequenceAdapter::GetChar(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);

    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }

    return mSequence->GetCharAt(mPosition++, value);
}

ECode CharSequenceAdapter::GetCharEx(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);

    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    return mSequence->GetCharAt(index, value);
}

ECode CharSequenceAdapter::GetChars(
    /* [out] */ ArrayOf<Char32>* dst)
{
    return CharBuffer::GetChars(dst);
}

ECode CharSequenceAdapter::GetCharsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Char32>* dst)
{
    VALIDATE_NOT_NULL(dst);

    Int32 length = dst->GetLength();
    if ((off < 0) || (len < 0) || (Int64)off + (Int64) len > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    Int32 remaining;
    Remaining(&remaining);
    if (len > remaining) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }

    Int32 newPosition = mPosition + len;
    String s;
    mSequence->ToString(&s);
    for(Int32 i = 0; i < newPosition - mPosition; ++i) {
        (*dst)[i + off] = s[mPosition + i];
    }
//    s.getChars(mPosition, newPosition, dst, off);
    mPosition = newPosition;

    return NOERROR;
}

ECode CharSequenceAdapter::GetOrder(
    /* [out] */ ByteOrder* order)
{
    *order = ByteOrder_LITTLE_ENDIAN;
    return NOERROR;
}

ECode CharSequenceAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

ECode CharSequenceAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

ECode CharSequenceAdapter::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = FALSE;

    return NOERROR;
}

ECode CharSequenceAdapter::PutChar(
    /* [in] */ Char32 c)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode CharSequenceAdapter::PutCharEx(
    /* [in] */ Int32 index,
    /* [in] */ Char32 c)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();
}

ECode CharSequenceAdapter::PutChars(
    /* [in] */ const ArrayOf<Char32>& src)
{
    return CharBuffer::PutChars(src);
}

ECode CharSequenceAdapter::PutCharsEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Char32>& src)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;

    /*
    if ((off < 0) || (len < 0) || (Int64)off + (Int64)len > src.GetLength()) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    Int32 remaining;
    Remaining(&remaining);
    if (len > remaining) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
//        throw new BufferOverflowException();
    }

    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();

    */
}

ECode CharSequenceAdapter::PutCharBuffer(
    /* [in] */ ICharBuffer* src)
{
    return CharBuffer::PutCharBuffer(src);
}

ECode CharSequenceAdapter::PutString(
    /* [in] */ const String& str)
{
    return CharBuffer::PutString(str);
}

ECode CharSequenceAdapter::PutStringEx(
    /* [in] */ const String& str,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return E_READ_ONLY_BUFFER_EXCEPTION;

    /*
    if ((start < 0) || (end < 0)
            || (Int64)start + (Int64) end > str.GetLength()) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    return E_READ_ONLY_BUFFER_EXCEPTION;
//    throw new ReadOnlyBufferException();

    */
}

ECode CharSequenceAdapter::Slice(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    AutoPtr<ICharSequence> seq;
    mSequence->SubSequence(mPosition, mLimit, (ICharSequence**)&seq);
    if (NULL == seq) {
        *buffer = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    Int32 len;
    seq->GetLength(&len);
    CharSequenceAdapter* buf = new CharSequenceAdapter(len, seq);
    if (NULL == buf) {
        *buffer = NULL;
        return E_NULL_POINTER_EXCEPTION;
    }

    /*
    if (buf->Probe(EIID_ICharBuffer) != NULL) {
        *buffer = (ICharBuffer*)buf->Probe(EIID_ICharBuffer);
    }
    else {
        *buffer = NULL;
    }
    */
    *buffer = (ICharBuffer*)buf;

    return NOERROR;
}

ECode CharSequenceAdapter::AppendChar(
    /* [in] */ Char32 c)
{
    return CharBuffer::AppendChar(c);
}

ECode CharSequenceAdapter::AppendChars(
    /* [in] */ ICharSequence* csq)
{
    return CharBuffer::AppendChars(csq);
}

ECode CharSequenceAdapter::AppendCharsEx(
    /* [in] */ ICharSequence* csq,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return CharBuffer::AppendCharsEx(csq, start, end);
}

ECode CharSequenceAdapter::Read(
    /* [in] */ ICharBuffer* target,
    /* [out] */ Int32* number)
{
    return CharBuffer::Read(target, number);
}

ECode CharSequenceAdapter::Capacity(
    /* [out] */ Int32* cap)
{
    return CharBuffer::Capacity(cap);
}

ECode CharSequenceAdapter::Clear()
{
    return CharBuffer::Clear();
}

ECode CharSequenceAdapter::Flip()
{
    return CharBuffer::Flip();
}

ECode CharSequenceAdapter::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return CharBuffer::HasArray(hasArray);
}

ECode CharSequenceAdapter::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return CharBuffer::HasRemaining(hasRemaining);
}

ECode CharSequenceAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = FALSE;
    return NOERROR;
}

ECode CharSequenceAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    VALIDATE_NOT_NULL(isReadOnly);
    *isReadOnly = TRUE;
    return NOERROR;
}

ECode CharSequenceAdapter::GetLimit(
    /* [out] */ Int32* limit)
{
    return CharBuffer::GetLimit(limit);
}

ECode CharSequenceAdapter::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return CharBuffer::SetLimit(newLimit);
}

ECode CharSequenceAdapter::Mark()
{
    return CharBuffer::Mark();
}

ECode CharSequenceAdapter::GetPosition(
    /* [out] */ Int32* pos)
{
    return CharBuffer::GetPosition(pos);
}

ECode CharSequenceAdapter::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return CharBuffer::SetPosition(newPosition);
}

ECode CharSequenceAdapter::Remaining(
    /* [out] */ Int32* remaining)
{
    return CharBuffer::Remaining(remaining);
}

ECode CharSequenceAdapter::Reset()
{
    return CharBuffer::Reset();
}

ECode CharSequenceAdapter::Rewind()
{
    return CharBuffer::Rewind();
}

ECode CharSequenceAdapter::GetLength(
    /* [out] */ Int32* number)
{
    return CharBuffer::GetLength(number);
}

ECode CharSequenceAdapter::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    return CharBuffer::GetCharAt(index, c);
}

ECode CharSequenceAdapter::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);

    Int32 remaining;
    Remaining(&remaining);
    if (end < start || start < 0 || end > remaining) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }

    CharSequenceAdapter* result = Copy(this);
    result->mPosition = mPosition + start;
    result->mLimit = mPosition + end;
    if (result->Probe(EIID_ICharSequence) != NULL) {
        *csq = (ICharSequence*)result->Probe(EIID_ICharBuffer);
    }
    else {
        *csq = NULL;
    }
    return NOERROR;
}

ECode CharSequenceAdapter::ToString(
    /* [out] */ String* str)
{
    return CharBuffer::ToString(str);
}

