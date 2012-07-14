
#ifndef __HH_SERVICEMANAGER_H
#define __HH_SERVICEMANAGER_H

#include <binder/Binder.h>
#include <binder/Parcel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include <clsinfo.h>
#include <elastos/HashMap.h>
#include <elastos/AutoPtr.h>
#include <elastos.h>
using namespace Elastos;

#define MAX_SERVICE_NUMBER 32
#define ELASTOS_SERVICEMGR_NAME "elastos.servicemanager"

struct StringEq
{
    Boolean operator()(String s1, String s2) const
    {
        return s1.Compare(s2) == 0;
    }
};

// come from marshal.h
typedef struct InterfacePack
{
    EMuid               m_clsid;             // clsid of object
    UInt32              m_uIndex;            // interface index in class
    android::sp<android::IBinder> m_pBinder; // dbus unique connection name of stub
} InterfacePack;

class ServiceManager : public android::BBinder
{
public:
    ServiceManager();
    virtual ~ServiceManager();
    
    enum {
        ADD_SERVICE = android::IBinder::FIRST_CALL_TRANSACTION,
        GET_SERVICE,
        REMOVE_SERVICE,
    };

protected:
    virtual android::status_t onTransact(
        /* [in] */ uint32_t code,
        /* [in] */ const android::Parcel& data,
        /* [in] */ android::Parcel* reply,
        /* [in] */ uint32_t flags = 0);

private:
    ECode AddService(
        /* [in] */ const String& name,
        /* [in] */ InterfacePack * ip);
        
    ECode GetService(
        /* [in] */ const String& name,
        /* [out] */ InterfacePack * pIp);
    
    ECode RemoveService(
        /* [in] */ const String& name);

private:
    HashMap<String, InterfacePack*, 
        Hash<String>, StringEq> mServices;
};

#endif // __HH_SERVICEMANAGER_H

