#ifndef __CharTOBYTEBUFFERADAPTER_H__
#define __CharTOBYTEBUFFERADAPTER_H__

#include "CharBuffer.h"
#include <elastos/ElRefBase.h>
#include <elastos/AutoPtr.h>

/**
 * This class wraps a byte buffer to be a Char buffer.
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
class CharToByteBufferAdapter
    : public ElRefBase
    , public ICharBuffer
    , public ICharSequence
    , public CharBuffer//implements DirectBuffer
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
        /* [out] */ ICharBuffer** buffer);

    CharToByteBufferAdapter(
        /* [in] */ Int32 capacity,
        /* [in] */ IByteBuffer* byteBuffer);

    CARAPI_(Int32) GetByteCapacity();

//    PlatformAddress getEffectiveAddress();

//    PlatformAddress getBaseAddress();

    CARAPI_(Boolean) IsAddressValid();

    CARAPI_(void) AddressValidityCheck();

    CARAPI_(void) Free();

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Char32>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ ICharBuffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ ICharBuffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ ICharBuffer** buffer);

    CARAPI GetChar(
        /* [out] */ Char32* value);

    CARAPI GetCharEx(
        /* [in] */ Int32 index,
        /* [out] */ Char32* value);

    CARAPI GetChars(
        /* [out] */ ArrayOf<Char32>* dst);

    CARAPI GetCharsEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Char32>* dst);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* order);

    CARAPI ProtectedArray(
        /* [out, callee] */ ArrayOf<Char32>** array);

    CARAPI ProtectedArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI ProtectedHasArray(
        /* [out] */ Boolean* result);

    CARAPI PutChar(
        /* [in] */ Char32 c);

    CARAPI PutCharEx(
        /* [in] */ Int32 index,
        /* [in] */ Char32 c);

    CARAPI PutChars(
        /* [in] */ const ArrayOf<Char32>& src);

    CARAPI PutCharsEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Char32>& src);

    CARAPI PutCharBuffer(
        /* [in] */ ICharBuffer* src);

    CARAPI PutString(
        /* [in] */ const String& str);

    CARAPI PutStringEx(
        /* [in] */ const String& str,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI Slice(
        /* [out] */ ICharBuffer** buffer);

    CARAPI AppendChar(
        /* [in] */ Char32 c);

    CARAPI AppendChars(
        /* [in] */ ICharSequence* csq);

    CARAPI AppendCharsEx(
        /* [in] */ ICharSequence* csq,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI Read(
        /* [in] */ ICharBuffer* target,
        /* [out] */ Int32* number);

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

    CARAPI GetLength(
        /* [out] */ Int32* number);

    CARAPI GetCharAt(
        /* [in] */ Int32 index,
        /* [out] */ Char32* c);

    CARAPI SubSequence(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ICharSequence** csq);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    AutoPtr<IByteBuffer> mByteBuffer;
};

#endif // __CharTOBYTEBUFFERADAPTER_H__