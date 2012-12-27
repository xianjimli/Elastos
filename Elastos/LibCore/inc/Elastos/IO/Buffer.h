#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <elastos.h>
#include <Elastos.IO_server.h>

using namespace Elastos;

extern "C" const InterfaceID EIID_Buffer;

/**
 * A buffer is a list of elements of a specific primitive type.
 * <p>
 * A buffer can be described by the following properties:
 * <ul>
 * <li>Capacity: the number of elements a buffer can hold. Capacity may not be
 * negative and never changes.</li>
 * <li>Position: a cursor of this buffer. Elements are read or written at the
 * position if you do not specify an index explicitly. Position may not be
 * negative and not greater than the limit.</li>
 * <li>Limit: controls the scope of accessible elements. You can only read or
 * write elements from index zero to <code>limit - 1</code>. Accessing
 * elements out of the scope will cause an exception. Limit may not be negative
 * and not greater than capacity.</li>
 * <li>Mark: used to remember the current position, so that you can reset the
 * position later. Mark may not be negative and no greater than position.</li>
 * <li>A buffer can be read-only or read-write. Trying to modify the elements
 * of a read-only buffer will cause a <code>ReadOnlyBufferException</code>,
 * while changing the position, limit and mark of a read-only buffer is OK.</li>
 * <li>A buffer can be direct or indirect. A direct buffer will try its best to
 * take advantage of native memory APIs and it may not stay in the Java heap,
 * thus it is not affected by garbage collection.</li>
 * </ul>
 * <p>
 * Buffers are not thread-safe. If concurrent access to a buffer instance is
 * required, then the callers are responsible to take care of the
 * synchronization issues.
 */
class Buffer
{
public:
    /**
     * Construct a buffer with the specified capacity.
     *
     * @param capacity
     *            The capacity of this buffer
     */
    Buffer(
        /* [in] */ Int32 capacity);

//    virtual CARAPI Array(
//        /* [out] */ IInterface** array) = 0;

    virtual CARAPI ArrayOffset(
       /* [out] */ Int32* offset) = 0;

    CARAPI Capacity(
        /* [out] */ Int32* cap);

    CARAPI Clear();

    CARAPI Flip();

    virtual CARAPI HasArray(
        /* [out] */ Boolean* hasArray) = 0;

    CARAPI HasRemaining(
        /* [out] */ Boolean* hasRemaining);

    virtual CARAPI IsDirect(
        /* [out] */ Boolean* isDirect) = 0;

    virtual CARAPI IsReadOnly(
        /* [out] */ Boolean* isReadOnly) = 0;

    CARAPI GetLimit(
        /* [out] */ Int32* limit);

    CARAPI SetLimit(
        /* [in] */ Int32 newLimit);

    CARAPI Mark();

    CARAPI GetPosition(
        /* [out] */ Int32* pos);

    CARAPI SetPosition(
        /* [in] */ Int32 newPosition);

    CARAPI Remaining(
        /* [out] */ Int32* remaining);

    CARAPI Reset();

    CARAPI Rewind();

public:
    /**
     * The capacity of this buffer, which never changes.
     */
    Int32 mCapacity;

    /**
     * <code>limit - 1</code> is the last element that can be read or written.
     * Limit must be no less than zero and no greater than <code>capacity</code>.
     */
    Int32 mLimit;

    /**
     * Mark is where position will be set when <code>reset()</code> is called.
     * Mark is not set by default. Mark is always no less than zero and no
     * greater than <code>position</code>.
     */
    Int32 mMark;

    /**
     * The current position of this buffer. Position is always no less than zero
     * and no greater than <code>limit</code>.
     */
    Int32 mPosition;

    // BEGIN android-added
    /**
     * The log base 2 of the element size of this buffer.  Each typed subclass
     * (ByteBuffer, CharBuffer, etc.) is responsible for initializing this
     * value.  The value is used by JNI code in frameworks/base/ to avoid the
     * need for costly 'instanceof' tests.
     */
    Int32 mElementSizeShift;

    /**
     * For direct buffers, the effective address of the data.  This is set
     * on first use.  If the field is zero, this is either not a direct
     * buffer or the field has not been initialized, and you need to issue
     * the getEffectiveAddress() call and use the result of that.
     *
     * This is an optimization used by the GetDirectBufferAddress JNI call.
     */
    Int32 mEffectiveDirectAddress;
};

#endif // __BUFFER_H__