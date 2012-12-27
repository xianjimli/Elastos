#ifndef __DoubleToByteBufferAdapter_H__
#define __DoubleToByteBufferAdapter_H__

#include "DoubleBuffer.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

/**
 * This class wraps a byte buffer to be a double buffer.
 * <p>
 * Implementation notice:
 * <ul>
 * <li>After a byte buffer instance is wrapped, it becomes privately owned by
 * the adapter. It must NOT be accessed outside the adapter any more.</li>
 * <li>The byte buffer's position and limit are NOT linked with the adapter.
 * The adapter extends Buffer, thus has its own position and limit.</li>
 * </ul>
 * </p>
 *
 */
class DoubleToByteBufferAdapter
    : public ElRefBase
    , public IDoubleBuffer
    , public DoubleBuffer//implements DirectBuffer
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    static CARAPI Wrap(
    	/* [in] */ IByteBuffer* byteBuffer,
        /* [out] */ IDoubleBuffer** buffer);

    DoubleToByteBufferAdapter(
        /* [in] */ Int32 capacity,
        /* [in] */ IByteBuffer* byteBuffer);

    CARAPI_(Int32) GetByteCapacity();

//    PlatformAddress getEffectiveAddress();

//    PlatformAddress getBaseAddress();

    CARAPI_(Boolean) IsAddressValid();

    CARAPI_(void) AddressValidityCheck();

    CARAPI_(void) Free();

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Double>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ IDoubleBuffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ IDoubleBuffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ IDoubleBuffer** buffer);

    CARAPI GetDouble(
        /* [out] */ Double* value);

    CARAPI GetDoubleEx(
        /* [in] */ Int32 index,
        /* [out] */ Double* value);

    CARAPI GetDoubles(
        /* [out] */ ArrayOf<Double>* dst);

    CARAPI GetDoublesEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Double>* dst);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI PutDouble(
        /* [in] */ Double d);

    CARAPI PutDoubleEx(
        /* [in] */ Int32 index,
        /* [in] */ Double d);

    CARAPI PutDoubles(
        /* [in] */ const ArrayOf<Double>& src);

    CARAPI PutDoublesEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Double>& src);

    CARAPI PutDoubleBuffer(
        /* [in] */ IDoubleBuffer* src);

    CARAPI Slice(
        /* [out] */ IDoubleBuffer** buffer);

    CARAPI Capacity(
        /* [out] */ Int32* cap);

    CARAPI Clear();

    CARAPI Flip();

    CARAPI HasArray(
        /* [out] */ Boolean* hasArray);

    CARAPI HasRemaining(
        /* [out] */ Boolean* hasRemaining);

    CARAPI IsDirect(
        /* [out] */ Boolean* isDirect);

    CARAPI IsReadOnly(
        /* [out] */ Boolean* isReadOnly);

    CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Double>** array);

    CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI ProtectedHasArray(
        /* [out] */ Boolean* result);

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

private:
    AutoPtr<IByteBuffer> mByteBuffer;
};

#endif // __DoubleToByteBufferAdapter_H__