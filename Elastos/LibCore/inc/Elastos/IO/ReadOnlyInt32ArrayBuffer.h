#ifndef __READONLYINT32ARRAYBUFFER_H__
#define __READONLYINT32ARRAYBUFFER_H__

#include "Int32ArrayBuffer.h"
#include <elastos/ElRefBase.h>

/**
 * IntArrayBuffer, ReadWriteIntArrayBuffer and ReadOnlyIntArrayBuffer compose
 * the implementation of array based int buffers.
 * <p>
 * ReadOnlyIntArrayBuffer extends IntArrayBuffer with all the write methods
 * throwing read only exception.
 * </p>
 * <p>
 * This class is marked final for runtime performance.
 * </p>
 *
 */
class ReadOnlyInt32ArrayBuffer
    : public ElRefBase
    , public IInt32Buffer
    , public Int32ArrayBuffer
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    static CARAPI_(ReadOnlyInt32ArrayBuffer*) Copy(
    	/* [in] */ Int32ArrayBuffer* other,
    	/* [in] */ Int32 markOfOther);

    ReadOnlyInt32ArrayBuffer(
        /* [in] */ Int32 capacity,
    	/* [in] */ ArrayOf<Int32>* backingArray,
    	/* [in] */ Int32 arrayOffset);

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Int32>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ IInt32Buffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ IInt32Buffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ IInt32Buffer** buffer);

    CARAPI GetInt32(
        /* [out] */ Int32* value);

    CARAPI GetInt32Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int32* value);

    CARAPI GetInt32s(
        /* [out] */ ArrayOf<Int32>* dst);

    CARAPI GetInt32sEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Int32>* dst);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI PutInt32(
        /* [in] */ Int32 d);

    CARAPI PutInt32Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int32 d);

    CARAPI PutInt32s(
        /* [in] */ const ArrayOf<Int32>& src);

    CARAPI PutInt32sEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Int32>& src);

    CARAPI PutInt32Buffer(
        /* [in] */ IInt32Buffer* src);

    CARAPI Slice(
        /* [out] */ IInt32Buffer** buffer);

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

    CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Int32>** array);

    CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI ProtectedHasArray(
        /* [out] */ Boolean* result);

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
};

#endif // __READONLYINT32ARRAYBUFFER_H__