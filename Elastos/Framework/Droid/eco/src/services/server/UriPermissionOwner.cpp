
#include "server/UriPermissionOwner.h"

UriPermissionOwner::UriPermissionOwner(
    /* [in] */ CActivityManagerService* service,
    /* [in] */ Handle32 owner)
{
    mService = service;
    mOwner = (IObject*)owner;
}

UriPermissionOwner::ExternalToken::ExternalToken(UriPermissionOwner* uriOwner)
{
    mUriOwner = uriOwner;
}

UriPermissionOwner* UriPermissionOwner::ExternalToken::GetOwner()
{
    return mUriOwner;
}

Binder* UriPermissionOwner::GetExternalTokenLocked()
{
    if (mExternalToken != NULL) {
        mExternalToken = new UriPermissionOwner::ExternalToken(this);
    }

    return mExternalToken;
}

UriPermissionOwner* UriPermissionOwner::FromExternalToken(
    /* [in] */ Binder* token)
{
    //TODO:
    // if (token instanceof ExternalToken) {
    //     return ((ExternalToken)token).getOwner();
    // }
    return NULL;
}

void UriPermissionOwner::RemoveUriPermissionsLocked()
{
    RemoveUriPermissionsLocked(Intent_FLAG_GRANT_READ_URI_PERMISSION
        | Intent_FLAG_GRANT_WRITE_URI_PERMISSION);
}

void UriPermissionOwner::RemoveUriPermissionsLocked(
    /* [in] */ Int32 mode)
{
    if ((mode & Intent_FLAG_GRANT_READ_URI_PERMISSION) != 0
            && mReadUriPermissions != NULL) {
        Set<UriPermission*>::Iterator it = mReadUriPermissions->Begin();
        for (; it != mReadUriPermissions->End(); it++) {
            (*it)->mReadOwners.Erase(this);
            if ((*it)->mReadOwners.GetSize() == 0 && ((*it)->mGlobalModeFlags
                    & Intent_FLAG_GRANT_READ_URI_PERMISSION) == 0) {
                (*it)->mModeFlags &= ~Intent_FLAG_GRANT_READ_URI_PERMISSION;
                mService->RemoveUriPermissionIfNeededLocked(*it);
            }
        }
        mReadUriPermissions = NULL;
    }
    if ((mode & Intent_FLAG_GRANT_WRITE_URI_PERMISSION) != 0
            && mWriteUriPermissions != NULL) {
        Set<UriPermission*>::Iterator it = mWriteUriPermissions->Begin();
        for (; it != mWriteUriPermissions->End(); it++) {
            (*it)->mWriteOwners.Erase(this);
            if ((*it)->mWriteOwners.GetSize() == 0 && ((*it)->mGlobalModeFlags
                    & Intent_FLAG_GRANT_WRITE_URI_PERMISSION) == 0) {
                (*it)->mModeFlags &= ~Intent_FLAG_GRANT_WRITE_URI_PERMISSION;
                mService->RemoveUriPermissionIfNeededLocked(*it);
            }
        }
        mWriteUriPermissions = NULL;
    }
}

void UriPermissionOwner::RemoveUriPermissionLocked(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 mode)
{
    if ((mode & Intent_FLAG_GRANT_READ_URI_PERMISSION) != 0
            && mReadUriPermissions != NULL) {
        Set<UriPermission*>::Iterator it = mReadUriPermissions->Begin();
        while (it != mReadUriPermissions->End()) {
            UriPermission* perm = (*it);
            Boolean result;
            uri->Equals(perm->mUri, &result);
            if (result) {
                perm->mReadOwners.Erase(this);
                if (perm->mReadOwners.GetSize() == 0 && (perm->mGlobalModeFlags
                        & Intent_FLAG_GRANT_READ_URI_PERMISSION) == 0) {
                    perm->mModeFlags &= ~Intent_FLAG_GRANT_READ_URI_PERMISSION;
                    mService->RemoveUriPermissionIfNeededLocked(perm);
                }
                mReadUriPermissions->Erase(it);
            }
            it++;
        }
        if (mReadUriPermissions->GetSize() == 0) {
            mReadUriPermissions = NULL;
        }
    }
    if ((mode & Intent_FLAG_GRANT_WRITE_URI_PERMISSION) != 0
            && mWriteUriPermissions != NULL) {
        Set<UriPermission*>::Iterator it = mWriteUriPermissions->Begin();
        while (it != mWriteUriPermissions->End()) {
            UriPermission* perm = (*it);
            Boolean result;
            uri->Equals(perm->mUri, &result);
            if (result) {
                perm->mWriteOwners.Erase(this);
                if (perm->mWriteOwners.GetSize() == 0 && (perm->mGlobalModeFlags
                        & Intent_FLAG_GRANT_WRITE_URI_PERMISSION) == 0) {
                    perm->mModeFlags &= ~Intent_FLAG_GRANT_WRITE_URI_PERMISSION;
                    mService->RemoveUriPermissionIfNeededLocked(perm);
                }
                mWriteUriPermissions->Erase(it);
            }
        }
        if (mWriteUriPermissions->GetSize() == 0) {
            mWriteUriPermissions = NULL;
        }
    }
}

void UriPermissionOwner::AddReadPermission(
    /* [in] */ UriPermission* perm)
{
    if (mReadUriPermissions == NULL) {
        mReadUriPermissions = new Set<UriPermission*>();
    }
    mReadUriPermissions->Insert(perm);
}

void UriPermissionOwner::AddWritePermission(
    /* [in] */ UriPermission* perm)
{
    if (mWriteUriPermissions == NULL) {
        mWriteUriPermissions = new Set<UriPermission*>();
    }
    mWriteUriPermissions->Insert(perm);
}

void UriPermissionOwner::RemoveReadPermission(
    /* [in] */ UriPermission* perm)
{
    mReadUriPermissions->Erase(perm);
    if (mReadUriPermissions->GetSize() == 0) {
        mReadUriPermissions = NULL;
    }
}

void UriPermissionOwner::RemoveWritePermission(
    /* [in] */ UriPermission* perm)
{
    mWriteUriPermissions->Erase(perm);
    if (mWriteUriPermissions->GetSize() == 0) {
        mWriteUriPermissions = NULL;
    }
}