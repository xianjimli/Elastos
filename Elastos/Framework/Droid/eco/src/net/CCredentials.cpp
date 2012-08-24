
#include "net/CCredentials.h"
#include "ext/frameworkdef.h"

ECode CCredentials::GetPid(
    /* [out] */ Int32* pid)
{
    VALIDATE_NOT_NULL(pid);

    *pid = mPid;
    return NOERROR;
}

ECode CCredentials::GetUid(
    /* [out] */ Int32* uid)
{
    VALIDATE_NOT_NULL(uid);

    *uid = mUid;
    return NOERROR;
}

ECode CCredentials::GetGid(
    /* [out] */ Int32* gid)
{
    VALIDATE_NOT_NULL(gid);

    *gid = mGid;
    return NOERROR;
}

ECode CCredentials::constructor(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    mPid = pid;
    mUid = uid;
    mGid = gid;
    return NOERROR;
}