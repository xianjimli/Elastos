#ifndef __DOUBLEARRAYBUFFER_H__
#define __DOUBLEARRAYBUFFER_H__

#include "DoubleBuffer.h"

/**
 * DoubleArrayBuffer, ReadWriteDoubleArrayBuffer and ReadOnlyDoubleArrayBuffer
 * compose the implementation of array based double buffers.
 * <p>
 * DoubleArrayBuffer implements all the shared readonly methods and is extended
 * by the other two classes.
 * </p>
 * <p>
 * All methods are marked final for runtime performance.
 * </p>
 *
 */
class DoubleArrayBuffer : public DoubleBuffer
{
public:
    DoubleArrayBuffer(
        /* [in] */ ArrayOf<Double>* array);

    DoubleArrayBuffer(
        /* [in] */ Int32 capacity);

    DoubleArrayBuffer(
        /* [in] */ Int32 capacity,
        /* [in] */ ArrayOf<Double>* backingArray,
        /* [in] */ Int32 offset);

    CARAPI GetDouble(
        /* [out] */ Double* value);

    CARAPI GetDoubleEx(
        /* [in] */ Int32 index,
        /* [out] */ Double* value);

    CARAPI GetDoublesEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Double>* dst);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI GetOrder(
       /* [out] */ ByteOrder* byteOrder);

    CARAPI_(String) ToString();

public:
    ArrayOf<Double>* mBackingArray;

    Int32 mOffset;
};

#endif // __DOUBLEARRAYBUFFER_H__