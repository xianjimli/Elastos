
#include "widget/CToastHelper.h"
#include "widget/CToast.h"
#include <Slogger.h>
#include <StringBuffer.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

ECode CToastHelper::MakeText(
    /* [in] */ IContext* context,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 duration,
    /* [out] */ IToast** toast)
{
    /*Toast result = new Toast(context);

    LayoutInflater inflate = (LayoutInflater)
            context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    View v = inflate.inflate(com.android.internal.R.layout.transient_notification, null);
    TextView tv = (TextView)v.findViewById(com.android.internal.R.id.message);
    tv.setText(text);
    
    result.mNextView = v;
    result.mDuration = duration;

    return result;*/

    assert(toast != NULL);
    CToast::NewByFriend(context, (CToast**)toast);

    AutoPtr<ILayoutInflater> inflate;
    context->GetSystemService(
        Context_LAYOUT_INFLATER_SERVICE, (IInterface**)&inflate);

    AutoPtr<IView> v ;
    //public static final int transient_notification=0x01090067;
    inflate->Inflate(0x01090067, NULL, (IView**)&v);

    AutoPtr<ITextView> tv ;
    //public static final int message=0x0102000b;
    v->FindViewById(0x0102000b, (IView**)&tv);
    tv->SetText(text);
    
    (*toast)->SetView(v);
    (*toast)->SetDuration(duration);

    return NOERROR;
}

ECode CToastHelper::MakeTextEx(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resId,
    /* [in] */ Int32 duration,
    /* [out] */ IToast** toast)
{
    //return makeText(context, context.getResources().getText(resId), duration);
    assert(toast != NULL);
    AutoPtr<IResources> res;
    context->GetResources((IResources**) &res);
    AutoPtr<ICharSequence> text;
    res->GetText(resId, (ICharSequence**)&text);

    return MakeText(context, text, duration, toast);
}
