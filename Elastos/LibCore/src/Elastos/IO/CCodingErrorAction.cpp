
#include "CCodingErrorAction.h"
#include "cmdef.h"

AutoPtr<ICodingErrorAction> CreateAction(
    /* [in] */ String action)
{
    AutoPtr<CCodingErrorAction> p;
    CCodingErrorAction::NewByFriend((CCodingErrorAction**)&p);
    p->mAction = action;

    return (ICodingErrorAction*)p.Get();
}

const AutoPtr<ICodingErrorAction> CCodingErrorAction::IGNORE = CreateAction(String("IGNORE"));
const AutoPtr<ICodingErrorAction> CCodingErrorAction::REPLACE = CreateAction(String("REPLACE"));
const AutoPtr<ICodingErrorAction> CCodingErrorAction::REPORT = CreateAction(String("REPORT"));

ECode CCodingErrorAction::GetIGNORE(
    /* [out] */ ICodingErrorAction** ignore)
{
    VALIDATE_NOT_NULL(ignore);
    *ignore = IGNORE;
    return NOERROR;
}

ECode CCodingErrorAction::GetREPLACE(
    /* [out] */ ICodingErrorAction** replace)
{
    VALIDATE_NOT_NULL(replace);
    *replace = REPLACE;
    return NOERROR;
}

ECode CCodingErrorAction::GetREPORT(
    /* [out] */ ICodingErrorAction** report)
{
    VALIDATE_NOT_NULL(report);
    *report = REPORT;
    return NOERROR;
}

ECode CCodingErrorAction::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String("Action: ");
    str->Append(mAction);
    return NOERROR;
}

