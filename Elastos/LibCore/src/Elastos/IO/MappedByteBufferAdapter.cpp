
#include "cmdef.h"
#include "MappedByteBufferAdapter.h"

const Int32 MappedByteBufferAdapter::CHAR_SIZE;

const Int32 MappedByteBufferAdapter::INT16_SIZE;

const Int32 MappedByteBufferAdapter::INT32_SIZE;

const Int32 MappedByteBufferAdapter::INT64_SIZE;

const Int32 MappedByteBufferAdapter::FLOAT_SIZE;

const Int32 MappedByteBufferAdapter::DOUBLE_SIZE;

PInterface MappedByteBufferAdapter::Probe(
        /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IBuffer) {
        return (IBuffer*)this;
    }
    else if (riid == EIID_IByteBuffer) {
        return (IByteBuffer*)this;
    }
    else if (riid == EIID_IMappedByteBuffer) {
        return (IMappedByteBuffer*)this;
    }
    else if (riid == EIID_Buffer) {
        return reinterpret_cast<PInterface>((Buffer*)this);
    }
    else if (riid == EIID_ByteBuffer) {
        return reinterpret_cast<PInterface>((ByteBuffer*)this);
    }

    return NULL;
}


UInt32 MappedByteBufferAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MappedByteBufferAdapter::Release()
{
    return ElRefBase::Release();
}

ECode MappedByteBufferAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

MappedByteBufferAdapter::MappedByteBufferAdapter(
    /* [in] */ IByteBuffer* buffer)
    : MappedByteBuffer(buffer)
{}

MappedByteBufferAdapter::MappedByteBufferAdapter(
    /* [in]  IPlatformAddress* address,*/
    /* [in] */ Int32 capa,
    /* [in] */ Int32 offset,
    /* [in] */ FileChannelMapMode mode)
    : MappedByteBuffer(capa, offset, mode)
{}

ECode MappedByteBufferAdapter::IsLoaded(
    /* [out] */ Boolean* isLoaded)
{
    return MappedByteBuffer::IsLoaded(isLoaded);
}

ECode MappedByteBufferAdapter::Load()
{
    return MappedByteBuffer::Load();
}

ECode MappedByteBufferAdapter::Force()
{
    return MappedByteBuffer::Force();
}

ECode MappedByteBufferAdapter::Array(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return MappedByteBuffer::Array(array);
}

ECode MappedByteBufferAdapter::ArrayOffset(
    /* [out] */ Int32* offset)
{
    return MappedByteBuffer::ArrayOffset(offset);
}

ECode MappedByteBufferAdapter::AsCharBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    return mWrapped->AsCharBuffer(buffer);
}

ECode MappedByteBufferAdapter::AsDoubleBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    return mWrapped->AsDoubleBuffer(buffer);
}

ECode MappedByteBufferAdapter::AsFloatBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    return mWrapped->AsFloatBuffer(buffer);
}

ECode MappedByteBufferAdapter::AsInt16Buffer(
    /* [out] */ IInt16Buffer** buffer)
{
    return mWrapped->AsInt16Buffer(buffer);
}

ECode MappedByteBufferAdapter::AsInt32Buffer(
    /* [out] */ IInt32Buffer** buffer)
{
    return mWrapped->AsInt32Buffer(buffer);
}

ECode MappedByteBufferAdapter::AsInt64Buffer(
    /* [out] */ IInt64Buffer** buffer)
{
    return mWrapped->AsInt64Buffer(buffer);
}

ECode MappedByteBufferAdapter::AsReadOnlyBuffer(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    AutoPtr<IByteBuffer> byteBuf;
    mWrapped->AsReadOnlyBuffer((IByteBuffer**)&byteBuf);
    MappedByteBufferAdapter* buf = new MappedByteBufferAdapter(byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    if (buf->Probe(EIID_IByteBuffer) != NULL) {
        *buffer = (IByteBuffer*)buf->Probe(EIID_IByteBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode MappedByteBufferAdapter::Compact()
{
    Boolean isReadOnly;
    mWrapped->IsReadOnly(&isReadOnly);
    if (isReadOnly) {
        return E_READ_ONLY_BUFFER_EXCEPTION;
//        throw new ReadOnlyBufferException();
    }
    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    mWrapped->Compact();
    mWrapped->Clear();
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = Buffer_UNSET_MARK;

    return NOERROR;
}

ECode MappedByteBufferAdapter::CompareTo(
    /* [in] */ IByteBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    return MappedByteBuffer::CompareTo(otherBuffer, result);
}

ECode MappedByteBufferAdapter::Duplicate(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    AutoPtr<IByteBuffer> byteBuf;
    mWrapped->AsReadOnlyBuffer((IByteBuffer**)&byteBuf);
    MappedByteBufferAdapter* buf = new MappedByteBufferAdapter(byteBuf);
    buf->mLimit = mLimit;
    buf->mPosition = mPosition;
    buf->mMark = mMark;
    if (buf->Probe(EIID_IByteBuffer) != NULL) {
        *buffer = (IByteBuffer*)buf->Probe(EIID_IByteBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode MappedByteBufferAdapter::GetByte(
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);

    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    mWrapped->GetByte(value);
    mPosition++;
    return NOERROR;
}

ECode MappedByteBufferAdapter::GetByteEx(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);

    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    return mWrapped->GetByteEx(index, value);
}

ECode MappedByteBufferAdapter::GetBytes(
    /* [out] */ ArrayOf<Byte>* dst)
{
    return MappedByteBuffer::GetBytes(dst);
}

ECode MappedByteBufferAdapter::GetBytesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Byte>* dst)
{
    return MappedByteBuffer::GetBytesEx(off, len, dst);
}

ECode MappedByteBufferAdapter::GetChar(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);

    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    mWrapped->GetChar(value);
    mPosition += CHAR_SIZE;

    return NOERROR;
}

ECode MappedByteBufferAdapter::GetCharEx(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value);

    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    return mWrapped->GetCharEx(index, value);
}

ECode MappedByteBufferAdapter::GetDouble(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);

    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    mWrapped->GetDouble(value);
    mPosition += DOUBLE_SIZE;

    return NOERROR;
}

ECode MappedByteBufferAdapter::GetDoubleEx(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);

    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    return mWrapped->GetDoubleEx(index, value);
}

//    PlatformAddress getEffectiveAddress();

ECode MappedByteBufferAdapter::GetFloat(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    mWrapped->GetFloat(value);
    mPosition += FLOAT_SIZE;

    return NOERROR;
}

ECode MappedByteBufferAdapter::GetFloatEx(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    return mWrapped->GetFloatEx(index, value);
}

ECode MappedByteBufferAdapter::GetInt32(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    mWrapped->GetInt32(value);
    mPosition += INT32_SIZE;

    return NOERROR;
}

ECode MappedByteBufferAdapter::GetInt32Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    return mWrapped->GetInt32Ex(index, value);
}

ECode MappedByteBufferAdapter::GetInt64(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    mWrapped->GetInt64(value);
    mPosition += INT64_SIZE;

    return NOERROR;
}

ECode MappedByteBufferAdapter::GetInt64Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    return mWrapped->GetInt64Ex(index, value);
}

ECode MappedByteBufferAdapter::GetInt16(
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);

    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    mWrapped->GetInt16(value);
    mPosition += INT16_SIZE;

    return NOERROR;
}

ECode MappedByteBufferAdapter::GetInt16Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);

    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    return mWrapped->GetInt16Ex(index, value);
}

ECode MappedByteBufferAdapter::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    return MappedByteBuffer::GetOrder(byteOrder);
}

ECode MappedByteBufferAdapter::SetOrder(
    /* [in] */ ByteOrder byteOrder)
{
    return MappedByteBuffer::SetOrder(byteOrder);
}

ECode MappedByteBufferAdapter::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = TRUE;
    return NOERROR;
}

ECode MappedByteBufferAdapter::IsReadOnly(
    /* [out] */ Boolean* isReadOnly)
{
    return mWrapped->IsReadOnly(isReadOnly);
}

ECode MappedByteBufferAdapter::SetOrderImpl(
    /* [in] */ ByteOrder byteOrder)
{
    MappedByteBuffer::SetOrderImpl(byteOrder);
    return mWrapped->SetOrder(byteOrder);
}

ECode MappedByteBufferAdapter::PutByte(
    /* [in] */ Byte b)
{
    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    mWrapped->PutByte(b);
    mPosition++;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutByteEx(
    /* [in] */ Int32 index,
    /* [in] */ Byte b)
{
    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    return mWrapped->PutByteEx(index, b);
}

ECode MappedByteBufferAdapter::PutBytes(
    /* [in] */ const ArrayOf<Byte>& src)
{
    return MappedByteBuffer::PutBytes(src);
}

ECode MappedByteBufferAdapter::PutBytesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [in] */ const ArrayOf<Byte>& src)
{
    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    mWrapped->PutBytesEx(off, len, src);
    mPosition += len;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutByteBuffer(
    /* [in] */ IByteBuffer* src)
{
    return MappedByteBuffer::PutByteBuffer(src);
}

ECode MappedByteBufferAdapter::PutChar(
    /* [in] */ Char32 value)
{
    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    mWrapped->PutChar(value);
    mPosition += CHAR_SIZE;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutCharEx(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    return mWrapped->PutCharEx(index, value);
}

ECode MappedByteBufferAdapter::PutDouble(
    /* [in] */ Double value)
{
    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    mWrapped->PutDouble(value);
    mPosition += DOUBLE_SIZE;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutDoubleEx(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    return mWrapped->PutDoubleEx(index, value);
}

ECode MappedByteBufferAdapter::PutFloat(
    /* [in] */ Float value)
{
    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    mWrapped->PutFloat(value);
    mPosition += FLOAT_SIZE;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutFloatEx(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    return mWrapped->PutFloatEx(index, value);
}

ECode MappedByteBufferAdapter::PutInt32(
    /* [in] */ Int32 value)
{
    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    mWrapped->PutInt32(value);
    mPosition += INT32_SIZE;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutInt32Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    return mWrapped->PutInt32Ex(index, value);
}

ECode MappedByteBufferAdapter::PutInt64(
    /* [in] */ Int64 value)
{
    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    mWrapped->PutInt64(value);
    mPosition += INT64_SIZE;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutInt64Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    return mWrapped->PutInt64Ex(index, value);
}

ECode MappedByteBufferAdapter::PutInt16(
    /* [in] */ Int16 value)
{
    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    mWrapped->PutInt16(value);
    mPosition += INT16_SIZE;
    return NOERROR;
}

ECode MappedByteBufferAdapter::PutInt16Ex(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    return mWrapped->PutInt16Ex(index, value);
}

ECode MappedByteBufferAdapter::Slice(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer);

    mWrapped->SetLimit(mLimit);
    mWrapped->SetPosition(mPosition);
    AutoPtr<IByteBuffer> result;
    mWrapped->Slice((IByteBuffer**)&result);
    MappedByteBufferAdapter* buf = new MappedByteBufferAdapter(result);
    mWrapped->Clear();
    if (buf->Probe(EIID_IByteBuffer) != NULL) {
        *buffer = (IByteBuffer*)buf->Probe(EIID_IByteBuffer);
    }
    else {
        *buffer = NULL;
    }

    return NOERROR;
}

ECode MappedByteBufferAdapter::ProtectedArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    return mWrapped->ProtectedArray(array);
}

ECode MappedByteBufferAdapter::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    return mWrapped->ProtectedArrayOffset(offset);
}

ECode MappedByteBufferAdapter::ProtectedHasArray(
    /* [out] */ Boolean* hasArray)
{
    return mWrapped->ProtectedHasArray(hasArray);
}

//    PlatformAddress getBaseAddress();

Boolean MappedByteBufferAdapter::IsAddressValid()
{
    return mWrapped->IsAddressValid();
}

ECode MappedByteBufferAdapter::AddressValidityCheck()
{
    return mWrapped->AddressValidityCheck();
}

void MappedByteBufferAdapter::Free()
{
    mWrapped->Free();
}

Int32 MappedByteBufferAdapter::GetByteCapacity()
{
    return mWrapped->GetByteCapacity();
}

ECode MappedByteBufferAdapter::Capacity(
    /* [out] */ Int32* cap)
{
    return MappedByteBuffer::Capacity(cap);
}

ECode MappedByteBufferAdapter::Clear()
{
    return MappedByteBuffer::Clear();
}

ECode MappedByteBufferAdapter::Flip()
{
    return MappedByteBuffer::Flip();
}

ECode MappedByteBufferAdapter::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return MappedByteBuffer::HasArray(hasArray);
}

ECode MappedByteBufferAdapter::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    return MappedByteBuffer::HasRemaining(hasRemaining);
}

ECode MappedByteBufferAdapter::GetLimit(
    /* [out] */ Int32* limit)
{
    return MappedByteBuffer::GetLimit(limit);
}

ECode MappedByteBufferAdapter::SetLimit(
    /* [in] */ Int32 newLimit)
{
    return MappedByteBuffer::SetLimit(newLimit);
}

ECode MappedByteBufferAdapter::Mark()
{
    return MappedByteBuffer::Mark();
}

ECode MappedByteBufferAdapter::GetPosition(
    /* [out] */ Int32* pos)
{
    return MappedByteBuffer::GetPosition(pos);
}

ECode MappedByteBufferAdapter::SetPosition(
    /* [in] */ Int32 newPosition)
{
    return MappedByteBuffer::SetPosition(newPosition);
}

ECode MappedByteBufferAdapter::Remaining(
    /* [out] */ Int32* remaining)
{
    return MappedByteBuffer::Remaining(remaining);
}

ECode MappedByteBufferAdapter::Reset()
{
    return MappedByteBuffer::Reset();
}

ECode MappedByteBufferAdapter::Rewind()
{
    return MappedByteBuffer::Rewind();
}


