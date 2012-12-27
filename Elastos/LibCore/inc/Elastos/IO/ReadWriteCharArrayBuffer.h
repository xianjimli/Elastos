#ifndef __ReadWriteCharArrayBuffer_H__
#define __ReadWriteCharArrayBuffer_H__

#include "CharArrayBuffer.h"
#include <elastos/ElRefBase.h>

/**
 * CharArrayBuffer, ReadWriteCharArrayBuffer and ReadOnlyCharArrayBuffer compose
 * the implementation of array based char buffers.
 * <p>
 * ReadWriteCharArrayBuffer extends CharArrayBuffer with all the write methods.
 * </p>
 * <p>
 * This class is marked final for runtime performance.
 * </p>
 *
 */
class ReadWriteCharArrayBuffer
    : public ElRefBase
    , public ICharBuffer
    , public ICharSequence
    , public CharArrayBuffer
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    static CARAPI_(ReadWriteCharArrayBuffer*) Copy(
    	/* [in] */ CharArrayBuffer* other,
    	/* [in] */ Int32 markOfOther);

    ReadWriteCharArrayBuffer(
    	/* [in] */ ArrayOf<Char32>* array);

    ReadWriteCharArrayBuffer(
    	/* [in] */ Int32 capacity);

    ReadWriteCharArrayBuffer(
        /* [in] */ Int32 capacity,
    	/* [in] */ ArrayOf<Char32>* backingArray,
    	/* [in] */ Int32 arrayOffset);

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

};

#endif // __ReadWriteCharArrayBuffer_H__