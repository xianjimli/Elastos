
#include "text/style/ClickableSpan.h"
#include "text/CTextPaint.h"

ECode ClickableSpan::UpdateDrawState(
    /* [in] */ ITextPaint* ds)
{
    ds->SetColor(((CTextPaint*)ds)->mLinkColor);
    ds->SetUnderlineText(TRUE);
    return NOERROR;
}
