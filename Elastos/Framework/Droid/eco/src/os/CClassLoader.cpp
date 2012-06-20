
#include "ext/frameworkext.h"
#include "os/CClassLoader.h"
#include <elastos/AutoPtr.h>
#include <StringBuffer.h>

using namespace Elastos::System;

ECode CClassLoader::LoadClass(
    /* [in] */ String className,
    /* [out] */ Handle32* _clazz)
{
    String modulename = "Elastos.Framework.eco";

    IClassInfo** clazz = (IClassInfo**)_clazz;
    StringBuffer _name("C");

    Int32 length = className.GetLength();
    StringBuf* buf = StringBuf::Alloc(length + 1);
    className.Substring(className.LastIndexOf('.') + 1, length, *buf);
    _name += buf->GetPayload();
    StringBuf::Free(buf);

    AutoPtr<IModuleInfo> moduleInfo;
    FAIL_RETURN(CReflector::AcquireModuleInfo(modulename, (IModuleInfo**)&moduleInfo));

    return moduleInfo->GetClassInfo(_name, clazz);
}
