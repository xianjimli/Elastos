
#include "ext/frameworkext.h"
#include "os/CClassLoader.h"
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

using namespace Elastos::Core;

ECode CClassLoader::LoadClass(
    /* [in] */ const String& className,
    /* [out] */ Handle32* _clazz)
{
    IClassInfo** clazz = (IClassInfo**)_clazz;
    CString modulename("Elastos.Framework.Core.eco");
    String rname("C");
    rname += className.Substring(className.LastIndexOf('.') + 1);

    AutoPtr<IModuleInfo> moduleInfo;
    FAIL_RETURN(CReflector::AcquireModuleInfo(modulename, (IModuleInfo**)&moduleInfo));

    return moduleInfo->GetClassInfo(rname, clazz);
}
