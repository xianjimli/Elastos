
#ifndef __CBYTEARRAYOUTPUTSTREAM_H__
#define __CBYTEARRAYOUTPUTSTREAM_H__

#include "_CByteArrayOutputStream.h"
#include "ByteArrayOutputStream.h"

CarClass(CByteArrayOutputStream), public ByteArrayOutputStream
{
public:
    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBuffer(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI Reset();

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI ToByteArray(
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI ToStringEx(
        /* [in] */ const String& enc,
        /* [out] */ String* str);

    CARAPI WriteTo(
        /* [in] */ IOutputStream* os);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 size);
protected:
    CARAPI WriteLocked(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBufferLocked(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBufferExLocked(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI ResetLocked();

    CARAPI ToByteArrayLocked(
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    CARAPI WriteToLocked(
        /* [in] */ IOutputStream* os);
private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __CBYTEARRAYOUTPUTSTREAM_H__
