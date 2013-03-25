
#include "server/usb/UsbHostManager.h"
#include <usbhost/usbhost.h>
#include <Logger.h>

using namespace Elastos::Utility::Logging;

const String UsbHostManager::TAG = String("UsbHostManager");
const Boolean UsbHostManager::LOG = FALSE;

UsbHostManager::SRRunnable::SRRunnable(
    /* [in] */ UsbHostManager* host)
{
    mHost = host;
}

ECode UsbHostManager::SRRunnable::Run()
{
    mHost->NativeMonitorUsbHostBus();
    return NOERROR;
}

PInterface UsbHostManager::SRRunnable::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }

    return NULL;
}

UInt32 UsbHostManager::SRRunnable::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 UsbHostManager::SRRunnable::Release()
{
    return ElRefBase::Release();
}

ECode UsbHostManager::SRRunnable::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

UsbHostManager::UsbHostManager(
    /* [in] */ IContext* context,
    /* [in] */ UsbSettingsManager* settingsManager)
{
    mContext = context;
    mSettingsManager = settingsManager;

    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    resources->GetStringArray(0x0107001f /* com.android.internal.R.array.config_usbHostBlacklist */, &mHostBlacklist);
}

UInt32 UsbHostManager::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 UsbHostManager::Release()
{
    return ElRefBase::Release();
}

ECode UsbHostManager::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

void UsbHostManager::SystemReady()
{
    Mutex::Autolock lock(mLock);

    AutoPtr<IRunnable> runnable = new SRRunnable(this);
    AutoPtr<IThread> thread;
    CThread::New(NULL, runnable, String("UsbService host thread"), (IThread**)&thread);
    thread->Start();
}

void UsbHostManager::GetDeviceList(
    /* [in] */ IBundle* devices)
{
    Mutex::Autolock lock(mLock);

    HashMap< String, AutoPtr<IUsbDevice> >::Iterator it;

    for (it = mDevices.Begin(); it != mDevices.End(); ++it)
    {
        String name = it->mFirst;
        AutoPtr<IUsbDevice> tempDevice = it->mSecond;
        IUsbDevice* device = tempDevice;
        devices->PutParcelable(name, (IParcelable*)device);
    }
}

ECode UsbHostManager::OpenDevice(
    /* [in] */ const String& deviceName,
    /* [out] */ IParcelFileDescriptor** pfd)
{
    VALIDATE_NOT_NULL(pfd);

    Mutex::Autolock lock(mLock);

    if (IsBlackListed(deviceName) == TRUE) {
        // throw new SecurityException("USB device is on a restricted bus");
        return E_SECURITY_EXCEPTION;
    }

    HashMap< String, AutoPtr<IUsbDevice> >::Iterator it = mDevices.Find(deviceName);

    if (it == mDevices.End()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IUsbDevice> device = it->mSecond;

    if (device == NULL) {
        // if it is not in mDevices, it either does not exist or is blacklisted
        // throw new IllegalArgumentException("device " + deviceName + " does not exist or is restricted");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mSettingsManager->CheckPermission(device);
    *pfd = NativeOpenDevice(deviceName);

    return NOERROR;
}

Boolean UsbHostManager::IsBlackListed(
    /* [in] */ const String& deviceName)
{
    Int32 count = mHostBlacklist->GetLength();

    for (Int32 i = 0; i < count; i++) {
        if (deviceName.StartWith((*mHostBlacklist)[i]) == FALSE) {
            continue;
        }

        return TRUE;
    }

    return FALSE;
}

Boolean UsbHostManager::IsBlackListed(
    /* [in] */ Int32 clazz,
    /* [in] */ Int32 subClass,
    /* [in] */ Int32 protocol)
{
    // blacklist hubs
    if (clazz == UsbConstants_USB_CLASS_HUB) {
        return TRUE;
    }

    // blacklist HID boot devices (mouse and keyboard)
    if (clazz == UsbConstants_USB_CLASS_HID && subClass == UsbConstants_USB_INTERFACE_SUBCLASS_BOOT) {
        return TRUE;
    }

    return FALSE;
}

void UsbHostManager::UsbDeviceAdded(
    /* [in] */ const String& deviceName,
    /* [in] */ Int32 vendorID,
    /* [in] */ Int32 productID,
    /* [in] */ Int32 deviceClass,
    /* [in] */ Int32 deviceSubclass,
    /* [in] */ Int32 deviceProtocol,
    /* [in] */ const ArrayOf<Int32>& interfaceValues,
    /* [in] */ const ArrayOf<Int32>& endpointValues)
{
    if (IsBlackListed(deviceName) || IsBlackListed(deviceClass, deviceSubclass, deviceProtocol)) {
        return;
    }

    Mutex::Autolock lock(mLock);

    if (IsDeviceExists(deviceName) == TRUE) {
        // Slog.w(TAG, "device already on mDevices list: " + deviceName);
        Logger::W(UsbHostManager::TAG, "device already on mDevices list:");
        Logger::W(UsbHostManager::TAG, deviceName);
        return;
    }

    Int32 numInterfaces = interfaceValues.GetLength() / 5;
    ArrayOf<IParcelable*>* interfaces = ArrayOf<IParcelable*>::Alloc(numInterfaces);

    // repackage interfaceValues as an array of UsbInterface
    Int32 intf, endp, ival = 0, eval = 0;
    for (intf = 0; intf < numInterfaces; intf++) {
        Int32 interfaceId = interfaceValues[ival++];
        Int32 interfaceClass = interfaceValues[ival++];
        Int32 interfaceSubclass = interfaceValues[ival++];
        Int32 interfaceProtocol = interfaceValues[ival++];
        Int32 numEndpoints = interfaceValues[ival++];

        ArrayOf<IParcelable*>* endpoints = ArrayOf<IParcelable*>::Alloc(numEndpoints);
        for (endp = 0; endp < numEndpoints; endp++) {
            Int32 address = endpointValues[eval++];
            Int32 attributes = endpointValues[eval++];
            Int32 maxPacketSize = endpointValues[eval++];
            Int32 interval = endpointValues[eval++];

            AutoPtr<IUsbEndpoint> tempEndpoint;
            CUsbEndpoint::New(address, attributes, maxPacketSize, interval, (IUsbEndpoint**)&tempEndpoint);

            IUsbEndpoint* endpoint = tempEndpoint;
            (*endpoints)[endp] = (IParcelable*)endpoint;
        }

        // don't allow if any interfaces are blacklisted
        if (IsBlackListed(interfaceClass, interfaceSubclass, interfaceProtocol)) {
            return;
        }

        AutoPtr<IUsbInterface> tempInterface;
        CUsbInterface::New(interfaceId, interfaceClass, interfaceSubclass, interfaceProtocol, *endpoints, (IUsbInterface**)&tempInterface);

        IUsbInterface* targetInterface = tempInterface;
        (*interfaces)[intf] = (IParcelable*)targetInterface;
    }

    AutoPtr<IUsbDevice> targetDevice;
    CUsbDevice::New(deviceName, vendorID, productID, deviceClass, deviceSubclass, deviceProtocol, *interfaces, (IUsbDevice**)&targetDevice);

    mDevices[deviceName] = targetDevice;
    mSettingsManager->DeviceAttached(targetDevice);
}

void UsbHostManager::UsbDeviceRemoved(
    /* [in] */ const String& deviceName)
{
    Mutex::Autolock lock(mLock);

    HashMap< String, AutoPtr<IUsbDevice> >::Iterator it = mDevices.Find(deviceName);

    if (it == mDevices.End()) {
        return;
    }

    AutoPtr<IUsbDevice> device = it->mSecond;
    mSettingsManager->DeviceDetached(device);
    mDevices.Erase(it);
}

void UsbHostManager::NativeMonitorUsbHostBus()
{
    struct usb_host_context* context = usb_host_init();

    if (!context) {
        Logger::E(UsbHostManager::TAG, "usb_host_init failed");
        return;
    }

    // this will never return so it is safe to pass thiz directly
    //usb_host_run(context, usb_device_added, usb_device_removed, NULL, (void *)thiz);

    // NOT IMPLEMENTED
}

IParcelFileDescriptor* UsbHostManager::NativeOpenDevice(
    /* [in] */ const String& deviceName)
{
    // NOT IMPLEMENTED
    return NULL;
}

Boolean UsbHostManager::IsDeviceExists(
    /* [in] */ const String& deviceName)
{
    HashMap< String, AutoPtr<IUsbDevice> >::Iterator it = mDevices.Find(deviceName);

    if (it == mDevices.End()) {
        return FALSE;
    }

    AutoPtr<IUsbDevice> device = it->mSecond;

    if (device == NULL) {
        return FALSE;
    }

    return TRUE;
}
