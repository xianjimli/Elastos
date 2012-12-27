#ifndef __Int16ArrayBuffer_H__
#define __Int16ArrayBuffer_H__

#include "Int16Buffer.h"

/**
 * ShortArrayBuffer, ReadWriteShortArrayBuffer and ReadOnlyShortArrayBuffer
 * compose the implementation of array based short buffers.
 * <p>
 * ShortArrayBuffer implements all the shared readonly methods and is extended
 * by the other two classes.
 * </p>
 * <p>
 * All methods are marked final for runtime performance.
 * </p>
 *
 */
class Int16ArrayBuffer : public Int16Buffer
{
public:
    Int16ArrayBuffer(
        /* [in] */ ArrayOf<Int16>* array);

    Int16ArrayBuffer(
        /* [in] */ Int32 capacity);

    Int16ArrayBuffer(
        /* [in] */ Int32 capacity,
        /* [in] */ ArrayOf<Int16>* backingArray,
        /* [in] */ Int32 offset);

    CARAPI GetInt16(
        /* [out] */ Int16* value);

    CARAPI GetInt16Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int16* value);

    CARAPI GetInt16sEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Int16>* dst);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI GetOrder(
       /* [out] */ ByteOrder* byteOrder);

public:
    ArrayOf<Int16>* mBackingArray;

    Int32 mOffset;
};

#endif // __Int16ArrayBuffer_H__