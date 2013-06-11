
#ifndef __CCONTENTSERVICE_H__
#define __CCONTENTSERVICE_H__
#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif
#include "_CContentService.h"
#include <elastos/AutoPtr.h>
#include <elastos/List.h>
#include <elastos/ElRefBase.h>
#include <elastos/Mutex.h>

using namespace Elastos;

CarClass(CContentService)
{
    class ObserverNode;

    class ObserverCall: public ElRefBase
    {
    public:
        ObserverCall(
            /* [in] */ ObserverNode* node,
            /* [in] */ IContentObserver* observer,
            /* [in] */ Boolean selfNotify);

        ~ObserverCall();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

    public:
        AutoPtr<ObserverNode> mNode;
        AutoPtr<IContentObserver> mObserver;
        Boolean mSelfNotify;

    };

    class ObserverNode: public ElRefBase
    {
    public:
        class ObserverEntry :  public ElRefBase, public IBinderDeathRecipient
            {
            public:
                ObserverEntry(
                    /* [in] */ ObserverNode* node,
                    /* [in] */ IContentObserver* observer,
                    /* [in] */ Boolean notify);

                ~ObserverEntry();

                CARAPI_(PInterface) Probe(
                    /* [in] */ REIID riid);

                CARAPI_(UInt32) AddRef();

                CARAPI_(UInt32) Release();

                CARAPI GetInterfaceID(
                    /* [in] */ IInterface *pObject,
                    /* [out] */ InterfaceID *pIID);

                CARAPI BinderDied();

            public:
                AutoPtr<ObserverNode> mObserverNode;
                AutoPtr<IContentObserver> mObserver;
                Boolean mNotifyForDescendents;
            private:
                Mutex mObserversLock;
            };

        public:
            ObserverNode(
                /* [in] */ String name);

            ~ObserverNode();

            CARAPI_(PInterface) Probe(
                /* [in] */ REIID riid);

            CARAPI_(UInt32) AddRef();

            CARAPI_(UInt32) Release();

            CARAPI GetInterfaceID(
                /* [in] */ IInterface *pObject,
                /* [out] */ InterfaceID *pIID);

            CARAPI AddObserverLocked(
                /* [in] */ ObserverNode* node,
                /* [in] */ IUri* uri,
                /* [in] */ IContentObserver* observer,
                /* [in] */ Boolean notifyForDescendents);

            CARAPI RemoveObserverLocked(
                /* [in] */ IContentObserver* observer,
                /* [out] */  Boolean* result);

            CARAPI CollectObserversLocked(
                /* [in] */ ObserverNode* node,
                /* [in] */ IUri* uri,
                /* [in] */ Int32 index,
                /* [in] */ IContentObserver* observer,
                /* [in] */ Boolean selfNotify,
                /* [in] */ List<AutoPtr<ObserverCall> >* calls);

        public:
            static const Int32 INSERT_TYPE = 0;
            static const Int32 UPDATE_TYPE = 1;
            static const Int32 DELETE_TYPE = 2;

        private:
            CARAPI_(String) GetUriSegment(
                /* [in] */ IUri* uri,
                /* [in] */ Int32 index);

            CARAPI_(Int32) CountUriSegments(
                /* [in] */ IUri* uri);

            CARAPI AddObserverLocked(
                /* [in] */ ObserverNode* node,
                /* [in] */ IUri* uri,
                /* [in] */ Int32 index,
                /* [in] */ IContentObserver* observer,
                /* [in] */ Boolean notifyForDescendents);

            CARAPI_(void) CollectMyObserversLocked(
                /* [in] */ ObserverNode* node,
                /* [in] */ Boolean leaf,
                /* [in] */ IContentObserver* observer,
                /* [in] */ Boolean selfNotify,
                /* [in] */ List<AutoPtr<ObserverCall> >* calls);

        public:
            String mName;
            List<AutoPtr<ObserverNode> >* mChildren;
            List<AutoPtr<ObserverNode::ObserverEntry> >* mObservers;
    };

public:
    CContentService();

    ~CContentService();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Boolean factoryTest);

    CARAPI RegisterContentObserver(
        /*[in]*/ IUri* uri,
        /*[in]*/ Boolean notifyForDescendentsn,
        /*[in]*/ IContentObserver* observer);

    CARAPI UnregisterContentObserver(
        /*[in]*/ IContentObserver* observer);

    CARAPI NotifyChange(
        /*[in]*/ IUri* uri,
        /*[in]*/ IContentObserver* observer,
        /*[in]*/ Boolean observerWantsSelfNotifications,
        /*[in]*/ Boolean syncToNetwork);

    CARAPI RequestSync(
        /*[in]*/ IAccount* account,
        /*[in]*/ const String& authority,
        /*[in]*/ IBundle* extras);

    CARAPI CancelSync(
        /*[in]*/ IAccount* account,
        /*[in]*/ const String& authority);

    CARAPI GetSyncAutomatically(
        /*[in]*/ IAccount* account,
        /*[in]*/ const String& providerName,
        /*[out]*/ Boolean* result);


    CARAPI SetSyncAutomatically(
        /*[in]*/ IAccount* account,
        /*[in]*/ const String& providerName,
        /*[in]*/ Boolean sync);

    CARAPI GetPeriodicSyncs(
        /*[in]*/ IAccount* account,
        /*[in]*/ const String& providerName,
        /*[out]*/ IObjectContainer** periodicSyncList);

    CARAPI AddPeriodicSync(
        /*[in]*/ IAccount* account,
        /*[in]*/ const String& providerName,
        /*[in]*/ IBundle* extras,
        /*[in]*/ Int64 pollFrequency);

    CARAPI RemovePeriodicSync(
        /*[in]*/ IAccount* account,
        /*[in]*/ const String& providerName,
        /*[in]*/ IBundle* extras);

    CARAPI GetIsSyncable(
        /*[in]*/ IAccount* account,
        /*[in]*/ const String& providerName,
        /*[out]*/ Int32* value);

    CARAPI SetIsSyncable(
        /*[in]*/ IAccount* account,
        /*[in]*/ const String& providerName,
        /*[in]*/ Int32 syncable);

    CARAPI SetMasterSyncAutomatically(
        /*[in]*/ Boolean flag);

    CARAPI GetMasterSyncAutomatically(
        /*[out]*/ Boolean* result);

    CARAPI IsSyncActive(
        /*[in]*/ IAccount* account,
        /*[in]*/ const String& authority,
        /*[out]*/ Boolean* isActive);

    CARAPI GetCurrentSync(
        /*[out]*/ ISyncInfo** syncInfo);

    CARAPI GetSyncAdapterTypes(
        /*[out, callee]*/ ArrayOf<ISyncAdapterType*>** result);

    CARAPI GetSyncStatus(
        /*[in]*/ IAccount* account,
        /*[in]*/ const String& authority,
        /*[out]*/ ISyncStatusInfo** result);

    CARAPI IsSyncPending(
        /*[in]*/ IAccount* account,
        /*[in]*/ const String& authority,
        /*[out]*/ Boolean* isPending);

    CARAPI AddStatusChangeListener(
        /*[in]*/ Int32 mask,
        /*[in]*/ ISyncStatusObserver* observer);

    CARAPI RemoveStatusChangeListener(
        /*[in]*/ ISyncStatusObserver* observer);

private:
    CARAPI_(AutoPtr<ISyncManager>) GetSyncManager();

private:
    static CString TAG;
    AutoPtr<IContext> mContext;
    AutoPtr<ObserverNode> mRootNode;
    AutoPtr<ISyncManager> mSyncManager;
    Boolean mFactoryTest;
    Mutex mSyncManagerLock;
    Mutex mRootNodeLock;
};

#endif // __CCONTENTSERVICE_H__

