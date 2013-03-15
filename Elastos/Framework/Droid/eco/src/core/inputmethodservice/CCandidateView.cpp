
#include "inputmethodservice/CCandidateView.h"
#include "graphics/CPaint.h"

IVIEW_METHODS_IMPL(CCandidateView, CandidateView, CandidateView);
IDrawableCallback_METHODS_IMPL(CCandidateView, CandidateView, CandidateView);
IKeyEventCallback_METHODS_IMPL(CCandidateView, CandidateView, CandidateView);
IAccessibilityEventSource_METHODS_IMPL(CCandidateView, CandidateView, CandidateView);

const Int32 CandidateView::OUT_OF_BOUNDS;
const Int32 CandidateView::MAX_SUGGESTIONS;
const Int32 CandidateView::SCROLL_PIXELS;
const Int32 CandidateView::X_GAP;
ArrayOf< AutoPtr<ICharSequence> >* CandidateView::EMPTY_LIST = ArrayOf< AutoPtr<ICharSequence> >::Alloc(0);

CandidateView::SelfSimpleOnGestureListener::SelfSimpleOnGestureListener(
    /* [in] */ CandidateView* host)
    : mHost(host)
{}

CandidateView::SelfSimpleOnGestureListener::~SelfSimpleOnGestureListener()
{}

UInt32 CandidateView::SelfSimpleOnGestureListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CandidateView::SelfSimpleOnGestureListener::Release()
{
    return ElRefBase::Release();
}

Boolean CandidateView::SelfSimpleOnGestureListener::OnScroll(
    /* [in] */ IMotionEvent* e1,
    /* [in] */ IMotionEvent* e2,
    /* [in] */ Float distanceX,
    /* [in] */ Float distanceY)
{
    assert(mHost != NULL);
    mHost->mScrolled = TRUE;
    Int32 sx = mHost->GetScrollX();
    sx += distanceX;
    if (sx < 0) {
        sx = 0;
    }
    if (sx + mHost->GetWidth() > mHost->mTotalWidth) {
        sx -= distanceX;
    }
    mHost->mTargetScrollX = sx;
    mHost->ScrollTo(sx, mHost->GetScrollY());
    mHost->Invalidate();
    return TRUE;
}


CandidateView::CandidateView()
    : mSuggestions(NULL)
    , mSelectedIndex(0)
    , mTouchX(OUT_OF_BOUNDS)
    , mTypedWordValid(FALSE)
    , mWordWidth(ArrayOf<Int32>::Alloc(MAX_SUGGESTIONS))
    , mWordX(ArrayOf<Int32>::Alloc(MAX_SUGGESTIONS))
    , mColorNormal(0)
    , mColorRecommended(0)
    , mColorOther(0)
    , mVerticalPadding(0)
    , mScrolled(FALSE)
    , mTargetScrollX(0)
    , mTotalWidth(0)
    , mGestureDetector(NULL)
{}

CandidateView::~CandidateView()
{
    if (mWordWidth != NULL) {
        ArrayOf<Int32>::Free(mWordWidth);
    }

    if (mWordX != NULL) {
        ArrayOf<Int32>::Free(mWordX);
    }

    if (mGestureDetector != NULL) {
        delete mGestureDetector;
    }
}

ECode CandidateView::Init(
    /* [in] */ IContext * context)
{
    assert(context != NULL);
    FAIL_RETURN(View::Init(context));
    AutoPtr<IResources> r;
    FAIL_RETURN(context->GetResources((IResources**)&r));
    r->GetDrawable(0x01080062 /*android.R.drawable.list_selector_background*/,
            (IDrawable**)&mSelectionHighlight);
    ArrayOf_<Int32, 4> states;
    states[0] = 0x0101009e /*android.R.attr.state_enabled*/;
    states[1] = 0x0101009c /*android.R.attr.state_focused*/;
    states[2] = 0x0101009d /*android.R.attr.state_window_focused*/;
    states[3] = 0x010100a7 /*android.R.attr.state_pressed*/;
    Boolean temp = FALSE;
    mSelectionHighlight->SetState(&states, &temp);

    Int32 color = 0;
    r->GetColor(0x7f050003 /*R.color.candidate_background*/, &color);
    SetBackgroundColor(color);

    r->GetColor(0x7f050000 /*R.color.candidate_normal*/, &mColorNormal);
    r->GetColor(0x7f050001 /*R.color.candidate_recommended*/, &mColorRecommended);
    r->GetColor(0x7f050002 /*R.color.candidate_other*/, &mColorOther);
    r->GetDimensionPixelSize(0x7f060002 /*R.dimen.candidate_vertical_padding*/, &mVerticalPadding);

    CPaint::New((IPaint**)&mPaint);
    mPaint->SetColor(mColorNormal);
    mPaint->SetAntiAlias(TRUE);
    Int32 tmp = 0;
    r->GetDimensionPixelSize(0x7f060001 /*R.dimen.candidate_font_height*/, &tmp);
    mPaint->SetTextSize(tmp);
    mPaint->SetStrokeWidth(0);

    mGestureDetector = new GestureDetector(NULL, new SelfSimpleOnGestureListener(this));
    SetHorizontalFadingEdgeEnabled(TRUE);
    SetWillNotDraw(FALSE);
    SetHorizontalScrollBarEnabled(FALSE);
    SetVerticalScrollBarEnabled(FALSE);
    return NOERROR;
}

ECode CandidateView::SetService(
    /* [in] */ ISoftKeyboard* listener)
{
    mService = listener;
    return NOERROR;
}

ECode CandidateView::ComputeHorizontalScrollRange(
    /* [out] */ Int32* range)
{
    assert(range != NULL);
    *range = mTotalWidth;
    return NOERROR;
}

void CandidateView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 measuredWidth = ResolveSize(50, widthMeasureSpec);

    // Get the desired height of the icon menu view (last row of items does
    // not have a divider below)
    AutoPtr<IRect> padding;
    CRect::New((IRect**)&padding);
    Boolean isPadding = FALSE;
    mSelectionHighlight->GetPadding(padding, &isPadding);
    Float size;
    Int32 top = 0, bottom = 0;
    mPaint->GetTextSize(&size);
    padding->GetTop(&top);
    padding->GetBottom(&bottom);
    Int32 desiredHeight = (Int32)size + mVerticalPadding + top + bottom;

    // Maximum possible width and desired height
    SetMeasuredDimension(measuredWidth,
            ResolveSize(desiredHeight, heightMeasureSpec));
}

void CandidateView::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    if (canvas != NULL) {
        View::OnDraw(canvas);
    }
    mTotalWidth = 0;
    if (mSuggestions == NULL) return;

    if (mBgPadding == NULL) {
        CRect::New(0, 0, 0, 0, (IRect**)&mBgPadding);
        AutoPtr<IDrawable> bg = GetBackground();
        if (bg != NULL) {
            Boolean isPadding = FALSE;
            bg->GetPadding(mBgPadding, &isPadding);
        }
    }
    Int32 x = 0;
    Int32 count = mSuggestions->GetLength() ;
    Int32 height = GetHeight();
    Int32 touchX = mTouchX;
    Int32 scrollX = GetScrollX();
    Boolean scrolled = mScrolled;
    Boolean typedWordValid = mTypedWordValid;
    Float textSize = 0, ascent = 0;
    mPaint->GetTextSize((Float*)&textSize);
    mPaint->Ascent((Float*)&ascent);
    Int32 y = (Int32) (((height - textSize) / 2) - ascent);

    Int32 top;
    mBgPadding->GetTop(&top);
    for (Int32 i = 0; i < count; i++) {
        ICharSequence* text = (*mSuggestions)[i];
        String suggestion;
        if (text != NULL) {
            text->ToString(&suggestion);
        }
        Float textWidth = 0;
        mPaint->MeasureTextEx2(suggestion, &textWidth);
        Int32 wordWidth = (Int32) textWidth + X_GAP * 2;

        (*mWordX)[i] = x;
        (*mWordWidth)[i] = wordWidth;
        mPaint->SetColor(mColorNormal);
        if (touchX + scrollX >= x && touchX + scrollX < x + wordWidth && !scrolled) {
            if (canvas != NULL) {
                canvas->Translate(x, 0);
                mSelectionHighlight->SetBounds(0, top, wordWidth, height);
                mSelectionHighlight->Draw(canvas);
                canvas->Translate(-x, 0);
            }
            mSelectedIndex = i;
        }

        if (canvas != NULL) {
            if ((i == 1 && !typedWordValid) || (i == 0 && typedWordValid)) {
                mPaint->SetFakeBoldText(TRUE);
                mPaint->SetColor(mColorRecommended);
            }
            else if (i != 0) {
                mPaint->SetColor(mColorOther);
            }
            canvas->DrawTextInString(suggestion, x + X_GAP, y, mPaint);
            mPaint->SetColor(mColorOther);
            canvas->DrawLine(x + wordWidth + 0.5f, top,
                    x + wordWidth + 0.5f, height + 1, mPaint);
            mPaint->SetFakeBoldText(FALSE);
        }
        x += wordWidth;
    }
    mTotalWidth = x;
    if (mTargetScrollX != GetScrollX()) {
        ScrollToTarget();
    };
}

void CandidateView::ScrollToTarget()
{
    Int32 sx = GetScrollX();
    if (mTargetScrollX > sx) {
        sx += SCROLL_PIXELS;
        if (sx >= mTargetScrollX) {
            sx = mTargetScrollX;
            RequestLayout();
        }
    }
    else {
        sx -= SCROLL_PIXELS;
        if (sx <= mTargetScrollX) {
            sx = mTargetScrollX;
            RequestLayout();
        }
    }
    ScrollTo(sx, GetScrollY());
    Invalidate();
}

ECode CandidateView::SetSuggestions(
    /* [in] */ ArrayOf<ICharSequence*>* suggestions,
    /* [in] */ Boolean completions,
    /* [in] */ Boolean typedWordValid)
{
    Clear();
    if (suggestions != NULL) {
        if (mSuggestions != NULL && mSuggestions != EMPTY_LIST) {
            for (Int32 i = 0; i < mSuggestions->GetLength(); ++i) {
                (*mSuggestions)[i] = NULL;
            }
        }
        ArrayOf< AutoPtr<ICharSequence> >::Free(mSuggestions);
        mSuggestions = ArrayOf< AutoPtr<ICharSequence> >::Alloc(suggestions->GetLength());
        for (Int32 i = 0; i < mSuggestions->GetLength(); ++i) {
            (*mSuggestions)[i] = (*suggestions)[i];
        }
    }
    mTypedWordValid = typedWordValid;
    ScrollTo(0, 0);
    mTargetScrollX = 0;
    // Compute the total width
    OnDraw(NULL);
    Invalidate();
    RequestLayout();
    return NOERROR;
}

ECode CandidateView::Clear()
{
    if (mSuggestions != NULL && mSuggestions != EMPTY_LIST) {
        for (Int32 i = 0; i < mSuggestions->GetLength(); ++i) {
            (*mSuggestions)[i] = NULL;
        }
        ArrayOf< AutoPtr<ICharSequence> >::Free(mSuggestions);
        mSuggestions = EMPTY_LIST;
    }
    mTouchX = OUT_OF_BOUNDS;
    mSelectedIndex = -1;
    Invalidate();
    return NOERROR;
}

Boolean CandidateView::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    if (mGestureDetector->OnTouchEvent(event)) {
        return TRUE;
    }

    Int32 action = 0, x = 0, y = 0;
    event->GetAction(&action);
    Float fx, fy;
    event->GetX(&fx);
    event->GetY(&fy);
    x = (Int32)fx;
    y = (Int32)fy;
    mTouchX = x;

    switch (action) {
    case MotionEvent_ACTION_DOWN:
        mScrolled = FALSE;
        Invalidate();
        break;
    case MotionEvent_ACTION_MOVE:
        if (y <= 0) {
            // Fling up!?
            if (mSelectedIndex >= 0) {
                mService->PickSuggestionManually(mSelectedIndex);
                mSelectedIndex = -1;
            }
        }
        Invalidate();
        break;
    case MotionEvent_ACTION_UP:
        if (!mScrolled) {
            if (mSelectedIndex >= 0) {
                mService->PickSuggestionManually(mSelectedIndex);
            }
        }
        mSelectedIndex = -1;
        RemoveHighlight();
        RequestLayout();
        break;
    }
    return TRUE;
}

void CandidateView::TakeSuggestionAt(
    /* [in] */ Float x)
{
    mTouchX = (Int32)x;
    // To detect candidate
    OnDraw(NULL);
    if (mSelectedIndex >= 0) {
        mService->PickSuggestionManually(mSelectedIndex);
    }
    Invalidate();
}

void CandidateView::RemoveHighlight()
{
    mTouchX = OUT_OF_BOUNDS;
    Invalidate();
}


ECode CCandidateView::constructor(
    /* [in] */ IContext * context)
{
    return CandidateView::Init(context);
}

PInterface CCandidateView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    return _CCandidateView::Probe(riid);
}

ECode CCandidateView::SetService(
    /* [in] */ ISoftKeyboard* listener)
{
    return CandidateView::SetService(listener);
}

ECode CCandidateView::SetSuggestions(
    /* [in] */ ArrayOf<ICharSequence*>* suggestions,
    /* [in] */ Boolean completions,
    /* [in] */ Boolean typedWordValid)
{
    return CandidateView::SetSuggestions(suggestions, completions, typedWordValid);
}

ECode CCandidateView::Clear()
{
    return CandidateView::Clear();
}

