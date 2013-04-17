#ifndef __HEAPBYTEBUFFER_H__
#define __HEAPBYTEBUFFER_H__

#include "BaseByteBuffer.h"

/**
 * HeapByteBuffer, ReadWriteHeapByteBuffer and ReadOnlyHeapByteBuffer compose
 * the implementation of array based byte buffers.
 * <p>
 * HeapByteBuffer implements all the shared readonly methods and is extended by
 * the other two classes.
 * </p>
 * <p>
 * All methods are marked final for runtime performance.
 * </p>
 *
 */
class HeapByteBuffer : public BaseByteBuffer
{
protected:
    HeapByteBuffer(
        /* [in] */ Int32 capacity);

    HeapByteBuffer(
        /* [in] */ ArrayOf<Byte>* backingArray);

    HeapByteBuffer(
        /* [in] */ ArrayOf<Byte>* backingArray,
        /* [in] */ Int32 capacity,
        /* [in] */ Int32 offset);

public:
    virtual ~HeapByteBuffer();

    CARAPI GetBytesEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Byte>* dst);

    CARAPI GetByte(
        /* [out] */ Byte* value);

    CARAPI GetByteEx(
        /* [in] */ Int32 index,
        /* [out] */ Byte* value);

    CARAPI GetDouble(
        /* [out] */ Double* value);

    CARAPI GetDoubleEx(
        /* [in] */ Int32 index,
        /* [out] */ Double* value);

    CARAPI GetFloat(
        /* [out] */ Float* value);

    CARAPI GetFloatEx(
        /* [in] */ Int32 index,
        /* [out] */ Float* value);

   	CARAPI GetInt32(
        /* [out] */ Int32* value);

    CARAPI GetInt32Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int32* value);

    CARAPI GetInt64(
        /* [out] */ Int64* value);

    CARAPI GetInt64Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int64* value);

    CARAPI GetInt16(
        /* [out] */ Int16* value);

    CARAPI GetInt16Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int16* value);

   	CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

protected:
	CARAPI_(Int32) LoadInt32(
		/* [in] */ Int32 index);

	CARAPI_(Int64) LoadInt64(
		/* [in] */ Int32 index);

	CARAPI_(Int16) LoadInt16(
		/* [in] */ Int32 index);

	CARAPI_(void) Store(
		/* [in] */ Int32 index,
		/* [in] */ Int32 value);

	CARAPI_(void) StoreEx(
		/* [in] */ Int32 index,
		/* [in] */ Int64 value);

	CARAPI_(void) StoreEx2(
		/* [in] */ Int32 index,
		/* [in] */ Int16 value);

public:
	ArrayOf<Byte>*  mBackingArray;
    Int32           mOffset;

private:
    Boolean         mIsAllocBySelf;
};

#endif // __HEAPBYTEBUFFER_H__
