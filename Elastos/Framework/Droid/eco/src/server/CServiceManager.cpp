
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/Binder.h>
#include "server/CServiceManager.h"
#include "ext/frameworkerr.h"
#include "utils/log.h"

#define ELASTOS_SERVICEMGR_NAME "elastos.servicemanager"

// come from marshal.h
typedef struct InterfacePack
{
    EMuid               m_clsid;             // clsid of object
    UInt32              m_uIndex;            // interface index in class
    android::sp<android::IBinder> m_pBinder; // dbus unique connection name of stub
} InterfacePack;

CServiceManager::CServiceManager() :
    mServiceCache(11)
{}

CServiceManager::~CServiceManager()
{
    mServiceCache.Clear();
}

android::sp<android::IBinder> get_service_manager()
{
    static android::sp<android::IBinder> binder;
    if (binder.get() != NULL) {
        return binder;
    }

    android::sp<android::IServiceManager> sm = android::defaultServiceManager();
    do {
        binder = sm->getService(android::String16(ELASTOS_SERVICEMGR_NAME));
        if (binder != 0) {
            break;
        }
        usleep(500000);
    } while (true);
    LOGD("elastos servicemanager getted.\n");
    return binder;
}

ECode CServiceManager::AddService(
    /* [in] */ const String& name,
    /* [in] */ IInterface * service)
{
    if (name.IsNullOrEmpty() || !service) {
        return E_INVALID_ARGUMENT;
    }

    ECode ec = NOERROR;
    android::Parcel data, reply;
    void * buf = NULL;
    Int32 size;

    // TODO: release buf.
    ec = _CObject_MarshalInterface(service, &buf, &size);
    if (FAILED(ec)) return ec;

    data.writeCString((const char *)name);
    data.writeInt32((int32_t)size);
    data.writeStrongBinder(((InterfacePack*)buf)->m_pBinder);
    ((InterfacePack*)buf)->m_pBinder = NULL;
    data.write((const void *)buf, (size_t)size);
    free(buf);
    get_service_manager()->transact(ADD_SERVICE, data, &reply);
    return (ECode)reply.readInt32();
}

ECode CServiceManager::GetService(
    /* [in] */ const String& name,
    /* [out] */ IInterface** service)
{
    AutoPtr<IInterface> svr = mServiceCache[name];
    if (svr != NULL) {
        *service = (IInterface*)svr;
        (*service)->AddRef();
        return NOERROR;
    }

    if (name.IsNullOrEmpty() || !service) {
        return E_INVALID_ARGUMENT;
    }

    ECode ec = NOERROR;
    android::Parcel data, reply;
    void * buf = NULL;
    android::sp<android::IBinder> binder;
    Int32 size;

    data.writeCString((const char *)name);
    get_service_manager()->transact(GET_SERVICE, data, &reply);

    ec = (ECode)reply.readInt32();
    if (FAILED(ec)) return ec;

    size = reply.readInt32();
    binder = reply.readStrongBinder();
    // TODO: release buf.
    buf = malloc(size);
    if (!buf) return E_OUT_OF_MEMORY;
    reply.read(buf, (size_t)size);
    ((InterfacePack*)buf)->m_pBinder = binder;
    ec = _CObject_UnmarshalInterface(
        UnmarshalFlag_Noncoexisting, buf, service, &size);
    free(buf);
    if (SUCCEEDED(ec)) {
        // TODO: release old key.
        mServiceCache[name] = *service;
    }
    return ec;
}

ECode CServiceManager::RemoveService(
    /* [in] */ const String& name)
{
    android::Parcel data, reply;
    data.writeCString((const char *)name);
    get_service_manager()->transact(REMOVE_SERVICE, data, &reply);
    return (ECode)reply.readInt32();
}

/**
 * This is only intended to be called when the process is first being brought
 * up and bound by the activity manager. There is only one thread in the process
 * at that time, so no locking is done.
 *
 * @param cache the cache of service references
 * @hide
 */
ECode CServiceManager::InitServiceCache(
    /* [in] */ IObjectStringMap* services)
{
    if (mServiceCache.GetSize() != 0 /*&& Process.supportsProcesses()*/) {
//        throw new IllegalStateException("setServiceCache may only be called once");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
//    sCache.putAll(cache);
    return NOERROR;
}

