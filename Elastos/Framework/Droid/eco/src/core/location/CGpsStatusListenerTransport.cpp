
#include "location/CGpsStatusListenerTransport.h"
#include "location/LocationManager.h"
#include "os/CApartment.h"

CGpsStatusListenerTransport::Nmea::Nmea(
    /* [in] */ Int64 timestamp,
    /* [in] */ const String& nmea)
    : mTimestamp(timestamp)
    , mNmea(nmea)
{}

ECode CGpsStatusListenerTransport::constructor(
    /* [in] */ Handle32 host,
    /* [in] */ ILocalGpsStatusListener* listener)
{
    mHost = (LocationManager*)host;
    mListener = listener;
    assert(SUCCEEDED(CApartment::GetMyApartment((IApartment**)&mGpsHandler))
        && (mGpsHandler != NULL));
    return NOERROR;
}

ECode CGpsStatusListenerTransport::constructor(
    /* [in] */ Handle32 host,
    /* [in] */ ILocalGpsStatusNmeaListener* listener)
{
    mHost = (LocationManager*)host;
    mNmeaListener = listener;
    assert(SUCCEEDED(CApartment::GetMyApartment((IApartment**)&mGpsHandler))
        && (mGpsHandler != NULL));
    return NOERROR;
}

ECode CGpsStatusListenerTransport::OnGpsStarted()
{
    if (mListener != NULL) {
        ECode (STDCALL CGpsStatusListenerTransport::*pHandlerFunc)(Int32);
        pHandlerFunc = &CGpsStatusListenerTransport::HandleGpsChanged;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(GpsStatus_GPS_EVENT_STARTED);

        return mGpsHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }

    return NOERROR;
}

ECode CGpsStatusListenerTransport::OnGpsStopped()
{
    if (mListener != NULL) {
        ECode (STDCALL CGpsStatusListenerTransport::*pHandlerFunc)(Int32);
        pHandlerFunc = &CGpsStatusListenerTransport::HandleGpsChanged;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(GpsStatus_GPS_EVENT_STOPPED);

        return mGpsHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }

    return NOERROR;
}

ECode CGpsStatusListenerTransport::OnFirstFix(
    /* [in] */ Int32 ttff)
{
    if (mListener != NULL) {
        mHost->mGpsStatus->SetTimeToFirstFix(ttff);

        ECode (STDCALL CGpsStatusListenerTransport::*pHandlerFunc)(Int32);
        pHandlerFunc = &CGpsStatusListenerTransport::HandleGpsChanged;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(GpsStatus_GPS_EVENT_FIRST_FIX);

        return mGpsHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }

    return NOERROR;
}

ECode CGpsStatusListenerTransport::OnSvStatusChanged(
    /* [in] */ Int32 svCount,
    /* [in] */ const ArrayOf<Int32>& prns,
    /* [in] */ const ArrayOf<Float>& snrs,
    /* [in] */ const ArrayOf<Float>& elevations,
    /* [in] */ const ArrayOf<Float>& azimuths,
    /* [in] */ Int32 ephemerisMask,
    /* [in] */ Int32 almanacMask,
    /* [in] */ Int32 usedInFixMask)
{
    if (mListener != NULL) {
        mHost->mGpsStatus->SetStatus(svCount, prns, snrs, elevations, azimuths,
                ephemerisMask, almanacMask, usedInFixMask);

        ECode (STDCALL CGpsStatusListenerTransport::*pHandlerFunc)(Int32);
        pHandlerFunc = &CGpsStatusListenerTransport::HandleGpsChanged;

        AutoPtr<IParcel> params;
        CCallbackParcel::New((IParcel**)&params);
        params->WriteInt32(GpsStatus_GPS_EVENT_SATELLITE_STATUS);

        mGpsHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);
        return mGpsHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, params, 0);
    }

    return NOERROR;
}

ECode CGpsStatusListenerTransport::OnNmeaReceived(
    /* [in] */ Int64 timestamp,
    /* [in] */ const String& nmea)
{
    if (mNmeaListener != NULL) {
        {
            Mutex::Autolock lock(mNmeaBufferLock);

            mNmeaBuffer.PushBack(new Nmea(timestamp, nmea));
        }

        ECode (STDCALL CGpsStatusListenerTransport::*pHandlerFunc)();
        pHandlerFunc = &CGpsStatusListenerTransport::HandleNmeaReceived;

        mGpsHandler->RemoveCppCallbacks((Handle32)this, *(Handle32*)&pHandlerFunc);
        return mGpsHandler->PostCppCallback((Handle32)this, *(Handle32*)&pHandlerFunc, NULL, 0);
    }

    return NOERROR;
}

ECode CGpsStatusListenerTransport::HandleGpsChanged(
    /* [in] */ Int32 event)
{
    // synchronize on mGpsStatus to ensure the data is copied atomically.
    Mutex::Autolock lock(mHost->mGpsStatusLock);

    return mListener->OnGpsStatusChanged(event);
}

ECode CGpsStatusListenerTransport::HandleNmeaReceived()
{
    Mutex::Autolock lock(mNmeaBufferLock);

    List< AutoPtr<Nmea> >::Iterator it;
    for (it = mNmeaBuffer.Begin(); it != mNmeaBuffer.End(); ++it) {
        Nmea* nmea = *it;
        mNmeaListener->OnNmeaReceived(nmea->mTimestamp, nmea->mNmea);
    }
    mNmeaBuffer.Clear();
    return NOERROR;
}
