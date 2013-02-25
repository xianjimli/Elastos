
#include <gesture/CFileGestureLibrary.h>
#include <gesture/CResourceGestureLibrary.h>
#include <gesture/GestureLibraries.h>
#include "ext/frameworkext.h"
#include <Elastos.IO.h>
#include <Logger.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

ECode GestureLibraries::FromFile(
    /* [in] */ const String& path,
    /* [out] */ IGestureLibrary ** gestureLibrary)
{

    IFile *file = NULL;
    CFile::New(path, &file);
    ECode ec = FromFileEx(file, gestureLibrary);
    file->Release();
    return ec;

//    return E_NOT_IMPLEMENTED;
}

ECode GestureLibraries::FromFileEx(
    /* [in] */ IFile * path,
    /* [out] */ IGestureLibrary ** gestureLibrary)
{

    IGestureLibrary *l = NULL;
    ECode ec = CFileGestureLibrary::New(path, &l);
    *gestureLibrary =(IGestureLibrary *) l;
    return ec;

//    return E_NOT_IMPLEMENTED;
}

ECode GestureLibraries::FromPrivateFile(
    /* [in] */ IContext * context,
    /* [in] */ const String& name,
    /* [out] */ IGestureLibrary ** gestureLibrary)
{
    // TODO: Add your code here
    //context.getFileStreamPath not defined
    //return FromFile(context.getFileStreamPath(name), gestureLibrary);
    return E_NOT_IMPLEMENTED;
}

ECode GestureLibraries::FromRawResource(
    /* [in] */ IContext * context,
    /* [in] */ Int32 resourceId,
    /* [out] */ IGestureLibrary ** gestureLibrary)
{

    IGestureLibrary *l = NULL;
    ECode ec = CResourceGestureLibrary::New(context, resourceId, &l);
    *gestureLibrary = (IGestureLibrary *)l;
    return ec;

    //return E_NOT_IMPLEMENTED;
}
