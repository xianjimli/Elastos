
#include "inputmethodservice/CKeyboardView.h"
#include "graphics/CPaint.h"
#include "widget/CPopupWindow.h"
#include "utils/CDisplayMetrics.h"

using namespace Elastos::Core;

IVIEW_METHODS_IMPL(CKeyboardView, KeyboardView, KeyboardView);
IDrawableCallback_METHODS_IMPL(CKeyboardView, KeyboardView, KeyboardView);
IKeyEventCallback_METHODS_IMPL(CKeyboardView, KeyboardView, KeyboardView);
IAccessibilityEventSource_METHODS_IMPL(CKeyboardView, KeyboardView, KeyboardView);


static Int32 R_Styleable_KeyboardView[] = {
    0x01010161, 0x01010164, 0x01010233, 0x01010234,
    0x01010235, 0x01010236, 0x01010237, 0x01010238,
    0x01010239, 0x0101023a, 0x0101023b, 0x010102d9
};

static Int32 R_Styleable_Theme[] = {
    0x01010030, 0x01010031, 0x01010032, 0x01010033,
    0x01010034, 0x01010035, 0x01010036, 0x01010037,
    0x01010038, 0x01010039, 0x0101003a, 0x0101003b,
    0x0101003c, 0x0101003d, 0x0101003e, 0x0101003f,
    0x01010040, 0x01010041, 0x01010042, 0x01010043,
    0x01010044, 0x01010045, 0x01010046, 0x01010047,
    0x01010048, 0x01010049, 0x0101004a, 0x0101004b,
    0x0101004c, 0x0101004d, 0x0101004e, 0x0101004f,
    0x01010050, 0x01010051, 0x01010052, 0x01010053,
    0x01010054, 0x01010055, 0x01010056, 0x01010057,
    0x01010058, 0x01010059, 0x0101005a, 0x0101005b,
    0x0101005c, 0x0101005d, 0x0101005e, 0x0101005f,
    0x01010060, 0x01010061, 0x01010062, 0x0101006a,
    0x0101006b, 0x0101006c, 0x0101006d, 0x0101006e,
    0x0101006f, 0x01010070, 0x01010071, 0x01010072,
    0x01010073, 0x01010074, 0x01010075, 0x01010076,
    0x01010077, 0x01010078, 0x01010079, 0x0101007a,
    0x0101007b, 0x0101007c, 0x0101007d, 0x0101007e,
    0x01010080, 0x01010081, 0x01010082, 0x01010083,
    0x01010084, 0x01010085, 0x01010086, 0x01010087,
    0x01010088, 0x01010089, 0x0101008a, 0x0101008b,
    0x0101008c, 0x0101008d, 0x0101008e, 0x0101008f,
    0x01010090, 0x01010091, 0x01010092, 0x01010093,
    0x01010094, 0x010100ae, 0x01010206, 0x01010207,
    0x01010208, 0x0101020d, 0x0101020f, 0x01010210,
    0x01010212, 0x01010213, 0x01010214, 0x01010219,
    0x0101021a, 0x0101021e, 0x0101021f, 0x01010222,
    0x0101022b, 0x01010230, 0x01010267, 0x01010287,
    0x01010288, 0x01010289, 0x0101028b, 0x01010292,
    0x010102a0, 0x010102a1, 0x010102ab, 0x010102ae,
    0x010102af, 0x010102b0, 0x010102b1, 0x010102b2,
    0x010102b3, 0x010102b6, 0x010102b9, 0x010102c5,
    0x010102c6, 0x010102c7, 0x010102c8, 0x010102cc,
    0x010102cd, 0x010102ce, 0x010102cf, 0x010102d0
};

CKeyboardView::CKeyboardView()
{
}

CKeyboardView::~CKeyboardView()
{
}

PInterface CKeyboardView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CKeyboardView::Probe(riid);
}

ECode CKeyboardView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(context, attrs, 0x010102d9 /*com.android.internal.R.attr.keyboardViewStyle*/);
}

ECode CKeyboardView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
	return Init(context, attrs, defStyle);
}

ECode CKeyboardView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    View::Init(context, attrs, defStyle);

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs,
    		ArrayOf<Int32>(R_Styleable_KeyboardView, sizeof(R_Styleable_KeyboardView) / sizeof(Int32)),
    		defStyle, 0,
        	(ITypedArray**) &a);


    AutoPtr<ILayoutInflater> inflate;
	context->GetSystemService(Context_LAYOUT_INFLATER_SERVICE, (IInterface**) &inflate);

    Int32 previewLayout = 0;
    Int32 keyTextSize = 0;

    Int32 n = 0;
    a->GetIndexCount(&n);

    for (Int32 i = 0; i < n; i++) {
        Int32 attr = 0;
        a->GetIndex(i, &attr);

        switch (attr) {
        case 2 /*com.android.internal.R.styleable.KeyboardView_keyBackground*/:
            a->GetDrawable(attr, (IDrawable**) &mKeyBackground);
            break;
        case 9 /*com.android.internal.R.styleable.KeyboardView_verticalCorrection*/:
            a->GetDimensionPixelOffset(attr, 0, &mVerticalCorrection);
            break;
        case 6 /*com.android.internal.R.styleable.KeyboardView_keyPreviewLayout*/:
            a->GetResourceId(attr, 0, &previewLayout);
            break;
        case 7 /*com.android.internal.R.styleable.KeyboardView_keyPreviewOffset*/:
            a->GetDimensionPixelOffset(attr, 0, &mPreviewOffset);
            break;
        case 8 /*com.android.internal.R.styleable.KeyboardView_keyPreviewHeight*/:
            a->GetDimensionPixelSize(attr, 80, &mPreviewHeight);
            break;
        case 3 /*com.android.internal.R.styleable.KeyboardView_keyTextSize*/:
            a->GetDimensionPixelSize(attr, 18, &mKeyTextSize);
            break;
        case 5 /*com.android.internal.R.styleable.KeyboardView_keyTextColor*/:
            a->GetColor(attr, 0xFF000000, &mKeyTextColor);
            break;
        case 4 /*com.android.internal.R.styleable.KeyboardView_labelTextSize*/:
            a->GetDimensionPixelSize(attr, 14, &mLabelTextSize);
            break;
        case 10 /*com.android.internal.R.styleable.KeyboardView_popupLayout*/:
            a->GetResourceId(attr, 0, &mPopupLayout);
            break;
        case 0 /*com.android.internal.R.styleable.KeyboardView_shadowColor*/:
            a->GetColor(attr, 0, &mShadowColor);
            break;
        case 1 /*com.android.internal.R.styleable.KeyboardView_shadowRadius*/:
            a->GetFloat(attr, 0.0, &mShadowRadius);
            break;
        }
    }

    a->Recycle();

    mContext->ObtainStyledAttributes(
    		ArrayOf<Int32>(R_Styleable_Theme, sizeof(R_Styleable_Theme) / sizeof(Int32)),
    		(ITypedArray**) &a);

    a->GetFloat(2 /*android.R.styleable.Theme_backgroundDimAmount*/, 0.5f, &mBackgroundDimAmount);
    a->Recycle();

    CPopupWindow::New(context, (IPopupWindow**) &mPreviewPopup);
    if (previewLayout != 0) {
        inflate->Inflate(previewLayout, NULL, (IView**) &mPreviewText);

        Float textSize = 0.0;
        mPreviewText->GetTextSize(&textSize);
        mPreviewTextSizeLarge = (Int32)textSize;
        mPreviewPopup->SetContentView(mPreviewText);
        mPreviewPopup->SetBackgroundDrawable(NULL);
    } else {
        mShowPreview = FALSE;
    }

    mPreviewPopup->SetTouchable(FALSE);

    CPopupWindow::New(context, (IPopupWindow**) &mPopupKeyboard);
    mPopupKeyboard->SetBackgroundDrawable(NULL);
    //mPopupKeyboard.setClippingEnabled(FALSE);

    mPopupParent = this;
    //mPredicting = TRUE;

    CPaint::New((IPaint**) &mPaint);
    mPaint->SetAntiAlias(TRUE);
    mPaint->SetTextSize(keyTextSize);

    mPaint->SetTextAlign(PaintAlign_CENTER /*Align.CENTER*/);
    mPaint->SetAlpha(255);

    CRect::New(0, 0, 0, 0, (IRect**) &mPadding);
    //mMiniKeyboardCache = new HashMap<Key,View>();

    Boolean isPadding = FALSE;
    mKeyBackground->GetPadding(mPadding, &isPadding);

    AutoPtr<IResources> res;
    GetResources((IResources**) &res);
    assert(res != NULL);

    AutoPtr<IDisplayMetrics> dis;
    res->GetDisplayMetrics((IDisplayMetrics**) &dis);
    mSwipeThreshold = (Int32) (500 * ((CDisplayMetrics*)dis.Get())->mDensity);

    res->GetBoolean(0x010d000e /*com.android.internal.R.bool.config_swipeDisambiguation*/,
    		&mDisambiguateSwipe);

    ResetMultiTap();
    InitGestureDetector();

    return NOERROR;
}

ECode CKeyboardView::OnClick(
    /* [in] */ IView* v)
{
    return KeyboardView::OnClick(v);
}

ECode CKeyboardView::SetOnKeyboardActionListener(
    /* [in] */ IOnKeyboardActionListener* listener)
{
    return KeyboardView::SetOnKeyboardActionListener(listener);
}
