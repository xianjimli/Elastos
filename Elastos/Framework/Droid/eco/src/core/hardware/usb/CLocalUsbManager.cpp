
#include "hardware/usb/CLocalUsbManager.h"
#include "hardware/usb/CUsbDeviceConnection.h"
#include <elastos/System.h>
#include <elastos/Character.h>
#include "os/SystemProperties.h"

using namespace Elastos::Core;

const String CLocalUsbManager::TAG("UsbManager");

ECode CLocalUsbManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IUsbManager* service)
{
    mContext = context;
    context->Release();

    mService = service;
    service->Release();

    return NOERROR;
}

ECode CLocalUsbManager::GetDeviceList(
    /* [out] */ IObjectStringMap** list)
{
    VALIDATE_NOT_NULL(list);

    AutoPtr<IBundle> bundle;
    mService->GetDeviceList((IBundle**)&bundle);

    AutoPtr<IObjectContainer> keySet;
    bundle->KeySet((IObjectContainer**)&keySet);

    if (keySet == NULL) {
        *list = NULL;
        return NOERROR;
    }

    AutoPtr<IObjectEnumerator> iter;
    keySet->GetObjectEnumerator((IObjectEnumerator**)&iter);
    Boolean hasNext;

    CObjectStringMap::New(list);
    while (iter->MoveNext(&hasNext), hasNext) {
        AutoPtr<ICharSequence> chars;
        iter->Current((IInterface**)&chars);

        String name;
        chars->ToString(&name);

        AutoPtr<IUsbDevice> dev;
        bundle->Get(name, (IInterface**)&dev);
        (*list)->Put(name, dev);
    }

    return NOERROR;
}

ECode CLocalUsbManager::OpenDevice(
    /* [in] */ IUsbDevice* device,
    /* [out] */ IUsbDeviceConnection** connection)
{
    VALIDATE_NOT_NULL(connection);

    String deviceName;
    device->GetDeviceName(&deviceName);

    AutoPtr<IParcelFileDescriptor> pfd = NULL;
    mService->OpenDevice(deviceName, (IParcelFileDescriptor**)&pfd);

    if (pfd == NULL) {
        *connection = NULL;
        return NOERROR;
    }

    AutoPtr<IUsbDeviceConnection> tempConn;
    CUsbDeviceConnection::New(device, (IUsbDeviceConnection**)&tempConn);

    Boolean result;
    tempConn->Open(deviceName, pfd, &result);

    if (result == FALSE) {
        *connection = NULL;
        return NOERROR;
    }

    *connection = tempConn;
    return NOERROR;
}

ECode CLocalUsbManager::GetAccessoryList(
    /* [out, callee] */ ArrayOf<IUsbAccessory*>** list)
{
    VALIDATE_NOT_NULL(list);

    AutoPtr<IUsbAccessory> accessory;
    mService->GetCurrentAccessory((IUsbAccessory**)&accessory);

    if (accessory == NULL) {
        *list = NULL;
        return NOERROR;
    }

    ArrayOf<IUsbAccessory*>* tempList = ArrayOf<IUsbAccessory*>::Alloc(1);
    (*tempList)[0] = accessory;
    *list = tempList;
    return NOERROR;
}

ECode CLocalUsbManager::OpenAccessory(
    /* [in] */ IUsbAccessory* accessory,
    /* [out] */ IParcelFileDescriptor** descriptor)
{
    /*
    try {
        return mService.openAccessory(accessory);
    } catch (RemoteException e) {
        Log.e(TAG, "RemoteException in openAccessory", e);
        return null;
    }
    */

    VALIDATE_NOT_NULL(descriptor);
    mService->OpenAccessory(accessory, descriptor);
    return NOERROR;
}

ECode CLocalUsbManager::HasDevicePermission(
    /* [in] */ IUsbDevice* device,
    /* [out] */ Boolean* result)
{
    /*
    try {
        return mService.hasDevicePermission(device);
    } catch (RemoteException e) {
        Log.e(TAG, "RemoteException in hasPermission", e);
        return false;
    }
    */

    VALIDATE_NOT_NULL(result);
    mService->HasDevicePermission(device, result);
    return NOERROR;
}

ECode CLocalUsbManager::HasAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [out] */ Boolean* result)
{
    /*
    try {
        return mService.hasAccessoryPermission(accessory);
    } catch (RemoteException e) {
        Log.e(TAG, "RemoteException in hasPermission", e);
        return false;
    }
    */

    VALIDATE_NOT_NULL(result);
    mService->HasAccessoryPermission(accessory, result);
    return NOERROR;
}

ECode CLocalUsbManager::RequestDevicePermission(
    /* [in] */ IUsbDevice* device,
    /* [in] */ IPendingIntent* pi)
{
    /*
    try {
        mService.requestDevicePermission(device, mContext.getPackageName(), pi);
    } catch (RemoteException e) {
        Log.e(TAG, "RemoteException in requestPermission", e);
    }
    */

    String name;
    mContext->GetCapsuleName(&name);
    mService->RequestDevicePermission(device, name, pi);
    return NOERROR;
}

ECode CLocalUsbManager::RequestAccessoryPermission(
    /* [in] */ IUsbAccessory* accessory,
    /* [in] */ IPendingIntent* pi)
{
    /*
    try {
        mService.requestAccessoryPermission(accessory, mContext.getPackageName(), pi);
    } catch (RemoteException e) {
        Log.e(TAG, "RemoteException in requestPermission", e);
    }
    */

    String name;
    mContext->GetCapsuleName(&name);
    mService->RequestAccessoryPermission(accessory, name, pi);
    return NOERROR;
}

ECode CLocalUsbManager::IsFunctionEnabled(
    /* [in] */ const String& function,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    String property("sys.usb.config");
    *result = PropertyContainsFunction(property, function);
    return NOERROR;
}

ECode CLocalUsbManager::GetDefaultFunction(
    /* [out] */ String* function)
{
    VALIDATE_NOT_NULL(function);

    String str;
    str = SystemProperties::Get("persist.sys.usb.config", "");

    int commaIndex = str.IndexOf(',');

    if (commaIndex > 0) {
        *function = str.Substring(0, commaIndex);
        return NOERROR;
    }

    *function = str;
    return NOERROR;
}

ECode CLocalUsbManager::SetCurrentFunction(
    /* [in] */ const String& function,
    /* [in] */ Boolean makeDefault)
{
    /*
    try {
        mService.setCurrentFunction(function, makeDefault);
    } catch (RemoteException e) {
        Log.e(TAG, "RemoteException in setCurrentFunction", e);
    }
    */

    mService->SetCurrentFunction(function, makeDefault);
    return NOERROR;
}

ECode CLocalUsbManager::SetMassStorageBackingFile(
    /* [in] */ const String& path)
{
    /*
    try {
        mService.setMassStorageBackingFile(path);
    } catch (RemoteException e) {
        Log.e(TAG, "RemoteException in setDefaultFunction", e);
    }
    */

    mService->SetMassStorageBackingFile(path);
    return NOERROR;
}

Boolean CLocalUsbManager::PropertyContainsFunction(
    /* [in] */ const String& property,
    /* [in] */ const String& function)
{
    String functions = SystemProperties::Get(property, "");
    Int32 index = functions.IndexOf(function);

    if (index < 0) {
        return FALSE;
    }

    Char32 c1;
    Character::GetCharAt(functions, (index - 1), &c1);
    if (index > 0 && c1 != ',') {
        return FALSE;
    }

    Int32 charAfter = index + function.GetLength();

    Char32 c2;
    Character::GetCharAt(functions, charAfter, &c2);

    if (charAfter < functions.GetLength() && c2 != ',') {
        return FALSE;
    }

    return TRUE;
}
