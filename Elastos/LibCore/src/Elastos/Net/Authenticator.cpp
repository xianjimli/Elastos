
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
    assert(CNetPermission::New(name, (INetPermission**)&netPermission));
    netPermission->AddRef();

    return netPermission;
}

AutoPtr<IAuthenticator> Authenticator::mThisAuthenticator;

AutoPtr<INetPermission> Authenticator::mRequestPasswordAuthenticationPermission =
        InitAuthenticationPermission(
                String("requestPasswordAuthentication"));

AutoPtr<INetPermission> Authenticator::mSetDefaultAuthenticatorPermission =
        InitAuthenticationPermission(
                String("setDefaultAuthenticator"));

Mutex* Authenticator::mLock;

Authenticator::Authenticator()
    : mPort(0)
{}

ECode Authenticator::GetPasswordAuthentication(
    /* [out] */ IPasswordAuthentication** passwordAuthentication)
{
    VALIDATE_NOT_NULL(passwordAuthentication);
    *passwordAuthentication = NULL;
    return NOERROR;
}

ECode Authenticator::GetRequestingPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    *port = mPort;
    return NOERROR;
}

ECode Authenticator::GetRequestingSite(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);
    *address = mAddr;
    return NOERROR;
}

ECode Authenticator::GetRequestingPrompt(
    /* [out] */ String* prompt)
{
    VALIDATE_NOT_NULL(prompt);
    *prompt = mPrompt;
    return NOERROR;
}

ECode Authenticator::GetRequestingProtocol(
    /* [out] */ String* protocol)
{
    VALIDATE_NOT_NULL(protocol);
    *protocol = mProtocol;
    return NOERROR;
}

ECode Authenticator::GetRequestingScheme(
    /* [out] */ String* scheme)
{
    VALIDATE_NOT_NULL(scheme);
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
    Mutex::Autolock lock(mLock);

    VALIDATE_NOT_NULL(passwordAuthentication);

//    SecurityManager sm = System.getSecurityManager();
//    if (sm != null) {
//        sm.checkPermission(requestPasswordAuthenticationPermission);
//    }
    if (mThisAuthenticator == NULL) {
        *passwordAuthentication = NULL;
        return NOERROR;
    }
    // set the requester info so it knows what it is requesting
    // authentication for
    Authenticator* authenticator = (Authenticator*)mThisAuthenticator->Probe(EIID_Authenticator);
    if (authenticator != NULL) {
        authenticator->mAddr = rAddr;
        authenticator->mPort = rPort;
        authenticator->mProtocol = rProtocol;
        authenticator->mPrompt = rPrompt;
        authenticator->mScheme = rScheme;
        authenticator->mRt = AuthenticatorRequestorType_SERVER;
    }

    // returns the authentication info obtained by the registered
    // Authenticator
    return authenticator->GetPasswordAuthentication(passwordAuthentication);
}

void Authenticator::SetDefault(
    /* [in] */ IAuthenticator* a)
{
    // SecurityManager sm = System.getSecurityManager();
    // if (sm != null) {
    //     sm.checkPermission(setDefaultAuthenticatorPermission);
    // }
    mThisAuthenticator = a;
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
    Mutex::Autolock lock(mLock);

    VALIDATE_NOT_NULL(passwordAuthentication);

    // SecurityManager sm = System.getSecurityManager();
    // if (sm != null) {
    //     sm.checkPermission(requestPasswordAuthenticationPermission);
    // }
    if (mThisAuthenticator == NULL) {
        *passwordAuthentication = NULL;
        return NOERROR;
    }
    // set the requester info so it knows what it is requesting
    // authentication for
    Authenticator* authenticator = (Authenticator*)mThisAuthenticator->Probe(EIID_Authenticator);
    if (authenticator != NULL) {
        authenticator->mHost = rHost;
        authenticator->mAddr = rAddr;
        authenticator->mPort = rPort;
        authenticator->mProtocol = rProtocol;
        authenticator->mPrompt = rPrompt;
        authenticator->mScheme = rScheme;
        authenticator->mRt = AuthenticatorRequestorType_SERVER;
    }

    // returns the authentication info obtained by the registered
    // Authenticator
    return authenticator->GetPasswordAuthentication(passwordAuthentication);
}

ECode Authenticator::GetRequestingHost(
    /* [out] */ String* hostName)
{
    VALIDATE_NOT_NULL(hostName);
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
    Mutex::Autolock lock(mLock);

    VALIDATE_NOT_NULL(passwordAuthentication);

    // SecurityManager sm = System.getSecurityManager();
    // if (null != sm) {
    //     sm.checkPermission(requestPasswordAuthenticationPermission);
    // }
    if (NULL == mThisAuthenticator) {
        *passwordAuthentication = NULL;
        return NOERROR;
    }
    // sets the requester info so it knows what it is requesting
    // authentication for
    Authenticator* authenticator = (Authenticator*)mThisAuthenticator->Probe(EIID_Authenticator);
    if (authenticator != NULL) {
        authenticator->mHost = rHost;
        authenticator->mAddr = rAddr;
        authenticator->mPort = rPort;
        authenticator->mProtocol = rProtocol;
        authenticator->mPrompt = rPrompt;
        authenticator->mScheme = rScheme;
        authenticator->mUrl = rURL;
        authenticator->mRt = reqType;
        authenticator->mRt = AuthenticatorRequestorType_SERVER;
    }

    // returns the authentication info obtained by the registered
    // Authenticator
    return authenticator->GetPasswordAuthentication(passwordAuthentication);
}

ECode Authenticator::GetRequestingURL(
    /* [out] */ IURL** url)
{
    VALIDATE_NOT_NULL(url);
    *url = mUrl;
    return NOERROR;
}

ECode Authenticator::GetRequestorType(
    /* [out] */ AuthenticatorRequestorType* requestorType)
{
    VALIDATE_NOT_NULL(requestorType);
    *requestorType = mRt;
    return NOERROR;
}
