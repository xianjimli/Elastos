#ifndef __READWRITEInt16ARRAYBUFFER_H__
#define __READWRITEInt16ARRAYBUFFER_H__

#include "Int16ArrayBuffer.h"
#include <elastos/ElRefBase.h>

/**
 * ShortArrayBuffer, ReadWriteShortArrayBuffer and ReadOnlyShortArrayBuffer
 * compose the implementation of array based short buffers.
 * <p>
 * ReadWriteShortArrayBuffer extends ShortArrayBuffer with all the write
 * methods.
 * </p>
 * <p>
 * This class is marked final for runtime performance.
 * </p>
 *
 */
class ReadWriteInt16ArrayBuffer
    : public ElRefBase
    , public IInt16Buffer
    , public Int16ArrayBuffer
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    static CARAPI_(ReadWriteInt16ArrayBuffer*) Copy(
    	/* [in] */ Int16ArrayBuffer* other,
    	/* [in] */ Int32 markOfOther);

    ReadWriteInt16ArrayBuffer(
        /* [in] */ ArrayOf<Int16>* array);

    ReadWriteInt16ArrayBuffer(
        /* [in] */ Int32 capacity);

    ReadWriteInt16ArrayBuffer(
        /* [in] */ Int32 capacity,
    	/* [in] */ ArrayOf<Int16>* backingArray,
    	/* [in] */ Int32 arrayOffset);

    CARAPI Array(
        /* [out, callee] */ ArrayOf<Int16>** array);

    CARAPI ArrayOffset(
        /* [out] */ Int32* offset);

    CARAPI AsReadOnlyBuffer(
        /* [out] */ IInt16Buffer** buffer);

    CARAPI Compact();

    CARAPI CompareTo(
        /* [in] */ IInt16Buffer* otherBuffer,
        /* [out] */ Int32* result);

    CARAPI Duplicate(
        /* [out] */ IInt16Buffer** buffer);

    CARAPI GetInt16(
        /* [out] */ Int16* value);

    CARAPI GetInt16Ex(
        /* [in] */ Int32 index,
        /* [out] */ Int16* value);

    CARAPI GetInt16s(
        /* [out] */ ArrayOf<Int16>* dst);

    CARAPI GetInt16sEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [out] */ ArrayOf<Int16>* dst);

    CARAPI GetOrder(
        /* [out] */ ByteOrder* byteOrder);

    CARAPI PutInt16(
        /* [in] */ Int16 d);

    CARAPI PutInt16Ex(
        /* [in] */ Int32 index,
        /* [in] */ Int16 d);

    CARAPI PutInt16s(
        /* [in] */ const ArrayOf<Int16>& src);

    CARAPI PutInt16sEx(
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ const ArrayOf<Int16>& src);

    CARAPI PutInt16Buffer(
        /* [in] */ IInt16Buffer* src);

    CARAPI Slice(
        /* [out] */ IInt16Buffer** buffer);

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
        /* [out, callee] */ ArrayOf<Int16>** array);

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
};

#endif // __READWRITEInt16ARRAYBUFFER_H__