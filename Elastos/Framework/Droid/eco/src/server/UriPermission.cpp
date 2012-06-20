
#include "server/CActivityManagerService.h"
#include "server/UriPermission.h"

UriPermission::UriPermission(
    /* [in] */ Int32 uid,
    /* [in] */ IUri* uri) :
    mUid(uid),
    mUri(uri),
    mModeFlags(0),
    mGlobalModeFlags(0)
{
}
