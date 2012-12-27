
#include "cmdef.h"
#include "Buffer.h"

//  {96B660B5-E243-45e1-BC7E-742F665BF00E}
extern "C" const InterfaceID EIID_Buffer =
    { 0x96b660b5, 0xe243, 0x45e1, { 0xbc, 0x7e, 0x74, 0x2f, 0x66, 0x5b, 0xf0, 0xe } };

Buffer::Buffer(
    /* [in] */ Int32 capacity)
    : mMark(Buffer_UNSET_MARK)
    , mPosition(0)
    , mElementSizeShift(0)
    , mEffectiveDirectAddress(0)
{
//    super();
    assert(capacity > 0);
//    if (capacity < 0) {
//        throw new IllegalArgumentException();
//    }
    mCapacity = mLimit = capacity;
}

//    ECode Buffer::Array(
//        /* [out] */ IInterface** array) = 0;

//    ECode Buffer::ArrayOffset(
//        /* [out] */ Int32* offset) = 0;

ECode Buffer::Capacity(
    /* [out] */ Int32* cap)
{
    VALIDATE_NOT_NULL(cap);
    *cap = mCapacity;
    return NOERROR;
}

ECode Buffer::Clear()
{
    mPosition = 0;
    mMark = Buffer_UNSET_MARK;
    mLimit = mCapacity;
    return NOERROR;
}

ECode Buffer::Flip()
{
    mLimit = mPosition;
    mPosition = 0;
    mMark = Buffer_UNSET_MARK;
    return NOERROR;
}

ECode Buffer::HasRemaining(
    /* [out] */ Boolean* hasRemaining)
{
    VALIDATE_NOT_NULL(hasRemaining);
    *hasRemaining = mPosition < mLimit;
    return NOERROR;
}

ECode Buffer::GetLimit(
    /* [out] */ Int32* limit)
{
    VALIDATE_NOT_NULL(limit);
    *limit = mLimit;
    return NOERROR;
}

ECode Buffer::SetLimit(
    /* [in] */ Int32 newLimit)
{
    if (newLimit < 0 || newLimit > mCapacity) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }

    mLimit = newLimit;
    if (mPosition > newLimit) {
        mPosition = newLimit;
    }
    if ((mMark != Buffer_UNSET_MARK) && (mMark > newLimit)) {
        mMark = Buffer_UNSET_MARK;
    }
    return NOERROR;
}

ECode Buffer::Mark()
{
    mMark = mPosition;
    return NOERROR;
}

ECode Buffer::GetPosition(
    /* [out] */ Int32* pos)
{
    VALIDATE_NOT_NULL(pos);
    *pos = mPosition;
    return NOERROR;
}

ECode Buffer::SetPosition(
    /* [in] */ Int32 newPosition)
{
    if (newPosition < 0 || newPosition > mLimit) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }

    mPosition = newPosition;
    if ((mMark != Buffer_UNSET_MARK) && (mMark > mPosition)) {
        mMark = Buffer_UNSET_MARK;
    }
    return NOERROR;
}

ECode Buffer::Remaining(
    /* [out] */ Int32* remaining)
{
    VALIDATE_NOT_NULL(remaining);
    *remaining = mLimit - mPosition;
    return NOERROR;
}

ECode Buffer::Reset()
{
    if (mMark == Buffer_UNSET_MARK) {
        return E_INVALID_MARK_EXCEPTION;
//        throw new InvalidMarkException();
    }
    mPosition = mMark;
    return NOERROR;
}

ECode Buffer::Rewind()
{
    mPosition = 0;
    mMark = Buffer_UNSET_MARK;
    return NOERROR;
}
