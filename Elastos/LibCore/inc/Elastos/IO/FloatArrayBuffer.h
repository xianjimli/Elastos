#ifndef __FLOATARRAYBUFFER_H__
#define __FLOATARRAYBUFFER_H__

#include "FloatBuffer.h"

/**
 * FloatArrayBuffer, ReadWriteFloatArrayBuffer and ReadOnlyFloatArrayBuffer
 * compose the implementation of array based float buffers.
 * <p>
 * FloatArrayBuffer implements all the shared readonly methods and is extended
 * by the other two classes.
 * </p>
 * <p>
 * All methods are marked final for runtime performance.
 * </p>
 *
 */
class FloatArrayBuffer : public FloatBuffer
{
public:
    FloatArrayBuffer(
        /* [in] */ ArrayOf<Float>* array);

    FloatArrayBuffer(
        /* [in] */ Int32 capacity);

    FloatArrayBuffer(
        /* [in] */ Int32 capacity,
        /* [in] */ ArrayOf<Float>* backingArray,
        /* [in] */ Int32 offset);

    CARAPI GetFloat(
        /* [out] */ Float* value);

    CARAPI GetFloatEx(
        /* [in] */ Int32 index,
        /* [out] */ Float* value);

    CARAPI GetFloatsEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Float>* dst);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI GetOrder(
       /* [out] */ ByteOrder* byteOrder);

public:
    ArrayOf<Float>* mBackingArray;

    Int32 mOffset;
};

#endif // __FLOATARRAYBUFFER_H__