
#ifndef __URIPERMISSION_H__
#define __URIPERMISSION_H__

#include "ext/frameworkext.h"
#include "server/UriPermissionOwner.h"
#include <elastos/AutoPtr.h>
#include <elastos/Set.h>

using namespace Elastos;

class UriPermissionOwner;

class UriPermission
{
public:
    UriPermission(
        /* [in] */ Int32 uid,
        /* [in] */ IUri* uri);

    CARAPI_(void) ClearModes(
        /* [in] */ Int32 modeFlagsToClear);

    CARAPI_(String) ToString();

    CARAPI_(void) Dump(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ String prefix);

public:
    Int32 mUid;
    AutoPtr<IUri> mUri;
    Int32 mModeFlags;
    Int32 mGlobalModeFlags;
    Set<UriPermissionOwner*> mReadOwners;
    Set<UriPermissionOwner*> mWriteOwners;
    String mStringName;
};

#endif //__URIPERMISSION_H__
