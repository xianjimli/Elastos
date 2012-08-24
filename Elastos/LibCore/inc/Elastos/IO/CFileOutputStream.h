
#ifndef __CFILEOUTPUTSTREAM_H__
#define __CFILEOUTPUTSTREAM_H__

#include "_CFileOutputStream.h"
#include "FileOutputStream.h"

CarClass(CFileOutputStream), public FileOutputStream
{
public:
    ~CFileOutputStream();

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

    CARAPI GetChannel(
        /* [out] */ IFileChannel** channel);

    CARAPI GetFD(
        /* [out] */ IFileDescriptor** fd);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI constructor(
        /* [in] */ IFile* file);

    CARAPI constructor(
        /* [in] */ IFile* file,
        /* [in] */ Boolean append);

    CARAPI constructor(
        /* [in] */ IFileDescriptor* fd);

    CARAPI constructor(
        /* [in] */ const String& fileName);

    CARAPI constructor(
        /* [in] */ const String& fileName,
        /* [in] */ Boolean append);

private:
    CARAPI_(Mutex*) GetSelfLock();
};

#endif // __CFILEOUTPUTSTREAM_H__
