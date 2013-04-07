
#include "server/CActivityManagerService.h"
#include "server/UriPermission.h"
#include <StringBuffer.h>
#include <elastos/AutoPtr.h>

UriPermission::UriPermission(
    /* [in] */ Int32 uid,
    /* [in] */ IUri* uri) :
    mUid(uid),
    mUri(uri),
    mModeFlags(0),
    mGlobalModeFlags(0)
{
}

void UriPermission::ClearModes(
    /* [in] */ Int32 modeFlagsToClear)
{
    if ((modeFlagsToClear & Intent_FLAG_GRANT_READ_URI_PERMISSION) != 0) {
        mGlobalModeFlags &= ~Intent_FLAG_GRANT_READ_URI_PERMISSION;
        mModeFlags &= ~Intent_FLAG_GRANT_READ_URI_PERMISSION;
        if (mReadOwners.GetSize() > 0) {
            Set<UriPermissionOwner*>::Iterator it = mReadOwners.Begin();
            for (; it != mReadOwners.End(); it++) {
                (*it)->RemoveReadPermission(this);
            }
            mReadOwners.Clear();
        }
    }
    if ((modeFlagsToClear & Intent_FLAG_GRANT_WRITE_URI_PERMISSION) != 0) {
        mGlobalModeFlags &= ~Intent_FLAG_GRANT_WRITE_URI_PERMISSION;
        mModeFlags &= ~Intent_FLAG_GRANT_WRITE_URI_PERMISSION;
        if (mWriteOwners.GetSize() > 0) {
            Set<UriPermissionOwner*>::Iterator it = mWriteOwners.Begin();
            for (; it != mWriteOwners.End(); it++) {
                (*it)->RemoveWritePermission(this);
            }
            mWriteOwners.Clear();
        }
    }
}

String UriPermission::ToString()
{
    if (mStringName != NULL) {
        return mStringName;
    }

    StringBuffer sb;
    sb += "UriPermission{";
    sb += this;//(Integer.toHexString(System.identityHashCode(this)));
    sb += ' ';
    sb += mUri;
    sb += '}';
    mStringName = String((const char*)sb);

    return mStringName;
}

void UriPermission::Dump(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ String prefix)
{
    String str;

    str.Append(prefix);
    str.Append("modeFlags=0x");
    str.Append(String::FromInt32(String::FromInt32(mModeFlags).ToInt32(16)));
    str.Append(" uid=");
    str.Append(String::FromInt32(mUid));
    str.Append(" mGlobalModeFlags=0x");
    str.Append("\n");
    if (mReadOwners.GetSize() != 0) {
        str.Append(prefix);
        str.Append("\n");
        Set<UriPermissionOwner*>::Iterator it = mReadOwners.Begin();
        for (; it != mReadOwners.End(); it++) {
            str.Append(prefix);
            str.Append("  * ");
            str.Append("\n");
        }
    }
    if (mWriteOwners.GetSize() != 0) {
        str.Append(prefix);
        str.Append("\n");
        Set<UriPermissionOwner*>::Iterator it = mWriteOwners.Begin();
        for (; it != mWriteOwners.End(); it++) {
            str.Append(prefix);
            str.Append("  * ");
            str.Append("\n");
        }
    }

    pw->WriteString(CString(str.string()));
}