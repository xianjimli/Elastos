#ifndef __INT64BUFFER_H__
#define __INT64BUFFER_H__

#include "Buffer.h"

extern "C" const InterfaceID EIID_Int64Buffer;

/**
 * A buffer of longs.
 * <p>
 * A long buffer can be created in either of the following ways:
 * <ul>
 * <li>{@link #allocate(int) Allocate} a new long array and create a buffer
 * based on it;</li>
 * <li>{@link #wrap(long[]) Wrap} an existing long array to create a new
 * buffer;</li>
 * <li>Use {@link java.nio.ByteBuffer#asLongBuffer() ByteBuffer.asLongBuffer}
 * to create a long buffer based on a byte buffer.</li>
 * </ul>
 */
class Int64Buffer : public Buffer
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid) = 0;

	/**
     * Creates a long buffer based on a newly allocated long array.
     *
     * @param capacity
     *            the capacity of the new buffer.
     * @return the created long buffer.
     * @throws IllegalArgumentException
     *             if {@code capacity} is less than zero.
     */
    static CARAPI Allocate(
    	/* [in] */ Int32 capacity,
    	/* [out] */ IInt64Buffer** buf);

    /**
     * Creates a new long buffer by wrapping the given long array.
     * <p>
     * Calling this method has the same effect as
     * {@code wrap(array, 0, array.length)}.
     *
     * @param array
     *            the long array which the new buffer will be based on.
     * @return the created long buffer.
     */
    static CARAPI Wrap(
    	/* [in] */ ArrayOf<Int64>* array,
    	/* [out] */ IInt64Buffer** buf);

    /**
     * Creates a new long buffer by wrapping the given long array.
     * <p>
     * The new buffer's position will be {@code start}, limit will be
     * {@code start + len}, capacity will be the length of the array.
     *
     * @param array
     *            the long array which the new buffer will be based on.
     * @param start
     *            the start index, must not be negative and not greater than
     *            {@code array.length}.
     * @param len
     *            the length, must not be negative and not greater than
     *            {@code array.length - start}.
     * @return the created long buffer.
     * @exception IndexOutOfBoundsException
     *                if either {@code start} or {@code len} is invalid.
     */
    static CARAPI WrapEx(
    	/* [in] */ ArrayOf<Int64>* array,
    	/* [in] */ Int32 start,
    	/* [in] */ Int32 len,
    	/* [out] */ IInt64Buffer** buf);

    /**
     * Constructs a {@code LongBuffer} with given capacity.
     *
     * @param capacity
     *            The capacity of the buffer
     */
    Int64Buffer(
        /* [in] */ Int32 capacity);

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Int64>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    virtual CARAPI AsReadOnlyBuffer(
        /* [out] */ IInt64Buffer** buffer) = 0;

    virtual CARAPI Compact() = 0;

    virtual CARAPI CompareTo(
        /* [in] */ IInt64Buffer* otherBuffer,
        /* [out] */ Int32* result);

    virtual CARAPI Duplicate(
        /* [out] */ IInt64Buffer** buffer) = 0;

    virtual CARAPI GetInt64(
        /* [out] */ Int64* value) = 0;

    virtual CARAPI GetInt64Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int64* value) = 0;

    virtual CARAPI GetInt64s(
        /* [out] */ ArrayOf<Int64>* dst);

    virtual CARAPI GetInt64sEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Int64>* dst);

    CARAPI HasArray(
        /* [out] */ Boolean* hasArray);

    virtual CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder) = 0;

    virtual CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Int64>** array) = 0;

    virtual CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset) = 0;

    virtual CARAPI ProtectedHasArray(
        /* [out] */ Boolean* hasArray) = 0;

    virtual CARAPI PutInt64(
        /* [in] */ Int64 d) = 0;

    virtual CARAPI PutInt64Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int64 d) = 0;

    CARAPI PutInt64s(
        /* [in] */ const ArrayOf<Int64>& src);

    virtual CARAPI PutInt64sEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Int64>& src);

    virtual CARAPI PutInt64Buffer(
        /* [in] */ IInt64Buffer* src);

    virtual CARAPI Slice(
        /* [out] */ IInt64Buffer** buffer) = 0;

    /**
     * Returns a string representing the state of this Int64 buffer.
     *
     * @return a string representing the state of this Int64 buffer.
     */
    virtual CARAPI_(String) ToString();
};

#endif // __INT64BUFFER_H__