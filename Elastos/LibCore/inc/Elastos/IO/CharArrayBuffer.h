#ifndef __CHARARRAYBUFFER_H__
#define __CHARARRAYBUFFER_H__

#include "CharBuffer.h"

/**
 * CharArrayBuffer, ReadWriteCharArrayBuffer and ReadOnlyCharArrayBuffer compose
 * the implementation of array based char buffers.
 * <p>
 * CharArrayBuffer implements all the shared readonly methods and is extended by
 * the other two classes.
 * </p>
 * <p>
 * All methods are marked final for runtime performance.
 * </p>
 *
 */
class CharArrayBuffer : public CharBuffer
{
public:
    CharArrayBuffer(
        /* [in] */ ArrayOf<Char32>* array);

    CharArrayBuffer(
        /* [in] */ Int32 capacity);

    CharArrayBuffer(
        /* [in] */ Int32 capacity,
        /* [in] */ ArrayOf<Char32>* backingArray,
        /* [in] */ Int32 offset);

    CARAPI GetChar(
        /* [out] */ Char32* value);

    CARAPI GetCharEx(
        /* [in] */ Int32 index,
        /* [out] */ Char32* value);

    CARAPI GetCharsEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Char32>* dst);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI GetOrder(
       /* [out] */ ByteOrder* byteOrder);

    CARAPI SubSequence(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ICharSequence** csq);

    CARAPI ToString(
        /* [out] */ String* str);

public:
    ArrayOf<Char32>* mBackingArray;

    Int32 mOffset;
};

#endif // __CHARARRAYBUFFER_H__