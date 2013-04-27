
#ifndef __URIPERMISSIONOWNER_H__
#define __URIPERMISSIONOWNER_H__

#include "server/CActivityManagerService.h"
#include "server/UriPermission.h"
#include "os/Binder.h"

class UriPermission;

class UriPermissionOwner
{
public:
    class ExternalToken : public Binder, public IBinder , public ElRefBase
    {
    public:
        ExternalToken(
            /* [in] */ UriPermissionOwner* uriOwner);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI GetInterfaceID(
            /* [in]  */ IInterface* pObject,
            /* [out]  */ InterfaceID* rIID);

        CARAPI GetDescription(
            /* [in] */ String* des);

        CARAPI_(UriPermissionOwner*) GetOwner();

    private:
        UriPermissionOwner* mUriOwner;
    };

public:
    UriPermissionOwner(
        /* [in] */ CActivityManagerService* service,
        /* [in] */ Handle32 owner);

    CARAPI_(IBinder*) GetExternalTokenLocked();

    static CARAPI_(UriPermissionOwner*) FromExternalToken(
        /* [in] */ IBinder* token);

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
    AutoPtr<CActivityManagerService> mService;
    AutoPtr<IObject> mOwner;
    AutoPtr<IBinder> mExternalToken;
    Set<UriPermission*>* mReadUriPermissions; // special access to reading uris.
    Set<UriPermission*>* mWriteUriPermissions; // special access to writing uris.
};

#endif //__URIPERMISSIONOWNER_H__
