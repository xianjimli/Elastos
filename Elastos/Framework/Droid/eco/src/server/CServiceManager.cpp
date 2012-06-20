
#ifdef _MSC_VER
#include <dbus/dbus.h>
#else
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/Binder.h>
#endif
#include "server/CServiceManager.h"
#include "ext/frameworkerr.h"
#include "utils/log.h"

#define ELASTOS_SERVICEMGR_NAME "elastos.servicemanager"

#ifdef _linux
// come from marshal.h
typedef struct InterfacePack
{
    EMuid               m_clsid;             // clsid of object
    UInt32              m_uIndex;            // interface index in class
    android::sp<android::IBinder> m_pBinder; // dbus unique connection name of stub
} InterfacePack;
#endif

CServiceManager::CServiceManager() :
    mServiceCache(11)
{}

CServiceManager::~CServiceManager()
{
    mServiceCache.Clear();
}

#ifdef _linux
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
#endif

ECode CServiceManager::AddService(
    /* [in] */ String name,
    /* [in] */ IInterface * service)
{
#ifdef _MSC_VER
    DBusError err;
    DBusConnection* conn;
    DBusMessage* msg;
    DBusMessage* reply;
    DBusMessageIter args;
    DBusMessageIter subarg;
    Int32 ret;
    const char* str;
    Int32 size;

    LOGD("Calling remote method AddService with service name:%s.\n", (const char*)name);

    // initialiset the errors
    dbus_error_init(&err);

    // connect to the system bus and check for errors
    conn = dbus_bus_get_private(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        LOGE("Connection Error (%s)\n", err.message);
        dbus_error_free(&err);
    }
    if (conn == NULL) {
        return E_FAIL;
    }

    //create CObjectStub

    // create a new method call and check for errors
    msg = dbus_message_new_method_call("Elastos.Framework.ServiceManager",// target for the method call
                                       "/Elastos/Framework/ServiceManager", // object to call on
                                       "Elastos.Framework.IServiceManager",    // interface to call on
                                       "AddService");             // method name
    if (msg == NULL) {
        LOGE("Message Null\n");
        return E_FAIL;
    }

    // append arguments
    dbus_message_iter_init_append(msg, &args);


    void* buf;
    _CObject_MarshalInterface(service, &buf, &size);

    str = (const char*)name;
    if (!dbus_message_iter_append_basic(&args,
                                        DBUS_TYPE_STRING,
                                        &str)) {
        LOGE("Out Of Memory!\n");
        return E_FAIL;
    }


    dbus_message_iter_open_container(&args,
                                     DBUS_TYPE_ARRAY,
                                     DBUS_TYPE_BYTE_AS_STRING,
                                     &subarg);
    dbus_message_iter_append_fixed_array(&subarg,
                                         DBUS_TYPE_BYTE,
                                         &buf,
                                         size);
    dbus_message_iter_close_container(&args, &subarg);


    // send message and get a handle for a reply
    reply = dbus_connection_send_with_reply_and_block(conn, msg, INT_MAX, &err);
    if (dbus_error_is_set(&err)) {
        LOGE("Send with reply Error (%s)\n", err.message);
        dbus_error_free(&err);
    }
    if (NULL == msg) {
        LOGE("Reply Null\n");
        return E_FAIL;
    }
    dbus_connection_flush(conn);

    LOGD("Request Sent\n");

    // free message
    dbus_message_unref(msg);

    // read the parameters
    if (!dbus_message_iter_init(reply, &args)) {
        LOGE("Message has no arguments!\n");
    }
    else if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args)) {
        LOGE("Argument is not boolean!\n");
    }
    else dbus_message_iter_get_basic(&args, &ret);

    LOGD("Got Reply: %d\n", ret);

    // free reply
    dbus_message_unref(reply);

    return (ECode)ret;
#else // _MSC_VER
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
#endif // _MSC_VER
}

ECode CServiceManager::GetService(
    /* [in] */ String name,
    /* [out] */ IInterface** service)
{
    AutoPtr<IInterface> svr = mServiceCache[name];
    if (svr != NULL) {
        *service = (IInterface*)svr;
        (*service)->AddRef();
        return NOERROR;
    }

#ifdef _MSC_VER
    DBusError err;
    DBusConnection* conn;
    DBusMessage* msg;
    DBusMessage* reply;
    DBusMessageIter args;
    DBusMessageIter subarg;
    Int32 ret;
    const char* str;
    void* buf;
    void* buf1;
    Int32 size;

    printf("Calling remote method AddService with service name:%s.\n", (const char *)name);

    // initialiset the errors
    dbus_error_init(&err);

    // connect to the system bus and check for errors
    conn = dbus_bus_get_private(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        LOGE("Connection Error (%s)\n", err.message);
        dbus_error_free(&err);
    }
    if (conn == NULL) {
        return E_FAIL;
    }

    //create CObjectStub

    // create a new method call and check for errors
    msg = dbus_message_new_method_call("Elastos.Framework.ServiceManager",// target for the method call
                                       "/Elastos/Framework/ServiceManager", // object to call on
                                       "Elastos.Framework.IServiceManager",    // interface to call on
                                       "GetService");             // method name
    if (msg == NULL) {
        LOGE("Message Null\n");
        return E_FAIL;
    }

    // append arguments
    dbus_message_iter_init_append(msg, &args);

    str = (const char*)name;
    if (!dbus_message_iter_append_basic(&args,
                                        DBUS_TYPE_STRING,
                                        &str)) {
        LOGE("Out Of Memory!\n");
        return E_FAIL;
    }

    // send message and get a handle for a reply
    reply = dbus_connection_send_with_reply_and_block(conn, msg, INT_MAX, &err);
    if (dbus_error_is_set(&err)) {
        LOGE("Send with reply Error (%s)\n", err.message);
        dbus_error_free(&err);
    }
    if (NULL == msg) {
        LOGE("Reply Null\n");
        return E_FAIL;
    }
    dbus_connection_flush(conn);

    LOGD("Request Sent\n");

    // free message
    dbus_message_unref(msg);

    // read the parameters
    if (!dbus_message_iter_init(reply, &args)) {
        LOGE("Message has no arguments!\n");
    }
    else if (DBUS_TYPE_INT32 != dbus_message_iter_get_arg_type(&args)) {
        LOGE("Argument is not boolean!\n");
    }
    else dbus_message_iter_get_basic(&args, &ret);

    LOGD("Got Reply: %d\n", ret);

    if (SUCCEEDED((ECode)ret)) {
        if (!dbus_message_iter_next(&args)) {
            LOGE("Message has too few arguments!\n");
        }
        else if (DBUS_TYPE_ARRAY != dbus_message_iter_get_arg_type(&args)) {
            LOGE("Argument is not array!\n");
        }
        else {
            dbus_message_iter_recurse(&args, &subarg);
            dbus_message_iter_get_fixed_array(&subarg, &buf, &size);
            buf1 = malloc(size);
            if (buf1 == NULL) {
                LOGE("Out of memory.\n");
                return E_FAIL;
            }
            memcpy(buf1, buf, size);

            _CObject_UnmarshalInterface(UnmarshalFlag_Noncoexisting,
                    buf1, service, &size);

            mServiceCache[name] = *service;
        }
    }

    // free reply
    dbus_message_unref(reply);

    return (ECode)ret;
#else   // _MSC_VER
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
        mServiceCache[String::Duplicate(name)] = *service;
    }
    return ec;
#endif  // _MSC_VER
}

ECode CServiceManager::RemoveService(
    /* [in] */ String name)
{
#ifdef _MSC_VER
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
#else   // _MSC_VER
    android::Parcel data, reply;
    data.writeCString((const char *)name);
    get_service_manager()->transact(REMOVE_SERVICE, data, &reply);
    return (ECode)reply.readInt32();
#endif  // _MSC_VER
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

