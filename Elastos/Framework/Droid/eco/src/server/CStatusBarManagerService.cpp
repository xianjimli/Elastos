
#include "server/CStatusBarManagerService.h"
#include "utils/CApartment.h"
#include "utils/CParcelableObjectContainer.h"
#include "statusbar/CStatusBarIcon.h"
#include <Slogger.h>

using namespace Elastos::Utility::Logging;

const CString CStatusBarManagerService::TAG = "StatusBarManagerService";
const Boolean CStatusBarManagerService::SPEW;

CStatusBarManagerService::CStatusBarManagerService()
    : mNotifications(11)
    , mDisabled(0)
{
    assert(SUCCEEDED(CApartment::GetDefaultApartment((IApartment**)&mHandler))
            && (mHandler != NULL));
    CStatusBarIconList::NewByFriend((CStatusBarIconList**)&mIcons);
}

CStatusBarManagerService::~CStatusBarManagerService()
{
    List<DisableRecord*>::Iterator it = mDisableRecords.Begin();
    for (; it != mDisableRecords.End(); ++it) {
        delete *it;
    }
}

ECode CStatusBarManagerService::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;

    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    AutoStringArray slots;
    res->GetStringArray(0x01070009/*com.android.internal.R.array.config_statusBarIcons*/, (ArrayOf<String>**)&slots);
    mIcons->DefineSlots(*slots);

    return NOERROR;
}

void CStatusBarManagerService::SetNotificationCallbacks(
    /* [in] */ CStatusBarManagerService::INotificationCallbacks* listener)
{
    mNotificationCallbacks = listener;
}

ECode CStatusBarManagerService::Expand()
{
    EnforceExpandStatusBar();

    if (mBar != NULL) {
        mBar->AnimateExpand();
    }
    return NOERROR;
}

ECode CStatusBarManagerService::Collapse()
{
    EnforceExpandStatusBar();

    if (mBar != NULL) {
        mBar->AnimateCollapse();
    }
    return NOERROR;
}

ECode CStatusBarManagerService::Disable(
    /* [in] */ Int32 what,
    /* [in] */ IBinder* token,
    /* [in] */ const String& cap)
{
    EnforceStatusBar();

    // It's important that the the callback and the call to mBar get done
    // in the same order when multiple threads are calling this function
    // so they are paired correctly.  The messages on the handler will be
    // handled in the order they were enqueued, but will be outside the lock.
    Mutex::Autolock lock(mDisableRecordsLock);

    ManageDisableListLocked(what, token, cap);
    Int32 net = GatherDisableActionsLocked();
    if (net != mDisabled) {
        mDisabled = net;
        Post(new DisableRunnable(net, this));
        if (mBar != NULL) {
            mBar->Disable(net);
        }
    }
    return NOERROR;
}

ECode CStatusBarManagerService::SetIcon(
    /* [in] */ const String& slot,
    /* [in] */ const String& iconCapsule,
    /* [in] */ Int32 iconId,
    /* [in] */ Int32 iconLevel)
{
    EnforceStatusBar();

    Mutex::Autolock lock(mIconsLock);

    Int32 index;
    mIcons->GetSlotIndex(slot, &index);
    if (index < 0) {
//        throw new SecurityException("invalid status bar icon slot: " + slot);
        return E_SECURITY_EXCEPTION;
    }

    AutoPtr<IStatusBarIcon> icon;
    CStatusBarIcon::New(iconCapsule, iconId, iconLevel, (IStatusBarIcon**)&icon);
    //Slog.d(TAG, "setIcon slot=" + slot + " index=" + index + " icon=" + icon);
    mIcons->SetIcon(index, icon);

    if (mBar != NULL) {
        mBar->SetIcon(index, icon);
    }

    return NOERROR;
}

ECode CStatusBarManagerService::SetIconVisibility(
    /* [in] */ const String& slot,
    /* [in] */ Boolean visible)
{
    EnforceStatusBar();

    Mutex::Autolock lock(mIconsLock);

    Int32 index;
    mIcons->GetSlotIndex(slot, &index);
    if (index < 0) {
//        throw new SecurityException("invalid status bar icon slot: " + slot);
        return E_SECURITY_EXCEPTION;
    }

    AutoPtr<IStatusBarIcon> icon;
    mIcons->GetIcon(index, (IStatusBarIcon**)&icon);
    if (icon == NULL) {
        return NOERROR;
    }

    if (((CStatusBarIcon*)icon.Get())->mVisible != visible) {
        ((CStatusBarIcon*)icon.Get())->mVisible = visible;

        if (mBar != NULL) {
            mBar->SetIcon(index, icon);
        }
    }

    return NOERROR;
}

ECode CStatusBarManagerService::RemoveIcon(
    /* [in] */ const String& slot)
{
    EnforceStatusBar();

    Mutex::Autolock lock(mIconsLock);

    Int32 index;
    mIcons->GetSlotIndex(slot, &index);
    if (index < 0) {
//        throw new SecurityException("invalid status bar icon slot: " + slot);
        return E_SECURITY_EXCEPTION;
    }

    mIcons->RemoveIcon(index);

    if (mBar != NULL) {
        mBar->RemoveIcon(index);
    }

    return NOERROR;
}

void CStatusBarManagerService::EnforceStatusBar()
{
    mContext->EnforceCallingOrSelfPermission(
            "elastos.Manifest.permission.STATUS_BAR"/*android.Manifest.permission.STATUS_BAR*/,
            "StatusBarManagerService");
}

void CStatusBarManagerService::EnforceExpandStatusBar()
{
    mContext->EnforceCallingOrSelfPermission(
            "elastos.Manifest.permission.EXPAND_STATUS_BAR"/*android.Manifest.permission.EXPAND_STATUS_BAR*/,
            "StatusBarManagerService");
}

void CStatusBarManagerService::EnforceStatusBarService()
{
    mContext->EnforceCallingOrSelfPermission(
            "android.Manifest.permission.STATUS_BAR_SERVICE"/*android.Manifest.permission.STATUS_BAR_SERVICE*/,
            "StatusBarManagerService");
}

// ================================================================================
// Callbacks from the status bar service.
// ================================================================================
ECode CStatusBarManagerService::RegisterStatusBar(
    /* [in] */ IStatusBar* bar,
    /* [out] */ IStatusBarIconList** iconList,
    /* [out] */ IObjectContainer** notificationKeys,
    /* [out] */ IObjectContainer** notifications)
{
    EnforceStatusBarService();

    Slogger::I(TAG, "registerStatusBar bar="/* + bar*/);
    mBar = bar;
    {
        Mutex::Autolock lock(mIconsLock);

        ASSERT_SUCCEEDED(CStatusBarIconList::New(iconList));
        (*iconList)->CopyFrom(mIcons);
    }
    {
        Mutex::Autolock lock(mNotificationsLock);

        if (mNotifications.Begin() != mNotifications.End()) {
            ASSERT_SUCCEEDED(CParcelableObjectContainer::New(notificationKeys));
            ASSERT_SUCCEEDED(CParcelableObjectContainer::New(notifications));

            HashMap< AutoPtr<IBinder>, AutoPtr<IStatusBarNotification>,
                    HashBinder, BinderEq>::Iterator it;
            for (it = mNotifications.Begin(); it != mNotifications.End(); ++it) {
                (*notificationKeys)->Add(it->mFirst.Get());
                (*notifications)->Add(it->mSecond.Get());
            }
        }
    }
    return NOERROR;
}

ECode CStatusBarManagerService::OnPanelRevealed()
{
    EnforceStatusBarService();

    // tell the notification manager to turn off the lights.
    return mNotificationCallbacks->OnPanelRevealed();
}

ECode CStatusBarManagerService::OnNotificationClick(
    /* [in] */ const String& cap,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id)
{
    EnforceStatusBarService();

    return mNotificationCallbacks->OnNotificationClick(cap, tag, id);
}

ECode CStatusBarManagerService::OnNotificationError(
    /* [in] */ const String& cap,
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 initialPid,
    /* [in] */ CString message)
{
    EnforceStatusBarService();

    // WARNING: this will call back into us to do the remove.  Don't hold any locks.
    return mNotificationCallbacks->OnNotificationError(cap, tag, id, uid, initialPid, message);
}

ECode CStatusBarManagerService::OnClearAllNotifications()
{
    EnforceStatusBarService();

    return mNotificationCallbacks->OnClearAll();
}

ECode CStatusBarManagerService::AddNotification(
    /* [in] */ IStatusBarNotification* notification,
    /* [out] */ IBinder** key)
{
    Mutex::Autolock lock(mNotificationsLock);

//    AutoPtr<IBinder> key = new Binder();
    mNotifications[*key] = notification;
    if (mBar != NULL) {
        mBar->AddNotification(*key, notification);
    }
    return NOERROR;
}

ECode CStatusBarManagerService::UpdateNotification(
    /* [in] */ IBinder* key,
    /* [out] */ IStatusBarNotification* notification)
{
    Mutex::Autolock lock(mNotificationsLock);

    if (mNotifications.Find(key) == mNotifications.End()) {
//        throw new IllegalArgumentException("updateNotification key not found: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mNotifications[key] = notification;
    if (mBar != NULL) {
        mBar->UpdateNotification(key, notification);
    }

    return NOERROR;
}

ECode CStatusBarManagerService::RemoveNotification(
    /* [in] */ IBinder* key)
{
    Mutex::Autolock lock(mNotificationsLock);

    AutoPtr<IStatusBarNotification> n;

    HashMap< AutoPtr<IBinder>, AutoPtr<IStatusBarNotification>,
            HashBinder, BinderEq>::Iterator it = mNotifications.Find(key);
    if (it != mNotifications.End()) {
        n = it->mSecond;
        mNotifications.Erase(it);
    }
    if (n == NULL) {
//        throw new IllegalArgumentException("removeNotification key not found: " + key);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (mBar != NULL) {
        mBar->RemoveNotification(key);
    }

    return NOERROR;
}

// ================================================================================
// Can be called from any thread
// ================================================================================

// lock on mDisableRecords
void CStatusBarManagerService::ManageDisableListLocked(
    /* [in] */ Int32 what,
    /* [in] */ IBinder* token,
    /* [in] */ const String& cap)
{
//    if (SPEW) {
//        Slog.d(TAG, "manageDisableList what=0x" + Integer.toHexString(what) + " pkg=" + pkg);
//    }
//    // update the list
//    synchronized (mDisableRecords) {
//        final int N = mDisableRecords.size();
//        DisableRecord tok = null;
//        int i;
//        for (i=0; i<N; i++) {
//            DisableRecord t = mDisableRecords.get(i);
//            if (t.token == token) {
//                tok = t;
//                break;
//            }
//        }
//        if (what == 0 || !token.isBinderAlive()) {
//            if (tok != null) {
//                mDisableRecords.remove(i);
//                tok.token.unlinkToDeath(tok, 0);
//            }
//        } else {
//            if (tok == null) {
//                tok = new DisableRecord();
//                try {
//                    token.linkToDeath(tok, 0);
//                }
//                catch (RemoteException ex) {
//                    return; // give up
//                }
//                mDisableRecords.add(tok);
//            }
//            tok.what = what;
//            tok.token = token;
//            tok.pkg = pkg;
//        }
//    }
}

// lock on mDisableRecords
Int32 CStatusBarManagerService::GatherDisableActionsLocked()
{
    List<DisableRecord*>::Iterator it = mDisableRecords.Begin();
    // gather the new net flags
    Int32 net = 0;
    for (; it != mDisableRecords.End(); ++it) {
        net |= (*it)->mWhat;
    }
    return net;
}

ECode CStatusBarManagerService::Post(
    /* [in] */ IRunnable* action)
{
    ECode (STDCALL IRunnable::*pHandlerFunc)();
    pHandlerFunc = &IRunnable::Run;

    return mHandler->PostCppCallback(
        (Handle32)action, *(Handle32*)&pHandlerFunc, NULL, 0);
}
