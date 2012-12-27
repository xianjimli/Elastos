#ifndef __BUFFERFACTORY_H__
#define __BUFFERFACTORY_H__

#include <elastos.h>
#include <Elastos.IO_server.h>

using namespace Elastos;

/**
 * Provide factory service of buffer classes.
 * <p>
 * Since all buffer impl classes are package private (except DirectByteBuffer),
 * this factory is the only entrance to access buffer functions from outside of
 * the impl package.
 * </p>
 */
class BufferFactory
{
public:
    /**
     * Returns a new byte buffer based on the specified byte array.
     *
     * @param array
     *            The byte array
     * @return A new byte buffer based on the specified byte array.
     */
    static CARAPI NewByteBuffer(
        /* [in] */ ArrayOf<Byte>* array,
        /* [out] */ IByteBuffer** buf);

    /**
     * Returns a new array based byte buffer with the specified capacity.
     *
     * @param capacity
     *            The capacity of the new buffer
     * @return A new array based byte buffer with the specified capacity.
     */
    static CARAPI NewByteBufferEx(
        /* [in] */ Int32 capacity,
        /* [out] */ IByteBuffer** buf);

    /**
     * Returns a new char buffer based on the specified char array.
     *
     * @param array
     *            The char array
     * @return A new char buffer based on the specified char array.
     */
    static CARAPI NewCharBuffer(
        /* [in] */ ArrayOf<Char32>* array,
        /* [out] */ ICharBuffer** buf);

    /**
     * Returns a new readonly char buffer based on the specified char sequence.
     *
     * @param chseq
     *            The char sequence
     * @return A new readonly char buffer based on the specified char sequence.
     */
    static CARAPI NewCharBufferEx(
        /* [in] */ ICharSequence* chseq,
        /* [out] */ ICharBuffer** buf);

    /**
     * Returns a new array based char buffer with the specified capacity.
     *
     * @param capacity
     *            The capacity of the new buffer
     * @return A new array based char buffer with the specified capacity.
     */
    static CARAPI NewCharBufferEx2(
        /* [in] */ Int32 capacity,
        /* [out] */ ICharBuffer** buf);

    /**
     * Returns a new direct byte buffer with the specified capacity.
     *
     * @param capacity
     *            The capacity of the new buffer
     * @return A new direct byte buffer with the specified capacity.
     */
    static CARAPI NewDirectByteBuffer(
        /* [in] */ Int32 capacity,
        /* [out] */ IByteBuffer** buf);

    /**
     * Returns a new double buffer based on the specified double array.
     *
     * @param array
     *            The double array
     * @return A new double buffer based on the specified double array.
     */
    static CARAPI NewDoubleBuffer(
        /* [in] */ ArrayOf<Double>* array,
        /* [out] */ IDoubleBuffer** buf);

    /**
     * Returns a new array based double buffer with the specified capacity.
     *
     * @param capacity
     *            The capacity of the new buffer
     * @return A new array based double buffer with the specified capacity.
     */
    static CARAPI NewDoubleBufferEx(
        /* [in] */ Int32 capacity,
        /* [out] */ IDoubleBuffer** buf);

    /**
     * Returns a new float buffer based on the specified float array.
     *
     * @param array
     *            The float array
     * @return A new float buffer based on the specified float array.
     */
    static CARAPI NewFloatBuffer(
        /* [in] */ ArrayOf<Float>* array,
        /* [out] */ IFloatBuffer** buf);

    /**
     * Returns a new array based float buffer with the specified capacity.
     *
     * @param capacity
     *            The capacity of the new buffer
     * @return A new array based float buffer with the specified capacity.
     */
    static CARAPI NewFloatBufferEx(
        /* [in] */ Int32 capacity,
        /* [out] */ IFloatBuffer** buf);

    /**
     * Returns a new array based int buffer with the specified capacity.
     *
     * @param capacity
     *            The capacity of the new buffer
     * @return A new array based int buffer with the specified capacity.
     */
    static CARAPI NewInt32Buffer(
        /* [in] */ Int32 capacity,
        /* [out] */ IInt32Buffer** buf);

    /**
     * Returns a new int buffer based on the specified int array.
     *
     * @param array
     *            The int array
     * @return A new int buffer based on the specified int array.
     */
    static CARAPI NewInt32BufferEx(
        /* [in] */ ArrayOf<Int32>* array,
        /* [out] */ IInt32Buffer** buf);

    /**
     * Returns a new array based long buffer with the specified capacity.
     *
     * @param capacity
     *            The capacity of the new buffer
     * @return A new array based long buffer with the specified capacity.
     */
    static CARAPI NewInt64Buffer(
        /* [in] */ Int32 capacity,
        /* [out] */ IInt64Buffer** buf);

    /**
     * Returns a new long buffer based on the specified long array.
     *
     * @param array
     *            The long array
     * @return A new long buffer based on the specified long array.
     */
    static CARAPI NewInt64BufferEx(
        /* [in] */ ArrayOf<Int64>* array,
        /* [out] */ IInt64Buffer** buf);

    /**
     * Returns a new array based short buffer with the specified capacity.
     *
     * @param capacity
     *            The capacity of the new buffer
     * @return A new array based short buffer with the specified capacity.
     */
    static CARAPI NewInt16Buffer(
        /* [in] */ Int32 capacity,
        /* [out] */ IInt16Buffer** buf);

    /**
     * Returns a new short buffer based on the specified short array.
     *
     * @param array
     *            The short array
     * @return A new short buffer based on the specified short array.
     */
    static CARAPI NewInt16BufferEx(
        /* [in] */ ArrayOf<Int16>* array,
        /* [out] */ IInt16Buffer** buf);
};

#endif // __BUFFERFACTORY_H__