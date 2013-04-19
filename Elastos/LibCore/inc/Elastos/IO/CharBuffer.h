#ifndef __CHARBUFFER_H__
#define __CHARBUFFER_H__

#include "Buffer.h"

extern "C" const InterfaceID EIID_CharBuffer;

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
 * <li>Use {@link java.nio.ByteBuffer#asCharBuffer() ByteBuffer.asCharBuffer}
 * to create a char buffer based on a byte buffer.</li>
 * </ul>
 */
class CharBuffer : public Buffer
{
protected:
    /**
     * Constructs a {@code CharBuffer} with given capacity.
     *
     * @param capacity
     *            the capacity of the buffer.
     */
    CharBuffer(
        /* [in] */ Int32 capacity);

public:
    virtual CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid) = 0;

	/**
     * Creates a char buffer based on a newly allocated char array.
     *
     * @param capacity
     *            the capacity of the new buffer.
     * @return the created char buffer.
     * @throws IllegalArgumentException
     *             if {@code capacity} is less than zero.
     */
    static CARAPI Allocate(
    	/* [in] */ Int32 capacity,
    	/* [out] */ ICharBuffer** buf);

    /**
     * Creates a new char buffer by wrapping the given char array.
     * <p>
     * Calling this method has the same effect as
     * {@code wrap(array, 0, array.length)}.
     *
     * @param array
     *            the char array which the new buffer will be based on.
     * @return the created char buffer.
     */
    static CARAPI Wrap(
    	/* [in] */ ArrayOf<Char32>* array,
    	/* [out] */ ICharBuffer** buf);

    /**
     * Creates a new char buffer by wrapping the given char array.
     * <p>
     * The new buffer's position will be {@code start}, limit will be
     * {@code start + len}, capacity will be the length of the array.
     *
     * @param array
     *            the char array which the new buffer will be based on.
     * @param start
     *            the start index, must not be negative and not greater than
     *            {@code array.length}.
     * @param len
     *            the length, must not be negative and not greater than
     *            {@code array.length - start}.
     * @return the created char buffer.
     * @exception IndexOutOfBoundsException
     *                if either {@code start} or {@code len} is invalid.
     */
    static CARAPI WrapEx(
    	/* [in] */ ArrayOf<Char32>* array,
    	/* [in] */ Int32 start,
    	/* [in] */ Int32 len,
    	/* [out] */ ICharBuffer** buf);

    /**
     * Creates a new char buffer by wrapping the given char sequence.
     * <p>
     * Calling this method has the same effect as
     * {@code wrap(chseq, 0, chseq.length())}.
     *
     * @param chseq
     *            the char sequence which the new buffer will be based on.
     * @return the created char buffer.
     */
    static CARAPI WrapEx2(
        /* [in] */ ICharSequence* chseq,
        /* [out] */ ICharBuffer** buf);

    /**
     * Creates a new char buffer by wrapping the given char sequence.
     * <p>
     * The new buffer's position will be {@code start}, limit will be
     * {@code end}, capacity will be the length of the char sequence. The new
     * buffer is read-only.
     *
     * @param chseq
     *            the char sequence which the new buffer will be based on.
     * @param start
     *            the start index, must not be negative and not greater than
     *            {@code chseq.length()}.
     * @param end
     *            the end index, must be no less than {@code start} and no
     *            greater than {@code chseq.length()}.
     * @return the created char buffer.
     * @exception IndexOutOfBoundsException
     *                if either {@code start} or {@code end} is invalid.
     */
    static CARAPI WrapEx3(
        /* [in] */ ICharSequence* chseq,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ICharBuffer** buf);

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Char32>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    virtual CARAPI AsReadOnlyBuffer(
        /* [out] */ ICharBuffer** buffer) = 0;

    /**
     * Get character at the specified index, with the first character
     * having index zero.
     *
     * @param index
     *            the index of the character to return.
     *
     * @param value
     *            the requested character.
     *
     * @Return NOERROR if successful or IndexOutOfBoundsException
     *             if {@code index < 0} or {@code index} is greater than the
     *             length of this sequence.
     */
    CARAPI GetCharAt(
        /* [in] */ Int32    index,
        /* [out] */ Char32* value);

    virtual CARAPI Compact() = 0;

    /**
     * Compare the remaining chars of this buffer to another char
     * buffer's remaining chars.
     *
     * @param otherBuffer
     *      another char buffer.
     *
     * @param result
     *      return a negative value if this is less than {@code otherBuffer}; 0 if
     *      this equals to {@code otherBuffer}; a positive value if this is
     *      greater than {@code otherBuffer}.
     *
     * @return NOERROR is successful, or  ClassCastException
     *                if {@code otherBuffer} is not a char buffer.
     */
    virtual CARAPI CompareTo(
        /* [in] */ ICharBuffer* otherBuffer,
        /* [out] */ Int32* result);

    virtual CARAPI Duplicate(
        /* [out] */ ICharBuffer** buffer) = 0;

    virtual CARAPI GetChar(
        /* [out] */ Char32* value) = 0;

    virtual CARAPI GetCharEx(
        /* [in] */ Int32 index,
        /* [out] */ Char32* value) = 0;

    virtual CARAPI GetChars(
        /* [out] */ ArrayOf<Char32>* dst);

    /**
     * Reads chars from the current position into the specified char array,
     * starting from the specified offset, and increases the position by the
     * number of chars read.
     *
     * @param dst
     *            the target char array.
     * @param off
     *            the offset of the char array, must not be negative and not
     *            greater than {@code dst.length}.
     * @param len
     *            The number of chars to read, must be no less than zero and no
     *            greater than {@code dst.length - off}.
     * @return this buffer.
     * @exception IndexOutOfBoundsException
     *                if either {@code off} or {@code len} is invalid.
     * @exception BufferUnderflowException
     *                if {@code len} is greater than {@code remaining()}.
     */
    virtual CARAPI GetCharsEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Char32>* dst);

    CARAPI GetLength(
        /* [out] */ Int32* number);

    virtual CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder) = 0;

    virtual CARAPI PutChar(
        /* [in] */ Char32 c) = 0;

    virtual CARAPI PutCharEx(
        /* [in] */ Int32 index,
        /* [in] */ Char32 c) = 0;

    CARAPI PutChars(
        /* [in] */ const ArrayOf<Char32>& src);

    virtual CARAPI PutCharsEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Char32>& src);

    virtual CARAPI PutCharBuffer(
        /* [in] */ ICharBuffer* src);

    CARAPI PutString(
        /* [in] */ const String& str);

    virtual CARAPI PutStringEx(
        /* [in] */ const String& str,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    virtual CARAPI Slice(
        /* [out] */ ICharBuffer** buffer) = 0;

    virtual CARAPI SubSequence(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ICharSequence** csq) = 0;

    /**
     * Returns a string representing the current remaining chars of this buffer.
     *
     * @return a string representing the current remaining chars of this buffer.
     */
    virtual CARAPI ToString(
        /* [out] */ String* str);

    virtual CARAPI AppendChar(
        /* [in] */ Char32 c);

    virtual CARAPI AppendChars(
        /* [in] */ ICharSequence* csq);

    virtual CARAPI AppendCharsEx(
        /* [in] */ ICharSequence* csq,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Reads characters from this buffer and puts them into {@code target}. The
     * number of chars that are copied is either the number of remaining chars
     * in this buffer or the number of remaining chars in {@code target},
     * whichever is smaller.
     *
     * @param target
     *            the target char buffer.
     * @throws IllegalArgumentException
     *             if {@code target} is this buffer.
     * @throws IOException
     *             if an I/O error occurs.
     * @throws ReadOnlyBufferException
     *             if no changes may be made to the contents of {@code target}.
     * @return the number of chars copied or -1 if there are no chars left to be
     *         read from this buffer.
     */
    virtual CARAPI Read(
        /* [in] */ ICharBuffer* target,
        /* [out] */ Int32* number);

    virtual CARAPI HasArray(
        /* [out] */ Boolean* hasArray);

protected:

    /**
     * Child class implements this method to realize {@code array()}.
     *
     * @see #array()
     */
    virtual CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Char32>** array) = 0;

    /**
     * Child class implements this method to realize {@code arrayOffset()}.
     *
     * @see #arrayOffset()
     */
    virtual CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset) = 0;

    /**
     * Child class implements this method to realize {@code hasArray()}.
     *
     * @see #hasArray()
     */
    virtual CARAPI ProtectedHasArray(
        /* [out] */ Boolean* result) = 0;
};

#endif // __CHARBUFFER_H__
