
#include <Elastos.Core.h>
#include "elastos/System.h"
#include "cmdef.h"
#include <stdlib.h>

namespace Elastos {
namespace Core {

Int64 System::GetCurrentTimeMillis()
{
    // we don't support the clocks here.
    struct timeval t;
    t.tv_sec = t.tv_usec = 0;
    gettimeofday(&t, NULL);
    return ((Int64)t.tv_sec * 1000 + (Int64)t.tv_usec / 1000.0);
}

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
