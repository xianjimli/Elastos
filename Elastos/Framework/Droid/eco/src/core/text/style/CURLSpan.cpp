
#include "ext/frameworkext.h"
#include "text/style/CURLSpan.h"

ECode CURLSpan::constructor(
    /* [in] */ const String& url)
{
    mURL = url;
    return NOERROR;
}

PInterface CURLSpan::Probe(
    /* [in] */ REIID riid)
{
    return _CURLSpan::Probe(riid);
}

ECode CURLSpan::GetSpanTypeId(
    /* [in] */ Int32* id)
{
    // VALIDATE_NOT_NULL(id);
    // *id = TextUtils::URL_SPAN;
    // return NOERROR;
    return E_NOT_IMPLEMENTED;
}

ECode CURLSpan::ReadFromParcel(
    /* [in] */ IParcel* src)
{
    assert(src != NULL);
    return src->ReadString(&mURL);
}

ECode CURLSpan::WriteToParcel(
    /* [in] */ IParcel* dst)
{
    assert(dst != NULL);
    return dst->WriteString(mURL);
}

ECode CURLSpan::GetURL(
    /* [in] */ String* url)
{
    VALIDATE_NOT_NULL(url);
    *url = mURL;
    return NOERROR;
}

ECode CURLSpan::OnClick(
    /* [in] */ IView* widget)
{
    // Uri uri = Uri.parse(getURL());
    // Context context = widget.getContext();
    // Intent intent = new Intent(Intent.ACTION_VIEW, uri);
    // intent.putExtra(Browser.EXTRA_APPLICATION_ID, context.getPackageName());
    // context.startActivity(intent);
    return E_NOT_IMPLEMENTED;
}

ECode CURLSpan::UpdateDrawState(
    /* [in] */ ITextPaint* tp)
{
    return ClickableSpan::UpdateDrawState(tp);
}

ECode CURLSpan::GetUnderlying(
    /* [in] */ ICharacterStyle** cs)
{
    return ClickableSpan::GetUnderlying(cs);
}

