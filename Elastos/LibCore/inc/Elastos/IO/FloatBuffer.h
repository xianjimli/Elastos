#ifndef __FLOATBUFFER_H__
#define __FLOATBUFFER_H__

#include "Buffer.h"

extern "C" const InterfaceID EIID_FloatBuffer;

/**
 * A buffer of floats.
 * <p>
 * A float buffer can be created in either of the following ways:
 * <ul>
 * <li>{@link #allocate(int) Allocate} a new float array and create a buffer
 * based on it;</li>
 * <li>{@link #wrap(float[]) Wrap} an existing float array to create a new
 * buffer;</li>
 * <li>Use {@link java.nio.ByteBuffer#asFloatBuffer() ByteBuffer.asFloatBuffer}
 * to create a float buffer based on a byte buffer.</li>
 * </ul>
 */
class FloatBuffer : public Buffer
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid) = 0;

	/**
     * Creates a float buffer based on a newly allocated float array.
     *
     * @param capacity
     *            the capacity of the new buffer.
     * @return the created float buffer.
     * @throws IllegalArgumentException
     *             if {@code capacity} is less than zero.
     */
    static CARAPI Allocate(
    	/* [in] */ Int32 capacity,
    	/* [out] */ IFloatBuffer** buf);

    /**
     * Creates a new float buffer by wrapping the given float array.
     * <p>
     * Calling this method has the same effect as
     * {@code wrap(array, 0, array.length)}.
     *
     * @param array
     *            the float array which the new buffer will be based on.
     * @return the created float buffer.
     */
    static CARAPI Wrap(
    	/* [in] */ ArrayOf<Float>* array,
    	/* [out] */ IFloatBuffer** buf);

    /**
     * Creates a new float buffer by wrapping the given float array.
     * <p>
     * The new buffer's position will be {@code start}, limit will be
     * {@code start + len}, capacity will be the length of the array.
     *
     * @param array
     *            the float array which the new buffer will be based on.
     * @param start
     *            the start index, must not be negative and not greater than
     *            {@code array.length}.
     * @param len
     *            the length, must not be negative and not greater than
     *            {@code array.length - start}.
     * @return the created float buffer.
     * @exception IndexOutOfBoundsException
     *                if either {@code start} or {@code len} is invalid.
     * @exception NullPointerException
     *                if {@code array} is null.
     */
    static CARAPI WrapEx(
    	/* [in] */ ArrayOf<Float>* array,
    	/* [in] */ Int32 start,
    	/* [in] */ Int32 len,
    	/* [out] */ IFloatBuffer** buf);

    /**
     * Constructs a {@code FloatBuffer} with given capacity.
     *
     * @param capacity  The capacity of the buffer
     */
    FloatBuffer(
        /* [in] */ Int32 capacity);

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Float>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    virtual CARAPI AsReadOnlyBuffer(
        /* [out] */ IFloatBuffer** buffer) = 0;

    virtual CARAPI Compact() = 0;

    virtual CARAPI CompareTo(
        /* [in] */ IFloatBuffer* otherBuffer,
        /* [out] */ Int32* result);

    virtual CARAPI Duplicate(
        /* [out] */ IFloatBuffer** buffer) = 0;

    virtual CARAPI GetFloat(
        /* [out] */ Float* value) = 0;

    virtual CARAPI GetFloatEx(
        /* [in] */ Int32 index,
        /* [out] */ Float* value) = 0;

    virtual CARAPI GetFloats(
        /* [out] */ ArrayOf<Float>* dst);

    virtual CARAPI GetFloatsEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Float>* dst);

    CARAPI HasArray(
        /* [out] */ Boolean* hasArray);

    virtual CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder) = 0;

    virtual CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Float>** array) = 0;

    virtual CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset) = 0;

    virtual CARAPI ProtectedHasArray(
        /* [out] */ Boolean* hasArray) = 0;

    virtual CARAPI PutFloat(
        /* [in] */ Float d) = 0;

    virtual CARAPI PutFloatEx(
        /* [in] */ Int32 index,
        /* [in] */ Float d) = 0;

    CARAPI PutFloats(
        /* [in] */ const ArrayOf<Float>& src);

    virtual CARAPI PutFloatsEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Float>& src);

    virtual CARAPI PutFloatBuffer(
        /* [in] */ IFloatBuffer* src);

    virtual CARAPI Slice(
        /* [out] */ IFloatBuffer** buffer) = 0;

    /**
     * Returns a string representing the state of this float buffer.
     *
     * @return a string representing the state of this float buffer.
     */
    virtual CARAPI_(String) ToString();
};

#endif // __FLOATBUFFER_H__