
#ifndef __HH_ENVIRONMENT_H
#define __HH_ENVIRONMENT_H

#include "ext/frameworkext.h"
#include <elastos.h>

using namespace Elastos;

class Environment
{
public:
    /**
     * Gets the Android root directory.
     */
    static CARAPI GetRootDirectory(
        /* [out] */ IFile** file);

    /**
     * Gets the Android data directory.
     */
    static CARAPI GetDataDirectory(
        /* [out] */ IFile** file);

    static CARAPI_(Boolean) IsEncryptedFilesystemEnabled() { return FALSE; }
};

#endif // __HH_ENVIRONMENT_H
