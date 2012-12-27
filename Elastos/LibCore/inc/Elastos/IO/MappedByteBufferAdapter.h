#ifndef __MappedByteBufferAdapter_H__
#define __MappedByteBufferAdapter_H__

#include "MappedByteBuffer.h"
#include <elastos/ElRefBase.h>

/**
 * @hide
 */
class MappedByteBufferAdapter
    : public ElRefBase
    , public IMappedByteBuffer
    , public MappedByteBuffer
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    static CARAPI_(MappedByteBufferAdapter*) Copy(
    	/* [in] */ MappedByteBuffer* other,
    	/* [in] */ Int32 markOfOther);

    MappedByteBufferAdapter(
        /* [in] */ IByteBuffer* buffer);

    MappedByteBufferAdapter(
        /* [in]  IPlatformAddress* address,*/
        /* [in] */ Int32 capa,
        /* [in] */ Int32 offset,
        /* [in] */ FileChannelMapMode mode);

    CARAPI IsLoaded(
        /* [out] */ Boolean* isLoaded);

    CARAPI Load();

    CARAPI Force();

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Byte>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsCharBuffer(
        /* [out] */ ICharBuffer** buffer);

    CARAPI AsDoubleBuffer(
        /* [out] */ IDoubleBuffer** buffer);

    CARAPI AsFloatBuffer(
        /* [out] */ IFloatBuffer** buffer);

    CARAPI AsInt16Buffer(
        /* [out] */ IInt16Buffer** buffer);

    CARAPI AsInt32Buffer(
        /* [out] */ IInt32Buffer** buffer);

    CARAPI AsInt64Buffer(
        /* [out] */ IInt64Buffer** buffer);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ IByteBuffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ IByteBuffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ IByteBuffer** buffer);

    CARAPI GetByte(
        /* [out] */ Byte* value);

    CARAPI GetByteEx(
        /* [in] */ Int32 index,
        /* [out] */ Byte* value);

    CARAPI GetBytes(
        /* [out] */ ArrayOf<Byte>* dst);

    CARAPI GetBytesEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Byte>* dst);

    CARAPI GetChar(
        /* [out] */ Char32* value);

    CARAPI GetCharEx(
        /* [in] */ Int32 index,
        /* [out] */ Char32* value);

    CARAPI GetDouble(
        /* [out] */ Double* value);

    CARAPI GetDoubleEx(
        /* [in] */ Int32 index,
        /* [out] */ Double* value);

//    PlatformAddress getEffectiveAddress();

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

    CARAPI GetOrder(
       /* [out] */ ByteOrder* byteOrder);

    CARAPI SetOrder(
        /* [in] */ ByteOrder byteOrder);

    CARAPI PutByte(
        /* [in] */ Byte b);

    CARAPI PutByteEx(
        /* [in] */ Int32 index,
        /* [in] */ Byte b);

    CARAPI PutBytes(
        /* [in] */ const ArrayOf<Byte>& src);

    CARAPI PutBytesEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Byte>& src);

    CARAPI PutByteBuffer(
        /* [in] */ IByteBuffer* src);

    CARAPI PutChar(
        /* [in] */ Char32 value);

    CARAPI PutCharEx(
        /* [in] */ Int32 index,
        /* [in] */ Char32 value);

    CARAPI PutDouble(
        /* [in] */ Double value);

    CARAPI PutDoubleEx(
        /* [in] */ Int32 index,
        /* [in] */ Double value);

    CARAPI PutFloat(
        /* [in] */ Float value);

    CARAPI PutFloatEx(
        /* [in] */ Int32 index,
        /* [in] */ Float value);

    CARAPI PutInt32(
        /* [in] */ Int32 value);

    CARAPI PutInt32Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int32 value);

    CARAPI PutInt64(
        /* [in] */ Int64 value);

    CARAPI PutInt64Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int64 value);

    CARAPI PutInt16(
        /* [in] */ Int16 value);

    CARAPI PutInt16Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int16 value);

    CARAPI Slice(
       /* [out] */ IByteBuffer** buffer);

    CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Byte>** array);

    CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI ProtectedHasArray(
        /* [out] */ Boolean* hasArray);

//    PlatformAddress getBaseAddress();

    CARAPI_(Boolean) IsAddressValid();

    CARAPI AddressValidityCheck();

    CARAPI_(void) Free();

    CARAPI_(Int32) GetByteCapacity();

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

    CARAPI SetOrderImpl(
        /* [in] */ ByteOrder byteOrder);

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
    static const Int32 CHAR_SIZE = 4;

    static const Int32 INT16_SIZE = 2;

    static const Int32 INT32_SIZE = 4;

    static const Int32 INT64_SIZE = 8;

    static const Int32 FLOAT_SIZE = 4;

    static const Int32 DOUBLE_SIZE = 8;
};

#endif // __MappedByteBufferAdapter_H__