#ifndef __READWRITEHEAPBYTEBUFFER_H__
#define __READWRITEHEAPBYTEBUFFER_H__

#include "HeapByteBuffer.h"
#include <elastos/ElRefBase.h>

/**
 * HeapByteReadWriteHeapByteBuffer, ReadWriteHeapByteReadWriteHeapByteBuffer and ReadOnlyHeapByteReadWriteHeapByteBuffer compose
 * the implementation of array bCARAPI Ased byte ReadWriteHeapByteBuffers.
 * <p>
 * ReadWriteHeapByteReadWriteHeapByteBuffer extends HeapByteReadWriteHeapByteBuffer with all the write methods.
 * </p>
 * <p>
 * This clCARAPI Ass is marked final for runtime performance.
 * </p>
 *
 */
class ReadWriteHeapByteBuffer
    : public ElRefBase
    , public IByteBuffer
    , public HeapByteBuffer
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    static CARAPI_(ReadWriteHeapByteBuffer*) Copy(
    	/* [in] */ HeapByteBuffer* other,
    	/* [in] */ Int32 markOfOther);

    ReadWriteHeapByteBuffer(
    	/* [in] */ ArrayOf<Byte>* backingArray);

    ReadWriteHeapByteBuffer(
    	/* [in] */ Int32 capacity);

    ReadWriteHeapByteBuffer(
    	/* [in] */ ArrayOf<Byte>* backingArray,
    	/* [in] */ Int32 capacity,
    	/* [in] */ Int32 arrayOffset);

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Byte>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsCharBuffer(
        /* [out] */ ICharBuffer** buffer);

    CARAPI AsDoubleBuffer(
        /* [out] */ IDoubleBuffer** buffer);

    CARAPI AsFloatBuffer(
        /* [out] */ IFloatBuffer** buffer);

    CARAPI AsInt16Buffer(
        /* [out] */ IInt16Buffer** buffer);

    CARAPI AsInt32Buffer(
        /* [out] */ IInt32Buffer** buffer);

    CARAPI AsInt64Buffer(
        /* [out] */ IInt64Buffer** buffer);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ IByteBuffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ IByteBuffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ IByteBuffer** buffer);

    CARAPI GetByte(
        /* [out] */ Byte* value);

    CARAPI GetByteEx(
        /* [in] */ Int32 index,
        /* [out] */ Byte* value);

    CARAPI GetBytes(
        /* [out] */ ArrayOf<Byte>* dst);

    CARAPI GetBytesEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Byte>* dst);

    CARAPI GetChar(
        /* [out] */ Char32* value);

    CARAPI GetCharEx(
        /* [in] */ Int32 index,
        /* [out] */ Char32* value);

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

    CARAPI GetOrder(
       /* [out] */ ByteOrder* byteOrder);

    CARAPI SetOrder(
        /* [in] */ ByteOrder byteOrder);

    CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Byte>** array);

    CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI ProtectedHasArray(
        /* [out] */ Boolean* hasArray);

    CARAPI PutByte(
        /* [in] */ Byte b);

    CARAPI PutByteEx(
        /* [in] */ Int32 index,
        /* [in] */ Byte b);

    CARAPI PutBytes(
        /* [in] */ const ArrayOf<Byte>& src);

    CARAPI PutBytesEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Byte>& src);

    CARAPI PutByteBuffer(
        /* [in] */ IByteBuffer* src);

    CARAPI PutChar(
        /* [in] */ Char32 value);

    CARAPI PutCharEx(
        /* [in] */ Int32 index,
        /* [in] */ Char32 value);

    CARAPI PutDouble(
        /* [in] */ Double value);

    CARAPI PutDoubleEx(
        /* [in] */ Int32 index,
        /* [in] */ Double value);

    CARAPI PutFloat(
        /* [in] */ Float value);

    CARAPI PutFloatEx(
        /* [in] */ Int32 index,
        /* [in] */ Float value);

    CARAPI PutInt16(
        /* [in] */ Int16 value);

    CARAPI PutInt16Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int16 value);

    CARAPI PutInt32(
        /* [in] */ Int32 value);

    CARAPI PutInt32Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int32 value);

    CARAPI PutInt64(
        /* [in] */ Int64 value);

    CARAPI PutInt64Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value);

    CARAPI Slice(
       /* [out] */ IByteBuffer** buffer);

    CARAPI Capacity(
        /* [out] */ Int32* cap);

    CARAPI Clear();

    CARAPI Flip();

    CARAPI HasArray(
        /* [out] */ Boolean* hasArray);

    CARAPI HasRemaining(
        /* [out] */ Boolean* hasRemaining);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI IsReadOnly(
        /* [out] */ Boolean* isReadOnly);

    CARAPI GetLimit(
        /* [out] */ Int32* limit);

    CARAPI SetLimit(
        /* [in] */ Int32 newLimit);

    CARAPI Mark();

    CARAPI GetPosition(
        /* [out] */ Int32* pos);

    CARAPI SetPosition(
        /* [in] */ Int32 newPosition);

    CARAPI Remaining(
        /* [out] */ Int32* remaining);

    CARAPI Reset();

    CARAPI Rewind();

    virtual CARAPI Equals(
        /* [in]  */ IInterface* other,
        /* [out] */ Boolean* isEquals);
};

#endif // __READWRITEHEAPBYTEBUFFER_H__
