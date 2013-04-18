
#include "widget/CheckedTextView.h"

static const Int32 R_Styleable_CheckedTextView_checked = 0;
static const Int32 R_Styleable_CheckedTextView_checkMark = 1;

static Int32 R_Styleable_CheckedTextView[] = {
    0x01010106, 0x01010108
};

static Int32 R_Attr_State_checked = 0x010100a0;

Int32 CheckedTextView::CHECKED_STATE_SET[] = {
    R_Attr_State_checked
};

CheckedTextView::CheckedTextView()
{

}

CheckedTextView::CheckedTextView(
    /* [in] */ IContext* context)
{
    Init(context);
}

CheckedTextView::CheckedTextView(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs)
{
    Init(context, attrs);
}

CheckedTextView::CheckedTextView(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs, 
    /* [in] */ Int32 defStyle) : TextView(context, attrs, defStyle)
{
    Init(context, attrs, defStyle);
}

void CheckedTextView::Init(
    /* [in] */ IContext* context, 
    /* [in] */ IAttributeSet* attrs /* = NULL */, 
    /* [in] */ Int32 defStyle /* = 0 */)
{
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs,
        ArrayOf<Int32>(R_Styleable_CheckedTextView, 2), defStyle, 0, (ITypedArray**)&a);

    AutoPtr<IDrawable> d;
    a->GetDrawable(R_Styleable_CheckedTextView_checkMark, (IDrawable**)&d);
    if (d != NULL) {
        SetCheckMarkDrawable(d);
    }

    Boolean checked;
    a->GetBoolean(R_Styleable_CheckedTextView_checked, FALSE, &checked);
    SetChecked(checked);

    a->Recycle();
}

ECode CheckedTextView::Toggle()
{
    SetChecked(!mChecked);

    return NOERROR;
}

Boolean CheckedTextView::IsChecked()
{
    return mChecked;
}

/**
 * <p>Changes the checked state of this text view.</p>
 *
 * @param checked true to check the text, FALSE to uncheck it
 */
ECode CheckedTextView::SetChecked(
    /* [in] */ Boolean checked)
{
    if (mChecked != checked) {
        mChecked = checked;
        RefreshDrawableState();
    }

    return NOERROR;
}


/**
 * Set the checkmark to a given Drawable, identified by its resourece id. This will be drawn
 * when {@link #isChecked()} is true.
 * 
 * @param resid The Drawable to use for the checkmark.
 */
ECode CheckedTextView::SetCheckMarkDrawable(
    /* [in] */ Int32 resid)
{
    if (resid != 0 && resid == mCheckMarkResource) {
        return E_INVALID_ARGUMENT;
    }

    mCheckMarkResource = resid;

    AutoPtr<IDrawable> d;
    if (mCheckMarkResource != 0) {
        GetResources()->GetDrawable(mCheckMarkResource, (IDrawable**)&d);
    }
    SetCheckMarkDrawable(d);

    return NOERROR;
}

/**
 * Set the checkmark to a given Drawable. This will be drawn when {@link #isChecked()} is true.
 *
 * @param d The Drawable to use for the checkmark.
 */
ECode CheckedTextView::SetCheckMarkDrawable(
    /* [in] */ IDrawable* d) 
{
    if (mCheckMarkDrawable != NULL) {
        mCheckMarkDrawable->SetCallback(NULL);
        UnscheduleDrawable(mCheckMarkDrawable);
    }
    if (d != NULL) {
        d->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));

        Boolean res;
        d->SetVisible(GetVisibility() == VISIBLE, FALSE, &res);

        ArrayOf<Int32> *set = ArrayOf<Int32>::Alloc(CHECKED_STATE_SET, sizeof(CHECKED_STATE_SET));
        d->SetState(set, &res);
        
        Int32 height;
        d->GetIntrinsicHeight(&height);
        SetMinHeight(height);
        
        d->GetIntrinsicWidth(&mCheckMarkWidth);
        mPaddingRight = mCheckMarkWidth + mBasePaddingRight;
        d->SetState(GetDrawableState(), &res);
    } else {
        mPaddingRight = mBasePaddingRight;
    }
    mCheckMarkDrawable = d;
    RequestLayout();

    return NOERROR;
}

ECode CheckedTextView::SetPadding(
    /* [in] */ Int32 left, 
    /* [in] */ Int32 top, 
    /* [in] */ Int32 right, 
    /* [in] */ Int32 bottom)
{
    TextView::SetPadding(left, top, right, bottom);
    mBasePaddingRight = mPaddingRight;

    return NOERROR;
}

void CheckedTextView::OnDraw(
    /* [in] */ ICanvas* canvas) 
{
    TextView::OnDraw(canvas);

    AutoPtr<IDrawable> checkMarkDrawable = mCheckMarkDrawable;
    if (checkMarkDrawable != NULL) {
        Int32 verticalGravity = GetGravity() & Gravity_VERTICAL_GRAVITY_MASK;
        Int32 height;
        checkMarkDrawable->GetIntrinsicHeight(&height);

        Int32 y = 0;

        switch (verticalGravity) {
            case Gravity_BOTTOM:
                y = GetHeight() - height;
                break;
            case Gravity_CENTER_VERTICAL:
                y = (GetHeight() - height) / 2;
                break;
        }
        
        Int32 right = GetWidth();
        checkMarkDrawable->SetBounds(
                right - mCheckMarkWidth - mBasePaddingRight, 
                y, 
                right - mBasePaddingRight, 
                y + height);
        checkMarkDrawable->Draw(canvas);
    }
}

ECode CheckedTextView::OnCreateDrawableState(
    /* [in] */ Int32 extraSpace,
    /* [out] */ ArrayOf<Int32>** drawableState)
{
    TextView::OnCreateDrawableState(extraSpace + 1, drawableState);
    if (IsChecked()) {
        ArrayOf<Int32>* set = ArrayOf<Int32>::Alloc(CHECKED_STATE_SET, sizeof(CHECKED_STATE_SET) / sizeof(Int32));
        MergeDrawableStates(*drawableState, set);
    }
    return NOERROR;
}

ECode CheckedTextView::DrawableStateChanged()
{
    TextView::DrawableStateChanged();
    
    if (mCheckMarkDrawable != NULL) {
        ArrayOf<Int32>* myDrawableState = GetDrawableState();
        
        Boolean res;
        // Set the state of the Drawable
        mCheckMarkDrawable->SetState(myDrawableState, &res);
        
        Invalidate();
    }

    return NOERROR;
}

Boolean CheckedTextView::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    Boolean populated = TextView::DispatchPopulateAccessibilityEvent(event);
    if (!populated) {
        //event->SetChecked(mChecked);
    }
    return populated;
}

