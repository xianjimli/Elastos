
#include "content/CAuthorityEntry.h"
#include "ext/frameworkdef.h"

ECode CAuthorityEntry::GetHost(
    /* [out] */ String* host)
{
    VALIDATE_NOT_NULL(host);

    *host = mOrigHost;
    return NOERROR;
}

ECode CAuthorityEntry::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);

    *port = mPort;
    return NOERROR;
}

ECode CAuthorityEntry::Match(
    /* [in] */ IUri* data,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    String host;
    data->GetHost(&host);
    if (host.IsNull()) {
        *result = IntentFilter_NO_MATCH_DATA;
        return NOERROR;
    }
//    if (Config.LOGV) Log.v("IntentFilter",
//            "Match host " + host + ": " + mHost);
    if (mWild) {
        if (host.GetLength() < mHost.GetLength()) {
            *result = IntentFilter_NO_MATCH_DATA;
            return NOERROR;
        }
        host = host.Substring(host.GetLength() - mHost.GetLength());
    }
    if (host.Compare(mHost, StringCase_Insensitive) != 0) {
        *result = IntentFilter_NO_MATCH_DATA;
        return NOERROR;
    }
    if (mPort >= 0) {
        Int32 port;
        data->GetPort(&port);
        if (mPort != port) {
            *result = IntentFilter_NO_MATCH_DATA;
            return NOERROR;
        }
        *result = IntentFilter_MATCH_CATEGORY_PORT;
        return NOERROR;
    }
    *result = IntentFilter_MATCH_CATEGORY_HOST;
    return NOERROR;
}

ECode CAuthorityEntry::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    Int32 value;
    source->ReadString(&mOrigHost);
    source->ReadString(&mHost);
    source->ReadInt32(&value);
    mWild = value != 0;
    source->ReadInt32(&mPort);
    return NOERROR;
}

ECode CAuthorityEntry::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    dest->WriteString(mOrigHost);
    dest->WriteString(mHost);
    dest->WriteInt32(mWild ? 1 : 0);
    dest->WriteInt32(mPort);
    return NOERROR;
}

ECode CAuthorityEntry::constructor(
    /* [in] */ const String& host,
    /* [in] */ const String& port)
{
    mOrigHost = host;
    mWild = host.GetLength() > 0 && host.GetChar(0) == '*';
    mHost = mWild ? host.Substring(1)/*.intern()*/ : host;
    mPort = !port.IsNull() ? port.ToInt32() : -1;
    return NOERROR;
}
