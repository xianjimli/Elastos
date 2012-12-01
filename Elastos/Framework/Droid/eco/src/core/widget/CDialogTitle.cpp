
#include "widget/CDialogTitle.h"
#include "utils/CDisplayMetrics.h"
#include <elastos/AutoPtr.h>



IVIEW_METHODS_IMPL(CDialogTitle, TextView, TextView);

IDrawableCallback_METHODS_IMPL(CDialogTitle, TextView, TextView);

IKeyEventCallback_METHODS_IMPL(CDialogTitle, TextView, TextView);

IAccessibilityEventSource_METHODS_IMPL(CDialogTitle, TextView, TextView);

ITEXTVIEW_METHODS_IMPL(CDialogTitle, TextView, TextView);


CDialogTitle::CDialogTitle()
{}

ECode CDialogTitle::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return TextView::Init(ctx, attrs, defStyle);
}

ECode CDialogTitle::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return TextView::Init(ctx, attrs);
}

ECode CDialogTitle::constructor(
    /* [in] */ IContext* ctx)
{
    return TextView::Init(ctx);
}

PInterface CDialogTitle::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }

    return _CDialogTitle::Probe(riid);
}

static Int32 R_Styleable_TextAppearance[] = {
    0x01010095, 0x01010096, 0x01010097, 0x01010098,
    0x01010099, 0x0101009a, 0x0101009b
};

void CDialogTitle::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    TextView::OnMeasure(widthMeasureSpec, heightMeasureSpec);

    AutoPtr<ILayout> layout = TextView::GetLayout();
    if (layout != NULL) {
        Int32 lineCount = 0;
        layout->GetLineCount(&lineCount);
        if (lineCount > 0) {
            Int32 ellipsisCount = 0;
            layout->GetEllipsisCount(lineCount - 1, &ellipsisCount);
            if (ellipsisCount > 0) {
                TextView::SetSingleLine(FALSE);

                AutoPtr<ITypedArray> a;
                mContext->ObtainStyledAttributesEx(
                    0x01030044, /*android.R.style.TextAppearance_Medium*/
                    ArrayOf<Int32>(R_Styleable_TextAppearance, /*android.R.styleable.TextAppearance*/
                    sizeof(R_Styleable_TextAppearance) / sizeof(Int32)),
                    (ITypedArray**)&a);

                AutoPtr<IDisplayMetrics> metrics;
                TextView::GetResources()->GetDisplayMetrics((IDisplayMetrics**)&metrics);
                Int32 density = (Int32)(20 * ((CDisplayMetrics*)metrics.Get())->mDensity);
                Int32 textSize = 0;
                a->GetDimensionPixelSize(
                        0/*android.R.styleable.TextAppearance_textSize*/,
                        density, &textSize);

                // textSize is already expressed in pixels
                TextView::SetTextSize(TypedValue_COMPLEX_UNIT_PX, textSize);
                TextView::SetMaxLines(2);
                TextView::OnMeasure(widthMeasureSpec, heightMeasureSpec);
            }
        }
    }
}

ECode CDialogTitle::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = TextView::OnPreDraw();
    return NOERROR;
}
