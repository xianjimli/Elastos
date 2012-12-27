#ifndef __READONLYFLOATARRAYBUFFER_H__
#define __READONLYFLOATARRAYBUFFER_H__

#include "FloatArrayBuffer.h"
#include <elastos/ElRefBase.h>

/**
 * FloatArrayBuffer, ReadWriteFloatArrayBuffer and ReadOnlyFloatArrayBuffer
 * compose the implementation of array based float buffers.
 * <p>
 * ReadOnlyFloatArrayBuffer extends FloatArrayBuffer with all the write methods
 * throwing read only exception.
 * </p>
 * <p>
 * This class is marked final for runtime performance.
 * </p>
 *
 */
class ReadOnlyFloatArrayBuffer
    : public ElRefBase
    , public IFloatBuffer
    , public FloatArrayBuffer
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    static CARAPI_(ReadOnlyFloatArrayBuffer*) Copy(
    	/* [in] */ FloatArrayBuffer* other,
    	/* [in] */ Int32 markOfOther);

    ReadOnlyFloatArrayBuffer(
        /* [in] */ Int32 capacity,
    	/* [in] */ ArrayOf<Float>* backingArray,
    	/* [in] */ Int32 arrayOffset);

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Float>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ IFloatBuffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ IFloatBuffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ IFloatBuffer** buffer);

    CARAPI GetFloat(
        /* [out] */ Float* value);

    CARAPI GetFloatEx(
        /* [in] */ Int32 index,
        /* [out] */ Float* value);

    CARAPI GetFloats(
        /* [out] */ ArrayOf<Float>* dst);

    CARAPI GetFloatsEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Float>* dst);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI PutFloat(
        /* [in] */ Float d);

    CARAPI PutFloatEx(
        /* [in] */ Int32 index,
        /* [in] */ Float d);

    CARAPI PutFloats(
        /* [in] */ const ArrayOf<Float>& src);

    CARAPI PutFloatsEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Float>& src);

    CARAPI PutFloatBuffer(
        /* [in] */ IFloatBuffer* src);

    CARAPI Slice(
        /* [out] */ IFloatBuffer** buffer);

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
        /* [out, callee] */ ArrayOf<Float>** array);

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

#endif // __READONLYFLOATARRAYBUFFER_H__