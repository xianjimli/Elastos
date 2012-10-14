
#ifndef __GPSNETINITIATEDHANDLER_H__
#define __GPSNETINITIATEDHANDLER_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>

class GpsNetInitiatedHandler
{
private:
	static const CString TAG;

    static const Boolean DEBUG = TRUE;
    static const Boolean VERBOSE = FALSE;

public:
    // NI verify activity for bringing up UI (not used yet)
    static const CString ACTION_NI_VERIFY;

    // string constants for defining data fields in NI Intent
    static const CString NI_INTENT_KEY_NOTIF_ID;
    static const CString NI_INTENT_KEY_TITLE;
    static const CString NI_INTENT_KEY_MESSAGE;
    static const CString NI_INTENT_KEY_TIMEOUT;
    static const CString NI_INTENT_KEY_DEFAULT_RESPONSE;

    // the extra command to send NI response to GpsLocationProvider
    static const CString NI_RESPONSE_EXTRA_CMD;

    // the extra command parameter names in the Bundle
    static const CString NI_EXTRA_CMD_NOTIF_ID;
    static const CString NI_EXTRA_CMD_RESPONSE;

    // these need to match GpsNiType constants in gps_ni.h
    static const Int32 GPS_NI_TYPE_VOICE = 1;
    static const Int32 GPS_NI_TYPE_UMTS_SUPL = 2;
    static const Int32 GPS_NI_TYPE_UMTS_CTRL_PLANE = 3;

    // these need to match GpsUserResponseType constants in gps_ni.h
    static const Int32 GPS_NI_RESPONSE_ACCEPT = 1;
    static const Int32 GPS_NI_RESPONSE_DENY = 2;
    static const Int32 GPS_NI_RESPONSE_NORESP = 3;

    // these need to match GpsNiNotifyFlags constants in gps_ni.h
    static const Int32 GPS_NI_NEED_NOTIFY = 0x0001;
    static const Int32 GPS_NI_NEED_VERIFY = 0x0002;
    static const Int32 GPS_NI_PRIVACY_OVERRIDE = 0x0004;

    // these need to match GpsNiEncodingType in gps_ni.h
    static const Int32 GPS_ENC_NONE = 0;
    static const Int32 GPS_ENC_SUPL_GSM_DEFAULT = 1;
    static const Int32 GPS_ENC_SUPL_UTF8 = 2;
    static const Int32 GPS_ENC_SUPL_UCS2 = 3;
    static const Int32 GPS_ENC_UNKNOWN = -1;

public:
    class GpsNiNotification
    {
    public:
        GpsNiNotification();

    public:
        Int32 mNotificationId;
        Int32 mNiType;
        Boolean mNeedNotify;
        Boolean mNeedVerify;
        Boolean mPrivacyOverride;
        Int32 mTimeout;
        Int32 mDefaultResponse;
        String mRequestorId;
        String mText;
        Int32 mRequestorIdEncoding;
        Int32 mTextEncoding;
        AutoPtr<IBundle> mExtras;
    };

    class GpsNiResponse
    {
    public:
        GpsNiResponse();

    public:
        /* User reponse, one of the values in GpsUserResponseType */
        Int32 mUserResponse;
        /* Optional extra data to pass with the user response */
        AutoPtr<IBundle> mExtras;
    };

public:
    GpsNetInitiatedHandler(
        /* [in] */ IContext* context);

    // Handles NI events from HAL
    CARAPI_(void) HandleNiNotification(
        /* [in] */ GpsNiNotification* notif);

private:
    AutoPtr<IContext> mContext;

    // parent gps location provider
    AutoPtr<ILocalLocationManager> mLocationManager;

    // configuration of notificaiton behavior
    Boolean mPlaySounds;
    Boolean mVisible;
    Boolean mPopupImmediately;

    // Set to true if string from HAL is encoded as Hex, e.g., "3F0039"
    static Boolean mIsHexInput;

    /**
     * The notification that is shown when a network-initiated notification
     * (and verification) event is received.
     * <p>
     * This is lazily created, so use {@link #setNINotification()}.
     */
    AutoPtr<INotification> mNiNotification;
};

#endif //__GPSNETINITIATEDHANDLER_H__
