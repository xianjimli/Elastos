
#include "ext/frameworkext.h"
#include "server/CClipboardService.h"

CClipboardService::CClipboardService()
{
    CStringWrapper::New(String(""), (ICharSequence**)&mClipboard);
}

ECode CClipboardService::GetClipboardText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    Mutex::Autolock lock(_m_syncLock);
    *text = mClipboard;
    (*text)->AddRef();
    return NOERROR;
}

ECode CClipboardService::SetClipboardText(
    /* [in] */ ICharSequence* text)
{
    Mutex::Autolock lock(_m_syncLock);
    if (text == NULL) {
        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(String(""), (ICharSequence**)&cs);
        mClipboard = cs;
    }
    else {
        mClipboard = text;
    }

    return NOERROR;
}

ECode CClipboardService::HasClipboardText(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Mutex::Autolock lock(_m_syncLock);
    Int32 length = 0;
    mClipboard->GetLength(&length);
    *result = length > 0;
    return NOERROR;
}

ECode CClipboardService::constructor(
    /*[in] */ IContext* context)
{
    return NOERROR;
}

