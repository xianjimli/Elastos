
#ifndef __GESTURELIBRARIES_H__
#define __GESTURELIBRARIES_H__

//#include "_CGestureLibraries.h"
#include <gesture/GestureLibrary.h>
#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;
using namespace Elastos::Core;

class GestureLibraries
{
public:
    static CARAPI FromFile(
        /* [in] */ const String& path,
        /* [out] */ IGestureLibrary ** gestureLibrary);

    static CARAPI FromFileEx(
        /* [in] */ IFile * path,
        /* [out] */ IGestureLibrary ** gestureLibrary);

    static CARAPI FromPrivateFile(
        /* [in] */ IContext * context,
        /* [in] */ const String& name,
        /* [out] */ IGestureLibrary ** gestureLibrary);

    static CARAPI FromRawResource(
        /* [in] */ IContext * context,
        /* [in] */ Int32 resourceId,
        /* [out] */ IGestureLibrary ** gestureLibrary);

};

#endif // __GESTURELIBRARIES_H__
