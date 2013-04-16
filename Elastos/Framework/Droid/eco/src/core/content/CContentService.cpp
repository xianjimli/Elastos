
#include "content/CContentService.h"
#include "ext/frameworkext.h"
#include <Logger.h>

using namespace Elastos;
using namespace Elastos::Core;

CString CContentService::TAG = "CContentService";

CContentService::ObserverCall::ObserverCall(
	/* [in] */ ObserverNode* node,
    /* [in] */ IContentObserver* observer,
    /* [in] */ Boolean selfNotify) :
    mNode(node),
    mObserver(observer),
    mSelfNotify(selfNotify)
{}

CContentService::ObserverCall::~ObserverCall()
{}

CContentService::ObserverNode::ObserverEntry::ObserverEntry(
    /* [in] */ ObserverNode* node,
    /* [in] */ IContentObserver* observer,
    /* [in] */ Boolean notify) :
    mObserverNode(node),
    mObserver(observer),
    mNotifyForDescendents(notify)
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
    Boolean isRemove;
    FAIL_RETURN(mObserverNode->RemoveObserverLocked(mObserver, &isRemove));
    return NOERROR;

}

CContentService::ObserverNode::ObserverNode(
	/* [in] */ String name)
{
    mName.SetTo(name);
}

CContentService::ObserverNode::~ObserverNode()
{}

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
    Boolean empty;
    Int32 count = 0;
    Int32 size = mChildren.GetSize();
    List<AutoPtr<ObserverNode> >::Iterator it;

    for (Int32 i = 0; i < size; i++) {
        it = mChildren.Begin();
        while (count != i) {
            it++;
            count++;
        }
        ObserverNode* pNode = *it;
        pNode->RemoveObserverLocked(observer, &empty);
        if (empty) {
            mChildren.Remove(pNode);
            i--;
            size--;
        }
        count = 0;
    }

    //TODO
    //IBinder* observerBinder;
    //observer->asBinder(&observerBinder);
    //List<AutoPtr<ObserverEntry> >::Iterator itor;
    //for (itor = mObservers->Begin(); itor != mObservers->End(); itor++) {
    //    ObserverEntry* pEntry = *itor;
    //    IContentObserver* pObserver = (IContentObserver*)pEntry->mObserver;
    //    IBinder* tmpBinder = NULL;
    //    pObserver->asBinder(tmpBinder);
    //    if(tmpBinder == observerBinder){
    //        mObservers->Remove(pEntry);
    //        // We no longer need to listen for death notifications. Remove it.
    //        observerBinder->unlinkToDeath(pEntry, 0);
    //        break;
    //    } 
    //}

    if (0 == mChildren.GetSize() && 0 == mObservers.GetSize()) {
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
    /* [in] */ List<ObserverCall*>* calls)
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
    } else if (index < segmentCount){
        segment = GetUriSegment(uri, index);
        // Notify any observers at this level who are interested in descendents
        CollectMyObserversLocked(node, FALSE, observer, selfNotify, calls);
    }

    for (it = mChildren.Begin(); it != mChildren.End(); it++) {
        ObserverNode* pNode = *it;
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
        ObserverEntry* oberverEntry = new ObserverEntry(node, observer, notifyForDescendents);
        mObservers.PushBack(oberverEntry);
        return NOERROR;
    }

    // Look to see if the proper child already exists
    String segment = GetUriSegment(uri, index);

    if(segment.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    List<AutoPtr<ObserverNode> >::Iterator it;

    for (it = mChildren.Begin(); it != mChildren.End(); it++) {
        ObserverNode* pNode = *it;
        if(!segment.Compare(pNode->mName)) {
            FAIL_RETURN(pNode->AddObserverLocked(node, uri, index+1 , observer, notifyForDescendents));
            return NOERROR;
        }
    }

    // No child found, create one
    ObserverNode* newNode = new ObserverNode(segment);
    mChildren.PushBack(newNode);
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
    uri->GetPathSegments(&pathSegments);
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
        } else {
            uri->GetPathSegments(&pathSegments);
            strSegment = (*pathSegments)[index - 1];
            return strSegment;
        }
    } else {
        return strSegment;
    }

}

void CContentService::ObserverNode::CollectMyObserversLocked(
    /* [in] */ ObserverNode* node,
    /* [in] */ Boolean leaf,
    /* [in] */ IContentObserver* observer,
    /* [in] */ Boolean selfNotify,
    /* [in] */ List<ObserverCall*>* calls)
{
    IBinder* observerBinder = NULL;
    //TODO
    //if(NULL != observer){
    //    observer->asBinder(&observerBinder);
    //}

    List<AutoPtr<ObserverEntry> >::Iterator itor;

    for (itor = mObservers.Begin(); itor != mObservers.End(); itor++) {
        ObserverEntry* pEntry = *itor;
        //IContentObserver* pObserver = (IContentObserver*)pEntry->mObserver;
        IBinder* tmpBinder = NULL;
        //pObserver->asBinder(&tmpBinder);

        // Don't notify the observer if it sent the notification and isn't interesed
        // in self notifications
        if (tmpBinder == observerBinder && !selfNotify) {
            continue;
        }

        // Make sure the observer is interested in the notification
        if (leaf || (!leaf && pEntry->mNotifyForDescendents)) {
            calls->PushBack(new ObserverCall(node, pEntry->mObserver, selfNotify));
        }
    }

}

CContentService::CContentService():
	mContext(NULL),
	mFactoryTest(FALSE)
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
    Boolean isRemove;

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
//    // This makes it so that future permission checks will be in the context of this
//    // process rather than the caller's process. We will restore this before returning.
//    //long identityToken = clearCallingIdentity();
//
//    List<ObserverCall*>* calls = new List<ObserverCall>();
//    Mutex::Autolock lock(mRootNodeLock);
//    FAIL_RETURN(mRootNode->CollectObserversLocked(uri, 0, observer, observerWantsSelfNotifications, calls));
//    List<ObserverCall*>::Iterator it;
//
//    for (it = calls->Begin(); it != calls->End(); it++) {
//        ObserverCall* pCall = *it;
//        if (FAILED(pCall->mObserver->OnChange(pCall->mSelfNotify))) {
//            Mutex::Autolock lock(mRootNodeLock);
//            Logger::W(TAG, "Found dead observer, removing");
//            IBinder* binder = NULL;
//            IBinder* tmpBinder = NULL;
//            pCall->mObserver->AsBinder(&binder);        //IBinder binder = oc.mObserver.asBinder();
//            Int32 count = 0;
//            List<ObserverNode::ObserverEntry*> list = pCall->mNode->mObservers;
//            Int32 size = list.GetSize();
//            List<ObserverNode::ObserverEntry*>::Iterator it;
//
//            for (Int32 i = 0; i < size; i++) {
//                it = list.Begin();
//                while (count != i) {
//                    it++;
//                    count++;
//                }
//                ObserverNode::ObserverEntry* pEntry = *it;
//                pEntry->mObserver->AsBinder(&tmpBinder);
//                if (binder == tmpBinder) {
//                    list.Remove();
//                }
//                if (empty) {
//                    mChildren.Remove(pEntry);
//                    i--;
//                    size--;
//                }
//                count = 0;
//            }
//        }
//
//    }
//    if (syncToNetwork) {
//        ISyncManager* syncManager = GetSyncManager();
//        if (NULL != syncManager) {
//            String authority;
//            uri->GetAuthority(&authority);
//            FAIL_RETURN(syncManager->ScheduleLocalSync(NULL /* all accounts */,authority));
//        }
//    }
//    //restoreCallingIdentity(identityToken);
    return E_NOT_IMPLEMENTED;
}

ECode CContentService::GetMasterSyncAutomatically(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContentService::SetMasterSyncAutomatically(
    /* [in] */ Boolean flag)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContentService::RequestSync(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& authority,
    /*[in]*/ IBundle* extras)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContentService::CancelSync(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& authority)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContentService::GetSyncAutomatically(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& providerName,
    /*[out]*/ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}


ECode CContentService::SetSyncAutomatically(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& providerName,
    /*[in]*/ Boolean sync)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContentService::GetPeriodicSyncs(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& providerName,
    /*[out]*/ IObjectContainer** periodicSyncList)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContentService::AddPeriodicSync(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& providerName,
    /*[in]*/ IBundle* extras,
    /*[in]*/ Int64 pollFrequency)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContentService::RemovePeriodicSync(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& providerName,
    /*[in]*/ IBundle* extras)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContentService::GetIsSyncable(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& providerName,
    /*[out]*/ Int32* value)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContentService::SetIsSyncable(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& providerName,
    /*[in]*/ Int32 syncable)
{
    return E_NOT_IMPLEMENTED;
}

ECode CContentService::IsSyncActive(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& authority,
    /*[in]*/ Boolean* isActive)
{
    return E_NOT_IMPLEMENTED;
}

//ECode CContentService::GetCurrentSync(
//    /*[out]*/ ISyncInfo** syncInfo)
//{
//    return E_NOT_IMPLEMENTED;
//}

//ECode CContentService::GetSyncAdapterTypes(
//    /*[out, callee]*/ ArrayOf<ISyncAdapterType*>* result)
//{
//    return E_NOT_IMPLEMENTED;
//}

//ECode CContentService::GetSyncStatus(
//    /*[in]*/ IAccount* account,
//    /*[in]*/ const String& authority,
//    /*[out]*/ ISyncStatusInfo** result)
//{
//    return E_NOT_IMPLEMENTED;
//}

ECode CContentService::IsSyncPending(
    /*[in]*/ IAccount* account,
    /*[in]*/ const String& authority,
    /*[out]*/ Boolean* isPending)
{
    return E_NOT_IMPLEMENTED;
}

//ECode CContentService::AddStatusChangeListener(
//    /*[in]*/ Int32 mask,
//    /*[in]*/ ISyncStatusObserver* callback)
//{
//    return E_NOT_IMPLEMENTED;
//}

//ECode CContentService::RemoveStatusChangeListener(
//    /*[in]*/ ISyncStatusObserver* callback)
//{
//    return E_NOT_IMPLEMENTED;
//}

//AutoPtr<ISyncManager> CContentService::GetSyncManager()
//{
//    Mutex::Autolock lock(mSyncManagerLock);
//
//    if(NULL == mSyncManager){
//        if (FAILED(CSyncManager::New(mContext, mFactoryTest, (ISyncManager*)&mSyncManager)){
//            Logger::W(TAG, "Can't create SyncManager");
//        }
//    }
//
//    return mSyncManager;
//}
