#ifndef __Int64ToByteBufferAdapter_H__
#define __Int64ToByteBufferAdapter_H__

#include "Int64Buffer.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

/**
 * This class wraps a byte buffer to be a long buffer.
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
class Int64ToByteBufferAdapter
    : public ElRefBase
    , public IInt64Buffer
    , public Int64Buffer//implements DirectBuffer
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
        /* [out] */ IInt64Buffer** buffer);

    Int64ToByteBufferAdapter(
        /* [in] */ Int32 capacity,
        /* [in] */ IByteBuffer* byteBuffer);

    CARAPI_(Int32) GetByteCapacity();

//    PlatformAddress getEffectiveAddress();

//    PlatformAddress getBaseAddress();

    CARAPI_(Boolean) IsAddressValid();

    CARAPI_(void) AddressValidityCheck();

    CARAPI_(void) Free();

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Int64>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ IInt64Buffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ IInt64Buffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ IInt64Buffer** buffer);

    CARAPI GetInt64(
        /* [out] */ Int64* value);

    CARAPI GetInt64Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int64* value);

    CARAPI GetInt64s(
        /* [out] */ ArrayOf<Int64>* dst);

    CARAPI GetInt64sEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Int64>* dst);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI PutInt64(
        /* [in] */ Int64 d);

    CARAPI PutInt64Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int64 d);

    CARAPI PutInt64s(
        /* [in] */ const ArrayOf<Int64>& src);

    CARAPI PutInt64sEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Int64>& src);

    CARAPI PutInt64Buffer(
        /* [in] */ IInt64Buffer* src);

    CARAPI Slice(
        /* [out] */ IInt64Buffer** buffer);

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
        /* [out, callee] */ ArrayOf<Int64>** array);

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

#endif // __Int64ToByteBufferAdapter_H__