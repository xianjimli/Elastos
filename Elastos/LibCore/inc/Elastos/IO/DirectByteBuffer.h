#ifndef __DirectByteBuffer_H__
#define __DirectByteBuffer_H__

#include "BaseByteBuffer.h"
#include <elastos/AutoPtr.h>

/**
 * DirectByteBuffer, ReadWriteDirectByteBuffer and ReadOnlyDirectByteBuffer
 * compose the implementation of platform memory based byte buffers.
 * <p>
 * DirectByteBuffer implements all the shared readonly methods and is extended
 * by the other two classes.
 * </p>
 * <p>
 * All methods are marked final for runtime performance.
 * </p>
 *
 */

class ReadWriteDirectByteBuffer;

class DirectByteBuffer : public BaseByteBuffer//implements DirectBuffer
{
friend class ReadWriteDirectByteBuffer;

 public:
     // This class will help us track whether the address is valid or not.
     class SafeAddress {
    public:
        SafeAddress();

    public:
        Boolean mIsValid;

//         AutoPtr<IPlatformAddress> mAddress;

//     protected:
//         SafeAddress(
//             /* [in] */ IPlatformAddress* address);
//         // {
//         //     super();
//         //     this.address = address;
//         // }
     };

public:
    /*
     * Constructs a new direct byte buffer of the given capacity on newly
     * allocated OS memory. The memory will have been zeroed. When the instance
     * is discarded the OS memory will be freed if it has not already been done
     * so by an explicit call to #free(). Callers are encouraged to explicitly
     * free the memory where possible.
     */
    DirectByteBuffer(
    	/* [in] */ Int32 capacity);

    DirectByteBuffer(
    	/* [in] */ SafeAddress* address,
    	/* [in] */ Int32 capacity,
    	/* [in] */ Int32 offset);

    CARAPI GetBytesEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Byte>* dst);

    CARAPI GetByte(
        /* [out] */ Byte* value);

    CARAPI GetByteEx(
        /* [in] */ Int32 index,
        /* [out] */ Byte* value);

    CARAPI GetDouble(
        /* [out] */ Double* value);

    CARAPI GetDoubleEx(
        /* [in] */ Int32 index,
        /* [out] */ Double* value);

    CARAPI GetFloat(
        /* [out] */ Float* value);

    CARAPI GetFloatEx(
        /* [in] */ Int32 index,
        /* [out] */ Float* value);

   	CARAPI GetInt32(
        /* [out] */ Int32* value);

    CARAPI GetInt32Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int32* value);

    CARAPI GetInt64(
        /* [out] */ Int64* value);

    CARAPI GetInt64Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int64* value);

    CARAPI GetInt16(
        /* [out] */ Int16* value);

    CARAPI GetInt16Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int16* value);

   	CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI_(Boolean) IsAddressValid();

    CARAPI AddressValidityCheck();

    /*
     * Returns the base address of the buffer (i.e. before offset).
     */
    // CARAPI GetBaseAddress(
    //     /* [out] */ IPlatformAddress** addr);

    /**
     * Returns the platform address of the start of this buffer instance.
     * <em>You must not attempt to free the returned address!!</em> It may not
     * be an address that was explicitly malloc'ed (i.e. if this buffer is the
     * result of a split); and it may be memory shared by multiple buffers.
     * <p>
     * If you can guarantee that you want to free the underlying memory call the
     * #free() method on this instance -- generally applications will rely on
     * the garbage collector to autofree this memory.
     * </p>
     *
     * @return the effective address of the start of the buffer.
     * @throws IllegalStateException
     *             if this buffer address is known to have been freed
     *             previously.
     */
    // CARAPI GetEffectiveAddress(
    //     /* [out] */ IPlatformAddress** addr);

    /**
     * Explicitly free the memory used by this direct byte buffer. If the memory
     * has already been freed then this is a no-op. Once the memory has been
     * freed then operations requiring access to the memory will throw an
     * <code>IllegalStateException</code>.
     * <p>
     * Note this is is possible that the memory is freed by code that reaches
     * into the address and explicitly frees it 'beneith' us -- this is bad
     * form.
     * </p>
     */
    CARAPI_(void) Free();

    CARAPI_(Int32) GetByteCapacity();

    CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Byte>** array);

    CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI ProtectedHasArray(
        /* [out] */ Boolean* hasArray);

private:
    CARAPI_(void) MarkAddressInvalid();

public:
    // This is a wrapped reference to the base address of the buffer memory.
    SafeAddress* mSafeAddress;

    // This is the offset from the base address at which this buffer logically
    // starts.
    Int32 mOffset;
};

#endif // __DirectByteBuffer_H__
