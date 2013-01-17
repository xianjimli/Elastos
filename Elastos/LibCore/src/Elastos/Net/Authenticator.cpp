
#include "cmdef.h"
#include "Authenticator.h"
#include "CNetPermission.h"
#include <Com.Kortide.Platform.h>


// {83E9D351-ACF2-49a2-AD77-643EC6CDDAF7}
extern "C" const InterfaceID EIID_Authenticator =
    { 0x83e9d351, 0xacf2, 0x49a2, { 0xad, 0x77, 0x64, 0x3e, 0xc6, 0xcd, 0xda, 0xf7 } };


AutoPtr<INetPermission> InitAuthenticationPermission(
    /* [in] */ const String& name)
{
    AutoPtr<INetPermission> netPermission;
    ASSERT_SUCCEEDED(CNetPermission::New(name, (INetPermission**)&netPermission));
    return netPermission;
}

AutoPtr<Authenticator> Authenticator::sThisAuthenticator;

AutoPtr<INetPermission> Authenticator::sRequestPasswordAuthenticationPermission =
        InitAuthenticationPermission(String("requestPasswordAuthentication"));

AutoPtr<INetPermission> Authenticator::sSetDefaultAuthenticatorPermission =
        InitAuthenticationPermission(String("setDefaultAuthenticator"));

Mutex Authenticator::sLock;

Authenticator::Authenticator()
    : mPort(0)
{}

ECode Authenticator::GetPasswordAuthentication(
    /* [out] */ IPasswordAuthentication** passwordAuthentication)
{
    assert(passwordAuthentication != NULL);
    *passwordAuthentication = NULL;
    return NOERROR;
}

ECode Authenticator::GetRequestingPort(
    /* [out] */ Int32* port)
{
    assert(port != NULL);
    *port = mPort;
    return NOERROR;
}

ECode Authenticator::GetRequestingSite(
    /* [out] */ IInetAddress** address)
{
    assert(address != NULL);
    *address = mAddr;
    if (*address != NULL) (*address)->AddRef();
    return NOERROR;
}

ECode Authenticator::GetRequestingPrompt(
    /* [out] */ String* prompt)
{
    assert(prompt != NULL);
    *prompt = mPrompt;
    return NOERROR;
}

ECode Authenticator::GetRequestingProtocol(
    /* [out] */ String* protocol)
{
    assert(protocol != NULL);
    *protocol = mProtocol;
    return NOERROR;
}

ECode Authenticator::GetRequestingScheme(
    /* [out] */ String* scheme)
{
    assert(scheme != NULL);
    *scheme = mScheme;
    return NOERROR;
}

ECode Authenticator::RequestPasswordAuthentication(
    /* [in] */ IInetAddress* rAddr,
    /* [in] */ Int32 rPort,
    /* [in] */ const String& rProtocol,
    /* [in] */ const String& rPrompt,
    /* [in] */ const String& rScheme,
    /* [out] */ IPasswordAuthentication** passwordAuthentication)
{
    assert(passwordAuthentication != NULL);

    Mutex::Autolock lock(sLock);

//    SecurityManager sm = System.getSecurityManager();
//    if (sm != null) {
//        sm.checkPermission(requestPasswordAuthenticationPermission);
//    }
    if (sThisAuthenticator == NULL) {
        *passwordAuthentication = NULL;
        return NOERROR;
    }
    // set the requester info so it knows what it is requesting
    // authentication for
    sThisAuthenticator->mAddr = rAddr;
    sThisAuthenticator->mPort = rPort;
    sThisAuthenticator->mProtocol = rProtocol;
    sThisAuthenticator->mPrompt = rPrompt;
    sThisAuthenticator->mScheme = rScheme;
    sThisAuthenticator->mRt = AuthenticatorRequestorType_SERVER;

    // returns the authentication info obtained by the registered
    // Authenticator
    return sThisAuthenticator->GetPasswordAuthentication(passwordAuthentication);
}

void Authenticator::SetDefault(
    /* [in] */ IAuthenticator* a)
{
    // SecurityManager sm = System.getSecurityManager();
    // if (sm != null) {
    //     sm.checkPermission(setDefaultAuthenticatorPermission);
    // }
    sThisAuthenticator = a == NULL ? NULL : (Authenticator*)a->Probe(EIID_Authenticator);
}

ECode Authenticator::RequestPasswordAuthenticationEx(
    /* [in] */ const String& rHost,
    /* [in] */ IInetAddress* rAddr,
    /* [in] */ Int32 rPort,
    /* [in] */ const String& rProtocol,
    /* [in] */ const String& rPrompt,
    /* [in] */ const String& rScheme,
    /* [out] */ IPasswordAuthentication** passwordAuthentication)
{
    assert(passwordAuthentication != NULL);

    Mutex::Autolock lock(sLock);

    // SecurityManager sm = System.getSecurityManager();
    // if (sm != null) {
    //     sm.checkPermission(requestPasswordAuthenticationPermission);
    // }
    if (sThisAuthenticator == NULL) {
        *passwordAuthentication = NULL;
        return NOERROR;
    }
    // set the requester info so it knows what it is requesting
    // authentication for
    sThisAuthenticator->mHost = rHost;
    sThisAuthenticator->mAddr = rAddr;
    sThisAuthenticator->mPort = rPort;
    sThisAuthenticator->mProtocol = rProtocol;
    sThisAuthenticator->mPrompt = rPrompt;
    sThisAuthenticator->mScheme = rScheme;
    sThisAuthenticator->mRt = AuthenticatorRequestorType_SERVER;

    // returns the authentication info obtained by the registered
    // Authenticator
    return sThisAuthenticator->GetPasswordAuthentication(passwordAuthentication);
}

ECode Authenticator::GetRequestingHost(
    /* [out] */ String* hostName)
{
    assert(hostName != NULL);
    *hostName = mHost;
    return NOERROR;
}

ECode Authenticator::RequestPasswordAuthenticationEx2(
    /* [in] */ const String& rHost,
    /* [in] */ IInetAddress* rAddr,
    /* [in] */ Int32 rPort,
    /* [in] */ const String& rProtocol,
    /* [in] */ const String& rPrompt,
    /* [in] */ const String& rScheme,
    /* [in] */ IURL* rURL,
    /* [in] */ AuthenticatorRequestorType reqType,
    /* [out] */ IPasswordAuthentication** passwordAuthentication)
{
    assert(passwordAuthentication != NULL);

    Mutex::Autolock lock(sLock);

    // SecurityManager sm = System.getSecurityManager();
    // if (null != sm) {
    //     sm.checkPermission(requestPasswordAuthenticationPermission);
    // }
    if (sThisAuthenticator == NULL) {
        *passwordAuthentication = NULL;
        return NOERROR;
    }
    // sets the requester info so it knows what it is requesting
    // authentication for
    sThisAuthenticator->mHost = rHost;
    sThisAuthenticator->mAddr = rAddr;
    sThisAuthenticator->mPort = rPort;
    sThisAuthenticator->mProtocol = rProtocol;
    sThisAuthenticator->mPrompt = rPrompt;
    sThisAuthenticator->mScheme = rScheme;
    sThisAuthenticator->mUrl = rURL;
    sThisAuthenticator->mRt = reqType;
    sThisAuthenticator->mRt = AuthenticatorRequestorType_SERVER;

    // returns the authentication info obtained by the registered
    // Authenticator
    return sThisAuthenticator->GetPasswordAuthentication(passwordAuthentication);
}

ECode Authenticator::GetRequestingURL(
    /* [out] */ IURL** url)
{
    assert(url != NULL);
    *url = mUrl;
    if (*url != NULL) (*url)->AddRef();
    return NOERROR;
}

ECode Authenticator::GetRequestorType(
    /* [out] */ AuthenticatorRequestorType* requestorType)
{
    assert(requestorType != NULL);
    *requestorType = mRt;
    return NOERROR;
}
