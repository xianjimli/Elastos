
#include <Elastos.Core.h>
#include "elastos/System.h"
#include "cmdef.h"
#include <stdlib.h>

namespace Elastos {
namespace Core {

ECode System::GetEnv(
    /* [in] */ CString name,
    /* [out] */ String* env)
{
    VALIDATE_NOT_NULL(env);

    if (name.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
//    SecurityManager secMgr = System.getSecurityManager();
//    if (secMgr != null) {
//        secMgr.checkPermission(new RuntimePermission("getenv." + name));
//    }

    *env = GetEnvByName(name);
    return NOERROR;
}

String System::GetEnvByName(
    /* [in] */ CString name)
{
    if (name.IsNull()) {
        return String(NULL);
    }
    return String(getenv(name));
}

} //Core
} //Elastos
