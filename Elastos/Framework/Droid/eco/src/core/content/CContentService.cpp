
#include "content/CContentResolverHelper.h"
#include "content/CContentService.h"
#include "content/CSyncManager.h"
#include "ext/frameworkext.h"
#include <Logger.h>

using namespace Elastos;
using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

CString CContentService::TAG = "CContentService";

CContentService::ObserverCall::ObserverCall(
    /* [in] */ ObserverNode* node,
    /* [in] */ IContentObserver* observer,
    /* [in] */ Boolean selfNotify) 
    : mNode(node)
    , mObserver(observer)
    , mSelfNotify(selfNotify)
{}

CContentService::ObserverCall::~ObserverCall()
{}

PInterface CContentService::ObserverCall::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 CContentService::ObserverCall::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CContentService::ObserverCall::Release()
{
    return ElRefBase::Release();
}

ECode CContentService::ObserverCall::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)this) {
        *pIID = EIID_IInterface;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

CContentService::ObserverNode::ObserverEntry::ObserverEntry(
    /* [in] */ ObserverNode* node,
    /* [in] */ IContentObserver* observer,
    /* [in] */ Boolean notify) 
    : mObserverNode(node)
    , mObserver(observer)
    , mNotifyForDescendents(notify)
{
    //try {
    //    observer.asBinder().linkToDeath(this, 0);
    //} catch (RemoteException e) {
    //    binderDied();
    //}
}

CContentService::ObserverNode::ObserverEntry::~ObserverEntry()
{}

PInterface CContentService::ObserverNode::ObserverEntry::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IBinderDeathRecipient) {
        return (IBinderDeathRecipient*)this;
    }

    return NULL;
}

UInt32 CContentService::ObserverNode::ObserverEntry::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CContentService::ObserverNode::ObserverEntry::Release()
{
    return ElRefBase::Release();
}

ECode CContentService::ObserverNode::ObserverEntry::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)this) {
        *pIID = EIID_IInterface;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CContentService::ObserverNode::ObserverEntry::BinderDied()
{
    Mutex::Autolock lock(mObserversLock);
    Boolean isRemove = FALSE;
    FAIL_RETURN(mObserverNode->RemoveObserverLocked(mObserver, &isRemove));
    return NOERROR;
}

CContentService::ObserverNode::ObserverNode(
    /* [in] */ String name)
{
    mChildren = new List<AutoPtr<ObserverNode> >();
    mObservers = new List<AutoPtr<ObserverEntry> >();
    mName.SetTo(name);
}

CContentService::ObserverNode::~ObserverNode()
{
    if (NULL != mChildren) {
        mChildren->Clear();
        delete mChildren;
    }
    if (NULL != mObservers) {
        mObservers->Clear();
        delete mObservers;
    }
}

PInterface CContentService::ObserverNode::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }

    return NULL;
}

UInt32 CContentService::ObserverNode::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CContentService::ObserverNode::Release()
{
    return ElRefBase::Release();
}

ECode CContentService::ObserverNode::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)this) {
        *pIID = EIID_IInterface;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode CContentService::ObserverNode::AddObserverLocked(
    /* [in] */ ObserverNode* node,
    /* [in] */ IUri* uri,
    /* [in] */ IContentObserver* observer,
    /* [in] */ Boolean notifyForDescendents)
{
    VALIDATE_NOT_NULL(observer);
    FAIL_RETURN(AddObserverLocked(node, uri, 0, observer, notifyForDescendents));
    return NOERROR;
}  

ECode CContentService::ObserverNode::RemoveObserverLocked(
    /* [in] */ IContentObserver* observer,
    /* [out] */  Boolean* result)   
{
    VALIDATE_NOT_NULL(observer);
    VALIDATE_NOT_NULL(result);
    Boolean empty = FALSE;
    Int32 size = mChildren->GetSize();
    List<AutoPtr<ObserverNode> >::Iterator it;

    for (Int32 i = 0; i < size; i++) {
        AutoPtr<ObserverNode> node = (*mChildren)[i];
        node->RemoveObserverLocked(observer, &empty);
        if (empty) {
            mChildren->Remove(node);
            i--;
            size--;
        }
    }

    //TODO
    //IBinder* observerBinder;
    //observer->asBinder(&observerBinder);
    List<AutoPtr<ObserverEntry> >::Iterator itor;
    for (itor = mObservers->Begin(); itor != mObservers->End(); itor++) {
        AutoPtr<ObserverEntry> entry = *itor;
        AutoPtr<IContentObserver> tmpObserver = (IContentObserver*)entry->mObserver;

        if (_CObject_Compare(observer, tmpObserver)){
            mObservers->Remove(entry);
            // We no longer need to listen for death notifications. Remove it.
            //observerBinder->unlinkToDeath(entry, 0);
            break;
        } 
    }

    if (0 == mChildren->GetSize() && 0 == mObservers->GetSize()) {
        *result = TRUE;
        return NOERROR;
    }

    *result = FALSE;
    return NOERROR;
} 

ECode CContentService::ObserverNode::CollectObserversLocked(
    /* [in] */ ObserverNode* node,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 index,
    /* [in] */ IContentObserver* observer,
    /* [in] */ Boolean selfNotify,
    /* [in] */ List<AutoPtr<ObserverCall> >* calls)
{
    VALIDATE_NOT_NULL(node);
    VALIDATE_NOT_NULL(observer);
    VALIDATE_NOT_NULL(calls);
    String segment;
    Int32 segmentCount = CountUriSegments(uri);
    List<AutoPtr<ObserverNode> >::Iterator it;

    if (index >= segmentCount) {
        // This is the leaf node, notify all observers
        CollectMyObserversLocked(node, TRUE, observer, selfNotify, calls);
    } 
    else if (index < segmentCount){
        segment = GetUriSegment(uri, index);
        // Notify any observers at this level who are interested in descendents
        CollectMyObserversLocked(node, FALSE, observer, selfNotify, calls);
    }

    for (it = mChildren->Begin(); it != mChildren->End(); it++) {
        AutoPtr<ObserverNode> pNode = *it;
        if (segment.IsNull() || !segment.Compare(pNode->mName)) {
            FAIL_RETURN(pNode->CollectObserversLocked(pNode, uri, index+1, observer, selfNotify, calls));
            if (!segment.IsNull()) {
                break;
            }
        }
    }

    return NOERROR;
}

ECode CContentService::ObserverNode::AddObserverLocked(
    /* [in] */ ObserverNode* node,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 index,
    /* [in] */ IContentObserver* observer,
    /* [in] */ Boolean notifyForDescendents)
{
    // If this is the leaf node add the observer
    if (index == CountUriSegments(uri)) {
        AutoPtr<ObserverEntry> oberverEntry = new ObserverEntry(node, observer, notifyForDescendents);
        mObservers->PushBack(oberverEntry);
        return NOERROR;
    }

    // Look to see if the proper child already exists
    String segment = GetUriSegment(uri, index);

    if (segment.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    List<AutoPtr<ObserverNode> >::Iterator it;

    for (it = mChildren->Begin(); it != mChildren->End(); it++) {
        AutoPtr<ObserverNode> pNode = *it;
        if (!segment.Compare(pNode->mName)) {
            FAIL_RETURN(pNode->AddObserverLocked(node, uri, index+1 , observer, notifyForDescendents));
            return NOERROR;
        }
    }

    // No child found, create one
    AutoPtr<ObserverNode> newNode = new ObserverNode(segment);
    mChildren->PushBack(newNode);
    FAIL_RETURN(newNode->AddObserverLocked(newNode, uri, index+1 , observer, notifyForDescendents));
    return NOERROR;
}

Int32 CContentService::ObserverNode::CountUriSegments(
    /* [in] */ IUri* uri)
{
    if (NULL == uri) {
        return 0;
    }

    ArrayOf<String>* pathSegments;
    FAIL_RETURN(uri->GetPathSegments(&pathSegments));
    return pathSegments->GetLength() + 1;
}

String CContentService::ObserverNode::GetUriSegment(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 index)
{   
    String strSegment;
    ArrayOf<String>* pathSegments;

    if (NULL != uri) {
        if (0 == index) {
            uri->GetAuthority(&strSegment);
            return strSegment;
        } 
        else {
            uri->GetPathSegments(&pathSegments);
            strSegment = (*pathSegments)[index - 1];
            return strSegment;
        }
    } 
    else {
        return strSegment;
    }

}

void CContentService::ObserverNode::CollectMyObserversLocked(
    /* [in] */ ObserverNode* node,
    /* [in] */ Boolean leaf,
    /* [in] */ IContentObserver* observer,
    /* [in] */ Boolean selfNotify,
    /* [in] */ List<AutoPtr<ObserverCall> >* calls)
{
    //IBinder* observerBinder = NULL;
    //TODO
    //if(NULL != observer){
    //    observer->asBinder(&observerBinder);
    //}

    List<AutoPtr<ObserverEntry> >::Iterator itor;

    for (itor = mObservers->Begin(); itor != mObservers->End(); itor++) {
        AutoPtr<ObserverEntry> pEntry = *itor;
        AutoPtr<IContentObserver> tmpObserver = pEntry->mObserver;
        //IBinder* tmpBinder = NULL;
        //pObserver->asBinder(&tmpBinder);

        // Don't notify the observer if it sent the notification and isn't interesed
        // in self notifications
        if (_CObject_Compare(observer, tmpObserver) && !selfNotify) {
            continue;
        }

        // Make sure the observer is interested in the notification
        if (leaf || (!leaf && pEntry->mNotifyForDescendents)) {
            calls->PushBack(new ObserverCall(node, pEntry->mObserver, selfNotify));
        }
    }

}

CContentService::CContentService()
    : mContext(NULL)
    , mFactoryTest(FALSE)
{
    mRootNode = new ObserverNode(String(""));
}

CContentService::~CContentService()
{}

ECode CContentService::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Boolean factoryTest)
{
    VALIDATE_NOT_NULL(context);
    mContext = context;
    mFactoryTest = factoryTest;
    //GetSyncManager();
    return NOERROR;
}

ECode CContentService::RegisterContentObserver(
    /* [in] */ IUri* uri,
    /* [in] */ Boolean notifyForDescendentsn,
    /* [in] */ IContentObserver* observer)
{
    if (NULL == observer || NULL == uri) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(mRootNodeLock);
    FAIL_RETURN(mRootNode->AddObserverLocked(mRootNode, uri, observer, notifyForDescendentsn));
    return NOERROR;
}

ECode CContentService::UnregisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    Boolean isRemove = FALSE;

    if (NULL == observer) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Mutex::Autolock lock(mRootNodeLock);
    FAIL_RETURN(mRootNode->RemoveObserverLocked(observer, &isRemove));
    return NOERROR;
}

ECode CContentService::NotifyChange(
    /* [in] */ IUri* uri,
    /* [in] */ IContentObserver* observer,
    /* [in] */ Boolean observerWantsSelfNotifications,
    /* [in] */ Boolean syncToNetwork)
{
    if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
        String str("Notifying update of ");
        if (NULL != uri) {
            String strUri;
            uri->ToString(&strUri);
            str.Append(strUri);
        } 
        else {
            str.Append(" null ");
        }
        str.Append(" from observer ");
        //TODO: observer->ToString();
        str.Append(", syncToNetwork ");
        str.Append(syncToNetwork ? " true ":" false ");
        Logger::V(TAG, str);
    }
    // This makes it so that future permission checks will be in the context of this
    // process rather than the caller's process. We will restore this before returning.
    //TODO: long identityToken = clearCallingIdentity();

    List<AutoPtr<ObserverCall> >* calls = new List<AutoPtr<ObserverCall> >();
    Mutex::Autolock lock(mRootNodeLock);
    FAIL_RETURN(mRootNode->CollectObserversLocked(mRootNode, uri, 0, observer, observerWantsSelfNotifications, calls));
    List<AutoPtr<ObserverCall> >::Iterator it;
    ECode ecode = NOERROR;
    AutoPtr<ObserverCall> oc;

    for (it = calls->Begin(); it != calls->End(); it++) {
        oc = *it;
        ecode = oc->mObserver->OnChange(oc->mSelfNotify);
        if (Logger::IsLoggable(TAG, Logger::VERBOSE)) {
            String str("Notified ");
            //TODO: oc->mObserver->ToString();
            str.Append(" of update at ");
            if (NULL != uri) {
                String strUri;
                uri->ToString(&strUri);
                str.Append(strUri);
            } 
            else {
                str.Append(" null ");
            }
            Logger::V(TAG, str);
        }
        if (ecode != NOERROR) {
            Mutex::Autolock lock(mRootNodeLock);
            Logger::W(TAG, "Found dead observer, removing");
            AutoPtr<IContentObserver> tmpObserver = oc->mObserver;
            //IBinder binder = oc.mObserver.asBinder();
            List<AutoPtr<ObserverNode::ObserverEntry> >* list = oc->mNode->mObservers;
            Int32 size = list->GetSize();
            List<AutoPtr<ObserverNode::ObserverEntry> >::Iterator iter;

            for (Int32 i = 0; i < size; i++) {
                AutoPtr<ObserverNode::ObserverEntry> oe = (*list)[i];
                if (_CObject_Compare(tmpObserver, oe->mObserver)){
                    list->Remove(oe);
                   i--;
                   size--;
                }
            }
        }

    }
    if (syncToNetwork) {
        AutoPtr<ISyncManager> syncManager = GetSyncManager();
        if (NULL != syncManager) {
            String authority;
            uri->GetAuthority(&authority);
            AutoPtr<IAccount> nullAccount;
            FAIL_RETURN(syncManager->ScheduleLocalSync(nullAccount /* all accounts */, &authority));
        }
    }
    //TODO: restoreCallingIdentity(identityToken);
    if (NULL != calls) {
        calls->Clear();
        delete calls;
    }
    return NOERROR;
}

ECode CContentService::GetMasterSyncAutomatically(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    mContext->EnforceCallingOrSelfPermission("android.permission.READ_SYNC_SETTINGS"/*Manifest.permission.READ_SYNC_SETTINGS*/, "no permission to read the sync settings");
    //TODO: long identityToken = clearCallingIdentity();
    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager) {
        AutoPtr<ISyncStorageEngine> engine;
        FAIL_RETURN(syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine));
        FAIL_RETURN(engine->GetMasterSyncAutomatically(result));
        return NOERROR;
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    *result = FALSE;
    return NOERROR;
}

ECode CContentService::SetMasterSyncAutomatically(
    /* [in] */ Boolean flag)
{
    mContext->EnforceCallingOrSelfPermission("android.permission.WRITE_SYNC_SETTINGS"/*Manifest.permission.WRITE_SYNC_SETTINGS*/, "no permission to write the sync settings");
    //TODO: long identityToken = clearCallingIdentity();
    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager) {
        AutoPtr<ISyncStorageEngine> engine;
        FAIL_RETURN(syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine));
        FAIL_RETURN(engine->SetMasterSyncAutomatically(flag));
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    return NOERROR;
}

ECode CContentService::RequestSync(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& authority,
    /*[in]*/ IBundle* extras)
{
    AutoPtr<IContentResolverHelper> contentResolverHelper;
    FAIL_RETURN(CContentResolverHelper::AcquireSingleton((IContentResolverHelper**)&contentResolverHelper));
    FAIL_RETURN(contentResolverHelper->ValidateSyncExtrasBundle(extras));
    // This makes it so that future permission checks will be in the context of this
    // process rather than the caller's process. We will restore this before returning.
    //TODO: long identityToken = clearCallingIdentity();

    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager) {
        String tmpAuthority(authority);
        FAIL_RETURN(syncManager->ScheduleSync(account, &tmpAuthority, extras, 0 /* no delay */, FALSE /* onlyThoseWithUnkownSyncableState */));
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    return NOERROR;
}

ECode CContentService::CancelSync(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& authority)
{
    // This makes it so that future permission checks will be in the context of this
    // process rather than the caller's process. We will restore this before returning.
    //TODO: long identityToken = clearCallingIdentity();

    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager) {
        String tmpAuthority(authority);
        FAIL_RETURN(syncManager->ClearScheduledSyncOperations(account, &tmpAuthority));
        FAIL_RETURN(syncManager->CancelActiveSync(account, &tmpAuthority));
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    return NOERROR;
}

ECode CContentService::GetSyncAutomatically(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& providerName,
    /*[out]*/ Boolean* result)
{
    mContext->EnforceCallingOrSelfPermission("android.permission.READ_SYNC_SETTINGS"/*Manifest.permission.READ_SYNC_SETTINGS*/, "no permission to read the sync settings");
    //TODO: long identityToken = clearCallingIdentity();
    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager) {
        AutoPtr<ISyncStorageEngine> engine;
        FAIL_RETURN(syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine));
        FAIL_RETURN(engine->GetSyncAutomatically(account, providerName, result));
        return NOERROR;
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    *result = FALSE;
    return NOERROR;
}

ECode CContentService::SetSyncAutomatically(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& providerName,
    /*[in]*/ Boolean sync)
{
    mContext->EnforceCallingOrSelfPermission("android.permission.WRITE_SYNC_SETTINGS"/*Manifest.permission.WRITE_SYNC_SETTINGS*/, "no permission to write the sync settings");
    //TODO: long identityToken = clearCallingIdentity();
    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager) {
        AutoPtr<ISyncStorageEngine> engine;
        FAIL_RETURN(syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine));
        FAIL_RETURN(engine->SetSyncAutomatically(account, providerName, sync));
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    return NOERROR;
}

ECode CContentService::GetPeriodicSyncs(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& providerName,
    /*[out]*/ IObjectContainer** periodicSyncList)
{
    mContext->EnforceCallingOrSelfPermission("android.permission.READ_SYNC_SETTINGS"/*Manifest.permission.READ_SYNC_SETTINGS*/, "no permission to read the sync settings");
    //TODO: long identityToken = clearCallingIdentity();
    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager) {
        AutoPtr<ISyncStorageEngine> engine;
        FAIL_RETURN(syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine));
        FAIL_RETURN(engine->GetPeriodicSyncs(account, providerName, periodicSyncList));
    }
    return NOERROR;
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
}

ECode CContentService::AddPeriodicSync(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& providerName,
    /*[in]*/ IBundle* extras,
    /*[in]*/ Int64 pollFrequency)
{
    mContext->EnforceCallingOrSelfPermission("android.permission.WRITE_SYNC_SETTINGS"/*Manifest.permission.WRITE_SYNC_SETTINGS*/, "no permission to write the sync settings");
    //TODO: long identityToken = clearCallingIdentity();
    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager) {
        AutoPtr<ISyncStorageEngine> engine;
        FAIL_RETURN(syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine));
        FAIL_RETURN(engine->AddPeriodicSync(account, providerName, extras, pollFrequency));
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    return NOERROR;
}

ECode CContentService::RemovePeriodicSync(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& providerName,
    /*[in]*/ IBundle* extras)
{
    mContext->EnforceCallingOrSelfPermission("android.permission.WRITE_SYNC_SETTINGS"/*Manifest.permission.WRITE_SYNC_SETTINGS*/, "no permission to write the sync settings");
    //TODO: long identityToken = clearCallingIdentity();
    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager) {
        AutoPtr<ISyncStorageEngine> engine;
        FAIL_RETURN(syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine));
        FAIL_RETURN(engine->RemovePeriodicSync(account, providerName, extras));
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    return NOERROR;
}

ECode CContentService::GetIsSyncable(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& providerName,
    /*[out]*/ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    mContext->EnforceCallingOrSelfPermission("android.permission.READ_SYNC_SETTINGS"/*Manifest.permission.READ_SYNC_SETTINGS*/, "no permission to read the sync settings");
    //TODO: long identityToken = clearCallingIdentity();
    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager) {
        AutoPtr<ISyncStorageEngine> engine;
        FAIL_RETURN(syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine));
        FAIL_RETURN(engine->GetIsSyncable(account, providerName, value));
        return NOERROR;
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    *value =  -1;
    return NOERROR;
}

ECode CContentService::SetIsSyncable(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& providerName,
    /*[in]*/ Int32 syncable)
{
    mContext->EnforceCallingOrSelfPermission("android.permission.WRITE_SYNC_SETTINGS"/*Manifest.permission.WRITE_SYNC_SETTINGS*/, "no permission to write the sync settings");
    //TODO: long identityToken = clearCallingIdentity();
    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager) {
        AutoPtr<ISyncStorageEngine> engine;
        FAIL_RETURN(syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine));
        FAIL_RETURN(engine->SetIsSyncable(account, providerName, syncable));
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    return NOERROR;
}

ECode CContentService::IsSyncActive(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& authority,
    /*[out]*/ Boolean* isActive)
{
    VALIDATE_NOT_NULL(isActive);
    mContext->EnforceCallingOrSelfPermission("android.permission.READ_SYNC_STATS"/*Manifest.permission.READ_SYNC_STATS*/, "no permission to read the sync stats");
    //TODO: long identityToken = clearCallingIdentity();
    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager) {
        AutoPtr<ISyncStorageEngine> engine;
        FAIL_RETURN(syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine));
        FAIL_RETURN(engine->IsSyncActive(account, authority, isActive));
        return NOERROR;
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    *isActive =  FALSE;
    return NOERROR;
}

ECode CContentService::GetCurrentSync(
    /*[out]*/ ISyncInfo** syncInfo)
{
    VALIDATE_NOT_NULL(syncInfo);
    mContext->EnforceCallingOrSelfPermission("android.permission.READ_SYNC_STATS"/*Manifest.permission.READ_SYNC_STATS*/, "no permission to read the sync stats");
    //TODO: long identityToken = clearCallingIdentity();
    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager) {
        AutoPtr<ISyncStorageEngine> engine;
        FAIL_RETURN(syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine));
        FAIL_RETURN(engine->GetCurrentSync(syncInfo));
        return NOERROR;
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    *syncInfo = NULL;
    return NOERROR;
}

ECode CContentService::GetSyncAdapterTypes(
    /*[out, callee]*/ ArrayOf<ISyncAdapterType*>** result)
{
    VALIDATE_NOT_NULL(result);
    // This makes it so that future permission checks will be in the context of this
    // process rather than the caller's process. We will restore this before returning.
    //TODO: long identityToken = clearCallingIdentity();
    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager) {
        AutoPtr<ISyncStorageEngine> engine;
        FAIL_RETURN(syncManager->GetSyncAdapterTypes(result));
        return NOERROR;
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    return NOERROR;
}

ECode CContentService::GetSyncStatus(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& authority,
    /*[out]*/ ISyncStatusInfo** result)
{
    VALIDATE_NOT_NULL(result);
    mContext->EnforceCallingOrSelfPermission("android.permission.READ_SYNC_STATS"/*Manifest.permission.READ_SYNC_STATS*/, "no permission to read the sync stats");
    //TODO: long identityToken = clearCallingIdentity();
    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager) {
        AutoPtr<ISyncStorageEngine> engine;
        FAIL_RETURN(syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine));
        FAIL_RETURN(engine->GetStatusByAccountAndAuthority(account, authority, result));
        return NOERROR;
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    *result = NULL;
    return NOERROR;
}

ECode CContentService::IsSyncPending(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& authority,
    /*[out]*/ Boolean* isPending)
{
    mContext->EnforceCallingOrSelfPermission("android.permission.READ_SYNC_STATS"/*Manifest.permission.READ_SYNC_STATS*/, "no permission to read the sync stats");
    //TODO: long identityToken = clearCallingIdentity();
    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager) {
        AutoPtr<ISyncStorageEngine> engine;
        FAIL_RETURN(syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine));
        FAIL_RETURN(engine->IsSyncPending(account, authority, isPending));
        return NOERROR;
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    *isPending = FALSE;
    return NOERROR;
}

ECode CContentService::AddStatusChangeListener(
    /*[in]*/ Int32 mask,
    /*[in]*/ ISyncStatusObserver* observer)
{
    //TODO: long identityToken = clearCallingIdentity();
    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager && NULL != observer) {
        AutoPtr<ISyncStorageEngine> engine;
        FAIL_RETURN(syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine));
        FAIL_RETURN(engine->AddStatusChangeListener(mask, observer));
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    return NOERROR;
}

ECode CContentService::RemoveStatusChangeListener(
    /*[in]*/ ISyncStatusObserver* observer)
{
    //TODO: long identityToken = clearCallingIdentity();
    //try {
    AutoPtr<ISyncManager> syncManager = GetSyncManager();
    if (NULL != syncManager && NULL != observer) {
        AutoPtr<ISyncStorageEngine> engine;
        FAIL_RETURN(syncManager->GetSyncStorageEngine((ISyncStorageEngine**)&engine));
        FAIL_RETURN(engine->RemoveStatusChangeListener(observer));
    }
    //} finally {
        //TODO: restoreCallingIdentity(identityToken);
    //}
    return NOERROR;
}

AutoPtr<ISyncManager> CContentService::GetSyncManager()
{
    Mutex::Autolock lock(mSyncManagerLock);

    if (NULL == mSyncManager) {
        if (FAILED(CSyncManager::New(mContext, mFactoryTest, (ISyncManager**)&mSyncManager))) {
            Logger::W(TAG, String("Can't create SyncManager"));
        }
    }

    return mSyncManager;
}