
#include "cmdef.h"
#include "Droid.System.h"
#include "CPlatform.h"
#include "OSFileSystem.h"
#include "WrappedFileSystem.h"

//private static final IFileSystem FILE_SYSTEM =
//            new BlockGuard.WrappedFileSystem(OSFileSystem.getOSFileSystem());

CPlatform::CPlatform()
{
    FILE_SYSTEM = new WrappedFileSystem(OSFileSystem::GetOSFileSystem());
}

/**
 * Checks to ensure that whoever is asking for the OS component is running
 * on the system classpath.
 */
ECode CPlatform::AccessCheck()
{
//    if (VMStack.getCallingClassLoader() != null) {
//        throw new SecurityException();
//    }
    return NOERROR;
}

ECode CPlatform::GetFileSystem(
    /* [out] */ IFileSystem** fs)
{
    VALIDATE_NOT_NULL(fs);
    AccessCheck();
    *fs = FILE_SYSTEM;
    (*fs)->AddRef();
    return NOERROR;
}
