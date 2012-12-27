#ifndef __FLOATTOBYTEBUFFERADAPTER_H__
#define __FLOATTOBYTEBUFFERADAPTER_H__

#include "FloatBuffer.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

/**
 * This class wraps a byte buffer to be a float buffer.
 * <p>
 * Implementation notice:
 * <ul>
 * <li>After a byte buffer instance is wrapped, it becomes privately owned by
 * the adapter. It must NOT be accessed outside the adapter any more.</li>
 * <li>The byte buffer's position and limit are NOT linked with the adapter.
 * The adapter extends Buffer, thus has its own position and limit.</li>
 * </ul>
 * </p>
 */
class FloatToByteBufferAdapter
    : public ElRefBase
    , public IFloatBuffer
    , public FloatBuffer//implements DirectBuffer
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
        /* [out] */ IFloatBuffer** buffer);

    FloatToByteBufferAdapter(
        /* [in] */ Int32 capacity,
        /* [in] */ IByteBuffer* byteBuffer);

    CARAPI_(Int32) GetByteCapacity();

//    PlatformAddress getEffectiveAddress();

//    PlatformAddress getBaseAddress();

    CARAPI_(Boolean) IsAddressValid();

    CARAPI_(void) AddressValidityCheck();

    CARAPI_(void) Free();

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Float>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ IFloatBuffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ IFloatBuffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ IFloatBuffer** buffer);

    CARAPI GetFloat(
        /* [out] */ Float* value);

    CARAPI GetFloatEx(
        /* [in] */ Int32 index,
        /* [out] */ Float* value);

    CARAPI GetFloats(
        /* [out] */ ArrayOf<Float>* dst);

    CARAPI GetFloatsEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Float>* dst);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI PutFloat(
        /* [in] */ Float d);

    CARAPI PutFloatEx(
        /* [in] */ Int32 index,
        /* [in] */ Float d);

    CARAPI PutFloats(
        /* [in] */ const ArrayOf<Float>& src);

    CARAPI PutFloatsEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Float>& src);

    CARAPI PutFloatBuffer(
        /* [in] */ IFloatBuffer* src);

    CARAPI Slice(
        /* [out] */ IFloatBuffer** buffer);

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
        /* [out, callee] */ ArrayOf<Float>** array);

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

#endif // __FLOATTOBYTEBUFFERADAPTER_H__