#ifndef __CDATAOUTPUTSTREAM_H__
#define __CDATAOUTPUTSTREAM_H__

#include "_CDataOutputStream.h"
#include "DataOutputStream.h"

using namespace Elastos;

CarClass(CDataOutputStream), public DataOutputStream
{
public:
    CDataOutputStream();

    ~CDataOutputStream();

    CARAPI constructor(
        /* [in] */ IOutputStream* out);

    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBuffer(
        /* [in] */ const ArrayOf<Byte> & buffer);

    CARAPI WriteBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Byte> & buffer);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI WriteBoolean(
        /* [in] */ Boolean val);

    CARAPI WriteByte(
        /* [in] */ Int32 val);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBytesEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteChar(
        /* [in] */ Int32 val);

    CARAPI WriteDouble(
        /* [in] */ Double val);

    CARAPI WriteFloat(
        /* [in] */ Float val);

    CARAPI WriteInt32(
        /* [in] */ Int32 val);

    CARAPI WriteInt64(
        /* [in] */ Int64 val);

    CARAPI WriteInt16(
        /* [in] */ Int32 val);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif