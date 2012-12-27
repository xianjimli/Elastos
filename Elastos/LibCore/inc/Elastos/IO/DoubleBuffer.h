#ifndef __DOUBLEBUFFER_H__
#define __DOUBLEBUFFER_H__

#include "Buffer.h"

extern "C" const InterfaceID EIID_DoubleBuffer;

/**
 * A buffer of chars.
 * <p>
 * A char buffer can be created in either one of the following ways:
 * <ul>
 * <li>{@link #allocate(int) Allocate} a new char array and create a buffer
 * based on it;</li>
 * <li>{@link #wrap(char[]) Wrap} an existing char array to create a new
 * buffer;</li>
 * <li>{@link #wrap(CharSequence) Wrap} an existing char sequence to create a
 * new buffer;</li>
 * <li>Use {@link java.nio.ByteBuffer#asDoubleBuffer() ByteBuffer.asDoubleBuffer}
 * to create a char buffer based on a byte buffer.</li>
 * </ul>
 */
class DoubleBuffer : public Buffer
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid) = 0;

	/**
     * Creates a double buffer based on a newly allocated double array.
     *
     * @param capacity
     *            the capacity of the new buffer.
     * @return the created double buffer.
     * @throws IllegalArgumentException
     *             if {@code capacity} is less than zero.
     */
    static CARAPI Allocate(
    	/* [in] */ Int32 capacity,
    	/* [out] */ IDoubleBuffer** buf);

    /**
     * Creates a new double buffer by wrapping the given double array.
     * <p>
     * Calling this method has the same effect as
     * {@code wrap(array, 0, array.length)}.
     *
     * @param array
     *            the double array which the new buffer will be based on.
     * @return the created double buffer.
     */
    static CARAPI Wrap(
    	/* [in] */ ArrayOf<Double>* array,
    	/* [out] */ IDoubleBuffer** buf);

    /**
     * Creates a new double buffer by wrapping the given double array.
     * <p>
     * The new buffer's position will be {@code start}, limit will be
     * {@code start + len}, capacity will be the length of the array.
     *
     * @param array
     *            the double array which the new buffer will be based on.
     * @param start
     *            the start index, must not be negative and not greater than
     *            {@code array.length}.
     * @param len
     *            the length, must not be negative and not greater than
     *            {@code array.length - start}.
     * @return the created double buffer.
     * @exception IndexOutOfBoundsException
     *                if either {@code start} or {@code len} is invalid.
     */
    static CARAPI WrapEx(
    	/* [in] */ ArrayOf<Double>* array,
    	/* [in] */ Int32 start,
    	/* [in] */ Int32 len,
    	/* [out] */ IDoubleBuffer** buf);

    /**
     * Constructs a {@code DoubleBuffer} with given capacity.
     *
     * @param capacity
     *            the capacity of the buffer.
     */
    DoubleBuffer(
        /* [in] */ Int32 capacity);

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Double>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    virtual CARAPI AsReadOnlyBuffer(
        /* [out] */ IDoubleBuffer** buffer) = 0;

    virtual CARAPI Compact() = 0;

    virtual CARAPI CompareTo(
        /* [in] */ IDoubleBuffer* otherBuffer,
        /* [out] */ Int32* result);

    virtual CARAPI Duplicate(
        /* [out] */ IDoubleBuffer** buffer) = 0;

    virtual CARAPI GetDouble(
        /* [out] */ Double* value) = 0;

    virtual CARAPI GetDoubleEx(
        /* [in] */ Int32 index,
        /* [out] */ Double* value) = 0;

    virtual CARAPI GetDoubles(
        /* [out] */ ArrayOf<Double>* dst);

    virtual CARAPI GetDoublesEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Double>* dst);

    virtual CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder) = 0;

    virtual CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Double>** array) = 0;

    virtual CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset) = 0;

    virtual CARAPI ProtectedHasArray(
        /* [out] */ Boolean* hasArray) = 0;

    virtual CARAPI PutDouble(
        /* [in] */ Double d) = 0;

    virtual CARAPI PutDoubleEx(
        /* [in] */ Int32 index,
        /* [in] */ Double d) = 0;

    CARAPI PutDoubles(
        /* [in] */ const ArrayOf<Double>& src);

    virtual CARAPI PutDoublesEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Double>& src);

    virtual CARAPI PutDoubleBuffer(
        /* [in] */ IDoubleBuffer* src);

    virtual CARAPI Slice(
        /* [out] */ IDoubleBuffer** buffer) = 0;

    CARAPI HasArray(
        /* [out] */ Boolean* hasArray);

    /**
     * Returns a string representing the state of this double buffer.
     *
     * @return a string representing the state of this double buffer.
     */
    virtual CARAPI_(String) ToString();
};

#endif // __DOUBLEBUFFER_H__