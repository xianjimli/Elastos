
#ifndef __URIPERMISSIONOWNER_H__
#define __URIPERMISSIONOWNER_H__

#include "server/CActivityManagerService.h"
#include "server/UriPermission.h"
#include "os/Binder.h"

class UriPermission;

class UriPermissionOwner
{
public:
    class ExternalToken : public Binder
    {
    public:
        ExternalToken(
        /* [in] */ UriPermissionOwner* uriOwner);

        CARAPI_(UriPermissionOwner*) GetOwner();

    private:
        UriPermissionOwner* mUriOwner;
    };

public:
    UriPermissionOwner(
        /* [in] */ CActivityManagerService* service,
        /* [in] */ Handle32 owner);

    CARAPI_(Binder*) GetExternalTokenLocked();

    CARAPI_(UriPermissionOwner*) FromExternalToken(
        /* [in] */ Binder* token);

    CARAPI_(void) RemoveUriPermissionsLocked();

    CARAPI_(void) RemoveUriPermissionsLocked(
        /* [in] */ Int32 mode);

    CARAPI_(void) RemoveUriPermissionLocked(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 mode);

    CARAPI_(void) AddReadPermission(
        /* [in] */ UriPermission* perm);

    CARAPI_(void) AddWritePermission(
        /* [in] */ UriPermission* perm);

    CARAPI_(void) RemoveReadPermission(
        /* [in] */ UriPermission* perm);

    CARAPI_(void) RemoveWritePermission(
        /* [in] */ UriPermission* perm);

public:
    CActivityManagerService* mService;
    IObject* mOwner;
    Binder* mExternalToken;
    Set<UriPermission*>* mReadUriPermissions; // special access to reading uris.
    Set<UriPermission*>* mWriteUriPermissions; // special access to writing uris.
};

#endif //__URIPERMISSIONOWNER_H__
