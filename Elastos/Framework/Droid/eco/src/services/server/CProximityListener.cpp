
#include "server/CProximityListener.h"
#include "server/CLocationManagerService.h"
#include <elastos/System.h>

using namespace Elastos::Core;

ECode CProximityListener::constructor(
    /* [in] */ Handle32 host)
{
    mHost = (CLocationManagerService*)host;
    mIsGpsAvailable = FALSE;
    return NOERROR;
}

// Note: this is called with the lock held.
ECode CProximityListener::OnLocationChanged(
    /* [in] */ ILocation* loc)
{
    // If Gps is available, then ignore updates from NetworkLocationProvider
    String provider;
    loc->GetProvider(&provider);
    if (provider.Equals(LocationManager_GPS_PROVIDER)) {
        mIsGpsAvailable = TRUE;
    }
    if (mIsGpsAvailable && provider.Equals(LocationManager_NETWORK_PROVIDER)) {
        return NOERROR;
    }

    // Process proximity alerts
    Int64 now = System::GetCurrentTimeMillis();
    Double latitude, longitude;
    loc->GetLatitude(&latitude);
    loc->GetLongitude(&longitude);
    Float accuracy;
    loc->GetAccuracy(&accuracy);
    List< AutoPtr<IPendingIntent> >* intentsToRemove = NULL;

    HashMap<AutoPtr<IPendingIntent>, AutoPtr<CLocationManagerService::ProximityAlert> >::Iterator it;
    for (it = mHost->mProximityAlerts.Begin();
            it != mHost->mProximityAlerts.End(); ++it) {
        AutoPtr<CLocationManagerService::ProximityAlert> alert = it->mSecond;
        AutoPtr<IPendingIntent> intent = alert->GetIntent();
        Int64 expiration = alert->GetExpiration();

        if (expiration == -1 || (now <= expiration)) {
            Boolean entered =
                    mHost->mProximitiesEntered.Find(alert) != mHost->mProximitiesEntered.End();
            Boolean inProximity =
                    alert->IsInProximity(latitude, longitude, accuracy);
            if (!entered && inProximity) {
//              if (LOCAL_LOGV) {
//                  Slog.v(TAG, "Entered alert");
//              }
                mHost->mProximitiesEntered.Insert(alert);
                AutoPtr<IIntent> enteredIntent;
                CIntent::New((IIntent**)&enteredIntent);
                enteredIntent->PutBooleanExtra(String(LocationManager_KEY_PROXIMITY_ENTERING), TRUE);
//              try {
                {
                    Mutex::Autolock lock(_m_syncLock);
                    // synchronize to ensure incrementPendingBroadcasts()
                    // is called before decrementPendingBroadcasts()
//                  ECode ec = intent->Send(mContext, 0, enteredIntent, this, mLocationHandler);
//                  if (ec == E_CANCELED_EXCEPTION) {
//                      if (LOCAL_LOGV) {
//                          Slog.v(TAG, "Canceled proximity alert: " + alert, e);
//                      }
//                      if (intentsToRemove == NULL) {
//                          intentsToRemove = new List<AutoPtr<IPendingIntent> >();
//                      }
//                      intentsToRemove->Inset(intent);
//                  }
                    // call this after broadcasting so we do not increment
                    // if we throw an exeption.
                    mHost->IncrementPendingBroadcasts();
                }
//              } catch (PendingIntent.CanceledException e) {
//                  if (LOCAL_LOGV) {
//                      Slog.v(TAG, "Canceled proximity alert: " + alert, e);
//                  }
//                  if (intentsToRemove == null) {
//                      intentsToRemove = new ArrayList<PendingIntent>();
//                  }
//                  intentsToRemove.add(intent);
//              }
            }
            else if (entered && !inProximity) {
//              if (LOCAL_LOGV) {
//                  Slog.v(TAG, "Exited alert");
//              }
                mHost->mProximitiesEntered.Erase(alert);
                AutoPtr<IIntent> exitedIntent;
                CIntent::New((IIntent**)&exitedIntent);
                exitedIntent->PutBooleanExtra(String(LocationManager_KEY_PROXIMITY_ENTERING), FALSE);
//              try {
                {
                    Mutex::Autolock lock(_m_syncLock);
                    // synchronize to ensure incrementPendingBroadcasts()
                    // is called before decrementPendingBroadcasts()
//                  ECode ec = intent->Send(mContext, 0, exitedIntent, this, mLocationHandler);
//                  if (ec == E_CANCELED_EXCEPTION) {
//                      if (LOCAL_LOGV) {
//                          Slog.v(TAG, "Canceled proximity alert: " + alert, e);
//                      }
//                      if (intentsToRemove == NULL) {
//                          intentsToRemove = intentsToRemove = new List<PendingIntent>();
//                      }
//                      intentsToRemove->Insert(intent);
//                  }
                    // call this after broadcasting so we do not increment
                    // if we throw an exeption.
                    mHost->IncrementPendingBroadcasts();
                }
//              } catch (PendingIntent.CanceledException e) {
//                  if (LOCAL_LOGV) {
//                      Slog.v(TAG, "Canceled proximity alert: " + alert, e);
//                  }
//                  if (intentsToRemove == null) {
//                      intentsToRemove = new ArrayList<PendingIntent>();
//                  }
//                  intentsToRemove.add(intent);
//              }
            }
        }
        else {
            // Mark alert for expiration
//          if (LOCAL_LOGV) {
//              Slog.v(TAG, "Expiring proximity alert: " + alert);
//          }
            if (intentsToRemove == NULL) {
                intentsToRemove = new List< AutoPtr<IPendingIntent> >();
            }
            intentsToRemove->PushBack(alert->GetIntent());
        }
    }

    // Remove expired alerts
    if (intentsToRemove != NULL) {
        List< AutoPtr<IPendingIntent> >::Iterator it;
        for (it = intentsToRemove->Begin(); it != intentsToRemove->End(); ++it) {
            AutoPtr<IPendingIntent> i = *it;
            HashMap<AutoPtr<IPendingIntent>,
                    AutoPtr<CLocationManagerService::ProximityAlert> >::Iterator it =
                    mHost->mProximityAlerts.Find(i);
            if (it != mHost->mProximityAlerts.End()) {
                mHost->mProximitiesEntered.Erase(it->mSecond);
            }
            mHost->RemoveProximityAlertLocked(i);
        }
    }
    return NOERROR;
}

// Note: this is called with the lock held.
ECode CProximityListener::OnProviderDisabled(
    /* [in] */ const String& provider)
{
    if (provider.Equals(LocationManager_GPS_PROVIDER)) {
        mIsGpsAvailable = FALSE;
    }
    return NOERROR;
}

// Note: this is called with the lock held.
ECode CProximityListener::OnProviderEnabled(
    /* [in] */ const String& provider)
{
    // ignore
    return NOERROR;
}

// Note: this is called with the lock held.
ECode CProximityListener::OnStatusChanged(
    /* [in] */ const String& provider,
    /* [in] */ Int32 status,
    /* [in] */ IBundle* extras)
{
    if ((provider.Equals(LocationManager_GPS_PROVIDER)) &&
        (status != LocationProvider_AVAILABLE)) {
        mIsGpsAvailable = FALSE;
    }
    return NOERROR;
}

//ECode CProximityListener::OnSendFinished(
//  /* [in] */ IPendingIntent* pendingIntent,
//  /* [in] */ IIntent* intent,
//  /* [in] */ Int32 resultCode,
//  /* [in] */ String resultData,
//  /* [in] */ IBundle* resultExtras)
//{
//  return E_NOT_IMPLEMENTED;
//}
