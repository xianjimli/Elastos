#ifndef __BYTEBUFFER_H__
#define __BYTEBUFFER_H__

#include "Buffer.h"

extern "C" const InterfaceID EIID_ByteBuffer;

class ByteBuffer : public Buffer
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid) = 0;

	/**
     * Creates a byte buffer based on a newly allocated byte array.
     *
     * @param capacity
     *            the capacity of the new buffer
     * @return the created byte buffer.
     * @throws IllegalArgumentException
     *             if {@code capacity < 0}.
     */
    static CARAPI Allocate(
    	/* [in] */ Int32 capacity,
    	/* [out] */ IByteBuffer** buf);

    /**
     * Creates a direct byte buffer based on a newly allocated memory block.
     *
     * @param capacity
     *            the capacity of the new buffer
     * @return the created byte buffer.
     * @throws IllegalArgumentException
     *             if {@code capacity < 0}.
     */
    static CARAPI AllocateDirect(
    	/* [in] */ Int32 capacity,
    	/* [out] */ IByteBuffer** buf);

    /**
     * Creates a new byte buffer by wrapping the given byte array.
     * <p>
     * Calling this method has the same effect as
     * {@code wrap(array, 0, array.length)}.
     *
     * @param array
     *            the byte array which the new buffer will be based on
     * @return the created byte buffer.
     */
    static CARAPI Wrap(
    	/* [in] */ ArrayOf<Byte>* array,
    	/* [out] */ IByteBuffer** buf);

    /**
     * Creates a new byte buffer by wrapping the given byte array.
     * <p>
     * The new buffer's position will be {@code start}, limit will be
     * {@code start + len}, capacity will be the length of the array.
     *
     * @param array
     *            the byte array which the new buffer will be based on.
     * @param start
     *            the start index, must not be negative and not greater than
     *            {@code array.length}.
     * @param len
     *            the length, must not be negative and not greater than
     *            {@code array.length - start}.
     * @return the created byte buffer.
     * @exception IndexOutOfBoundsException
     *                if either {@code start} or {@code len} is invalid.
     */
    static CARAPI WrapEx(
    	/* [in] */ ArrayOf<Byte>* array,
    	/* [in] */ Int32 start,
    	/* [in] */ Int32 len,
    	/* [out] */ IByteBuffer** buf);

    /**
     * Constructs a {@code ByteBuffer} with given capacity.
     *
     * @param capacity
     *            the capacity of the buffer.
     */
    ByteBuffer(
        /* [in] */ Int32 capacity);

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Byte>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    virtual CARAPI AsCharBuffer(
        /* [out] */ ICharBuffer** buffer) = 0;

    virtual CARAPI AsDoubleBuffer(
        /* [out] */ IDoubleBuffer** buffer) = 0;

    virtual CARAPI AsFloatBuffer(
        /* [out] */ IFloatBuffer** buffer) = 0;

    virtual CARAPI AsInt16Buffer(
        /* [out] */ IInt16Buffer** buffer) = 0;

    virtual CARAPI AsInt32Buffer(
        /* [out] */ IInt32Buffer** buffer) = 0;

    virtual CARAPI AsInt64Buffer(
        /* [out] */ IInt64Buffer** buffer) = 0;

    virtual CARAPI AsReadOnlyBuffer(
        /* [out] */ IByteBuffer** buffer) = 0;

    virtual CARAPI Compact() = 0;

    virtual CARAPI CompareTo(
        /* [in] */ IByteBuffer* otherBuffer,
        /* [out] */ Int32* result);

    virtual CARAPI Duplicate(
        /* [out] */ IByteBuffer** buffer) = 0;

    virtual CARAPI GetByte(
        /* [out] */ Byte* value) = 0;

    virtual CARAPI GetByteEx(
        /* [in] */ Int32 index,
        /* [out] */ Byte* value) = 0;

    virtual CARAPI GetBytes(
        /* [out] */ ArrayOf<Byte>* dst);

    virtual CARAPI GetBytesEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Byte>* dst);

    virtual CARAPI GetChar(
        /* [out] */ Char32* value) = 0;

    virtual CARAPI GetCharEx(
        /* [in] */ Int32 index,
        /* [out] */ Char32* value) = 0;

    virtual CARAPI GetDouble(
        /* [out] */ Double* value) = 0;

    virtual CARAPI GetDoubleEx(
        /* [in] */ Int32 index,
        /* [out] */ Double* value) = 0;

    virtual CARAPI GetFloat(
        /* [out] */ Float* value) = 0;

    virtual CARAPI GetFloatEx(
        /* [in] */ Int32 index,
        /* [out] */ Float* value) = 0;

    virtual CARAPI GetInt32(
        /* [out] */ Int32* value) = 0;

    virtual CARAPI GetInt32Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int32* value) = 0;

    virtual CARAPI GetInt64(
        /* [out] */ Int64* value) = 0;

    virtual CARAPI GetInt64Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int64* value) = 0;

    virtual CARAPI GetInt16(
        /* [out] */ Int16* value) = 0;

    virtual CARAPI GetInt16Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int16* value) = 0;

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI SetOrder(
        /* [in] */ ByteOrder byteOrder);

    virtual CARAPI SetOrderImpl(
        /* [in] */ ByteOrder byteOrder);

    virtual CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Byte>** array) = 0;

    virtual CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset) = 0;

    virtual CARAPI ProtectedHasArray(
        /* [out] */ Boolean* hasArray) = 0;

    virtual CARAPI PutByte(
        /* [in] */ Byte b) = 0;

    virtual CARAPI PutByteEx(
        /* [in] */ Int32 index,
        /* [in] */ Byte b) = 0;

    CARAPI PutBytes(
        /* [in] */ const ArrayOf<Byte>& src);

    virtual CARAPI PutBytesEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Byte>& src);

    virtual CARAPI PutByteBuffer(
        /* [in] */ IByteBuffer* src);

    virtual CARAPI PutChar(
        /* [in] */ Char32 value) = 0;

    virtual CARAPI PutCharEx(
        /* [in] */ Int32 index,
        /* [in] */ Char32 value) = 0;

    virtual CARAPI PutDouble(
        /* [in] */ Double value) = 0;

    virtual CARAPI PutDoubleEx(
        /* [in] */ Int32 index,
        /* [in] */ Double value) = 0;

    virtual CARAPI PutFloat(
        /* [in] */ Float value) = 0;

    virtual CARAPI PutFloatEx(
        /* [in] */ Int32 index,
        /* [in] */ Float value) = 0;

    virtual CARAPI PutInt16(
        /* [in] */ Int16 value) = 0;

    virtual CARAPI PutInt16Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int16 value) = 0;

    virtual CARAPI PutInt32(
        /* [in] */ Int32 value) = 0;

    virtual CARAPI PutInt32Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int32 value) = 0;

    virtual CARAPI PutInt64(
        /* [in] */ Int64 value) = 0;

    virtual CARAPI PutInt64Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value) = 0;

    virtual CARAPI Slice(
        /* [out] */ IByteBuffer** buffer) = 0;

    CARAPI HasArray(
        /* [out] */ Boolean* hasArray);

    /**
     * Returns a string representing the state of this byte buffer.
     *
     * @return a string representing the state of this byte buffer.
     */
    virtual CARAPI_(String) ToString();

public:
	/**
     * The byte order of this buffer, default is {@code BIG_ENDIAN}.
     */
    ByteOrder mOrder;
};

#endif // __BYTEBUFFER_H__