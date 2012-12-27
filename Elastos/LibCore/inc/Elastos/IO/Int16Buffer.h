#ifndef __Int16Buffer_H__
#define __Int16Buffer_H__

#include "Buffer.h"

extern "C" const InterfaceID EIID_Int16Buffer;

/**
 * A buffer of shorts.
 * <p>
 * A short buffer can be created in either of the following ways:
 * <ul>
 * <li>{@link #allocate(int) Allocate} a new short array and create a buffer
 * based on it;</li>
 * <li>{@link #wrap(short[]) Wrap} an existing short array to create a new
 * buffer;</li>
 * <li>Use {@link java.nio.ByteBuffer#asShortBuffer() ByteBuffer.asShortBuffer}
 * to create a short buffer based on a byte buffer.</li>
 * </ul>
 */
class Int16Buffer : public Buffer
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid) = 0;

	/**
     * Creates a short buffer based on a newly allocated short array.
     *
     * @param capacity
     *            the capacity of the new buffer.
     * @return the created short buffer.
     * @throws IllegalArgumentException
     *             if {@code capacity} is less than zero.
     */
    static CARAPI Allocate(
    	/* [in] */ Int32 capacity,
    	/* [out] */ IInt16Buffer** buf);

    /**
     * Creates a new short buffer by wrapping the given short array.
     * <p>
     * Calling this method has the same effect as
     * {@code wrap(array, 0, array.length)}.
     *
     * @param array
     *            the short array which the new buffer will be based on.
     * @return the created short buffer.
     */
    static CARAPI Wrap(
    	/* [in] */ ArrayOf<Int16>* array,
    	/* [out] */ IInt16Buffer** buf);

    /**
     * Creates a new short buffer by wrapping the given short array.
     * <p>
     * The new buffer's position will be {@code start}, limit will be
     * {@code start + len}, capacity will be the length of the array.
     *
     * @param array
     *            the short array which the new buffer will be based on.
     * @param start
     *            the start index, must not be negative and not greater than
     *            {@code array.length}.
     * @param len
     *            the length, must not be negative and not greater than
     *            {@code array.length - start}.
     * @return the created short buffer.
     * @exception IndexOutOfBoundsException
     *                if either {@code start} or {@code len} is invalid.
     */
    static CARAPI WrapEx(
    	/* [in] */ ArrayOf<Int16>* array,
    	/* [in] */ Int32 start,
    	/* [in] */ Int32 len,
    	/* [out] */ IInt16Buffer** buf);

    /**
     * Constructs a {@code ShortBuffer} with given capacity.
     *
     * @param capacity
     *            The capacity of the buffer
     */
    Int16Buffer(
        /* [in] */ Int32 capacity);

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Int16>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    virtual CARAPI AsReadOnlyBuffer(
        /* [out] */ IInt16Buffer** buffer) = 0;

    virtual CARAPI Compact() = 0;

    virtual CARAPI CompareTo(
        /* [in] */ IInt16Buffer* otherBuffer,
        /* [out] */ Int32* result);

    virtual CARAPI Duplicate(
        /* [out] */ IInt16Buffer** buffer) = 0;

    virtual CARAPI GetInt16(
        /* [out] */ Int16* value) = 0;

    virtual CARAPI GetInt16Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int16* value) = 0;

    virtual CARAPI GetInt16s(
        /* [out] */ ArrayOf<Int16>* dst);

    virtual CARAPI GetInt16sEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Int16>* dst);

    CARAPI HasArray(
        /* [out] */ Boolean* hasArray);

    virtual CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder) = 0;

    virtual CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Int16>** array) = 0;

    virtual CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset) = 0;

    virtual CARAPI ProtectedHasArray(
        /* [out] */ Boolean* hasArray) = 0;

    virtual CARAPI PutInt16(
        /* [in] */ Int16 d) = 0;

    virtual CARAPI PutInt16Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int16 d) = 0;

    CARAPI PutInt16s(
        /* [in] */ const ArrayOf<Int16>& src);

    virtual CARAPI PutInt16sEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Int16>& src);

    virtual CARAPI PutInt16Buffer(
        /* [in] */ IInt16Buffer* src);

    virtual CARAPI Slice(
        /* [out] */ IInt16Buffer** buffer) = 0;

    /**
     * Returns a string representing the state of this Int16 buffer.
     *
     * @return a string representing the state of this Int16 buffer.
     */
    virtual CARAPI_(String) ToString();
};

#endif // __Int16Buffer_H__