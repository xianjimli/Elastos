#include "cmdef.h"
#include "CFileDescriptor.h"
#include <unistd.h>

AutoPtr<IFileDescriptor> CreateFD(
    /* [in] */ Int32 descriptor)
{
    AutoPtr<IFileDescriptor> fd;
    CFileDescriptor::New((IFileDescriptor**)&fd);
    ((CFileDescriptor*)fd.Get())->mDescriptor = descriptor;

    return fd;
}

const AutoPtr<IFileDescriptor> CFileDescriptor::in = CreateFD(0);
const AutoPtr<IFileDescriptor> CFileDescriptor::out = CreateFD(1);
const AutoPtr<IFileDescriptor> CFileDescriptor::err = CreateFD(2);

CFileDescriptor::CFileDescriptor()
    : mDescriptor(-1)
    , mReadOnly(FALSE)
{
}

ECode CFileDescriptor::Sync()
{
    // if the descriptor is a read-only one, do nothing
    if (!mReadOnly) {
        SyncImpl();
    }
    return NOERROR;
}

ECode CFileDescriptor::SyncImpl()
{
    Int32 fd = 123;/////////////////////////////////////////////E_NOT_IMPLEMENTED
    Int32 rc = fsync(fd);
//    if (rc == -1) {
//
//    }
    return NOERROR;
}

ECode CFileDescriptor::Valid(
    /* [out] */ Boolean* isValid)
{
    VALIDATE_NOT_NULL(isValid);

    *isValid = mDescriptor != -1;
    return NOERROR;
}

ECode CFileDescriptor::ToString(
    /* [out] */ String* str)
{
    /////////////////////////////////////     E_NOT_IMPLEMENTED
    return E_NOT_IMPLEMENTED;
}
