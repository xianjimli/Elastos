
#include "location/CListenerTransport.h"
#include "location/LocationManager.h"
#include "location/CLocation.h"
#include "os/CApartment.h"


ECode CListenerTransport::constructor(
    /* [in] */ Handle32 host,
    /* [in] */ ILocalLocationListener* listener,
    /* [in] */ IApartment* apartment)
{
    mHost = (LocationManager*)host;
    mListener = listener;

    if (apartment == NULL) {
        assert(SUCCEEDED(CApartment::GetMyApartment((IApartment**)&mListenerHandler))
        && (mListenerHandler != NULL));
    }
    else {
        mListenerHandler = apartment;
    }
    return NOERROR;
}

ECode CListenerTransport::OnLocationChanged(
    /* [in] */ ILocation* location)
{
    ECode (STDCALL CListenerTransport::*pHandlerFunc)(ILocation*);
    pHandlerFunc = &CListenerTransport::HandleLocationChanged;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteInterfacePtr((IInterface*)location);

    return mListenerHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CListenerTransport::OnStatusChanged(
    /* [in] */ const String& provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras)
{
    ECode (STDCALL CListenerTransport::*pHandlerFunc)(const String&, Int32, IBundle*);
    pHandlerFunc = &CListenerTransport::HandleStatusChanged;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(provider);
    params->WriteInt32(status);
    if (extras != NULL) {
        params->WriteInterfacePtr((IInterface*)extras);
    }

    return mListenerHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CListenerTransport::OnProviderEnabled(
    /* [in] */ const String& provider)
{
    ECode (STDCALL CListenerTransport::*pHandlerFunc)(const String&);
    pHandlerFunc = &CListenerTransport::HandleProviderEnabled;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(provider);

    return mListenerHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CListenerTransport::OnProviderDisabled(
    /* [in] */ const String& provider)
{
    ECode (STDCALL CListenerTransport::*pHandlerFunc)(const String&);
    pHandlerFunc = &CListenerTransport::HandleProviderDisabled;

    AutoPtr<IParcel> params;
    CCallbackParcel::New((IParcel**)&params);
    params->WriteString(provider);

    return mListenerHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
}

ECode CListenerTransport::HandleLocationChanged(
    /* [in] */ ILocation* location)
{
    AutoPtr<ILocation> nativeLocation;
    CLocation::New(location, (ILocation**)&nativeLocation);
    mListener->OnLocationChanged(nativeLocation);
    // try {
    return mHost->mService->LocationCallbackFinished(this);
    // } catch (RemoteException e) {
    //     Log.e(TAG, "locationCallbackFinished: RemoteException", e);
    // }
}

ECode CListenerTransport::HandleStatusChanged(
    /* [in] */ const String& provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras)
{
    mListener->OnStatusChanged(provider, status, extras);
    // try {
    return mHost->mService->LocationCallbackFinished(this);
    // } catch (RemoteException e) {
    //     Log.e(TAG, "locationCallbackFinished: RemoteException", e);
    // }
}

ECode CListenerTransport::HandleProviderEnabled(
    /* [in] */ const String& provider)
{
    mListener->OnProviderEnabled(provider);
    // try {
    return mHost->mService->LocationCallbackFinished(this);
    // } catch (RemoteException e) {
    //     Log.e(TAG, "locationCallbackFinished: RemoteException", e);
    // }
}

ECode CListenerTransport::HandleProviderDisabled(
    /* [in] */ const String& provider)
{
    mListener->OnProviderDisabled(provider);
    // try {
    return mHost->mService->LocationCallbackFinished(this);
    // } catch (RemoteException e) {
    //     Log.e(TAG, "locationCallbackFinished: RemoteException", e);
    // }
}
