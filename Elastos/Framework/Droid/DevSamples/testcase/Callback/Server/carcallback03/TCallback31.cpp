
#include "TCallback31.h"

ECode TCallback31::RequestService(
    /* [in] */ const String& serviceName,
    /* [in] */ const String& arguments,
    /* [in] */ IAppletSession * pSession)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode TCallback31::Notify(
    /* [in] */ const String& eventName,
    /* [in] */ const MemoryBuf & arguments)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode TCallback31::ObjectEnter(
    /* [in] */ PInterface pObject)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode TCallback31::ObjectLeave(
    /* [in] */ PInterface pObject)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode TCallback31::CreateObject(
    /* [in] */ const ClassID & clsid,
    /* [in] */ const InterfaceID & iid,
    /* [out] */ IInterface ** ppObject)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode TCallback31::Hello31()
{
    Callback::OnTrigger31();
    return NOERROR;
}

ECode TCallback31::Main(
    /* [in] */ const BufferOf<String> & args)
{
    return NOERROR;
}
