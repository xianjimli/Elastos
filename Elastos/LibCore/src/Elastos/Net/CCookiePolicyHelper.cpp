
#include "cmdef.h"
#include "CCookiePolicyHelper.h"
#include "CPAcceptAll.h"
#include "CPAcceptNone.h"
#include "CPAcceptOriginalServer.h"

ECode CCookiePolicyHelper::GetCookiePolicy(
    /* [in] */ const String& type,
    /* [out] */ ICookiePolicy** policy)
{
    VALIDATE_NOT_NULL(policy);

    if (type == CookiePolicy_ACCEPT_ALL) {
        CPAcceptAll* cp = new CPAcceptAll();
        if (cp != NULL && cp->Probe(EIID_ICookiePolicy) != NULL) {
            *policy = (ICookiePolicy*)cp->Probe(EIID_ICookiePolicy);
        }
        else {
            *policy = NULL;
        }
    }
    else if (type == CookiePolicy_ACCEPT_NONE) {
        CPAcceptNone* cp = new CPAcceptNone();
        if (cp != NULL && cp->Probe(EIID_ICookiePolicy) != NULL) {
            *policy = (ICookiePolicy*)cp->Probe(EIID_ICookiePolicy);
        }
        else {
            *policy = NULL;
        }
    }
    else if (type == CookiePolicy_ACCEPT_ORIGINAL_SERVER) {
        CPAcceptOriginalServer* cp = new CPAcceptOriginalServer();
        if (cp != NULL && cp->Probe(EIID_ICookiePolicy) != NULL) {
            *policy = (ICookiePolicy*)cp->Probe(EIID_ICookiePolicy);
        }
        else {
            *policy = NULL;
        }
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }



    return NOERROR;
}
