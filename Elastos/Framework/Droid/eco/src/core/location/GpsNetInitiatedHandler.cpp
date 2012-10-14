
#include "location/GpsNetInitiatedHandler.h"

const CString GpsNetInitiatedHandler::TAG = "GpsNetInitiatedHandler";
const Boolean GpsNetInitiatedHandler::DEBUG;
const Boolean GpsNetInitiatedHandler::VERBOSE;
const CString GpsNetInitiatedHandler::ACTION_NI_VERIFY = "android.intent.action.NETWORK_INITIATED_VERIFY";
const CString GpsNetInitiatedHandler::NI_INTENT_KEY_NOTIF_ID = "notif_id";
const CString GpsNetInitiatedHandler::NI_INTENT_KEY_TITLE = "title";
const CString GpsNetInitiatedHandler::NI_INTENT_KEY_MESSAGE = "message";
const CString GpsNetInitiatedHandler::NI_INTENT_KEY_TIMEOUT = "timeout";
const CString GpsNetInitiatedHandler::NI_INTENT_KEY_DEFAULT_RESPONSE = "default_resp";
const CString GpsNetInitiatedHandler::NI_RESPONSE_EXTRA_CMD = "send_ni_response";
const CString GpsNetInitiatedHandler::NI_EXTRA_CMD_NOTIF_ID = "notif_id";
const CString GpsNetInitiatedHandler::NI_EXTRA_CMD_RESPONSE = "response";
const Int32 GpsNetInitiatedHandler::GPS_NI_TYPE_VOICE;
const Int32 GpsNetInitiatedHandler::GPS_NI_TYPE_UMTS_SUPL;
const Int32 GpsNetInitiatedHandler::GPS_NI_TYPE_UMTS_CTRL_PLANE;
const Int32 GpsNetInitiatedHandler::GPS_NI_RESPONSE_ACCEPT;
const Int32 GpsNetInitiatedHandler::GPS_NI_RESPONSE_DENY;
const Int32 GpsNetInitiatedHandler::GPS_NI_RESPONSE_NORESP;
const Int32 GpsNetInitiatedHandler::GPS_NI_NEED_NOTIFY;
const Int32 GpsNetInitiatedHandler::GPS_NI_NEED_VERIFY;
const Int32 GpsNetInitiatedHandler::GPS_NI_PRIVACY_OVERRIDE;
const Int32 GpsNetInitiatedHandler::GPS_ENC_NONE;
const Int32 GpsNetInitiatedHandler::GPS_ENC_SUPL_GSM_DEFAULT;
const Int32 GpsNetInitiatedHandler::GPS_ENC_SUPL_UTF8;
const Int32 GpsNetInitiatedHandler::GPS_ENC_SUPL_UCS2;
const Int32 GpsNetInitiatedHandler::GPS_ENC_UNKNOWN;

Boolean GpsNetInitiatedHandler::mIsHexInput = TRUE;

GpsNetInitiatedHandler::GpsNiNotification::GpsNiNotification()
    : mNotificationId(0)
    , mNiType(0)
    , mNeedNotify(FALSE)
    , mNeedVerify(FALSE)
    , mPrivacyOverride(FALSE)
    , mTimeout(0)
    , mDefaultResponse(0)
    , mRequestorIdEncoding(0)
    , mTextEncoding(0)
{
}

GpsNetInitiatedHandler::GpsNiResponse::GpsNiResponse()
    : mUserResponse(0)
{
}

GpsNetInitiatedHandler::GpsNetInitiatedHandler(
    /* [in] */ IContext* context)
    : mContext(context)
    , mPlaySounds(FALSE)
    , mVisible(TRUE)
    , mPopupImmediately(TRUE)
{
    ASSERT_SUCCEEDED(context->GetSystemService(Context_LOCATION_SERVICE, (IInterface**)&mLocationManager));
}

void GpsNetInitiatedHandler::HandleNiNotification(
    /* [in] */ GpsNiNotification* notif)
{
}
