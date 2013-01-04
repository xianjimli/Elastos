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

    /**
     * Constructs a {@code CharBuffer} with given capacity.
     *
     * @param capacity
     *            the capacity of the buffer.
     */
    CharBuffer(
        /* [in] */ Int32 capacity);

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Char32>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    virtual CARAPI AsReadOnlyBuffer(
        /* [out] */ ICharBuffer** buffer) = 0;

    CARAPI GetCharAt(
        /* [in] */ Int32 index,
        /* [out] */ Char32* value);

    virtual CARAPI Compact() = 0;

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

    virtual CARAPI GetCharsEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Char32>* dst);

    CARAPI GetLength(
        /* [out] */ Int32* number);

    virtual CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder) = 0;

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

    virtual CARAPI Read(
        /* [in] */ ICharBuffer* target,
        /* [out] */ Int32* number);

    virtual CARAPI HasArray(
        /* [out] */ Boolean* hasArray);
};

#endif // __CHARBUFFER_H__