
#include "cmdef.h"
#include "CSocketPermission.h"
#include <StringBuffer.h>
#include "SocketPermissionCollection.h"
#include "InetAddress.cpp"

const Int32 CSocketPermission::SP_CONNECT = 1;
const Int32 CSocketPermission::SP_LISTEN = 2;
const Int32 CSocketPermission::SP_ACCEPT = 4;
const Int32 CSocketPermission::SP_RESOLVE = 8;

const Int64 CSocketPermission::mSerialVersionUID = -7204263841984476862L;

const String CSocketPermission::mActionNames[] = { String(""), String("connect"), String("listen"), String(""),
            String("accept"), String(""), String(""), String(""), String("resolve") };

const Int32 CSocketPermission::HIGHEST_PORT = 65535;
const Int32 CSocketPermission::LOWEST_PORT = 0;

CSocketPermission::CSocketPermission()
    : Permission()
    , mResolved(FALSE)
    , mPortMin(LOWEST_PORT)
    , mPortMax(HIGHEST_PORT)
    , mActionsMask(SP_RESOLVE)
    , mIsPartialWild(FALSE)
    , mIsWild(FALSE)
{}

ECode CSocketPermission::GetActions(
    /* [out] */ String* actions)
{
    VALIDATE_NOT_NULL(actions);
    *actions = mActions;

    return NOERROR;
}

ECode CSocketPermission::SetActions(
    /* [in] */ const String& actions)
{
    if (actions.IsEmpty()) {
        return NOERROR;
    }
    Boolean parsing = TRUE;
    String action;
    StringBuffer sb;
    Int32 pos = 0, length = actions.GetLength();
    while (parsing) {
        Char32 c;
//        sb.setLength(0);
        while (pos < length && (c = actions[pos++]) != ',') {
            sb += c;
        }
        if (pos == length) {
            parsing = FALSE;
        }
        action = ((String)sb).Trim();
        action.ToLowerCase();
        if (action.Equals(mActionNames[SP_CONNECT])) {
            mActionsMask |= SP_CONNECT;
        }
        else if (action.Equals(mActionNames[SP_LISTEN])) {
            mActionsMask |= SP_LISTEN;
        }
        else if (action.Equals(mActionNames[SP_ACCEPT])) {
            mActionsMask |= SP_ACCEPT;
        }
        else if (action.Equals(mActionNames[SP_RESOLVE])) {
            // do nothing
        }
        else {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
//            throw new IllegalArgumentException("Invalid action: " + action);
        }
    }

    return NOERROR;
}

ECode CSocketPermission::Implies(
    /* [in] */ IPermission* permission,
    /* [out] */ Boolean* isImplied)
{
    AutoPtr<ISocketPermission> sp;
//    try {
    if (permission != NULL && permission->Probe(EIID_ISocketPermission) != NULL) {
        sp = (ISocketPermission*)permission->Probe(EIID_ISocketPermission);
    }
    else {
        *isImplied = FALSE;
        return NOERROR;
    }
//    } catch (ClassCastException e) {
//        return false;
//    }

    // tests if the action list of p is the subset of the one of the
    // receiver
    AutoPtr<CSocketPermission> spCls = (CSocketPermission*)sp.Get();
    if ((mActionsMask & spCls->mActionsMask) != spCls->mActionsMask) {
        *isImplied = FALSE;
        return NOERROR;
    }

    // only check the port range if the action string of the current object
    // is not "resolve"
    String actions;
    permission->GetActions(&actions);
    if (!actions.Equals(String("resolve"))) {
        if ((spCls->mPortMin < mPortMin) || (spCls->mPortMax > mPortMax)) {
            *isImplied = FALSE;
            return NOERROR;
        }
    }

    // Verify the host is valid
    *isImplied = CheckHost(sp);

    return NOERROR;
}

ECode CSocketPermission::NewPermissionCollection(
    /* [out] */ IPermissionCollection** permissionCollection)
{
    VALIDATE_NOT_NULL(permissionCollection);

    SocketPermissionCollection* socketPermissionCollection= new SocketPermissionCollection();
    *permissionCollection =
            (IPermissionCollection*)socketPermissionCollection->Probe(EIID_IPermissionCollection);

    return NOERROR;
}

ECode CSocketPermission::ParsePort(
    /* [in] */ const String& hostPort,
    /* [in] */ const String& host)
{
    String port = hostPort.Substring(host.GetLength());
    String emptyString("");

    if (emptyString.Equals(port)) {
        // Not specified
        mPortMin = 80;
        mPortMax = 80;
        return NOERROR;
    }

    if (String(":*").Equals(port)) {
        // The port range should be 0-65535
        mPortMin = 0;
        mPortMax = 65535;
        return NOERROR;
    }

    // Omit ':'
    port = port.Substring(1);
    Int32 negIdx = port.IndexOf('-');
    String strPortMin = emptyString;
    String strPortMax = emptyString;
    if (-1 == negIdx) {
        // No neg mark, only one number
        strPortMin = port;
        strPortMax = port;
    }
    else {
        strPortMin = port.Substring(0, negIdx);
        strPortMax = port.Substring(negIdx + 1);
        if (emptyString.Equals(strPortMin)) {
            strPortMin = String("0");
        }
        if (emptyString.Equals(strPortMax)) {
            strPortMax = String("65535");
        }
    }
//    try {
    mPortMin = strPortMin.ToInt32();//Integer.valueOf(strPortMin).intValue();
    mPortMax = strPortMax.ToInt32();//Integer.valueOf(strPortMax).intValue();

    if (mPortMin > mPortMax) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("MinPort is greater than MaxPort: " + port);
    }
    // } catch (NumberFormatException e) {
    //     throw new IllegalArgumentException("Invalid port number: " + port);
    // }

    return NOERROR;
}

String CSocketPermission::ToCanonicalActionString(
    /* [in] */ const String& action)
{
    if (action.IsNull() || action.IsEmpty() || mActionsMask == SP_RESOLVE) {
        return mActionNames[SP_RESOLVE]; // If none specified return the
    }
    // implied action resolve
    StringBuffer sb;
    if ((mActionsMask & SP_CONNECT) == SP_CONNECT) {
        sb += ',';
        sb += mActionNames[SP_CONNECT];
    }
    if ((mActionsMask & SP_LISTEN) == SP_LISTEN) {
        sb += ',';
        sb += mActionNames[SP_LISTEN];
    }
    if ((mActionsMask & SP_ACCEPT) == SP_ACCEPT) {
        sb += ',';
        sb += mActionNames[SP_ACCEPT];
    }
    sb += ',';
    sb += mActionNames[SP_RESOLVE];// Resolve is always implied
    // Don't copy the first ','.
    return mActions = sb.Substring(1, sb.GetLength());
}

String CSocketPermission::GetIPString(
    /* [in] */ Boolean isCheck)
{
    if (!mResolved) {
//        try {
        InetAddress::GetHostNameInternal(mHostName, isCheck, &mIpString);
//        } catch (UnknownHostException e) {
            // ignore
//        }
        mResolved = TRUE;
    }
    return mIpString;
}

ECode CSocketPermission::GetHostString(
    /* [in] */ const String& host,
    /* [out] */ String* hostName)
{
    VALIDATE_NOT_NULL(hostName);

    String hostTemp = host;
    hostTemp = hostTemp.Trim();
    Int32 idx = -1;
    idx = host.IndexOf(':');
    mIsPartialWild = (hostTemp.GetLength() > 0 && hostTemp[0] == '*');
    if (mIsPartialWild) {
        mResolved = TRUE;
        mIsWild = (hostTemp.GetLength() == 1);
        if (mIsWild) {
            *hostName = hostTemp;
            return NOERROR;
        }
        if (idx > -1) {
            hostTemp = hostTemp.Substring(0, idx);
        }
        *hostName = hostTemp;
        hostName->ToLowerCase();
        return NOERROR;
    }

    Int32 lastIdx = hostTemp.LastIndexOf(':');

    if (idx == lastIdx) {
        if (-1 != idx) {
            // only one colon, should be port
            hostTemp = hostTemp.Substring(0, idx);
        }
        *hostName = hostTemp;
        hostName->ToLowerCase();
        return NOERROR;
    }
        // maybe ipv6
    Boolean isFirstBracket = (hostTemp[0] == '[');
    if (!isFirstBracket) {
        // No bracket, should be in full form
        Int32 colonNum = 0;
        for (Int32 i = 0; (UInt32)i < hostTemp.GetLength(); ++i) {
            if (hostTemp[i] == ':') {
                colonNum++;
            }
        }
        // Get rid of the colon before port
        if (8 == colonNum) {
            hostTemp = hostTemp.Substring(0, lastIdx);
        }
        if (IsIP6AddressInFullForm(hostTemp)) {
            *hostName = hostTemp;
            hostName->ToLowerCase();
            return NOERROR;
        }
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("Invalid port number: " + host);
    }
    // forward bracket found
    Int32 bbracketIdx = hostTemp.IndexOf(']');
    if (-1 == bbracketIdx) {
        // no back bracket found, wrong
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("Invalid port number: " + host);
    }
    hostTemp = hostTemp.Substring(0, bbracketIdx + 1);
    if (IsValidIP6Address(hostTemp)) {
        *hostName = hostTemp;
        hostName->ToLowerCase();
        return NOERROR;
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
//    throw new IllegalArgumentException("Invalid port number: " + host);
}

Boolean CSocketPermission::IsValidHexChar(
    /* [in] */ const Char32& c)
{
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

Boolean CSocketPermission::IsValidIP4Word(
    /* [in] */ const String& word)
{
    Char32 c;
    if (word.GetLength() < 1 || word.GetLength() > 3) {
        return FALSE;
    }
    for (Int32 i = 0; (UInt32)i < word.GetLength(); i++) {
        c = word[i];
        if (!(c >= '0' && c <= '9')) {
            return FALSE;
        }
    }
    if (word.ToInt32()/*Integer.parseInt(word)*/ > 255) {
        return FALSE;
    }
    return TRUE;
}

Boolean CSocketPermission::IsIP6AddressInFullForm(
    /* [in] */ const String& ipAddress)
{
    if (IsValidIP6Address(ipAddress)) {
        Int32 doubleColonIndex = ipAddress.IndexOf("::");
        if (doubleColonIndex >= 0) {
            // Simplified form which contains ::
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

Boolean CSocketPermission::IsValidIP6Address(
    /* [in] */ const String& ipAddress)
{
    Int32 length = ipAddress.GetLength();
    Boolean doubleColon = FALSE;
    Int32 numberOfColons = 0;
    Int32 numberOfPeriods = 0;
    Int32 numberOfPercent = 0;
    String word("");
    Char32 c = 0;
    Char32 prevChar = 0;
    Int32 offset = 0; // offset for [] IP addresses

    if (length < 2) {
        return FALSE;
    }

    for (Int32 i = 0; i < length; i++) {
        prevChar = c;
        c = ipAddress[i];
        switch (c) {

            // case for an open bracket [x:x:x:...x]
        case '[':
            if (i != 0) {
                return FALSE; // must be first character
            }
            if (ipAddress[length - 1] != ']') {
                return FALSE; // must have a close ]
            }
            offset = 1;
            if (length < 4) {
                return FALSE;
            }
            break;

            // case for a closed bracket at end of IP [x:x:x:...x]
        case ']':
            if (i != length - 1) {
                return FALSE; // must be last character
            }
            if (ipAddress[0] != '[') {
                return FALSE; // must have a open [
            }
            break;

            // case for the last 32-bits represented as IPv4 x:x:x:x:x:x:d.d.d.d
        case '.':
            numberOfPeriods++;
            if (numberOfPeriods > 3) {
                return FALSE;
            }
            if (!IsValidIP4Word(word)) {
                return FALSE;
            }
            if (numberOfColons != 6 && !doubleColon) {
                return FALSE;
            }
            // a special case ::1:2:3:4:5:d.d.d.d allows 7 colons with an
            // IPv4 ending, otherwise 7 :'s is bad
            if (numberOfColons == 7 && ipAddress[0 + offset] != ':'
                    && ipAddress[1 + offset] != ':') {
                return FALSE;
            }
            word = "";
            break;

        case ':':
            numberOfColons++;
            if (numberOfColons > 7) {
                return FALSE;
            }
            if (numberOfPeriods > 0) {
                return FALSE;
            }
            if (prevChar == ':') {
                if (doubleColon) {
                    return FALSE;
                }
                doubleColon = TRUE;
            }
            word = "";
            break;
        case '%':
            if (numberOfColons == 0) {
                return FALSE;
            }
            numberOfPercent++;

            // validate that the stuff after the % is valid
            if ((i + 1) >= length) {
                // in this case the percent is there but no number is
                // available
                return FALSE;
            }
//            try {
            ipAddress.Substring(i + 1).ToInt32();//Integer.parseInt(ipAddress.substring(i + 1));
//            } catch (NumberFormatException e) {
                // right now we just support an integer after the % so if
                // this is not
                // what is there then return
//                return FALSE;
//            }
            break;

        default:
            if (numberOfPercent == 0) {
                if (word.GetLength() > 3) {
                    return FALSE;
                }
                if (!IsValidHexChar(c)) {
                    return FALSE;
                }
            }
            word += (const char*)c;
        }
    }

    // Check if we have an IPv4 ending
    if (numberOfPeriods > 0) {
        if (numberOfPeriods != 3 || !IsValidIP4Word(word)) {
            return FALSE;
        }
    }
    else {
        // If we're at then end and we haven't had 7 colons then there is a
        // problem unless we encountered a doubleColon
        if (numberOfColons != 7 && !doubleColon) {
            return FALSE;
        }

        // If we have an empty word at the end, it means we ended in either
        // a : or a .
        // If we did not end in :: then this is invalid
        if (numberOfPercent == 0) {
            if (word == "" && ipAddress[length - 1 - offset] == ':'
                    && ipAddress[length - 2 - offset] != ':') {
                return FALSE;
            }
        }
    }

    return TRUE;
}

Boolean CSocketPermission::CheckHost(
    /* [in] */ ISocketPermission* sp)
{
    AutoPtr<CSocketPermission> spCls = (CSocketPermission*)sp->Probe(EIID_CSocketPermission);
    if (mIsPartialWild) {
        if (mIsWild) {
            return TRUE; // Match on any host
        }
        Int32 length = mHostName.GetLength() - 1;
        String s = spCls->mHostName.Substring(spCls->mHostName.GetLength() - length);
        return s.Equals(mHostName.Substring(1, length));
    }
    // The ipString may not be the same, some hosts resolve to
    // multiple ips
    return (!GetIPString(FALSE).IsNull() && mIpString.Equals(spCls->GetIPString(FALSE)))
            || mHostName.Equals(spCls->mHostName);
}

ECode CSocketPermission::constructor(
    /* [in] */ const String& host,
    /* [in] */ const String& action)
{
    Permission::Init(host.IsEmpty() ? String("localhost") : host);
    GetHostString(host, &mHostName);
    if (action.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
//        throw new NullPointerException();
    }
    if (action.IsEmpty()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }

    SetActions(action);
    mActions = ToCanonicalActionString(action);
    // Use host since we are only checking for port presence
    ParsePort(host, mHostName);

    return NOERROR;
}

