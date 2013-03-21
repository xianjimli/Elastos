#ifndef __CBYTEBUFFERHELPER_H__
#define __CBYTEBUFFERHELPER_H__

#include "_CByteBufferHelper.h"

CarClass(CByteBufferHelper)
{
public:
    /**
     * Creates a byte buffer based on a newly allocated byte array.
     *
     * @param capacity
     *            the capacity of the new buffer
     * @return the created byte buffer.
     * @throws IllegalArgumentException
     *             if {@code capacity < 0}.
     */
    CARAPI Allocate(
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
    CARAPI AllocateDirect(
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
    CARAPI Wrap(
        /* [in] */ const ArrayOf<Byte>& array,
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
    CARAPI WrapEx(
        /* [in] */ const ArrayOf<Byte>& array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 len,
        /* [out] */ IByteBuffer** buf);
};

#endif // __CBYTEBUFFERHELPER_H__