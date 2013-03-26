
#include "hardware/usb/CUsbRequest.h"
#include "hardware/usb/CUsbDeviceConnection.h"
#include <elastos/System.h>
#include <Logger.h>
#include <usbhost/usbhost.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

const String CUsbRequest::TAG = String("UsbRequest");

CUsbRequest::~CUsbRequest()
{
    Finalize();
}

ECode CUsbRequest::constructor()
{
    return NOERROR;
}

ECode CUsbRequest::Initialize(
    /* [in] */ IUsbDeviceConnection* connection,
    /* [in] */ IUsbEndpoint* endpoint,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 address;
    endpoint->GetAddress(&address);

    Int32 attributes;
    endpoint->GetAttributes(&attributes);

    Int32 size;
    endpoint->GetMaxPacketSize(&size);

    Int32 interval;
    endpoint->GetInterval(&interval);

    *result = NativeInit(connection, address, attributes, size, interval);
    return NOERROR;
}

ECode CUsbRequest::Close()
{
    mEndpoint->Release();
    NativeClose();
    return NOERROR;
}

ECode CUsbRequest::GetEndpoint(
    /* [out] */ IUsbEndpoint** endpoint)
{
    VALIDATE_NOT_NULL(endpoint);
    *endpoint = mEndpoint;
    return NOERROR;
}

ECode CUsbRequest::GetClientData(
    /* [out] */ IInterface** data)
{
    VALIDATE_NOT_NULL(data);
    *data = mClientData;
    return NOERROR;
}

ECode CUsbRequest::SetClientData(
    /* [in] */ IInterface* data)
{
    mClientData = data;
    data->Release();
    return NOERROR;
}

ECode CUsbRequest::Queue(
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int32 length,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Boolean isDirectSpecified;
    buffer->IsDirect(&isDirectSpecified);

    Boolean hasArraySpecified;
    buffer->HasArray(&hasArraySpecified);

    Int32 direction;
    mEndpoint->GetDirection(&direction);
    Boolean endpointOutDirection = (direction == UsbConstants_USB_DIR_OUT);

    if (isDirectSpecified == TRUE) {
        *result = NativeQueueDirect(buffer, length, endpointOutDirection);
    } else if (hasArraySpecified == TRUE) {
        ArrayOf<Byte>* array;
        buffer->Array(&array);
        *result = NativeQueueArray(array, length, endpointOutDirection);
    } else {
        Logger::E(CUsbRequest::TAG,"buffer is not direct and has no array");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (*result == TRUE) {
        // save our buffer for when the request has completed
        mBuffer = buffer;
        buffer->Release();

        mLength = length;
    }

    return NOERROR;
}

ECode CUsbRequest::Dequeue()
{
    Boolean isOutDirect;
    ((IBuffer*)mBuffer)->IsDirect(&isOutDirect);

    if (isOutDirect == TRUE) {
        NativeDequeueDirect();
    } else {
        ArrayOf<Byte>* array;
        mBuffer->Array(&array);

        Int32 direction;
        mEndpoint->GetDirection(&direction);
        Boolean endpointOutDirection = (direction == UsbConstants_USB_DIR_OUT);

        NativeDequeueArray(array, mLength, endpointOutDirection);
    }

    mBuffer->Release();
    mLength = 0;

    return NOERROR;
}

ECode CUsbRequest::Cancel(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NativeCancel();
    return NOERROR;
}

void CUsbRequest::Finalize()
{
    if (mEndpoint == NULL) {
        return;
    }

    /*
     * Log.v(TAG, "endpoint still open in finalize(): " + this);
     * close();
     */
    Logger::V(CUsbRequest::TAG, "endpoint still open in finalize().");
    Close();
}

struct usb_device* Get_device_from_object(IUsbDeviceConnection* connection)
{
    return (struct usb_device*)(((CUsbDeviceConnection*)connection)->mNativeContext);
}

struct usb_request* Get_request_from_object(IUsbRequest* java_request)
{
    return (struct usb_request*)(((CUsbRequest*)java_request)->mNativeContext);
}

Boolean CUsbRequest::NativeInit(
    /* [in] */ IUsbDeviceConnection* connection,
    /* [in] */ Int32 ep_address,
    /* [in] */ Int32 ep_attributes,
    /* [in] */ Int32 ep_max_packet_size,
    /* [in] */ Int32 ep_interval)
{
    //ALOGD("init\n");
    Logger::D(CUsbRequest::TAG, "init\n");
    struct usb_device* device =  Get_device_from_object(connection);
    if (!device) {
        //ALOGE("device null in native_init");
        Logger::D(CUsbRequest::TAG, "device null in native_init");
        return FALSE;
    }

    // construct an endpoint descriptor from the Java object fields
    struct usb_endpoint_descriptor desc;
    desc.bLength = USB_DT_ENDPOINT_SIZE;
    desc.bDescriptorType = USB_DT_ENDPOINT;
    desc.bEndpointAddress = ep_address;
    desc.bmAttributes = ep_attributes;
    desc.wMaxPacketSize = ep_max_packet_size;
    desc.bInterval = ep_interval;

    struct usb_request* request = usb_request_new(device, &desc);
    if (request){
        (((CUsbDeviceConnection*)connection)->mNativeContext) = (Int32)request;
    }

    return (request != NULL);
}

void CUsbRequest::NativeClose()
{
    //ALOGD("close\n");
    Logger::D(CUsbRequest::TAG, "close\n");
    struct usb_request* request = Get_request_from_object(this);
    if (request) {
        usb_request_free(request);
        this->mNativeContext = 0;
    }
}

Boolean CUsbRequest::NativeQueueArray(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 length,
    /* [in] */ Boolean out)
{
    struct usb_request* request = Get_request_from_object(this);
    if (!request) {
        //ALOGE("request is closed in native_queue");
        Logger::E(CUsbRequest::TAG, "request is closed in native_queue");
        return FALSE;
    }

    if (buffer && length) {
        request->buffer = malloc(length);
        if (!request->buffer){
            return FALSE;
        }

        if (out) {
            // copy data from Java buffer to native buffer
            //GetByteArrayRegion(buffer, 0, length, (Byte *)request->buffer);
            Byte * tmp = (Byte *)(request->buffer);
            for (Int32 i=0; i<length ;i++)
            {
                tmp[i] = (Byte)(*buffer)[i];    
            }
        }
    } 

    else {
        request->buffer = NULL;
    }

    request->buffer_length = length;

    if (usb_request_queue(request)) {
        if (request->buffer) {
            // free our buffer if usb_request_queue fails
            free(request->buffer);
            request->buffer = NULL;
        }
        return FALSE;
    } 

    else {
        // save a reference to ourselves so UsbDeviceConnection.waitRequest() can find us
        request->client_data = (void *)this;
        return TRUE;
    }
}

void CUsbRequest::NativeDequeueArray(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 length,
    /* [in] */ Boolean out)
{
    struct usb_request* request = Get_request_from_object(this);
    if (!request) {
        //ALOGE("request is closed in native_dequeue");
        Logger::E(CUsbRequest::TAG, "request is closed in native_dequeue");
        return;
    }

    if (buffer && length && request->buffer && !out) {
        // copy data from native buffer to Java buffer
        //SetByteArrayRegion(buffer, 0, length, (Byte*)request->buffer);
        Byte * tmp = (Byte *)(request->buffer);
        for (Int32 i=0; i<length ;i++)
        {
            (*buffer)[i] = tmp[i];    
        }
    }
    free(request->buffer);
    free(request->client_data);
}

Boolean CUsbRequest::NativeQueueDirect(
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int32 length,
    /* [in] */ Boolean out)
{
    struct usb_request* request = Get_request_from_object(this);
    if (!request) {
        //ALOGE("request is closed in native_queue");
        Logger::E(CUsbRequest::TAG, "request is closed in native_queue");
        return FALSE;
    }

    if (buffer && length) {
        request->buffer = &buffer ;//GetDirectBufferAddress(buffer)
        if (!request->buffer)
            return FALSE;
    } 

    else {
        request->buffer = NULL;
    }

    request->buffer_length = length;

    if (usb_request_queue(request)) {
        request->buffer = NULL;
        return FALSE;
    } 

    else {
        // save a reference to ourselves so UsbDeviceConnection.waitRequest() can find us
        // we also need this to make sure our native buffer is not deallocated
        // while IO is active
        request->client_data = (void *)this;
        return TRUE;
    }
}

void CUsbRequest::NativeDequeueDirect()
{
    struct usb_request* request = Get_request_from_object(this);
    if (!request) {
        //ALOGE("request is closed in native_dequeue");
        Logger::E(CUsbRequest::TAG, "request is closed in native_dequeue");
        return;
    }
    // all we need to do is delete our global ref
    free(request->client_data);
}

Boolean CUsbRequest::NativeCancel()
{
    struct usb_request* request = Get_request_from_object(this);
    if (!request) {
        //ALOGE("request is closed in native_cancel");
        Logger::E(CUsbRequest::TAG, "request is closed in native_cancel");
        return FALSE;
    }

    return (usb_request_cancel(request) == 0);
}
