
#include "os/CParcelFileDescriptor.h"


CParcelFileDescriptor::CParcelFileDescriptor()
    : mClosed(FALSE)
{
}

ECode CParcelFileDescriptor::Open(
    /* [in] */ IFile* file,
    /* [in] */ Int32 mode,
    /* [out] */ IParcelFileDescriptor** descriptor)
{
    assert(file != NULL);
    String path;
    file->GetPath(&path);

    //TODO
    // SecurityManager security = System.getSecurityManager();
    // if (security != NULL) {
    //     security.checkRead(path);
    //     if ((mode&MODE_WRITE_ONLY) != 0) {
    //         security.checkWrite(path);
    //     }
    // }

    if ((mode & ParcelFileDescriptor_MODE_READ_WRITE) == 0) {
        assert(0);
        // throw new IllegalArgumentException(
        //         "Must specify MODE_READ_ONLY, MODE_WRITE_ONLY, or MODE_READ_WRITE");
    }

    //TODO
    assert(0);
    // AutoPtr<IFileDescriptor> fd = Parcel.openFileDescriptor(path, mode);
    // return fd != NULL ? new ParcelFileDescriptor(fd) : NULL;
    return NOERROR;
}

ECode CParcelFileDescriptor::Close()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CParcelFileDescriptor::GetFileDescriptor(
    /* [out] */ IFileDescriptor** des)
{
    assert(des != NULL);
    *des = mFileDescriptor.Get();
    if (*des != NULL) {
        (*des)->AddRef();
    }

    return NOERROR;
}

ECode CParcelFileDescriptor::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CParcelFileDescriptor::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CParcelFileDescriptor::constructor(
    /* [in] */ IParcelFileDescriptor* descriptor)
{
    //super();
    mParcelDescriptor = descriptor;
    assert(mParcelDescriptor != NULL);
    mFileDescriptor = ((CParcelFileDescriptor*)mParcelDescriptor.Get())->mFileDescriptor;

    return NOERROR;
}

ECode CParcelFileDescriptor::constructor(
    /* [in] */ IFileDescriptor* descriptor)
{
    //super();
    if (descriptor == NULL) {
        assert(0);
        //throw new NullPointerException("descriptor must not be null");
    }

    mFileDescriptor = descriptor;
    mParcelDescriptor = NULL;

    return NOERROR;
}
