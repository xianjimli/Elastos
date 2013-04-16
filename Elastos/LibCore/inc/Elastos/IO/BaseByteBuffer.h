#ifndef __BASEBYTEBUFFER_H__
#define __BASEBYTEBUFFER_H__

#include "ByteBuffer.h"

/**
 * Serves as the root of other byte buffer impl classes, implements common
 * methods that can be shared by child classes.
 */
class BaseByteBuffer : public ByteBuffer
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid) = 0;

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

    CARAPI GetChar(
        /* [out] */ Char32* value);

    CARAPI GetCharEx(
        /* [in] */ Int32 index,
        /* [out] */ Char32* value);

    CARAPI PutChar(
        /* [in] */ Char32 value);

    CARAPI PutCharEx(
        /* [in] */ Int32 index,
        /* [in] */ Char32 value);

// protected:
    BaseByteBuffer(
        /* [in] */ Int32 capacity);
};

#endif // __BASEBYTEBUFFER_H__
