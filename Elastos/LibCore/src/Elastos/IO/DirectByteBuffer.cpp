
#include "cmdef.h"
#include "DirectByteBuffer.h"

DirectByteBuffer::SafeAddress::SafeAddress()
    : mIsValid(TRUE)
{}

DirectByteBuffer::DirectByteBuffer(
    /* [in] */ Int32 capacity)
    : BaseByteBuffer(capacity)
    , mOffset(0)
{
    // this(new SafeAddress(PlatformAddressFactory.alloc(capacity, (byte) 0)),
    //         capacity, 0);
    // safeAddress.address.autoFree();
}

DirectByteBuffer::DirectByteBuffer(
    /* [in] */ SafeAddress* address,
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 offset)
    : BaseByteBuffer(capacity)
    , mSafeAddress(address)
    , mOffset(offset)
{
    // BEGIN android-added
    // AutoPtr<IPlatformAddress> baseAddress = address->mAddress;
    // Int64 baseSize;
    // baseAddress->GetSize(&baseSize);

//    if ((baseSize >= 0) && ((offset + capacity) > baseSize)) {
//        throw new IllegalArgumentException("slice out of range");
//    }
    // END android-added
}

ECode DirectByteBuffer::GetBytesEx(
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ ArrayOf<Byte>* dst)
{
    VALIDATE_NOT_NULL(dst);

    Int32 length = dst->GetLength();
    if ((off < 0) || (len < 0) || (Int64)off + (Int64)len > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    Int32 remaining;
    Remaining(&remaining);
    if (len > remaining) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    // AutoPtr<IPlatformAddress> addr;
    // GetBaseAddress((IPlatformAddress**)&addr);
    // addr->GetByteArray(offset + position, dst, off, len);
    mPosition += len;

    return NOERROR;
}

ECode DirectByteBuffer::GetByte(
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);

    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }

//    return getBaseAddress().getByte(offset + position++);

    return E_NOT_IMPLEMENTED;
}

ECode DirectByteBuffer::GetByteEx(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);

    if (index < 0 || index >= mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
//    return getBaseAddress().getByte(offset + index);

    return E_NOT_IMPLEMENTED;
}

ECode DirectByteBuffer::GetDouble(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 newPosition = mPosition + 8;
    if (newPosition > mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    // AutoPtr<IPlatformAddress> addr;
    // GetBaseAddress((IPlatformAddress**)&addr);
    // addr->GetDouble(offset + mPosition, order, value);
//    getBaseAddress().getDouble(offset + position, order);
    mPosition = newPosition;

    return E_NOT_IMPLEMENTED;
}

ECode DirectByteBuffer::GetDoubleEx(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);

    if (index < 0 || (Int64) index + 8 > mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    // AutoPtr<IPlatformAddress> addr;
    // GetBaseAddress((IPlatformAddress**)&addr);
    // addr->GetDouble(offset + index, order, value);
//    return getBaseAddress().getDouble(offset + index, order);

    return E_NOT_IMPLEMENTED;
}

ECode DirectByteBuffer::GetFloat(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 newPosition = mPosition + 4;
    if (newPosition > mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    // AutoPtr<IPlatformAddress> addr;
    // GetBaseAddress((IPlatformAddress**)&addr);
    // addr->GetFloat(offset + mPosition, order, value);
//    getBaseAddress().getFloat(offset + position, order);
    mPosition = newPosition;

    return E_NOT_IMPLEMENTED;
}

ECode DirectByteBuffer::GetFloatEx(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    if (index < 0 || (Int64) index + 4 > mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    // AutoPtr<IPlatformAddress> addr;
    // GetBaseAddress((IPlatformAddress**)&addr);
    // addr->GetFloat(offset + index, order, value);
//    return getBaseAddress().getFloat(offset + index, order);

    return E_NOT_IMPLEMENTED;
}

ECode DirectByteBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 newPosition = mPosition + 4;
    if (newPosition > mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    // AutoPtr<IPlatformAddress> addr;
    // GetBaseAddress((IPlatformAddress**)&addr);
    // addr->GetInt32(offset + mPosition, order, value);
//    getBaseAddress().getInt(offset + position, order);
    mPosition = newPosition;

    return E_NOT_IMPLEMENTED;
}

ECode DirectByteBuffer::GetInt32Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    if (index < 0 || (Int64) index + 4 > mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    // AutoPtr<IPlatformAddress> addr;
    // GetBaseAddress((IPlatformAddress**)&addr);
    // addr->GetInt32(offset + index, order, value);
//    return getBaseAddress().getInt(offset + index, order);

    return E_NOT_IMPLEMENTED;
}

ECode DirectByteBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 newPosition = mPosition + 8;
    if (newPosition > mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    // AutoPtr<IPlatformAddress> addr;
    // GetBaseAddress((IPlatformAddress**)&addr);
    // addr->GetInt64(offset + mPosition, order, value);
//    getBaseAddress().getLong(offset + position, order);
    mPosition = newPosition;

    return E_NOT_IMPLEMENTED;
}

ECode DirectByteBuffer::GetInt64Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    if (index < 0 || (Int64) index + 8 > mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    // AutoPtr<IPlatformAddress> addr;
    // GetBaseAddress((IPlatformAddress**)&addr);
    // addr->GetInt64(offset + index, order, value);
//    return getBaseAddress().getLong(offset + index, order);

    return E_NOT_IMPLEMENTED;
}

ECode DirectByteBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);

    Int32 newPosition = mPosition + 2;
    if (newPosition > mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
//        throw new BufferUnderflowException();
    }
    // AutoPtr<IPlatformAddress> addr;
    // GetBaseAddress((IPlatformAddress**)&addr);
    // addr->GetInt16(offset + mPosition, order, value);
//    getBaseAddress().getShort(offset + position, order);
    mPosition = newPosition;

    return E_NOT_IMPLEMENTED;
}

ECode DirectByteBuffer::GetInt16Ex(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);

    if (index < 0 || (Int64) index + 2 > mLimit) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new IndexOutOfBoundsException();
    }
    // AutoPtr<IPlatformAddress> addr;
    // GetBaseAddress((IPlatformAddress**)&addr);
    // addr->GetInt16(offset + index, order, value);
//    return getBaseAddress().getShort(offset + index, order);

    return E_NOT_IMPLEMENTED;
}

ECode DirectByteBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = TRUE;
    return NOERROR;
}

Boolean DirectByteBuffer::IsAddressValid()
{
    return mSafeAddress->mIsValid;
}

ECode DirectByteBuffer::AddressValidityCheck()
{
    if (!IsAddressValid()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalStateException("Cannot use a direct byte buffer after it has been explicitly freed");
    }

    return NOERROR;
}

void DirectByteBuffer::MarkAddressInvalid()
{
//    mSafeAddress->mIsValid = FALSE;
}

// GetBaseAddress(
//      /* [out] */ IPlatformAddress** addr)

// GetEffectiveAddress(
//      /* [out] */ IPlatformAddress** addr)

void DirectByteBuffer::Free()
{
    if (IsAddressValid()) {
        MarkAddressInvalid();
//        mSafeAddress->mAddress->Free();
    }
}

ECode DirectByteBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    VALIDATE_NOT_NULL(array);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

ECode DirectByteBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
//    throw new UnsupportedOperationException();
}

ECode DirectByteBuffer::ProtectedHasArray(
    /* [out] */ Boolean* hasArray)
{
    VALIDATE_NOT_NULL(hasArray);
    *hasArray = FALSE;
    return NOERROR;
}

Int32 DirectByteBuffer::GetByteCapacity()
{
    return mCapacity;
}
