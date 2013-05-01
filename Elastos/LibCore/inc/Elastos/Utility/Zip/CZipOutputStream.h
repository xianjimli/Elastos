
#ifndef __CZIPOUTPUTSTREAM_H__
#define __CZIPOUTPUTSTREAM_H__


#include "_CZipOutputStream.h"
#include "ZipOutputStream.h"


CarClass(CZipOutputStream), public ZipOutputStream
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

    CARAPI Finish();

    CARAPI CloseEntry();

    CARAPI PutNextEntry(
        /* [in] */ IZipEntry* ze);

    CARAPI SetComment(
        /* [in] */ const String& comment);

    CARAPI SetLevel(
        /* [in] */ Int32 level);

    CARAPI SetMethod(
        /* [in] */ Int32 method);

    /**
     * Constructs a new {@code ZipOutputStream} with the specified output
     * stream.
     *
     * @param p1
     *            the {@code OutputStream} to write the data to.
     */
    CARAPI constructor(
        /* [in] */ IOutputStream* os);
};

#endif //__CZIPOUTPUTSTREAM_H__
