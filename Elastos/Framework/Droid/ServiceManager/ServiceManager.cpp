
#include "ServiceManager.h"

#include <utils/Errors.h>

#include <elastos.h>
using namespace Elastos;

ServiceManager::ServiceManager()
    : mServices(MAX_SERVICE_NUMBER)
{
}

ServiceManager::~ServiceManager()
{
    HashMap<String, InterfacePack*,
        Hash<String>, StringEq>::Iterator it = mServices.Begin();
    while (it != mServices.End()) {
        //Pair<const String, InterfacePack*> & pair = *iterator;
        if (it->mSecond != NULL) {
            ((InterfacePack *)it->mSecond)->m_pBinder = NULL;
            free(it->mSecond);
        }
        it++;
    }
}

android::status_t ServiceManager::onTransact(
    /* [in] */ uint32_t code,
    /* [in] */ const android::Parcel& data,
    /* [in] */ android::Parcel* reply,
    /* [in] */ uint32_t flags)
{
    ECode ec = NOERROR;
    const char * name;

    switch (code) {
        case ADD_SERVICE: {
            InterfacePack * ip = NULL;
            name = data.readCString();
            int32_t size = data.readInt32();
            ip = (InterfacePack *)malloc(size);
            if (!ip) {
                reply->writeInt32((int32_t)E_OUT_OF_MEMORY);
            } else {
                const android::sp<android::IBinder> binder = data.readStrongBinder();
                data.read((void *)ip, (size_t)size);
                ip->m_pBinder = binder;
                ec = AddService(String(name), ip);
                reply->writeInt32((int32_t)ec);
            }
            return android::NO_ERROR;
        } break;
        case GET_SERVICE: {
            InterfacePack ip;
            name = data.readCString();
            ec = GetService(String(name), &ip);
            reply->writeInt32((int32_t)ec);
            if (SUCCEEDED(ec)) {
                int32_t size = sizeof(InterfacePack);
                reply->writeInt32((int32_t)size);
                reply->writeStrongBinder(ip.m_pBinder);
                ip.m_pBinder = NULL;
                reply->write((const void *)&ip, (int32_t)size);
            }
            return android::NO_ERROR;
        } break;
        case REMOVE_SERVICE: {
            name = data.readCString();
            ec = RemoveService(String(name));
            reply->writeInt32((int32_t)ec);
            return android::NO_ERROR;
        } break;
        default:
            return android::BBinder::onTransact(code, data, reply, flags);
    }
}

ECode ServiceManager::AddService(
    /* [in] */ const String& name,
    /* [in] */ InterfacePack * ip)
{
    // TODO: release key.
    mServices[name] = ip;
    return NOERROR;
}

ECode ServiceManager::GetService(
    /* [in] */ const String& name,
    /* [out] */ InterfacePack * pIp)
{
    InterfacePack * ip = mServices[name];
    if (!ip) return E_FAIL;
    memset(pIp, 0, sizeof(InterfacePack));
    pIp->m_clsid = ip->m_clsid;
    pIp->m_uIndex = ip->m_uIndex;
    pIp->m_pBinder = ip->m_pBinder;
    return NOERROR;
}

ECode ServiceManager::RemoveService(
    /* [in] */ const String& name)
{
    void * buf = mServices[name];
    if (buf) {
        mServices.Erase(name);
        ((InterfacePack *)buf)->m_pBinder = NULL;
        free(buf);
    }
    return NOERROR;
}
