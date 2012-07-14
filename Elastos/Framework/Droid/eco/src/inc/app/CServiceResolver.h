
#ifndef __CSERVICERESOLVER_H__
#define __CSERVICERESOLVER_H__

#include <elastos.h>
#include <clsinfo.h>

#include "app/marshal.h"
#include "ext/frameworkext.h"
#include <elastos/ElRefBase.h>

#include <elastos/AutoPtr.h>

using namespace Elastos;

#define SYS_PROXY_RET_OFFSET    9
extern Address sResolverEntryAddress;

#define PROXY_ENTRY_BASE    sResolverEntryAddress
#define PROXY_ENTRY_SIZE    0x10
#define PROXY_ENTRY_MASK    0x0f
#define PROXY_ENTRY_SHIFT   4
#define PROXY_ENTRY_NUM     0x80

class CServiceResolver : public ElRefBase, public IInterface
{
public:
    CServiceResolver();

    ~CServiceResolver();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *object,
        /* [out] */ InterfaceID *iid);

    static CARAPI_(void) InitEntry();

    static CARAPI_(IInterface*) Create(
        /* [in] */ IInterface* object,
        /* [in] */ CIClassInfo* classInfo,
        /* [in] */ IServiceApartment* apartment);

public:
    class CInterfaceProxy
    {
    public:
        static CARAPI_(PInterface) Probe(
                /* [in] */ CInterfaceProxy *pThis,
                /* [in] */ REIID riid);

        static CARAPI_(UInt32) AddRef(
                /* [in] */ CInterfaceProxy *pThis);

        static CARAPI_(UInt32) Release(
                /* [in] */ CInterfaceProxy *pThis);

        static CARAPI GetInterfaceID(
                /* [in] */ CInterfaceProxy *pThis,
                /* [in] */ IInterface *pObject,
                /* [out] */ InterfaceID *pIID);

        CARAPI WriteParameters(
                /* [in] */ UInt32 uMethodIndex,
                /* [in] */ UInt32* args,
                /* [in] */ MarshalHeader* header,
                /* [in, out] */ IParcel* params);

        inline CARAPI_(UInt32) CountMethodArgs(
                /* [in] */ UInt32 uMethodIndex);

        static CARAPI ProxyEntry(
                /* [in] */ UInt32 *puArgs);

    public:
        PVoid                   mVtptr;       // must be first member
        PVoid                   mProxyEntry; // must be m_pvVptr's next member

        UInt32                  mIndex;       // interface index in object
        PContext                mContext;
        CServiceResolver*       mOwner;
        const CIInterfaceInfo*  mInfo;
    };

    class CInterfaceStub
    {
    public:
        typedef struct InterfaceStruct
        {
            struct
            {
                UInt32 mVtable[1];
            }* mVtptr;
        }InterfaceStruct;

         union
        {
            IInterface*         mObject;
            InterfaceStruct*    mInterface;
        };

        CServiceResolver*       mOwner;
        const CIInterfaceInfo*  mInfo;
    };

private:
    friend class CServiceApartment;

    Int32               mInterfaceNum;
    CIClassInfo*        mClassInfo;
    CInterfaceProxy*    mInterfaceProxies;
    CInterfaceStub*     mInterfaceStubs;

    AutoPtr<IObject>            mService;
    AutoPtr<IServiceApartment>  mServiceApartment;
};


#endif //__CSERVICERESOLVER_H__
