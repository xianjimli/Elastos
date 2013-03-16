
#include "hardware/usb/CUsbDeviceConnection.h"
#include <elastos/System.h>

using namespace Elastos::Core;

const String CUsbDeviceConnection::TAG("UsbDeviceConnection");

ECode CUsbDeviceConnection::constructor(
    /* [in] */ IUsbDevice* device)
{
    mDevice = device;
    device->Release();
    return NOERROR;
}

ECode CUsbDeviceConnection::Open(
    /* [in] */ const String& name,
    /* [in] */ IParcelFileDescriptor* pfd,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IFileDescriptor> fd = NULL;
    pfd->GetFileDescriptor((IFileDescriptor**)&fd);
    *result = NativeOpen(name, fd);

    return NOERROR;
}

ECode CUsbDeviceConnection::Close()
{
    NativeClose();
    return NOERROR;
}

ECode CUsbDeviceConnection::GetFileDescriptor(
    /* [out] */ Int32* descriptor)
{
    VALIDATE_NOT_NULL(descriptor);

    *descriptor = NativeGetFd();
    return NOERROR;
}

ECode CUsbDeviceConnection::GetRawDescriptors(
    /* [out, callee] */ ArrayOf<Byte>** descriptors)
{
    VALIDATE_NOT_NULL(descriptors);

    *descriptors = NativeGetDesc();
    return NOERROR;
}

ECode CUsbDeviceConnection::ClaimInterface(
    /* [in] */ IUsbInterface* intf,
    /* [in] */ Boolean force,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 id;
    intf->GetId(&id);
    *result = NativeClaimInterface(id, force);
    return NOERROR;
}

ECode CUsbDeviceConnection::ReleaseInterface(
    /* [in] */ IUsbInterface* intf,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 id;
    intf->GetId(&id);
    *result = NativeReleaseInterface(id);
    return NOERROR;
}

ECode CUsbDeviceConnection::ControlTransfer(
    /* [in] */ Int32 requestType,
    /* [in] */ Int32 request,
    /* [in] */ Int32 value,
    /* [in] */ Int32 index,
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 length,
    /* [in] */ Int32 timeout,
    /* [out] */ Int32* transferredLength)
{
    VALIDATE_NOT_NULL(transferredLength);

    *transferredLength = NativeControlRequest(requestType, request, value, index, buffer, length, timeout);
    return NOERROR;
}

ECode CUsbDeviceConnection::BulkTransfer(
    /* [in] */ IUsbEndpoint* endpoint,
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 length,
    /* [in] */ Int32 timeout,
    /* [out] */ Int32* transferredLength)
{
    VALIDATE_NOT_NULL(transferredLength);

    Int32 address;
    endpoint->GetAddress(&address);
    *transferredLength = NativeBulkRequest(address, buffer, length, timeout);
    return NOERROR;
}

ECode CUsbDeviceConnection::RequestWait(
    /* [out] */ IUsbRequest** request)
{
    VALIDATE_NOT_NULL(request);

    *request = NativeRequestWait();

    if (*request != NULL) {
        (*request)->Dequeue();
    }

    return NOERROR;
}

ECode CUsbDeviceConnection::GetSerial(
    /* [out] */ String* serialNumber)
{
    VALIDATE_NOT_NULL(serialNumber);

    *serialNumber = NativeGetSerial();
    return NOERROR;
}

Boolean CUsbDeviceConnection::NativeOpen(
        /* [in] */ const String& deviceName,
        /* [in] */ IFileDescriptor* pfd)
{
    /*
    int fd = jniGetFDFromFileDescriptor(env, fileDescriptor);
    // duplicate the file descriptor, since ParcelFileDescriptor will eventually close its copy
    fd = dup(fd);
    if (fd < 0)
        return false;

    const char *deviceNameStr = env->GetStringUTFChars(deviceName, NULL);
    struct usb_device* device = usb_device_new(deviceNameStr, fd);
    if (device) {
        env->SetIntField(thiz, field_context, (int)device);
    } else {
        ALOGE("usb_device_open failed for %s", deviceNameStr);
        close(fd);
    }

    env->ReleaseStringUTFChars(deviceName, deviceNameStr);
    return (device != NULL);
    */
    // NOT IMPLEMENTED
    return FALSE;
}

void CUsbDeviceConnection::NativeClose()
{
    /*
    ALOGD("close\n");
    struct usb_device* device = get_device_from_object(env, thiz);
    if (device) {
        usb_device_close(device);
        env->SetIntField(thiz, field_context, 0);
    }
    */
    // NOT IMPLEMENTED
}

Int32 CUsbDeviceConnection::NativeGetFd()
{
    /*
    struct usb_device* device = get_device_from_object(env, thiz);
    if (!device) {
        ALOGE("device is closed in native_get_fd");
        return -1;
    }
    return usb_device_get_fd(device);
    */
    // NOT IMPLEMENTED
    return -1;
}

ArrayOf<Byte>* CUsbDeviceConnection::NativeGetDesc()
{
    /*
    char buffer[16384];
    int fd = android_hardware_UsbDeviceConnection_get_fd(env, thiz);
    if (fd < 0) return NULL;
    lseek(fd, 0, SEEK_SET);
    int length = read(fd, buffer, sizeof(buffer));
    if (length < 0) return NULL;

    jbyteArray ret = env->NewByteArray(length);
    if (ret) {
        jbyte* bytes = (jbyte*)env->GetPrimitiveArrayCritical(ret, 0);
        if (bytes) {
            memcpy(bytes, buffer, length);
            env->ReleasePrimitiveArrayCritical(ret, bytes, 0);
        }
    }
    return ret;
    */
    // NOT IMPLEMENTED
    return NULL;
}

Boolean CUsbDeviceConnection::NativeClaimInterface(
    /* [in] */ Int32 interfaceID,
    /* [in] */ Boolean force)
{
    /*
    struct usb_device* device = get_device_from_object(env, thiz);
    if (!device) {
        ALOGE("device is closed in native_claim_interface");
        return -1;
    }

    int ret = usb_device_claim_interface(device, interfaceID);
    if (ret && force && errno == EBUSY) {
        // disconnect kernel driver and try again
        usb_device_connect_kernel_driver(device, interfaceID, false);
        ret = usb_device_claim_interface(device, interfaceID);
    }
    return ret == 0;
    */
    // NOT IMPLEMENTED
    return FALSE;
}

Boolean CUsbDeviceConnection::NativeReleaseInterface(
    /* [in] */ Int32 interfaceID)
{
    /*
    struct usb_device* device = get_device_from_object(env, thiz);
    if (!device) {
        ALOGE("device is closed in native_release_interface");
        return -1;
    }
    int ret = usb_device_release_interface(device, interfaceID);
    if (ret == 0) {
        // allow kernel to reconnect its driver
        usb_device_connect_kernel_driver(device, interfaceID, true);
    }
    return ret;
    */
    // NOT IMPLEMENTED
    return FALSE;
}

Int32 CUsbDeviceConnection::NativeControlRequest(
    /* [in] */ Int32 requestType,
    /* [in] */ Int32 request,
    /* [in] */ Int32 value,
    /* [in] */ Int32 index,
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 length,
    /* [in] */ Int32 timeout)
{
    /*
    struct usb_device* device = get_device_from_object(env, thiz);
    if (!device) {
        ALOGE("device is closed in native_control_request");
        return -1;
    }

    jbyte* bufferBytes = NULL;
    if (buffer) {
        if (env->GetArrayLength(buffer) < length) {
            jniThrowException(env, "java/lang/ArrayIndexOutOfBoundsException", NULL);
            return -1;
        }
        bufferBytes = env->GetByteArrayElements(buffer, 0);
    }

    jint result = usb_device_control_transfer(device, requestType, request, value, index, bufferBytes, length, timeout);

    if (bufferBytes)
        env->ReleaseByteArrayElements(buffer, bufferBytes, 0);

    return result;
    */
    // NOT IMPLEMENTED
    return -1;
}

Int32 CUsbDeviceConnection::NativeBulkRequest(
    /* [in] */ Int32 endpoint,
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 length,
    /* [in] */ Int32 timeout)
{
    /*
    struct usb_device* device = get_device_from_object(env, thiz);
    if (!device) {
        ALOGE("device is closed in native_control_request");
        return -1;
    }

    jbyte* bufferBytes = NULL;
    if (buffer) {
        if (env->GetArrayLength(buffer) < length) {
            jniThrowException(env, "java/lang/ArrayIndexOutOfBoundsException", NULL);
            return -1;
        }
        bufferBytes = env->GetByteArrayElements(buffer, 0);
    }

    jint result = usb_device_bulk_transfer(device, endpoint, bufferBytes, length, timeout);

    if (bufferBytes)
        env->ReleaseByteArrayElements(buffer, bufferBytes, 0);

    return result;
    */
    // NOT IMPLEMENTED
    return -1;
}

IUsbRequest* CUsbDeviceConnection::NativeRequestWait()
{
    /*
    struct usb_device* device = get_device_from_object(env, thiz);
    if (!device) {
        ALOGE("device is closed in native_request_wait");
        return NULL;
    }

    struct usb_request* request = usb_request_wait(device);
    if (request)
        return (jobject)request->client_data;
    else
        return NULL;
    */
    // NOT IMPLEMENTED
    return NULL;
}

String CUsbDeviceConnection::NativeGetSerial()
{
    /*
    struct usb_device* device = get_device_from_object(env, thiz);
    if (!device) {
        ALOGE("device is closed in native_request_wait");
        return NULL;
    }
    char* serial = usb_device_get_serial(device);
    if (!serial)
        return NULL;
    jstring result = env->NewStringUTF(serial);
    free(serial);
    return result;
    */
    // NOT IMPLEMENTED
    return String("");
}
