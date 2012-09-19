
#ifndef __CZIPINPUTSTREAM_H__
#define __CZIPINPUTSTREAM_H__


#include "_CZipInputStream.h"
#include "ZipInputStream.h"


CarClass(CZipInputStream), public ZipInputStream
{
public:
    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBuffer(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBufferEx(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI CloseEntry();

    CARAPI GetNextEntry(
        /* [out] */ IZipEntry** entry);

    /**
     * Constructs a new {@code ZipInputStream} from the specified input stream.
     *
     * @param stream
     *            the input stream to representing a ZIP archive.
     */
    CARAPI constructor(
        /* [in] */ IInputStream* stream);
};


#endif //__CZIPINPUTSTREAM_H__
