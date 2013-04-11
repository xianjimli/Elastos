#ifndef __ReadWriteDirectByteBuffer_H__
#define __ReadWriteDirectByteBuffer_H__

#include "DirectByteBuffer.h"
#include <elastos/ElRefBase.h>


extern "C" const InterfaceID EIID_ReadWriteDirectByteBuffer;

/**
 * DirectByteBuffer, ReadWriteDirectByteBuffer and ReadOnlyDirectByteBuffer
 * compose the implementation of platform memory based byte buffers.
 * <p>
 * ReadWriteDirectByteBuffer extends DirectByteBuffer with all the write
 * methods.
 * </p>
 * <p>
 * This class is marked final for runtime performance.
 * </p>
 */
class ReadWriteDirectByteBuffer
    : public ElRefBase
    , public IByteBuffer
    , public DirectByteBuffer
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    static CARAPI_(ReadWriteDirectByteBuffer*) Copy(
    	/* [in] */ DirectByteBuffer* other,
    	/* [in] */ Int32 markOfOther);

    ReadWriteDirectByteBuffer(
    	/* [in] */ Int32 capacity);

    // BEGIN android-added
    ReadWriteDirectByteBuffer(
        /* [in] */ Int32 pointer,
        /* [in] */ Int32 capacity);

    ReadWriteDirectByteBuffer(
        /* [in] */ SafeAddress* address,
        /* [in] */ Int32 capacity,
        /* [in] */ Int32 offset);

    //ReadWriteDirectByteBuffer(
    //    /* [in] */ IPlatformAddress* address,
    //    /* [in] */ Int32 aCapacity,
    //    /* [in] */ Int32 anOffset);

    CARAPI GetAddress(
        /* [out] */ Int32* addr);

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

    // BEGIN android-added
    /**
     * Writes <code>short</code>s in the given short array, starting from the
     * specified offset, to the current position and increase the position by
     * the number of <code>short</code>s written.
     *
     * @param src
     *            The source short array
     * @param off
     *            The offset of short array, must be no less than zero and no
     *            greater than <code>src.length</code>
     * @param len
     *            The number of <code>short</code>s to write, must be no less
     *            than zero and no greater than <code>src.length - off</code>
     * @return This buffer
     * @exception BufferOverflowException
     *                If <code>remaining()</code> is less than
     *                <code>len</code>
     * @exception IndexOutOfBoundsException
     *                If either <code>off</code> or <code>len</code> is
     *                invalid
     * @exception ReadOnlyBufferException
     *                If no changes may be made to the contents of this buffer
     */
    CARAPI PutInt16s(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Int16>& src);

    /**
     * Writes <code>int</code>s in the given int array, starting from the
     * specified offset, to the current position and increase the position by
     * the number of <code>int</code>s written.
     *
     * @param src
     *            The source int array
     * @param off
     *            The offset of int array, must be no less than zero and no
     *            greater than <code>src.length</code>
     * @param len
     *            The number of <code>int</code>s to write, must be no less
     *            than zero and no greater than <code>src.length - off</code>
     * @return This buffer
     * @exception BufferOverflowException
     *                If <code>remaining()</code> is less than
     *                <code>len</code>
     * @exception IndexOutOfBoundsException
     *                If either <code>off</code> or <code>len</code> is
     *                invalid
     * @exception ReadOnlyBufferException
     *                If no changes may be made to the contents of this buffer
     */
    CARAPI PutInt32s(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Int32>& src);

    /**
     * Writes <code>float</code>s in the given float array, starting from the
     * specified offset, to the current position and increase the position by
     * the number of <code>float</code>s written.
     *
     * @param src
     *            The source float array
     * @param off
     *            The offset of float array, must be no less than zero and no
     *            greater than <code>src.length</code>
     * @param len
     *            The number of <code>float</code>s to write, must be no less
     *            than zero and no greater than <code>src.length - off</code>
     * @return This buffer
     * @exception BufferOverflowException
     *                If <code>remaining()</code> is less than
     *                <code>len</code>
     * @exception IndexOutOfBoundsException
     *                If either <code>off</code> or <code>len</code> is
     *                invalid
     * @exception ReadOnlyBufferException
     *                If no changes may be made to the contents of this buffer
     */
    CARAPI PutFloats(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Float>& src);

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

#endif // __ReadWriteDirectByteBuffer_H__
