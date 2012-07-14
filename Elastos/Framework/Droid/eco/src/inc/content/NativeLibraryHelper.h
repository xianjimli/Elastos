
#ifndef __HH_NATIVELIBRARYHELPER_H
#define __HH_NATIVELIBRARYHELPER_H

#include "ext/frameworkext.h"

class NativeLibraryHelper
{
public:
    // Convenience method to call RemoveNativeBinariesFromDirLI(File)
    static CARAPI_(Boolean) RemoveNativeBinariesLI(
        /* [in] */ const String& nativeLibraryPath) { return FALSE; }

    // Remove the native binaries of a given package. This simply
    // gets rid of the files in the 'lib' sub-directory.
    static CARAPI_(Boolean) RemoveNativeBinariesFromDirLI(
        /* [in] */ IFile* nativeLibraryDir) { return FALSE; }

    static CARAPI_(Int32) CopyNativeBinariesLI(
        /* [in] */ IFile* scanFile,
        /* [in] */ IFile* sharedLibraryDir) { return -1; }
};

#endif // __HH_NATIVELIBRARYHELPER_H

