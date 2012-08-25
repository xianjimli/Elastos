
#ifndef __URIPERMISSIONOWNER_H__
#define __URIPERMISSIONOWNER_H__

#include "server/CActivityManagerService.h"
#include "server/UriPermission.h"

class UriPermission;

class UriPermissionOwner
{
public:
    UriPermissionOwner(
        /* [in] */ CActivityManagerService* service,
        /* [in] */ Handle32 owner);

    CARAPI_(void) RemoveUriPermissionsLocked();

    CARAPI_(void) AddReadPermission(
        /* [in] */ UriPermission* perm);

    CARAPI_(void) AddWritePermission(
        /* [in] */ UriPermission* perm);
};

#endif //__URIPERMISSIONOWNER_H__
