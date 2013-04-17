#ifndef __CCHARBUFFERHELPER_H__
#define __CCHARBUFFERHELPER_H__

#include "_CCharBufferHelper.h"

CarClass(CCharBufferHelper)
{
public:
    CARAPI constructor();

    /*
     * Creates a char buffer based on a newly allocated char array.
     *
     * @param capacity:
     *      The capacity of then new buffer;
     *
     * @param charBuffer:
     *      The created char buffer interface;
     *
     * @Return NOERROR if successful;
     * @Return E_ILLEGAL_ARGUMENT of capacity is less than zero.
     */
    CARAPI Allocate(
        /* [in]  */ Int32         capacity,
        /* [out] */ ICharBuffer** charBuffer);

    /*
     * Creates a char buffer wrapping the given char array.
     * Calling this interface has the same effect as
     * wrap(array, 0, array.length);
     *
     * @param array:
     *      The char array which the new buffer will be based on;
     *
     * @param charBuffer:
     *      The created char buffer interface;
     *
     * @Return NOERROR if successful;
     */
    CARAPI WrapArray(
        /* [in]  */ ArrayOf<Char32>* array,
        /* [out] */ ICharBuffer**    charBuffer);

    /*
     * Creates a new char buffer by wrapping the given char array.
     *
     * The new buffer's position will be 'start', limit will be
     * 'start + len', capacity will be the length of the array.

     * @Param array:
     *      The char array which the new buffer will be based on;

     * @Param start:
     *      The start index, must not be the negative and not greater
     *      than array's length;

     * @Param len:
     *      The length, must not be negative and not greater than
     *      array' length - start;

     * Return the char buffer is successful, or index out of bound error
     * either 'start' or 'len' is invalid;
     */
    CARAPI WrapArrayEx(
        /*[in]  */  ArrayOf<Char32>* array,
        /*[in]  */  Int32            start,
        /*[in]  */  Int32            len,
        /*[out] */  ICharBuffer**    charBuffer);

    /*
     * Creates a new char buffer by wrapping the given char sequence.

     * Calling this method has the same effect as
     * WrapSequenceEx(charSequence, 0, charSequence.length);

     * @Param charSequence:
     *      The char sequence which the new buffer will be based on;

     * @Param the created char buffer;

     * @Return NOERROR;
     */
    CARAPI WrapSequence(
        /* [in]  */  ICharSequence* charSequence,
        /* [out] */  ICharBuffer**  charBuffer);

    /*
     * Creates a new char buffer based by wrapping the given char sequence.

     * The new buffer's positon will be 'start', limit will be 'end', capacity
     * will be the length of the char sequence. The new buffer is read-only;

     * @Param charSequence:
     *      The char sequence which the new buffer will be based on;

     * @Param start:
     *      The start index, must not be the negative and not greater than
     *      char sequence length;

     * @Param end:
     *      The end index, must be no less than 'start', and no greater than
     *      sequence length;

     * @Param charBuffer:
     *      The created char buffer;

     * @Return NOERROR if successful or index out of bound error when either
     * 'start' or 'end' is invalid;
     */
    CARAPI WrapSequenceEx(
        /* [in]  */ ICharSequence* charSequence,
        /* [in]  */ Int32          start,
        /* [in]  */ Int32          end,
        /* [out] */ ICharBuffer**  charBuffer);
};

#endif // __CCHARBUFFERHELPER_H__
