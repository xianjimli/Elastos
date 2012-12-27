#ifndef __Int32Buffer_H__
#define __Int32Buffer_H__

#include "Buffer.h"

extern "C" const InterfaceID EIID_Int32Buffer;

/**
 * A buffer of ints.
 * <p>
 * A int buffer can be created in either of the following ways:
 * <ul>
 * <li>{@link #allocate(int) Allocate} a new int array and create a buffer
 * based on it;</li>
 * <li>{@link #wrap(int[]) Wrap} an existing int array to create a new buffer;</li>
 * <li>Use {@link java.nio.ByteBuffer#asIntBuffer() ByteBuffer.asIntBuffer} to
 * create a int buffer based on a byte buffer.</li>
 * </ul>
 */
class Int32Buffer : public Buffer
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid) = 0;

	/**
     * Creates an int buffer based on a newly allocated int array.
     *
     * @param capacity
     *            the capacity of the new buffer.
     * @return the created int buffer.
     * @throws IllegalArgumentException
     *             if {@code capacity} is less than zero.
     */
    static CARAPI Allocate(
    	/* [in] */ Int32 capacity,
    	/* [out] */ IInt32Buffer** buf);

    /**
     * Creates a new int buffer by wrapping the given int array.
     * <p>
     * Calling this method has the same effect as
     * {@code wrap(array, 0, array.length)}.
     *
     * @param array
     *            the int array which the new buffer will be based on.
     * @return the created int buffer.
     */
    static CARAPI Wrap(
    	/* [in] */ ArrayOf<Int32>* array,
    	/* [out] */ IInt32Buffer** buf);

    /**
     * Creates a new int buffer by wrapping the given int array.
     * <p>
     * The new buffer's position will be {@code start}, limit will be
     * {@code start + len}, capacity will be the length of the array.
     *
     * @param array
     *            the int array which the new buffer will be based on.
     * @param start
     *            the start index, must not be negative and not greater than
     *            {@code array.length}
     * @param len
     *            the length, must not be negative and not greater than
     *            {@code array.length - start}.
     * @return the created int buffer.
     * @exception IndexOutOfBoundsException
     *                if either {@code start} or {@code len} is invalid.
     */
    static CARAPI WrapEx(
    	/* [in] */ ArrayOf<Int32>* array,
    	/* [in] */ Int32 start,
    	/* [in] */ Int32 len,
    	/* [out] */ IInt32Buffer** buf);

    /**
     * Constructs a {@code IntBuffer} with given capacity.
     *
     * @param capacity
     *            the capacity of the buffer.
     */
    Int32Buffer(
        /* [in] */ Int32 capacity);

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Int32>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    virtual CARAPI AsReadOnlyBuffer(
        /* [out] */ IInt32Buffer** buffer) = 0;

    virtual CARAPI Compact() = 0;

    virtual CARAPI CompareTo(
        /* [in] */ IInt32Buffer* otherBuffer,
        /* [out] */ Int32* result);

    virtual CARAPI Duplicate(
        /* [out] */ IInt32Buffer** buffer) = 0;

    virtual CARAPI GetInt32(
        /* [out] */ Int32* value) = 0;

    virtual CARAPI GetInt32Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int32* value) = 0;

    virtual CARAPI GetInt32s(
        /* [out] */ ArrayOf<Int32>* dst);

    virtual CARAPI GetInt32sEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Int32>* dst);

    CARAPI HasArray(
        /* [out] */ Boolean* hasArray);

    virtual CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder) = 0;

    virtual CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Int32>** array) = 0;

    virtual CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset) = 0;

    virtual CARAPI ProtectedHasArray(
        /* [out] */ Boolean* hasArray) = 0;

    virtual CARAPI PutInt32(
        /* [in] */ Int32 d) = 0;

    virtual CARAPI PutInt32Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int32 d) = 0;

    CARAPI PutInt32s(
        /* [in] */ const ArrayOf<Int32>& src);

    virtual CARAPI PutInt32sEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Int32>& src);

    virtual CARAPI PutInt32Buffer(
        /* [in] */ IInt32Buffer* src);

    virtual CARAPI Slice(
        /* [out] */ IInt32Buffer** buffer) = 0;

    /**
     * Returns a string representing the state of this Int32 buffer.
     *
     * @return a string representing the state of this Int32 buffer.
     */
    virtual CARAPI_(String) ToString();
};

#endif // __Int32Buffer_H__