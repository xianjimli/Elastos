/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * Displays text to the user and optionally allows them to edit it.  A TextView
 * is a complete text editor, however the basic class is configured to not
 * allow editing; see {@link EditText} for a subclass that configures the text
 * view for editing.
 *
 * <p>
 * <b>XML attributes</b>
 * <p>
 * See {@link android.R.styleable#TextView TextView Attributes},
 * {@link android.R.styleable#View View Attributes}
 *
 * @attr ref android.R.styleable#TextView_text
 * @attr ref android.R.styleable#TextView_bufferType
 * @attr ref android.R.styleable#TextView_hint
 * @attr ref android.R.styleable#TextView_textColor
 * @attr ref android.R.styleable#TextView_textColorHighlight
 * @attr ref android.R.styleable#TextView_textColorHint
 * @attr ref android.R.styleable#TextView_textAppearance
 * @attr ref android.R.styleable#TextView_textColorLink
 * @attr ref android.R.styleable#TextView_textSize
 * @attr ref android.R.styleable#TextView_textScaleX
 * @attr ref android.R.styleable#TextView_typeface
 * @attr ref android.R.styleable#TextView_textStyle
 * @attr ref android.R.styleable#TextView_cursorVisible
 * @attr ref android.R.styleable#TextView_maxLines
 * @attr ref android.R.styleable#TextView_maxHeight
 * @attr ref android.R.styleable#TextView_lines
 * @attr ref android.R.styleable#TextView_height
 * @attr ref android.R.styleable#TextView_minLines
 * @attr ref android.R.styleable#TextView_minHeight
 * @attr ref android.R.styleable#TextView_maxEms
 * @attr ref android.R.styleable#TextView_maxWidth
 * @attr ref android.R.styleable#TextView_ems
 * @attr ref android.R.styleable#TextView_width
 * @attr ref android.R.styleable#TextView_minEms
 * @attr ref android.R.styleable#TextView_minWidth
 * @attr ref android.R.styleable#TextView_gravity
 * @attr ref android.R.styleable#TextView_scrollHorizontally
 * @attr ref android.R.styleable#TextView_password
 * @attr ref android.R.styleable#TextView_singleLine
 * @attr ref android.R.styleable#TextView_selectAllOnFocus
 * @attr ref android.R.styleable#TextView_includeFontPadding
 * @attr ref android.R.styleable#TextView_maxLength
 * @attr ref android.R.styleable#TextView_shadowColor
 * @attr ref android.R.styleable#TextView_shadowDx
 * @attr ref android.R.styleable#TextView_shadowDy
 * @attr ref android.R.styleable#TextView_shadowRadius
 * @attr ref android.R.styleable#TextView_autoLink
 * @attr ref android.R.styleable#TextView_linksClickable
 * @attr ref android.R.styleable#TextView_numeric
 * @attr ref android.R.styleable#TextView_digits
 * @attr ref android.R.styleable#TextView_phoneNumber
 * @attr ref android.R.styleable#TextView_inputMethod
 * @attr ref android.R.styleable#TextView_capitalize
 * @attr ref android.R.styleable#TextView_autoText
 * @attr ref android.R.styleable#TextView_editable
 * @attr ref android.R.styleable#TextView_freezesText
 * @attr ref android.R.styleable#TextView_ellipsize
 * @attr ref android.R.styleable#TextView_drawableTop
 * @attr ref android.R.styleable#TextView_drawableBottom
 * @attr ref android.R.styleable#TextView_drawableRight
 * @attr ref android.R.styleable#TextView_drawableLeft
 * @attr ref android.R.styleable#TextView_drawablePadding
 * @attr ref android.R.styleable#TextView_lineSpacingExtra
 * @attr ref android.R.styleable#TextView_lineSpacingMultiplier
 * @attr ref android.R.styleable#TextView_marqueeRepeatLimit
 * @attr ref android.R.styleable#TextView_inputType
 * @attr ref android.R.styleable#TextView_imeOptions
 * @attr ref android.R.styleable#TextView_privateImeOptions
 * @attr ref android.R.styleable#TextView_imeActionLabel
 * @attr ref android.R.styleable#TextView_imeActionId
 * @attr ref android.R.styleable#TextView_editorExtras
 */

#include "widget/TextView.h"
#include "os/SystemClock.h"
#include "os/CApartment.h"
#include "content/CResourcesFactory.h"
#include "content/CColorStateList.h"
#include "content/CCompatibilityInfo.h"
#include "utils/CTypedValue.h"
#include "utils/CDisplayMetrics.h"
#include "text/CTextPaint.h"
#include "text/CBoringLayoutMetrics.h"
#include "text/CBoringLayout.h"
#include "text/CStaticLayout.h"
#include "text/CDynamicLayout.h"
#include "text/CSpannableString.h"
#include "text/Selection.h"
#include "text/CEditableFactory.h"
#include "text/CSpannableFactory.h"
#include "text/CLengthFilter.h"
#include "text/method/MetaKeyKeyListener.h"
#include "text/method/TextKeyListener.h"
#include "view/CKeyEvent.h"
#include "view/CViewGroupLayoutParams.h"
#include "view/CWindowManagerLayoutParams.h"
#include "view/animation/AnimationUtils.h"
#include "view/inputmethod/CLocalInputMethodManager.h"
#include "view/inputmethod/CExtractedTextRequest.h"
#include "view/inputmethod/CEditorInfo.h"
#include "view/inputmethod/BaseInputConnection.h"
#include "graphics/CPaint.h"
#include "graphics/Typeface.h"
#include "graphics/CPath.h"
#include "widget/CTextViewSavedState.h"
#include "widget/CPopupWindow.h"
#include "widget/CEditableInputConnection.h"
#include "widget/CTextViewCommitSelectionReceiver.h"
#include <elastos/Math.h>
#include <elastos/Character.h>
#include <Logger.h>


using namespace Elastos::Utility::Logging;
using namespace Elastos::Core;


TextView::Drawables::Drawables()
    : mDrawableSizeTop(0)
    , mDrawableSizeBottom(0)
    , mDrawableSizeLeft(0)
    , mDrawableSizeRight(0)
    , mDrawableWidthTop(0)
    , mDrawableWidthBottom(0)
    , mDrawableHeightLeft(0)
    , mDrawableHeightRight(0)
    , mDrawablePadding(0)
{
    CRect::NewByFriend((CRect**)&mCompoundRect);
}

TextView::CharWrapper::CharWrapper(
    /* [in] */ const ArrayOf<Char8>& chars,
    /* [in] */ Int32 start,
    /* [in] */ Int32 len)
    : mChars(chars.Clone())
    , mStart(start)
    , mLength(len)
{}

TextView::CharWrapper::~CharWrapper()
{
    ArrayOf<Char8>::Free(mChars);
}

PInterface TextView::CharWrapper::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_ICharSequence) {
        return (ICharSequence*)this;
    }

    return NULL;
}

UInt32 TextView::CharWrapper::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TextView::CharWrapper::Release()
{
    return ElRefBase::Release();
}

ECode TextView::CharWrapper::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

void TextView::CharWrapper::Set(
    /* [in] */ const ArrayOf<Char8>& chars,
    /* [in] */ Int32 start,
    /* [in] */ Int32 len)
{
    ArrayOf<Char8>::Free(mChars);
    mChars = chars.Clone();
    mStart = start;
    mLength = len;
}

ECode TextView::CharWrapper::GetLength(
    /* [out] */ Int32* len)
{
    VALIDATE_NOT_NULL(len);
    *len = mLength;
    return NOERROR;
}

ECode TextView::CharWrapper::GetCharAt(
    /* [in] */ Int32 off,
    /* [out] */ Char32* c)
{
    VALIDATE_NOT_NULL(c);
    *c = String((const char*)mChars->GetPayload()).GetChar(off + mStart);
    return NOERROR;
}

ECode TextView::CharWrapper::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String((const char*)mChars->GetPayload()).Substring(mStart, mLength);

    return NOERROR;
}

ECode TextView::CharWrapper::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);
    if (start < 0 || end < 0 || start > mLength || end > mLength) {
        //throw new IndexOutOfBoundsException(start + ", " + end);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    String subStr = String((const char*)mChars->GetPayload()).Substring(
        mStart + start, end - start);
    return CStringWrapper::New(subStr, csq);
}

ECode TextView::CharWrapper::GetChars(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Char8>* dest,
    /* [in] */ Int32 destoff)
{
    if (start < 0 || end < 0 || start > mLength || end > mLength) {
        //throw new IndexOutOfBoundsException(start + ", " + end);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    //dest->Copy(mChars, start + mStart, buf, off, end - start);
    memcpy(&(*dest[destoff]), &mChars[start + mStart], end - start);
    return NOERROR;
}

ECode TextView::CharWrapper::DrawText(
    /* [in] */ ICanvas* c,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* p)
{
    return c->DrawTextInBuffer(*mChars, start + mStart, end - start, x, y, p);
}

ECode TextView::CharWrapper::MeasureText(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IPaint* p,
    /* [out] */ Float* width)
{
    return p->MeasureText(*mChars, start + mStart, end - start, width);
}

ECode TextView::CharWrapper::GetTextWidths(
    /* [in] */ int start,
    /* [in] */ int end,
    /* [out] */ ArrayOf<Float>* widths,
    /* [in] */ IPaint* p,
    /* [out] */ Int32* count)
{
    return p->GetTextWidths(*mChars, start + mStart, end - start, widths, count);
}


const Int32 TextView::_HandleView::LEFT;
const Int32 TextView::_HandleView::CENTER;
const Int32 TextView::_HandleView::RIGHT;

PInterface TextView::CursorController::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IOnTouchModeChangeListener) {
        return (IOnTouchModeChangeListener*)this;
    }

    return NULL;
}

UInt32 TextView::CursorController::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TextView::CursorController::Release()
{
    return ElRefBase::Release();
}

ECode TextView::CursorController::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);
    if (pObject == (IInterface*)(IOnTouchModeChangeListener*)this) {
        *pIID = EIID_IOnTouchModeChangeListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

TextView::_HandleView::_HandleView(
    /* [in] */ CursorController* controller,
    /* [in] */ Int32 pos,
    /* [in] */ TextView* host)
    : View(host->mContext)
    , mPositionOnTop(FALSE)
    , mIsDragging(FALSE)
    , mHost(host)
{
    mController = controller;
    ASSERT_SUCCEEDED(CPopupWindow::New(
        host->mContext, NULL,
        0x010102c8/*com.android.internal.R.attr.textSelectHandleWindowStyle*/,
        (IPopupWindow**)&mContainer));

    mContainer->SetSplitTouchEnabled(TRUE);
    mContainer->SetClippingEnabled(FALSE);
    mContainer->SetWindowLayoutType(WindowManagerLayoutParams_TYPE_APPLICATION_SUB_PANEL);

    SetOrientation(pos);
}

void TextView::_HandleView::SetOrientation(
    /* [in] */ Int32 pos)
{
    Int32 handleWidth;
    switch (pos) {
    case LEFT:
        {
            if (mHost->mSelectHandleLeft == NULL) {
                AutoPtr<IResources> rs;
                mContext->GetResources((IResources**)&rs);
                rs->GetDrawable(
                    mHost->mTextSelectHandleLeftRes,
                    (IDrawable**)&mHost->mSelectHandleLeft);
            }
            mDrawable = mHost->mSelectHandleLeft;
            mDrawable->GetIntrinsicWidth(&handleWidth);
            mHotspotX = (handleWidth * 3) / 4;
        }
        break;
    case RIGHT:
        {
            if (mHost->mSelectHandleRight == NULL) {
                AutoPtr<IResources> rs;
                mContext->GetResources((IResources**)&rs);
                rs->GetDrawable(
                    mHost->mTextSelectHandleRightRes,
                    (IDrawable**)&mHost->mSelectHandleRight);
            }
            mDrawable = mHost->mSelectHandleRight;
            mDrawable->GetIntrinsicWidth(&handleWidth);
            mHotspotX = handleWidth / 4;
        }
        break;
    case CENTER:
    default:
        {
            if (mHost->mSelectHandleCenter == NULL) {
                AutoPtr<IResources> rs;
                mContext->GetResources((IResources**)&rs);
                rs->GetDrawable(
                    mHost->mTextSelectHandleRes,
                    (IDrawable**)&mHost->mSelectHandleCenter);
            }
            mDrawable = mHost->mSelectHandleCenter;
            mDrawable->GetIntrinsicWidth(&handleWidth);
            mHotspotX = handleWidth / 2;
        }
        break;
    }

    Int32 handleHeight;
    mDrawable->GetIntrinsicHeight(&handleHeight);

    mTouchOffsetY = -handleHeight * 0.3f;
    mHotspotY = 0;
    mHeight = handleHeight;
    Invalidate();
}

//@Override
void TextView::_HandleView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 w, h;
    mDrawable->GetIntrinsicWidth(&w);
    mDrawable->GetIntrinsicHeight(&h);
    SetMeasuredDimension(w, h);
}

void TextView::_HandleView::Show()
{
    if (!IsPositionVisible()) {
        Hide();
        return;
    }
    mContainer->SetContentView((IView*)this->Probe(EIID_IView));
    Int32* coords = mHost->mTempCoords;
    mHost->GetLocationInWindow(coords, coords + 1);
    coords[0] += mPositionX;
    coords[1] += mPositionY;
    mContainer->ShowAtLocation(
        (IView*)mHost->Probe(EIID_IView), 0, coords[0], coords[1]);
}

void TextView::_HandleView::Hide()
{
    mIsDragging = FALSE;
    mContainer->Dismiss();
}

Boolean TextView::_HandleView::IsShowing()
{
    Boolean res;
    mContainer->IsShowing(&res);
    return res;
}

Boolean TextView::_HandleView::IsPositionVisible()
{
    // Always show a dragging handle.
    if (mIsDragging) {
        return TRUE;
    }

    if (mHost->IsInBatchEditMode()) {
        return FALSE;
    }

    Int32 extendedPaddingTop = mHost->GetExtendedPaddingTop();
    Int32 extendedPaddingBottom = mHost->GetExtendedPaddingBottom();
    Int32 compoundPaddingLeft = mHost->GetCompoundPaddingLeft();
    Int32 compoundPaddingRight = mHost->GetCompoundPaddingRight();

    Int32 left = 0;
    Int32 right = mHost->GetWidth();
    Int32 top = 0;
    Int32 bottom = mHost->GetHeight();

    if (mHost->mTempRect == NULL) {
        ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mHost->mTempRect));
    }
    CRect* clip = mHost->mTempRect;
    clip->mLeft = left + compoundPaddingLeft;
    clip->mTop = top + extendedPaddingTop;
    clip->mRight = right - compoundPaddingRight;
    clip->mBottom = bottom - extendedPaddingBottom;

    AutoPtr<IViewParent> parent = mHost->GetParent();
    Boolean res;
    if (parent == NULL || !(parent->GetChildVisibleRect(
        (IView*)mHost->Probe(EIID_IView), clip, NULL, &res), res)) {
        return FALSE;
    }

    Int32* coords = mHost->mTempCoords;
    mHost->GetLocationInWindow(coords, coords + 1);
    Int32 posX = coords[0] + mPositionX + (Int32)mHotspotX;
    Int32 posY = coords[1] + mPositionY + (Int32)mHotspotY;

    return posX >= clip->mLeft && posX <= clip->mRight &&
        posY >= clip->mTop && posY <= clip->mBottom;
}

void TextView::_HandleView::MoveTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    mPositionX = x - mHost->mScrollX;
    mPositionY = y - mHost->mScrollY;
    if (IsPositionVisible()) {
        Int32* coords = NULL;
        Boolean isShowing;
        mContainer->IsShowing(&isShowing);
        if (isShowing)
        {
            coords = mHost->mTempCoords;
            mHost->GetLocationInWindow(coords, coords + 1);
            mContainer->UpdateEx2(coords[0] + mPositionX, coords[1] + mPositionY,
                    mRight - mLeft, mBottom - mTop);
        }
        else {
            Show();
        }

        if (mIsDragging) {
            if (coords == NULL) {
                coords = mHost->mTempCoords;
                mHost->GetLocationInWindow(coords, coords + 1);
            }
            if (coords[0] != mLastParentX || coords[1] != mLastParentY) {
                mTouchToWindowOffsetX += coords[0] - mLastParentX;
                mTouchToWindowOffsetY += coords[1] - mLastParentY;
                mLastParentX = coords[0];
                mLastParentY = coords[1];
            }
        }
    }
    else {
        Hide();
    }
}

//@Override
void TextView::_HandleView::OnDraw(
    /* [in] */ ICanvas* c)
{
    mDrawable->SetBounds(0, 0, mRight - mLeft, mBottom - mTop);
    if (mPositionOnTop) {
        Int32 save;
        c->Save(&save);
        c->RotateEx(180, (mRight - mLeft) / 2, (mBottom - mTop) / 2);
        mDrawable->Draw(c);
        c->Restore();
    }
    else {
        mDrawable->Draw(c);
    }
}

//@Override
Boolean TextView::_HandleView::OnTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    Int32 actionMasked;
    ev->GetActionMasked(&actionMasked);
    switch (actionMasked) {
    case MotionEvent_ACTION_DOWN:
        {
            Float rawX;
            ev->GetRawX(&rawX);
            Float rawY;
            ev->GetRawY(&rawY);
            mTouchToWindowOffsetX = rawX - mPositionX;
            mTouchToWindowOffsetY = rawY - mPositionY;
            Int32* coords = mHost->mTempCoords;
            mHost->GetLocationInWindow(coords, coords + 1);
            mLastParentX = coords[0];
            mLastParentY = coords[1];
            mIsDragging = TRUE;
        }
        break;
    case MotionEvent_ACTION_MOVE:
        {
            Float rawX;
            ev->GetRawX(&rawX);
            Float rawY;
            ev->GetRawY(&rawY);
            Float newPosX = rawX - mTouchToWindowOffsetX + mHotspotX;
            Float newPosY = rawY - mTouchToWindowOffsetY + mHotspotY + mTouchOffsetY;
            mController->UpdatePosition(
                (HandleView*)this, Math::Round(newPosX), Math::Round(newPosY));
        }
        break;
    case MotionEvent_ACTION_UP:
    case MotionEvent_ACTION_CANCEL:
        mIsDragging = FALSE;
    }
    return TRUE;
}

Boolean TextView::_HandleView::IsDragging()
{
    return mIsDragging;
}

void TextView::_HandleView::PositionAtCursor(
    /* [in] */ Int32 offset,
    /* [in] */ Boolean bottom)
{
    Int32 width;
    mDrawable->GetIntrinsicWidth(&width);
    Int32 height;
    mDrawable->GetIntrinsicHeight(&height);
    Int32 line;
    mHost->mLayout->GetLineForOffset(offset, &line);
    Int32 lineTop;
    mHost->mLayout->GetLineTop(line, &lineTop);
    Int32 lineBottom;
    mHost->mLayout->GetLineBottom(line, &lineBottom);

    CRect* bounds = sCursorControllerTempRect;
    Float ph;
    mHost->mLayout->GetPrimaryHorizontal(offset, &ph);
    bounds->mLeft = (Int32)(ph - mHotspotX) + mHost->mScrollX;
    bounds->mTop = (bottom ? lineBottom : lineTop - mHeight) + mHost->mScrollY;

    bounds->mRight = bounds->mLeft + width;
    bounds->mBottom = bounds->mTop + height;

    mHost->ConvertFromViewportToContentCoordinates(bounds);
    MoveTo(bounds->mLeft, bounds->mTop);
}

IVIEW_METHODS_IMPL(
    TextView::HandleView, TextView::_HandleView, TextView::_HandleView);
IDrawableCallback_METHODS_IMPL(
    TextView::HandleView, TextView::_HandleView, TextView::_HandleView);
IKeyEventCallback_METHODS_IMPL(
    TextView::HandleView, TextView::_HandleView, TextView::_HandleView);
IAccessibilityEventSource_METHODS_IMPL(
    TextView::HandleView, TextView::_HandleView, TextView::_HandleView);

TextView::HandleView::HandleView(
    /* [in] */ CursorController* controller,
    /* [in] */ Int32 pos,
    /* [in] */ TextView* host)
    : _HandleView(controller, pos, host)
{}

PInterface TextView::HandleView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IView*)this;
    }
    else if (riid == EIID_IView) {
        return (IView*)this;
    }
    else if (riid == EIID_IDrawableCallback) {
        return (IDrawableCallback*)this;
    }
    else if (riid == EIID_IKeyEventCallback) {
        return (IKeyEventCallback*)this;
    }
    else if (riid == EIID_IAccessibilityEvent) {
        return (IAccessibilityEvent*)this;
    }
    else if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)(_HandleView*)this);
    }

    return NULL;
}

UInt32 TextView::HandleView::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TextView::HandleView::Release()
{
    return ElRefBase::Release();
}

ECode TextView::HandleView::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);
    if (pObject == (IInterface*)(IView*)this) {
        *pIID = EIID_IView;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

const Int32 TextView::InsertionPointCursorController::DELAY_BEFORE_FADE_OUT;

TextView::InsertionPointCursorController::InsertionPointCursorController(
    /* [in] */ TextView* host)
    : mHost(host)
{
    mHandle = new HandleView(this, HandleView::CENTER, mHost);
    mHider = new MyRunnable(this);
}

void TextView::InsertionPointCursorController::Show()
{
    UpdatePosition();
    mHandle->Show();
    HideDelayed(DELAY_BEFORE_FADE_OUT);
}

void TextView::InsertionPointCursorController::Hide()
{
    mHandle->Hide();
    mHost->RemoveCallbacks(mHider);
}

void TextView::InsertionPointCursorController::HideDelayed(
    /* [in] */ Int32 msec)
{
    mHost->RemoveCallbacks(mHider);
    mHost->PostDelayed(mHider, msec);
}

Boolean TextView::InsertionPointCursorController::IsShowing()
{
    return mHandle->IsShowing();
}

void TextView::InsertionPointCursorController::UpdatePosition(
    /* [in] */ HandleView* handle,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    Int32 previousOffset = mHost->GetSelectionStart();
    Int32 offset = mHost->GetHysteresisOffset(x, y, previousOffset);

    if (offset != previousOffset) {
        Selection::SetSelection(ISpannable::Probe(mHost->mText), offset);
        UpdatePosition();
    }
    HideDelayed(DELAY_BEFORE_FADE_OUT);
}

void TextView::InsertionPointCursorController::UpdatePosition()
{
    Int32 offset = mHost->GetSelectionStart();

    if (offset < 0) {
        // Should never happen, safety check.pos
        Logger::W(TEXT_VIEW_LOG_TAG, "Update cursor controller position called with no cursor");
        Hide();
        return;
    }

    mHandle->PositionAtCursor(offset, TRUE);
}

Boolean TextView::InsertionPointCursorController::OnTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    return FALSE;
}

ECode TextView::InsertionPointCursorController::OnTouchModeChanged(
    /* [in] */ Boolean isInTouchMode)
{
    if (!isInTouchMode) {
        Hide();
    }

    return NOERROR;
}

//@Override
void TextView::InsertionPointCursorController::OnDetached()
{
    mHost->RemoveCallbacks(mHider);
}

TextView::SelectionModifierCursorController::SelectionModifierCursorController(
    /* [in] */ TextView* host)
    : mIsShowing(FALSE)
    , mPreviousTapUpTime(0)
    , mHost(host)
{
    mStartHandle = new HandleView(this, HandleView::LEFT, host);
    mEndHandle = new HandleView(this, HandleView::RIGHT, host);
    ResetTouchOffsets();
}

void TextView::SelectionModifierCursorController::Show()
{
    if (mHost->IsInBatchEditMode()) {
        return;
    }

    mIsShowing = TRUE;
    UpdatePosition();
    mStartHandle->Show();
    mEndHandle->Show();
    mHost->HideInsertionPointCursorController();
}

void TextView::SelectionModifierCursorController::Hide()
{
    mStartHandle->Hide();
    mEndHandle->Hide();
    mIsShowing = FALSE;
}

Boolean TextView::SelectionModifierCursorController::IsShowing()
{
    return mIsShowing;
}

void TextView::SelectionModifierCursorController::UpdatePosition(
    /* [in] */ HandleView* handle,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    Int32 selectionStart = mHost->GetSelectionStart();
    Int32 selectionEnd = mHost->GetSelectionEnd();

    Int32 previousOffset = handle == mStartHandle ? selectionStart : selectionEnd;
    Int32 offset = mHost->GetHysteresisOffset(x, y, previousOffset);

    // Handle the case where start and end are swapped, making sure start <= end
    if (handle == mStartHandle) {
        if (selectionStart == offset || offset > selectionEnd) {
            return; // no change, no need to redraw;
        }
        // If the user "closes" the selection entirely they were probably trying to
        // select a single character. Help them out.
        if (offset == selectionEnd) {
            offset = selectionEnd - 1;
        }
        selectionStart = offset;
    }
    else {
        if (selectionEnd == offset || offset < selectionStart) {
            return; // no change, no need to redraw;
        }
        // If the user "closes" the selection entirely they were probably trying to
        // select a single character. Help them out.
        if (offset == selectionStart) {
            offset = selectionStart + 1;
        }
        selectionEnd = offset;
    }

    Selection::SetSelection(
        ISpannable::Probe(mHost->mText), selectionStart, selectionEnd);
    UpdatePosition();
}

void TextView::SelectionModifierCursorController::UpdatePosition()
{
    if (!IsShowing()) {
        return;
    }

    Int32 selectionStart = mHost->GetSelectionStart();
    Int32 selectionEnd = mHost->GetSelectionEnd();

    if ((selectionStart < 0) || (selectionEnd < 0)) {
        // Should never happen, safety check.
        Logger::W(TEXT_VIEW_LOG_TAG, "Update selection controller position called with no cursor");
        Hide();
        return;
    }

    mStartHandle->PositionAtCursor(selectionStart, TRUE);
    mEndHandle->PositionAtCursor(selectionEnd, TRUE);
}

Boolean TextView::SelectionModifierCursorController::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    // This is done even when the View does not have focus, so that Int64 presses can start
    // selection and tap can move cursor from this tap position.
    if (mHost->IsTextEditable()) {
        Int32 actionMasked;
        event->GetActionMasked(&actionMasked);
        switch (actionMasked) {
        case MotionEvent_ACTION_DOWN:
            {
                Float fx, fy;
                event->GetX(&fx);
                event->GetY(&fy);
                Int32 x = (Int32)fx;
                Int32 y = (Int32)fy;

                // Remember finger down position, to be able to start selection from there
                mMinTouchOffset = mMaxTouchOffset = mHost->GetOffset(x, y);

                // Double tap detection
                Int64 duration = SystemClock::GetUptimeMillis() - mPreviousTapUpTime;
                if (duration <= ViewConfiguration::GetDoubleTapTimeout()) {
                    Int32 deltaX = x - mPreviousTapPositionX;
                    Int32 deltaY = y - mPreviousTapPositionY;
                    Int32 distanceSquared = deltaX * deltaX + deltaY * deltaY;
                    Int32 doubleTapSlop = ViewConfiguration::Get(
                        mHost->GetContext())->GetScaledDoubleTapSlop();
                    Int32 slopSquared = doubleTapSlop * doubleTapSlop;
                    if (distanceSquared < slopSquared) {
                        mHost->StartTextSelectionMode();
                        // prevents onTapUpEvent from opening a context menu with cut/copy
                        mHost->mNoContextMenuOnUp = TRUE;
                    }
                }
                mPreviousTapPositionX = x;
                mPreviousTapPositionY = y;
            }
            break;
        case MotionEvent_ACTION_POINTER_DOWN:
        case MotionEvent_ACTION_POINTER_UP:
            // Handle multi-point gestures. Keep min and max offset positions.
            // Only activated for devices that correctly handle multi-touch.
            //if (mHost->mContext->GetCapsuleManager()->HasSystemFeature(
            //        CapsuleManager_FEATURE_TOUCHSCREEN_MULTITOUCH_DISTINCT)) {
                UpdateMinAndMaxOffsets(event);
            //}
            break;
        case MotionEvent_ACTION_UP:
            mPreviousTapUpTime = SystemClock::GetUptimeMillis();
            break;
        }
    }
    return FALSE;
}

/**
 * @param event
 */
void TextView::SelectionModifierCursorController::UpdateMinAndMaxOffsets(
    /* [in] */ IMotionEvent* event)
{
    Int32 pointerCount;
    event->GetPointerCount(&pointerCount);
    for (Int32 index = 0; index < pointerCount; index++) {
        Float fx, fy;
        event->GetX(&fx);
        event->GetY(&fy);
        Int32 x = (Int32)fx;
        Int32 y = (Int32)fy;

        Int32 offset = mHost->GetOffset(x, y);
        if (offset < mMinTouchOffset)
            mMinTouchOffset = offset;
        if (offset > mMaxTouchOffset)
            mMaxTouchOffset = offset;
    }
}

Int32 TextView::SelectionModifierCursorController::GetMinTouchOffset()
{
    return mMinTouchOffset;
}

Int32 TextView::SelectionModifierCursorController::GetMaxTouchOffset()
{
    return mMaxTouchOffset;
}

void TextView::SelectionModifierCursorController::ResetTouchOffsets()
{
    mMinTouchOffset = mMaxTouchOffset = -1;
}

/**
 * @return TRUE iff this controller is currently used to move the selection start.
 */
Boolean TextView::SelectionModifierCursorController::IsSelectionStartDragged()
{
    return mStartHandle->IsDragging();
}

ECode TextView::SelectionModifierCursorController::OnTouchModeChanged(
    /* [in] */ Boolean isInTouchMode)
{
    if (!isInTouchMode) {
        Hide();
    }

    return NOERROR;
}

//@Override
void TextView::SelectionModifierCursorController::OnDetached()
{}

const InterfaceID EIID_ChangeWatcher =
    {0x269872b1, 0x63a3, 0x475d, { 0x9b, 0x7, 0x80, 0xc1, 0xc0, 0x98, 0x98, 0x84 }};

TextView::ChangeWatcher::ChangeWatcher(
    /* [in] */ TextView* host)
    : mHost(host)
{}

PInterface TextView::ChangeWatcher::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ITextWatcher*)this;
    }
    else if (riid == EIID_ITextWatcher) {
        return (ITextWatcher*)this;
    }
    else if (riid == EIID_ISpanWatcher) {
        return (ISpanWatcher*)this;
    }
    else if (riid == EIID_INoCopySpan) {
        return (INoCopySpan*)(ISpanWatcher*)this;
    }
    else if (riid == EIID_ChangeWatcher) {
        return reinterpret_cast<PInterface>(this);
    }

    return NULL;
}

UInt32 TextView::ChangeWatcher::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TextView::ChangeWatcher::Release()
{
    return ElRefBase::Release();
}

ECode TextView::ChangeWatcher::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode TextView::ChangeWatcher::BeforeTextChanged(
    /* [in] */ ICharSequence* buffer,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    //if (DEBUG_EXTRACT) Log.v(LOG_TAG, "beforeTextChanged start=" + start
    //        + " before=" + before + " after=" + after + ": " + buffer);

    //if (AccessibilityManager.getInstance(mContext).isEnabled()
    //        && mHost->!IsPasswordInputType(mHost->mInputType)) {
    //    mBeforeText = buffer.toString();
    //}

    mHost->SendBeforeTextChanged(buffer, start, before, after);
    return NOERROR;
}

ECode TextView::ChangeWatcher::OnTextChanged(
    /* [in] */ ICharSequence* buffer,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    //if (DEBUG_EXTRACT) Log.v(LOG_TAG, "onTextChanged start=" + start
    //        + " before=" + before + " after=" + after + ": " + buffer);
    mHost->HandleTextChanged(buffer, start, before, after);

    //if (AccessibilityManager.getInstance(mContext).isEnabled() &&
    //    (mHost->IsFocused() || mHost->IsSelected() && mHost->IsShown())) {
    //    SendAccessibilityEventTypeViewTextChanged(mBeforeText, start, before, after);
    //    mBeforeText = NULL;
    //}

    return NOERROR;
}

ECode TextView::ChangeWatcher::AfterTextChanged(
    /* [in] */ IEditable* buffer)
{
    //if (DEBUG_EXTRACT) Log.v(LOG_TAG, "afterTextChanged: " + buffer);
    mHost->SendAfterTextChanged(buffer);

    if (MetaKeyKeyListener::GetMetaState(buffer,
        MetaKeyKeyListener::META_SELECTING) != 0) {
            MetaKeyKeyListener::StopSelecting(
                (IView*)mHost->Probe(EIID_IView), buffer);
    }

    return NOERROR;
}

ECode TextView::ChangeWatcher::OnSpanChanged(
    /* [in] */ ISpannable* buf,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 s,
    /* [in] */ Int32 e,
    /* [in] */ Int32 st,
    /* [in] */ Int32 en)
{
    //if (DEBUG_EXTRACT) Log.v(LOG_TAG, "onSpanChanged s=" + s + " e=" + e
    //        + " st=" + st + " en=" + en + " what=" + what + ": " + buf);
    return mHost->SpanChange(buf, what, s, st, e, en);
}

ECode TextView::ChangeWatcher::OnSpanAdded(
    /* [in] */ ISpannable* buf,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 s,
    /* [in] */ Int32 e)
{
    //if (DEBUG_EXTRACT) Log.v(LOG_TAG, "onSpanAdded s=" + s + " e=" + e
    //        + " what=" + what + ": " + buf);
    return mHost->SpanChange(buf, what, -1, s, -1, e);
}

ECode TextView::ChangeWatcher::OnSpanRemoved(
    /* [in] */ ISpannable* buf,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 s,
    /* [in] */ Int32 e)
{
    //if (DEBUG_EXTRACT) Log.v(LOG_TAG, "onSpanRemoved s=" + s + " e=" + e
    //        + " what=" + what + ": " + buf);
    return  mHost->SpanChange(buf, what, s, -1, e, -1);
}

TextView::Blink::Blink(
    /* [in] */ TextView* v)
    : mView(v)
{
    //mView = new WeakReference<TextView>(v);
    ASSERT_SUCCEEDED(CApartment::GetMainApartment((IApartment**)&mApartment));
}

ECode TextView::Blink::Run()
{
    if (mCancelled) {
        return NOERROR;
    }

    RemoveCallbacks(this);

    TextView* tv = mView;

    if (tv != NULL && tv->IsFocused()) {
        Int32 st = tv->GetSelectionStart();
        Int32 en = tv->GetSelectionEnd();

        if (st == en && st >= 0 && en >= 0) {
            if (tv->mLayout != NULL) {
                tv->InvalidateCursorPath();
            }

            PostAtTime(this, SystemClock::GetUptimeMillis() + BLINK);
        }
    }
    return NOERROR;
}

void TextView::Blink::Cancel()
{
    if (!mCancelled) {
        RemoveCallbacks(this);
        mCancelled = TRUE;
    }
}

void TextView::Blink::Uncancel()
{
    mCancelled = FALSE;
}

void TextView::Blink::PostAtTime(
    /* [in] */ IRunnable* action,
    /* [in] */ Int64 millis)
{
    ECode (STDCALL IRunnable::*pHandlerFunc)();
        pHandlerFunc = &IRunnable::Run;

    mApartment->PostCppCallbackAtTime(
        (Handle32)action, *(Handle32*)&pHandlerFunc, NULL, 0, millis);
}

void TextView::Blink::RemoveCallbacks(
    /* [in] */ IRunnable* action)
{
    mApartment->RemoveCppCallbacks(
            (Handle32)action, NULL);
}

TextView::MenuHandler::MenuHandler(
    /* [in] */ TextView* host)
    :mHost(host)
{
}

UInt32 TextView::MenuHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TextView::MenuHandler::Release()
{
    return ElRefBase::Release();
}

PInterface TextView::MenuHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IOnMenuItemClickListener) {
        return (PInterface)(IOnMenuItemClickListener*)this;
    }

    return NULL;
}

ECode TextView::MenuHandler::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);
    if (pObject == (IInterface*)(IOnMenuItemClickListener*)this) {
        *pIID = EIID_IOnMenuItemClickListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode TextView::MenuHandler::OnMenuItemClick(
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* isConsumed)
{
    assert(isConsumed != NULL);

    Int32 id = 0;
    item->GetItemId(&id);
    *isConsumed = mHost->OnTextContextMenuItem(id);
    return NOERROR;
}


AutoPtr<CRect> InitsCCTempRect()
{
    AutoPtr<CRect> r;
    CRect::NewByFriend((CRect**)&r);
    return r;
}

AutoPtr<CRectF> InitTempRect()
{
    AutoPtr<CRectF> r;
    CRectF::NewByFriend((CRectF**)&r);
    return r;
}

AutoPtr<IBoringLayoutMetrics> InitUNKNOWN_BORING()
{
    AutoPtr<CBoringLayoutMetrics> metris;
    CBoringLayoutMetrics::NewByFriend((CBoringLayoutMetrics**)&metris);
    return (IBoringLayoutMetrics*)metris.Get();
}

const char* TextView::TEXT_VIEW_LOG_TAG = "TextView";
const Boolean TextView::DEBUG_EXTRACT = FALSE;
const Int32 TextView::PRIORITY = 100;
const Int32 TextView::PREDRAW_NOT_REGISTERED = 0;
const Int32 TextView::PREDRAW_PENDING = 1;
const Int32 TextView::PREDRAW_DONE = 2;
const Int32 TextView::SANS = 1;
const Int32 TextView::SERIF = 2;
const Int32 TextView::MONOSPACE = 3;
const Int32 TextView::SIGNED = 2;
const Int32 TextView::DECIMAL = 4;
const Int32 TextView::ID_SELECT_ALL;
const Int32 TextView::ID_START_SELECTING_TEXT;
const Int32 TextView::ID_CUT;
const Int32 TextView::ID_COPY;
const Int32 TextView::ID_PASTE;
const Int32 TextView::ID_COPY_URL;
const Int32 TextView::ID_SWITCH_INPUT_METHOD;
const Int32 TextView::ID_ADD_TO_DICTIONARY;
AutoPtr<CRect> TextView::sCursorControllerTempRect = InitsCCTempRect();
const Int32 TextView::LINES = 1;
const Int32 TextView::EMS = LINES;
const Int32 TextView::PIXELS = 2;
const AutoPtr<CRectF> TextView::sTempRect = InitTempRect();
Mutex TextView::sTempRectLock;
const Int32 TextView::VERY_WIDE = 16384;
const Int32 TextView::BLINK = 500;
const Int32 TextView::ANIMATED_SCROLL_GAP = 250;
const Int32 TextView::EXTRACT_NOTHING = -2;
const Int32 TextView::EXTRACT_UNKNOWN = -1;
const AutoPtr<IBoringLayoutMetrics> TextView::UNKNOWN_BORING = InitUNKNOWN_BORING();

TextView::TextView()
    : mCurTextColor(0)
    , mFreezesText(FALSE)
    , mFrozenWithFocus(FALSE)
    , mTemporaryDetach(FALSE)
    , mDispatchTemporaryDetach(FALSE)
    , mEatTouchRelease(FALSE)
    , mScrolled(FALSE)
    , mPreDrawState(PREDRAW_NOT_REGISTERED)
    , mEllipsize(-1)
    , mErrorWasChanged(FALSE)
    , mShowErrorAfterAttach(FALSE)
    , mSelectionMoved(FALSE)
    , mTouchFocusSelected(FALSE)
    , mRestartMarquee(FALSE)
    , mMarqueeRepeatLimit(3)
    , mCreatedWithASelection(FALSE)
    , mNoContextMenuOnUp(FALSE)
    , mBufferType(BufferType_NORMAL)
    , mUserSetTextScaleX(FALSE)
    , mHighlightColor(0xCC475925)
    , mCursorVisible(TRUE)
    , mInsertionControllerEnabled(FALSE)
    , mSelectionControllerEnabled(FALSE)
    , mInBatchEditControllers(FALSE)
    , mIsInTextSelectionMode(FALSE)
    , mDPadCenterIsDown(FALSE)
    , mEnterKeyIsDown(FALSE)
    , mContextMenuTriggeredByKey(FALSE)
    , mSelectAllOnFocus(FALSE)
    , mGravity(Gravity_TOP | Gravity_LEFT)
    , mHorizontallyScrolling(FALSE)
    , mLinksClickable(TRUE)
    , mSpacingMult(1)
    , mSpacingAdd(0)
    , mMaximum(Math::INT32_MAX_VALUE)
    , mMaxMode(LINES)
    , mMinimum(0)
    , mMinMode(LINES)
    , mMaxWidth(Math::INT32_MAX_VALUE)
    , mMaxWidthMode(PIXELS)
    , mMinWidth(0)
    , mMinWidthMode(PIXELS)
    , mSingleLine(FALSE)
    , mDesiredHeightAtMeasure(-1)
    , mIncludePad(TRUE)
    , mHighlightPathBogus(TRUE)
    , mScroller(NULL)
{
    ASSERT_SUCCEEDED(CObjectContainer::New((IObjectContainer**)&mFilters));
}

TextView::TextView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : View(context, attrs, defStyle)
    , mCurTextColor(0)
    , mFreezesText(FALSE)
    , mFrozenWithFocus(FALSE)
    , mTemporaryDetach(FALSE)
    , mDispatchTemporaryDetach(FALSE)
    , mEatTouchRelease(FALSE)
    , mScrolled(FALSE)
    , mPreDrawState(PREDRAW_NOT_REGISTERED)
    , mEllipsize(-1)
    , mErrorWasChanged(FALSE)
    , mShowErrorAfterAttach(FALSE)
    , mSelectionMoved(FALSE)
    , mTouchFocusSelected(FALSE)
    , mRestartMarquee(FALSE)
    , mMarqueeRepeatLimit(3)
    , mCreatedWithASelection(FALSE)
    , mNoContextMenuOnUp(FALSE)
    , mBufferType(BufferType_NORMAL)
    , mUserSetTextScaleX(FALSE)
    , mHighlightColor(0xCC475925)
    , mCursorVisible(TRUE)
    , mInsertionControllerEnabled(FALSE)
    , mSelectionControllerEnabled(FALSE)
    , mInBatchEditControllers(FALSE)
    , mIsInTextSelectionMode(FALSE)
    , mDPadCenterIsDown(FALSE)
    , mEnterKeyIsDown(FALSE)
    , mContextMenuTriggeredByKey(FALSE)
    , mSelectAllOnFocus(FALSE)
    , mGravity(Gravity_TOP | Gravity_LEFT)
    , mHorizontallyScrolling(FALSE)
    , mLinksClickable(TRUE)
    , mSpacingMult(1)
    , mSpacingAdd(0)
    , mMaximum(Math::INT32_MAX_VALUE)
    , mMaxMode(LINES)
    , mMinimum(0)
    , mMinMode(LINES)
    , mMaxWidth(Math::INT32_MAX_VALUE)
    , mMaxWidthMode(PIXELS)
    , mMinWidth(0)
    , mMinWidthMode(PIXELS)
    , mSingleLine(FALSE)
    , mDesiredHeightAtMeasure(-1)
    , mIncludePad(TRUE)
    , mHighlightPathBogus(TRUE)
    , mScroller(NULL)
{
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
}

void TextView::SetTypefaceByIndex(
    /* [in] */ Int32 typefaceIndex,
    /* [in] */ Int32 styleIndex)
{
    ITypeface* tf = NULL;
    switch (typefaceIndex) {
        case SANS:
            tf = Typeface::SANS_SERIF;
            break;

        case SERIF:
            tf = Typeface::SERIF;
            break;

        case MONOSPACE:
            tf = Typeface::MONOSPACE;
            break;
    }

    SetTypeface(tf, styleIndex);
}


/**
  * Sets the typeface and style in which the text should be displayed,
  * and turns on the fake bold and italic bits in the Paint if the
  * Typeface that you provided does not have all the bits in the
  * style that you specified.
  *
  * @attr ref android.R.styleable#TextView_typeface
  * @attr ref android.R.styleable#TextView_textStyle
  */
ECode TextView::SetTypeface(
    /* [in] */ ITypeface* tf,
    /* [in] */ Int32 style)
{
    if (style > 0) {
        if (tf == NULL) {
            Typeface::DefaultFromStyle(style, &tf);
        }
        else {
            Typeface::Create(tf, style, &tf);
        }

        SetTypeface(tf);
        // now compute what (if any) algorithmic styling is needed
        Int32 typefaceStyle = 0;
        if (tf != NULL) {
            tf->GetStyle(&typefaceStyle);
        }
        Int32 need = style & ~typefaceStyle;
        mTextPaint->SetFakeBoldText((need & Typeface_BOLD) != 0);
        mTextPaint->SetTextSkewX((need & Typeface_ITALIC) != 0 ? -0.25f : 0);
    }
    else {
        mTextPaint->SetFakeBoldText(FALSE);
        mTextPaint->SetTextSkewX(0);
        SetTypeface(tf);
    }
    return NOERROR;
}

/**
 * Subclasses override this to specify that they have a KeyListener
 * by default even if not specifically called for in the XML options.
 */
Boolean TextView::GetDefaultEditable()
{
    return FALSE;
}

/**
 * Subclasses override this to specify a default movement method.
 */
AutoPtr<IMovementMethod> TextView::GetDefaultMovementMethod()
{
    return NULL;
}

/**
 * Return the text the TextView is displaying. If setText() was called with
 * an argument of BufferType.SPANNABLE or BufferType.EDITABLE, you can cast
 * the return value from this method to Spannable or Editable, respectively.
 *
 * Note: The content of the return value should not be modified. If you want
 * a modifiable one, you should make your own copy first.
 */
AutoPtr<ICharSequence> TextView::GetText()
{
    return mText;
}

/**
 * Returns the length, in characters, of the text managed by this TextView
 */
Int32 TextView::GetLength()
{
    Int32 len;
    mText->GetLength(&len);
    return len;
}

/**
 * Return the text the TextView is displaying as an Editable object.  If
 * the text is not editable, NULL is returned.
 *
 * @see #getText
 */
AutoPtr<IEditable> TextView::GetEditableText()
{
    return AutoPtr<IEditable>(IEditable::Probe(mText));
}

/**
 * @return the height of one standard line in pixels.  Note that markup
 * within the text can cause individual lines to be taller or shorter
 * than this height, and the layout may contain additional first-
 * or last-line padding.
 */
Int32 TextView::GetLineHeight()
{
    Int32 spacing;
    mTextPaint->GetFontMetricsInt(NULL, &spacing);
    return Math::Round(spacing * mSpacingMult + mSpacingAdd);
}

/**
 * @return the Layout that is currently being used to display the text.
 * This can be NULL if the text or width has recently changes.
 */
AutoPtr<ILayout> TextView::GetLayout()
{
    return mLayout;
}

/**
 * @return the current key listener for this TextView.
 * This will frequently be NULL for non-EditText TextViews.
 */
AutoPtr<IKeyListener> TextView::GetKeyListener()
{
    return mInput;
}

/**
 * Sets the key listener to be used with this TextView.  This can be NULL
 * to disallow user input.  Note that this method has significant and
 * subtle interactions with soft keyboards and other input method:
 * see {@link KeyListener#getInputType() KeyListener.getContentType()}
 * for important details.  Calling this method will replace the current
 * content type of the text view with the content type returned by the
 * key listener.
 * <p>
 * Be warned that if you want a TextView with a key listener or movement
 * method not to be focusable, or if you want a TextView without a
 * key listener or movement method to be focusable, you must call
 * {@link #setFocusable} again after calling this to get the focusability
 * back the way you want it.
 *
 * @attr ref android.R.styleable#TextView_numeric
 * @attr ref android.R.styleable#TextView_digits
 * @attr ref android.R.styleable#TextView_phoneNumber
 * @attr ref android.R.styleable#TextView_inputMethod
 * @attr ref android.R.styleable#TextView_capitalize
 * @attr ref android.R.styleable#TextView_autoText
 */
ECode TextView::SetKeyListener(
    /* [in] */ IKeyListener* input)
{
    SetKeyListenerOnly(input);
    FixFocusableAndClickableSettings();

     if (input != NULL) {
         ECode ec = mInput->GetInputType(&mInputType);
         if (FAILED(ec)) {
             mInputType = InputType_TYPE_CLASS_TEXT;
         }
         if ((mInputType&InputType_TYPE_MASK_CLASS)
             == InputType_TYPE_CLASS_TEXT) {
             if (mSingleLine) {
                 mInputType &= ~InputType_TYPE_TEXT_FLAG_MULTI_LINE;
             }
             else {
                 mInputType |= InputType_TYPE_TEXT_FLAG_MULTI_LINE;
             }
         }
     }
     else {
         mInputType = InputType_TYPE_NULL;
     }

    AutoPtr<ILocalInputMethodManager> imm = CLocalInputMethodManager::PeekInstance();
    if (imm != NULL) {
        imm->RestartInput((IView*)this->Probe(EIID_IView));
    }

    return NOERROR;
}

void TextView::SetKeyListenerOnly(
    /* [in] */ IKeyListener* input)
{
    mInput = input;
    if (mInput != NULL && !(IEditable::Probe(mText)))
        SetText(mText);

    SetFilters(IEditable::Probe(mText), mFilters);
}

/**
 * @return the movement method being used for this TextView.
 * This will frequently be NULL for non-EditText TextViews.
 */
AutoPtr<IMovementMethod> TextView::GetMovementMethod()
{
    return mMovement;
}

/**
 * Sets the movement method (arrow key handler) to be used for
 * this TextView.  This can be NULL to disallow using the arrow keys
 * to move the cursor or scroll the view.
 * <p>
 * Be warned that if you want a TextView with a key listener or movement
 * method not to be focusable, or if you want a TextView without a
 * key listener or movement method to be focusable, you must call
 * {@link #setFocusable} again after calling this to get the focusability
 * back the way you want it.
 */
ECode TextView::SetMovementMethod(
    /* [in] */ IMovementMethod* movement)
{
    mMovement = movement;

    if (mMovement != NULL && !(ISpannable::Probe(mText)))
        SetText(mText);

    FixFocusableAndClickableSettings();

    // SelectionModifierCursorController depends on textCanBeSelected, which depends on mMovement
    PrepareCursorControllers();

    return NOERROR;
}

void TextView::FixFocusableAndClickableSettings()
{
    if ((mMovement != NULL) || mInput != NULL) {
        SetFocusable(TRUE);
        SetClickable(TRUE);
        SetLongClickable(TRUE);
    }
    else {
        SetFocusable(FALSE);
        SetClickable(FALSE);
        SetLongClickable(FALSE);
    }
}

/**
 * @return the current transformation method for this TextView.
 * This will frequently be NULL except for single-line and password
 * fields.
 */
//public final TransformationMethod getTransformationMethod() {
//    return mTransformation;
//}

/**
 * Sets the transformation that is applied to the text that this
 * TextView is displaying.
 *
 * @attr ref android.R.styleable#TextView_password
 * @attr ref android.R.styleable#TextView_singleLine
 */
ECode TextView::SetTransformationMethod(
    /* [in] */ ITransformationMethod* method)
{
    if (method == mTransformation.Get()) {
        // Avoid the setText() below if the transformation is
        // the same.
        return NOERROR;
    }

    if (mTransformation != NULL) {
        if (ISpannable::Probe(mText.Get())) {
            ISpannable::Probe(mText.Get())->RemoveSpan(mTransformation.Get());
        }
    }

    mTransformation = method;

    return SetText(mText);
}

/**
 * Returns the top padding of the view, plus space for the top
 * IDrawable* if any.
 */
Int32 TextView::GetCompoundPaddingTop()
{
    Drawables* dr = mDrawables;
    if (dr == NULL || dr->mDrawableTop == NULL) {
        return mPaddingTop;
    }
    else {
        return mPaddingTop + dr->mDrawablePadding + dr->mDrawableSizeTop;
    }
}

/**
 * Returns the bottom padding of the view, plus space for the bottom
 * IDrawable* if any.
 */
Int32 TextView::GetCompoundPaddingBottom()
{
    Drawables* dr = mDrawables;
    if (dr == NULL || dr->mDrawableBottom == NULL) {
        return mPaddingBottom;
    }
    else {
        return mPaddingBottom + dr->mDrawablePadding + dr->mDrawableSizeBottom;
    }
}

/**
 * Returns the left padding of the view, plus space for the left
 * IDrawable* if any.
 */
Int32 TextView::GetCompoundPaddingLeft()
{
    Drawables* dr = mDrawables;
    if (dr == NULL || dr->mDrawableLeft == NULL) {
        return mPaddingLeft;
    }
    else {
        return mPaddingLeft + dr->mDrawablePadding + dr->mDrawableSizeLeft;
    }
}

/**
 * Returns the right padding of the view, plus space for the right
 * IDrawable* if any.
 */
Int32 TextView::GetCompoundPaddingRight()
{
    Drawables* dr = mDrawables;
    if (dr == NULL || dr->mDrawableRight == NULL) {
        return mPaddingRight;
    }
    else {
        return mPaddingRight + dr->mDrawablePadding + dr->mDrawableSizeRight;
    }
}

/**
 * Returns the extended top padding of the view, including both the
 * top IDrawable* if any and any extra space to keep more than maxLines
 * of text from showing.  It is only valid to call this after measuring.
 */
Int32 TextView::GetExtendedPaddingTop()
{
    if (mMaxMode != LINES) {
        return GetCompoundPaddingTop();
    }

    Int32 number;
    mLayout->GetLineCount(&number);
    if (number <= mMaximum) {
        return GetCompoundPaddingTop();
    }

    Int32 top = GetCompoundPaddingTop();
    Int32 bottom = GetCompoundPaddingBottom();
    Int32 viewht = GetHeight() - top - bottom;
    Int32 layoutht;
    mLayout->GetLineTop(mMaximum, &layoutht);

    if (layoutht >= viewht) {
        return top;
    }

    Int32 gravity = mGravity & Gravity_VERTICAL_GRAVITY_MASK;
    if (gravity == Gravity_TOP) {
        return top;
    }
    else if (gravity == Gravity_BOTTOM) {
        return top + viewht - layoutht;
    }
    else { // (gravity == Gravity_CENTER_VERTICAL)
        return top + (viewht - layoutht) / 2;
    }
}

/**
 * Returns the extended bottom padding of the view, including both the
 * bottom IDrawable* if any and any extra space to keep more than maxLines
 * of text from showing.  It is only valid to call this after measuring.
 */
Int32 TextView::GetExtendedPaddingBottom()
{
    if (mMaxMode != LINES) {
        return GetCompoundPaddingBottom();
    }

    Int32 number;
    mLayout->GetLineCount(&number);
    if (number <= mMaximum) {
        return GetCompoundPaddingBottom();
    }

    Int32 top = GetCompoundPaddingTop();
    Int32 bottom = GetCompoundPaddingBottom();
    Int32 viewht = GetHeight() - top - bottom;
    Int32 layoutht;
    mLayout->GetLineTop(mMaximum, &layoutht);

    if (layoutht >= viewht) {
        return bottom;
    }

    Int32 gravity = mGravity & Gravity_VERTICAL_GRAVITY_MASK;
    if (gravity == Gravity_TOP) {
        return bottom + viewht - layoutht;
    }
    else if (gravity == Gravity_BOTTOM) {
        return bottom;
    }
    else { // (gravity == Gravity_CENTER_VERTICAL)
        return bottom + (viewht - layoutht) / 2;
    }
}

/**
 * Returns the total left padding of the view, including the left
 * IDrawable* if any.
 */
Int32 TextView::GetTotalPaddingLeft()
{
    return GetCompoundPaddingLeft();
}

/**
 * Returns the total right padding of the view, including the right
 * IDrawable* if any.
 */
Int32 TextView::GetTotalPaddingRight()
{
    return GetCompoundPaddingRight();
}

/**
 * Returns the total top padding of the view, including the top
 * IDrawable* if any, the extra space to keep more than maxLines
 * from showing, and the vertical offset for gravity, if any.
 */
Int32 TextView::GetTotalPaddingTop()
{
    return GetExtendedPaddingTop() + GetVerticalOffset(TRUE);
}

/**
 * Returns the total bottom padding of the view, including the bottom
 * IDrawable* if any, the extra space to keep more than maxLines
 * from showing, and the vertical offset for gravity, if any.
 */
Int32 TextView::GetTotalPaddingBottom()
{
    return GetExtendedPaddingBottom() + GetBottomVerticalOffset(TRUE);
}

/**
 * Sets the Drawables (if any) to appear to the left of, above,
 * to the right of, and below the text.  Use NULL if you do not
 * want a IDrawable* there.  The Drawables must already have had
 * {@link IDrawable*#setBounds} called.
 *
 * @attr ref android.R.styleable#TextView_drawableLeft
 * @attr ref android.R.styleable#TextView_drawableTop
 * @attr ref android.R.styleable#TextView_drawableRight
 * @attr ref android.R.styleable#TextView_drawableBottom
 */
ECode TextView::SetCompoundDrawables(
    /* [in] */ IDrawable* left,
    /* [in] */ IDrawable* top,
    /* [in] */ IDrawable* right,
    /* [in] */ IDrawable* bottom)
{
    Drawables* dr = mDrawables;

    Boolean drawables = left != NULL || top != NULL
            || right != NULL || bottom != NULL;

    if (!drawables) {
        // Clearing drawables...  can we free the data structure?
        if (dr != NULL) {
            if (dr->mDrawablePadding == 0) {
                delete mDrawables;
                mDrawables = NULL;
            }
            else {
                // We need to retain the last set padding, so just clear
                // out all of the fields in the existing structure.
                if (dr->mDrawableLeft != NULL) dr->mDrawableLeft->SetCallback(NULL);
                dr->mDrawableLeft = NULL;
                if (dr->mDrawableTop != NULL) dr->mDrawableTop->SetCallback(NULL);
                dr->mDrawableTop = NULL;
                if (dr->mDrawableRight != NULL) dr->mDrawableRight->SetCallback(NULL);
                dr->mDrawableRight = NULL;
                if (dr->mDrawableBottom != NULL) dr->mDrawableBottom->SetCallback(NULL);
                dr->mDrawableBottom = NULL;
                dr->mDrawableSizeLeft = dr->mDrawableHeightLeft = 0;
                dr->mDrawableSizeRight = dr->mDrawableHeightRight = 0;
                dr->mDrawableSizeTop = dr->mDrawableWidthTop = 0;
                dr->mDrawableSizeBottom = dr->mDrawableWidthBottom = 0;
            }
        }
    }
    else {
        if (dr == NULL) {
            mDrawables = dr = new Drawables();
        }

        if (dr->mDrawableLeft.Get() != left && dr->mDrawableLeft != NULL) {
            dr->mDrawableLeft->SetCallback(NULL);
        }
        dr->mDrawableLeft = left;

        if (dr->mDrawableTop.Get() != top && dr->mDrawableTop != NULL) {
            dr->mDrawableTop->SetCallback(NULL);
        }
        dr->mDrawableTop = top;

        if (dr->mDrawableRight.Get() != right && dr->mDrawableRight != NULL) {
            dr->mDrawableRight->SetCallback(NULL);
        }
        dr->mDrawableRight = right;

        if (dr->mDrawableBottom.Get() != bottom && dr->mDrawableBottom != NULL) {
            dr->mDrawableBottom->SetCallback(NULL);
        }
        dr->mDrawableBottom = bottom;

        CRect* compoundRect = dr->mCompoundRect;

        ArrayOf<Int32>* state = GetDrawableState();
        Boolean temp;

        if (left != NULL) {
            left->SetState(state, &temp);
            left->CopyBounds(compoundRect);
            left->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
            dr->mDrawableSizeLeft = compoundRect->GetWidth();
            dr->mDrawableHeightLeft = compoundRect->GetHeight();
        } else {
            dr->mDrawableSizeLeft = dr->mDrawableHeightLeft = 0;
        }

        if (right != NULL) {
            right->SetState(state, &temp);
            right->CopyBounds(compoundRect);
            right->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
            dr->mDrawableSizeRight = compoundRect->GetWidth();
            dr->mDrawableHeightRight = compoundRect->GetHeight();
        } else {
            dr->mDrawableSizeRight = dr->mDrawableHeightRight = 0;
        }

        if (top != NULL) {
            top->SetState(state, &temp);
            top->CopyBounds(compoundRect);
            top->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
            dr->mDrawableSizeTop = compoundRect->GetHeight();
            dr->mDrawableWidthTop = compoundRect->GetWidth();
        } else {
            dr->mDrawableSizeTop = dr->mDrawableWidthTop = 0;
        }

        if (bottom != NULL) {
            bottom->SetState(state, &temp);
            bottom->CopyBounds(compoundRect);
            bottom->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
            dr->mDrawableSizeBottom = compoundRect->GetHeight();
            dr->mDrawableWidthBottom = compoundRect->GetWidth();
        } else {
            dr->mDrawableSizeBottom = dr->mDrawableWidthBottom = 0;
        }
    }

    Invalidate();
    RequestLayout();
    return NOERROR;
}

/**
 * Sets the Drawables (if any) to appear to the left of, above,
 * to the right of, and below the text.  Use 0 if you do not
 * want a IDrawable* there. The Drawables' bounds will be set to
 * their intrinsic bounds.
 *
 * @param left Resource identifier of the left IDrawable*.
 * @param top Resource identifier of the top IDrawable*.
 * @param right Resource identifier of the right IDrawable*.
 * @param bottom Resource identifier of the bottom IDrawable*.
 *
 * @attr ref android.R.styleable#TextView_drawableLeft
 * @attr ref android.R.styleable#TextView_drawableTop
 * @attr ref android.R.styleable#TextView_drawableRight
 * @attr ref android.R.styleable#TextView_drawableBottom
 */
ECode TextView::SetCompoundDrawablesWithIntrinsicBounds(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    AutoPtr<IResources> resources;
    GetContext()->GetResources((IResources**)&resources);

    AutoPtr<IDrawable> leftDrawable;
    resources->GetDrawable(left, (IDrawable**)&leftDrawable);

    AutoPtr<IDrawable> topDrawable;
    resources->GetDrawable(top, (IDrawable**)&topDrawable);

    AutoPtr<IDrawable> rightDrawable;
    resources->GetDrawable(right, (IDrawable**)&rightDrawable);

    AutoPtr<IDrawable> bottomDrawable;
    resources->GetDrawable(bottom, (IDrawable**)&bottomDrawable);

    SetCompoundDrawablesWithIntrinsicBounds(left != 0 ? leftDrawable : NULL,
            top != 0 ? topDrawable : NULL,
            right != 0 ? rightDrawable : NULL,
            bottom != 0 ? bottomDrawable : NULL);
    return NOERROR;
}

/**
 * Sets the Drawables (if any) to appear to the left of, above,
 * to the right of, and below the text.  Use NULL if you do not
 * want a IDrawable* there. The Drawables' bounds will be set to
 * their intrinsic bounds.
 *
 * @attr ref android.R.styleable#TextView_drawableLeft
 * @attr ref android.R.styleable#TextView_drawableTop
 * @attr ref android.R.styleable#TextView_drawableRight
 * @attr ref android.R.styleable#TextView_drawableBottom
 */
ECode TextView::SetCompoundDrawablesWithIntrinsicBounds(
    /* [in] */ IDrawable* left,
    /* [in] */ IDrawable* top,
    /* [in] */ IDrawable* right,
    /* [in] */ IDrawable* bottom)
{
    Int32 width, height;
    if (left != NULL) {
        left->GetIntrinsicWidth(&width);
        left->GetIntrinsicHeight(&height);
        left->SetBounds(0, 0, width, height);
    }
    if (right != NULL) {
        right->GetIntrinsicWidth(&width);
        right->GetIntrinsicHeight(&height);
        right->SetBounds(0, 0, width, height);
    }
    if (top != NULL) {
        top->GetIntrinsicWidth(&width);
        top->GetIntrinsicHeight(&height);
        top->SetBounds(0, 0, width, height);
    }
    if (bottom != NULL) {
        bottom->GetIntrinsicWidth(&width);
        bottom->GetIntrinsicHeight(&height);
        bottom->SetBounds(0, 0, width, height);
    }
    SetCompoundDrawables(left, top, right, bottom);
    return NOERROR;
}

/**
 * Returns drawables for the left, top, right, and bottom borders.
 */
ECode TextView::GetCompoundDrawables(
    /* [out, callee] */ ArrayOf<IDrawable*>** _drawables)
{
    assert(_drawables != NULL);
    ArrayOf<IDrawable*>* drawables = ArrayOf<IDrawable*>::Alloc(4);

    Drawables* dr = mDrawables;
    if (dr != NULL) {
        (*drawables)[0] = dr->mDrawableLeft;
        if ((*drawables)[0] != NULL) ((*drawables)[0])->AddRef();
        (*drawables)[1] = dr->mDrawableTop;
        if ((*drawables)[1] != NULL) ((*drawables)[1])->AddRef();
        (*drawables)[2] = dr->mDrawableRight;
        if ((*drawables)[2] != NULL) ((*drawables)[2])->AddRef();
        (*drawables)[3] = dr->mDrawableBottom;
        if ((*drawables)[3] != NULL) ((*drawables)[3])->AddRef();
    } else {
        (*drawables)[0] = (*drawables)[1] = (*drawables)[2] = (*drawables)[3] = NULL;
    }

    *_drawables = drawables;
    return NOERROR;
}

/**
 * Sets the size of the padding between the compound drawables and
 * the text.
 *
 * @attr ref android.R.styleable#TextView_drawablePadding
 */
ECode TextView::SetCompoundDrawablePadding(
    /* [in] */ Int32 pad)
{
    Drawables* dr = mDrawables;
    if (pad == 0) {
        if (dr != NULL) {
            dr->mDrawablePadding = pad;
        }
    } else {
        if (dr == NULL) {
            mDrawables = dr = new Drawables();
        }
        dr->mDrawablePadding = pad;
    }

    Invalidate();
    RequestLayout();
    return NOERROR;
}

/**
 * Returns the padding between the compound drawables and the text.
 */
Int32 TextView::GetCompoundDrawablePadding()
{
    Drawables* dr = mDrawables;
    return dr != NULL ? dr->mDrawablePadding : 0;
}

ECode TextView::SetPadding(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    if (left != mPaddingLeft ||
        right != mPaddingRight ||
        top != mPaddingTop ||
        bottom != mPaddingBottom) {
        NullLayouts();
    }

    // the super call will requestLayout()
    View::SetPadding(left, top, right, bottom);
    Invalidate();
    return NOERROR;
}

/**
 * Gets the autolink mask of the text.  See {@link
 * android.text.util.Linkify#ALL Linkify.ALL} and peers for
 * possible values.
 *
 * @attr ref android.R.styleable#TextView_autoLink
 */
Int32 TextView::GetAutoLinkMask()
{
    return mAutoLinkMask;
}

static Int32 R_Styleable_TextAppearance[] = {
    0x01010095, 0x01010096, 0x01010097, 0x01010098,
    0x01010099, 0x0101009a, 0x0101009b
};

/**
 * Sets the text color, size, style, hint color, and highlight color
 * from the specified TextAppearance resource.
 */
ECode TextView::SetTextAppearance(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resid)
{
    AutoPtr<ITypedArray> appearance;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributesEx(resid,
            ArrayOf<Int32>(R_Styleable_TextAppearance, sizeof(R_Styleable_TextAppearance) / sizeof(Int32)),/*com.android.internal.R.styleable.TextAppearance*/
            (ITypedArray**)&appearance));

    Int32 color;
    AutoPtr<IColorStateList> colors;
    Int32 ts;

    appearance->GetColor(4/*com.android.internal.R.styleable.TextAppearance_textColorHighlight*/,
            0, &color);
    if (color != 0) {
        SetHighlightColor(color);
    }

    appearance->GetColorStateList(3/*com.android.internal.R.styleable.TextAppearance_textColor*/,
            (IColorStateList**)&colors);
    if (colors != NULL) {
        SetTextColor(colors);
    }

    appearance->GetDimensionPixelSize(0/*com.android.internal.R.styleable.TextAppearance_textSize*/,
            0, &ts);
    if (ts != 0) {
        SetRawTextSize(ts);
    }

    colors = NULL;
    appearance->GetColorStateList(5/*com.android.internal.R.styleable.TextAppearance_textColorHint*/,
            (IColorStateList**)&colors);
    if (colors != NULL) {
        SetHintTextColor(colors);
    }

    colors = NULL;
    appearance->GetColorStateList(6/*com.android.internal.R.styleable.TextAppearance_textColorLink*/,
            (IColorStateList**)&colors);
    if (colors != NULL) {
        SetLinkTextColor(colors);
    }

    Int32 typefaceIndex, styleIndex;

    appearance->GetInt32(1/*com.android.internal.R.styleable.TextAppearance_typeface*/,
            -1, &typefaceIndex);
    appearance->GetInt32(2/*com.android.internal.R.styleable.TextAppearance_textStyle*/,
            -1, &styleIndex);

    SetTypefaceByIndex(typefaceIndex, styleIndex);
    appearance->Recycle();
    return NOERROR;
}

/**
 * @return the size (in pixels) of the default text size in this TextView.
 */
Float TextView::GetTextSize()
{
    Float size;
    mTextPaint->GetTextSize(&size);
    return size;
}

/**
 * Set the default text size to the given value, interpreted as "scaled
 * pixel" units.  This size is adjusted based on the current density and
 * user font size preference.
 *
 * @param size The scaled pixel size.
 *
 * @attr ref android.R.styleable#TextView_textSize
 */
ECode TextView::SetTextSize(
    /* [in] */ Float size)
{
    return SetTextSize(TypedValue_COMPLEX_UNIT_SP, size);
}

/**
 * Set the default text size to a given unit and value.  See {@link
 * TypedValue} for the possible dimension units.
 *
 * @param unit The desired dimension unit.
 * @param size The desired size in the given units.
 *
 * @attr ref android.R.styleable#TextView_textSize
 */
ECode TextView::SetTextSize(
    /* [in] */ Int32 unit,
    /* [in] */ Float size)
{
    AutoPtr<IContext> c = GetContext();
    AutoPtr<IResources> r;

    if (c == NULL) {
        AutoPtr<IResourcesFactory> rf;
        CResourcesFactory::AcquireSingleton((IResourcesFactory**)&rf);
        rf->GetSystem((IResources**)&r);
    }
    else {
        c->GetResources((IResources**)&r);
    }

    AutoPtr<IDisplayMetrics> metrics;
    r->GetDisplayMetrics((IDisplayMetrics**)&metrics);

    SetRawTextSize(CTypedValue::ApplyDimension(unit, size, metrics));
    return NOERROR;
}

void TextView::SetRawTextSize(
    /* [in] */ Float size)
{
    Float textSize;
    mTextPaint->GetTextSize(&textSize);
    if (size != textSize) {
        mTextPaint->SetTextSize(size);

        if (mLayout != NULL) {
            NullLayouts();
            RequestLayout();
            Invalidate();
        }
    }
}

/**
 * @return the extent by which text is currently being stretched
 * horizontally.  This will usually be 1.
 */
Float TextView::GetTextScaleX()
{
    Float scaleX;
    mTextPaint->GetTextScaleX(&scaleX);
    return scaleX;
}

/**
 * Sets the extent by which text should be stretched horizontally.
 *
 * @attr ref android.R.styleable#TextView_textScaleX
 */
ECode TextView::SetTextScaleX(
    /* [in] */ Float size)
{
    Float scaleX;
    mTextPaint->GetTextScaleX(&scaleX);
    if (size != scaleX) {
        mUserSetTextScaleX = TRUE;
        mTextPaint->SetTextScaleX(size);

        if (mLayout != NULL) {
            NullLayouts();
            RequestLayout();
            Invalidate();
        }
    }
    return NOERROR;
}

/**
 * Sets the typeface and style in which the text should be displayed.
 * Note that not all Typeface families actually have bold and italic
 * variants, so you may need to use
 * {@link #setTypeface(Typeface, Int32)} to get the appearance
 * that you actually want.
 *
 * @attr ref android.R.styleable#TextView_typeface
 * @attr ref android.R.styleable#TextView_textStyle
 */
ECode TextView::SetTypeface(
    /* [in] */ ITypeface* tf)
{
    AutoPtr<ITypeface> face;
    mTextPaint->GetTypeface((ITypeface**)&face);
    if (face.Get() != tf) {
        mTextPaint->SetTypeface(tf);

        if (mLayout != NULL) {
            NullLayouts();
            RequestLayout();
            Invalidate();
        }
    }
    return NOERROR;
}

/**
 * @return the current typeface and style in which the text is being
 * displayed.
 */
ECode TextView::GetTypeface(
    /* [out] */ ITypeface** tf)
{
    assert(tf != NULL);

    return mTextPaint->GetTypeface(tf);
}

/**
 * Sets the text color for all the states (normal, selected,
 * focused) to be this color.
 *
 * @attr ref android.R.styleable#TextView_textColor
 */
ECode TextView::SetTextColor(
    /* [in] */ Int32 color)
{
    CColorStateList::ValueOf(color, (IColorStateList**)&mTextColor);
    UpdateTextColors();
    return NOERROR;
}

/**
 * Sets the text color.
 *
 * @attr ref android.R.styleable#TextView_textColor
 */
ECode TextView::SetTextColor(
    /* [in] */ IColorStateList* colors)
{
    if (colors == NULL) {
        //throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

    mTextColor = colors;
    UpdateTextColors();
    return NOERROR;
}

/**
 * Return the set of text colors.
 *
 * @return Returns the set of text colors.
 */
ECode TextView::GetTextColors(
    /* [out] */ IColorStateList** colors)
{
    assert(colors != NULL);
    *colors = mTextColor;
    if (*colors != NULL) (*colors)->AddRef();

    return NOERROR;
}

/**
 * <p>Return the current color selected for normal text.</p>
 *
 * @return Returns the current text color.
 */
Int32 TextView::GetCurrentTextColor()
{
    return mCurTextColor;
}

/**
 * Sets the color used to display the selection highlight.
 *
 * @attr ref android.R.styleable#TextView_textColorHighlight
 */
ECode TextView::SetHighlightColor(
    /* [in] */ Int32 color)
{
    if (mHighlightColor != color) {
        mHighlightColor = color;
        Invalidate();
    }
    return NOERROR;
}

/**
 * Gives the text a shadow of the specified radius and color, the specified
 * distance from its normal position.
 *
 * @attr ref android.R.styleable#TextView_shadowColor
 * @attr ref android.R.styleable#TextView_shadowDx
 * @attr ref android.R.styleable#TextView_shadowDy
 * @attr ref android.R.styleable#TextView_shadowRadius
 */
ECode TextView::SetShadowLayer(
    /* [in] */ Float radius,
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [in] */ Int32 color)
{
    mTextPaint->SetShadowLayer(radius, dx, dy, color);

    mShadowRadius = radius;
    mShadowDx = dx;
    mShadowDy = dy;

    Invalidate();
    return NOERROR;
}

/**
 * @return the base paint used for the text.  Please use this only to
 * consult the Paint's properties and not to change them.
 */
AutoPtr<ITextPaint> TextView::GetPaint()
{
    return mTextPaint;
}

/**
 * Sets the autolink mask of the text.  See {@link
 * android.text.util.Linkify#ALL Linkify.ALL} and peers for
 * possible values.
 *
 * @attr ref android.R.styleable#TextView_autoLink
 */
ECode TextView::SetAutoLinkMask(
    /* [in] */ Int32 mask)
{
    mAutoLinkMask = mask;
    return NOERROR;
}

/**
 * Sets whether the movement method will automatically be set to
 * {@link LinkMovementMethod} if {@link #setAutoLinkMask} has been
 * set to nonzero and links are detected in {@link #setText}.
 * The default is TRUE.
 *
 * @attr ref android.R.styleable#TextView_linksClickable
 */
ECode TextView::SetLinksClickable(
    /* [in] */ Boolean whether)
{
    mLinksClickable = whether;
    return NOERROR;
}

/**
 * Returns whether the movement method will automatically be set to
 * {@link LinkMovementMethod} if {@link #setAutoLinkMask} has been
 * set to nonzero and links are detected in {@link #setText}.
 * The default is TRUE.
 *
 * @attr ref android.R.styleable#TextView_linksClickable
 */
Boolean TextView::GetLinksClickable()
{
    return mLinksClickable;
}

/**
 * Returns the list of URLSpans attached to the text
 * (by {@link Linkify} or otherwise) if any.  You can call
 * {@link URLSpan#getURL} on them to find where they link to
 * or use {@link Spanned#getSpanStart} and {@link Spanned#getSpanEnd}
 * to find the region of the text they are attached to.
 */
//URLSpan[] getUrls() {
//    if (mText instanceof Spanned) {
//        return ((Spanned) mText).getSpans(0, mText.length(), URLSpan.class);
//    } else {
//        return new URLSpan[0];
//    }
//}

/**
 * Sets the color of the hint text.
 *
 * @attr ref android.R.styleable#TextView_textColorHint
 */
ECode TextView::SetHintTextColor(
    /* [in] */ Int32 color)
{
    CColorStateList::ValueOf(color, (IColorStateList**)&mHintTextColor);
    UpdateTextColors();
    return NOERROR;
}

/**
 * Sets the color of the hint text.
 *
 * @attr ref android.R.styleable#TextView_textColorHint
 */
ECode TextView::SetHintTextColor(
    /* [in] */ IColorStateList* colors)
{
    mHintTextColor = colors;
    UpdateTextColors();
    return NOERROR;
}

/**
 * <p>Return the color used to paint the hint text.</p>
 *
 * @return Returns the list of hint text colors.
 */
ECode TextView::GetHintTextColors(
    /* [out] */ IColorStateList** color)
{
    assert(color != NULL);
    *color = mHintTextColor;
    if (*color != NULL) (*color)->AddRef();
    return NOERROR;
}

/**
 * <p>Return the current color selected to paint the hint text.</p>
 *
 * @return Returns the current hint text color.
 */
Int32 TextView::GetCurrentHintTextColor()
{
    return mHintTextColor != NULL ? mCurHintTextColor : mCurTextColor;
}

/**
 * Sets the color of links in the text.
 *
 * @attr ref android.R.styleable#TextView_textColorLink
 */
ECode TextView::SetLinkTextColor(
    /* [in] */ Int32 color)
{
    CColorStateList::ValueOf(color, (IColorStateList**)&mLinkTextColor);
    UpdateTextColors();
    return NOERROR;
}

/**
 * Sets the color of links in the text.
 *
 * @attr ref android.R.styleable#TextView_textColorLink
 */
ECode TextView::SetLinkTextColor(
    /* [in] */ IColorStateList* colors)
{
    mLinkTextColor = colors;
    UpdateTextColors();
    return NOERROR;
}

/**
 * <p>Returns the color used to paint links in the text.</p>
 *
 * @return Returns the list of link text colors.
 */
ECode TextView::GetLinkTextColors(
    /* [out] */ IColorStateList** color)
{
    assert(color != NULL);
    *color = mLinkTextColor;
    if (*color != NULL) (*color)->AddRef();
    return NOERROR;
}

/**
 * Sets the horizontal alignment of the text and the
 * vertical gravity that will be used when there is extra space
 * in the TextView beyond what is required for the text itself.
 *
 * @see android.view.Gravity
 * @attr ref android.R.styleable#TextView_gravity
 */
ECode TextView::SetGravity(
    /* [in] */ Int32 gravity)
{
    if ((gravity & Gravity_HORIZONTAL_GRAVITY_MASK) == 0) {
        gravity |= Gravity_LEFT;
    }
    if ((gravity & Gravity_VERTICAL_GRAVITY_MASK) == 0) {
        gravity |= Gravity_TOP;
    }

    Boolean newLayout = FALSE;

    if ((gravity & Gravity_HORIZONTAL_GRAVITY_MASK) !=
        (mGravity & Gravity_HORIZONTAL_GRAVITY_MASK)) {
        newLayout = TRUE;
    }

    if (gravity != mGravity) {
        Invalidate();
    }

    mGravity = gravity;

    if (mLayout != NULL && newLayout) {
        // XXX this is heavy-handed because no actual content changes.
        Int32 want = 0, hintWant = 0;
        mLayout->GetWidth(&want);
        if (mHintLayout != NULL) mHintLayout->GetWidth(&hintWant);

        MakeNewLayout(want, hintWant, UNKNOWN_BORING, UNKNOWN_BORING,
                mRight - mLeft - GetCompoundPaddingLeft() -
                GetCompoundPaddingRight(), TRUE);
    }
    return NOERROR;
}

/**
 * Returns the horizontal and vertical alignment of this TextView.
 *
 * @see android.view.Gravity
 * @attr ref android.R.styleable#TextView_gravity
 */
Int32 TextView::GetGravity()
{
    return mGravity;
}

/**
 * @return the flags on the Paint being used to display the text.
 * @see Paint#getFlags
 */
Int32 TextView::GetPaintFlags()
{
    Int32 flag;
    mTextPaint->GetFlags(&flag);
    return flag;
}

/**
 * Sets flags on the Paint being used to display the text and
 * reflows the text if they are different from the old flags.
 * @see Paint#setFlags
 */
ECode TextView::SetPaintFlags(
    /* [in] */ Int32 flags)
{
    Int32 oldFlags;
    mTextPaint->GetFlags(&oldFlags);
    if (oldFlags != flags) {
        mTextPaint->SetFlags(flags);

        if (mLayout != NULL) {
            NullLayouts();
            RequestLayout();
            Invalidate();
        }
    }
    return NOERROR;
}

/**
 * Sets whether the text should be allowed to be wider than the
 * View is.  If FALSE, it will be wrapped to the width of the View.
 *
 * @attr ref android.R.styleable#TextView_scrollHorizontally
 */
ECode TextView::SetHorizontallyScrolling(
    /* [in] */ Boolean whether)
{
    mHorizontallyScrolling = whether;

    if (mLayout != NULL) {
        NullLayouts();
        RequestLayout();
        Invalidate();
    }
    return NOERROR;
}

/**
 * Makes the TextView at least this many lines tall
 *
 * @attr ref android.R.styleable#TextView_minLines
 */
ECode TextView::SetMinLines(
    /* [in] */ Int32 minlines)
{
    mMinimum = minlines;
    mMinMode = LINES;

    RequestLayout();
    Invalidate();
    return NOERROR;
}

/**
 * Makes the TextView at least this many pixels tall
 *
 * @attr ref android.R.styleable#TextView_minHeight
 */
ECode TextView::SetMinHeight(
    /* [in] */ Int32 minHeight)
{
    mMinimum = minHeight;
    mMinMode = PIXELS;

    RequestLayout();
    Invalidate();
    return NOERROR;
}

/**
 * Makes the TextView at most this many lines tall
 *
 * @attr ref android.R.styleable#TextView_maxLines
 */
ECode TextView::SetMaxLines(
    /* [in] */ Int32 maxlines)
{
    mMaximum = maxlines;
    mMaxMode = LINES;

    RequestLayout();
    Invalidate();
    return NOERROR;
}

/**
 * Makes the TextView at most this many pixels tall
 *
 * @attr ref android.R.styleable#TextView_maxHeight
 */
ECode TextView::SetMaxHeight(
    /* [in] */ Int32 maxHeight)
{
    mMaximum = maxHeight;
    mMaxMode = PIXELS;

    RequestLayout();
    Invalidate();
    return NOERROR;
}

/**
 * Makes the TextView exactly this many lines tall
 *
 * @attr ref android.R.styleable#TextView_lines
 */
ECode TextView::SetLines(
    /* [in] */ Int32 lines)
{
    mMaximum = mMinimum = lines;
    mMaxMode = mMinMode = LINES;

    RequestLayout();
    Invalidate();
    return NOERROR;
}

/**
 * Makes the TextView exactly this many pixels tall.
 * You could do the same thing by specifying this number in the
 * LayoutParams.
 *
 * @attr ref android.R.styleable#TextView_height
 */
ECode TextView::SetHeight(
    /* [in] */ Int32 pixels)
{
    mMaximum = mMinimum = pixels;
    mMaxMode = mMinMode = PIXELS;

    RequestLayout();
    Invalidate();
    return NOERROR;
}

/**
 * Makes the TextView at least this many ems wide
 *
 * @attr ref android.R.styleable#TextView_minEms
 */
ECode TextView::SetMinEms(
    /* [in] */ Int32 minems)
{
    mMinWidth = minems;
    mMinWidthMode = EMS;

    RequestLayout();
    Invalidate();
    return NOERROR;
}

/**
 * Makes the TextView at least this many pixels wide
 *
 * @attr ref android.R.styleable#TextView_minWidth
 */
ECode TextView::SetMinWidth(
    /* [in] */ Int32 minpixels)
{
    mMinWidth = minpixels;
    mMinWidthMode = PIXELS;

    RequestLayout();
    Invalidate();
    return NOERROR;
}

/**
 * Makes the TextView at most this many ems wide
 *
 * @attr ref android.R.styleable#TextView_maxEms
 */
ECode TextView::SetMaxEms(
    /* [in] */ Int32 maxems)
{
    mMaxWidth = maxems;
    mMaxWidthMode = EMS;

    RequestLayout();
    Invalidate();
    return NOERROR;
}

/**
 * Makes the TextView at most this many pixels wide
 *
 * @attr ref android.R.styleable#TextView_maxWidth
 */
ECode TextView::SetMaxWidth(
    /* [in] */ Int32 maxpixels)
{
    mMaxWidth = maxpixels;
    mMaxWidthMode = PIXELS;

    RequestLayout();
    Invalidate();
    return NOERROR;
}

/**
 * Makes the TextView exactly this many ems wide
 *
 * @attr ref android.R.styleable#TextView_ems
 */
ECode TextView::SetEms(
    /* [in] */ Int32 ems)
{
    mMaxWidth = mMinWidth = ems;
    mMaxWidthMode = mMinWidthMode = EMS;

    RequestLayout();
    Invalidate();
    return NOERROR;
}

/**
 * Makes the TextView exactly this many pixels wide.
 * You could do the same thing by specifying this number in the
 * LayoutParams.
 *
 * @attr ref android.R.styleable#TextView_width
 */
ECode TextView::SetWidth(
    /* [in] */ Int32 pixels)
{
    mMaxWidth = mMinWidth = pixels;
    mMaxWidthMode = mMinWidthMode = PIXELS;

    RequestLayout();
    Invalidate();
    return NOERROR;
}

/**
 * Sets line spacing for this TextView.  Each line will have its height
 * multiplied by <code>mult</code> and have <code>add</code> added to it.
 *
 * @attr ref android.R.styleable#TextView_lineSpacingExtra
 * @attr ref android.R.styleable#TextView_lineSpacingMultiplier
 */
ECode TextView::SetLineSpacing(
    /* [in] */ Float add,
    /* [in] */ Float mult)
{
    mSpacingMult = mult;
    mSpacingAdd = add;

    if (mLayout != NULL) {
        NullLayouts();
        RequestLayout();
        Invalidate();
    }
    return NOERROR;
}

/**
 * Convenience method: Append the specified text to the TextView's
 * display buffer, upgrading it to BufferType.EDITABLE if it was
 * not already editable.
 */
ECode TextView::Append(
    /* [in] */ ICharSequence* text)
{
    assert(text != NULL);

    Int32 len;
    text->GetLength(&len);
    return Append(text, 0, len);
}

/**
 * Convenience method: Append the specified text slice to the TextView's
 * display buffer, upgrading it to BufferType.EDITABLE if it was
 * not already editable.
 */
ECode TextView::Append(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (IEditable::Probe(mText) == NULL) {
        SetText(mText, BufferType_EDITABLE);
    }

    AutoPtr<IEditable> editable;
    return (IEditable::Probe(mText))->AppendEx(
        text, start, end, (IEditable**)&editable);
}

void TextView::UpdateTextColors()
{
    Boolean inval = FALSE;
    Int32 color;
    mTextColor->GetColorForState(GetDrawableState(), 0, &color);
    if (color != mCurTextColor) {
        mCurTextColor = color;
        inval = TRUE;
    }
    if (mLinkTextColor != NULL) {
        mLinkTextColor->GetColorForState(GetDrawableState(), 0, &color);
        if (color != ((CTextPaint*)mTextPaint.Get())->mLinkColor) {
            ((CTextPaint*)mTextPaint.Get())->mLinkColor = color;
            inval = TRUE;
        }
    }
    if (mHintTextColor != NULL) {
        mHintTextColor->GetColorForState(GetDrawableState(), 0, &color);
        Int32 len;
        mText->GetLength(&len);
        if (color != mCurHintTextColor && len == 0) {
            mCurHintTextColor = color;
            inval = TRUE;
        }
    }
    if (inval) {
        Invalidate();
    }
}

ECode TextView::DrawableStateChanged()
{
    View::DrawableStateChanged();

    Boolean textColor, hintColor, linkColor;
    mTextColor->IsStateful(&textColor);
    mHintTextColor->IsStateful(&hintColor);
    mLinkTextColor->IsStateful(&linkColor);

    if ((mTextColor != NULL && textColor)
            || (mHintTextColor != NULL && hintColor)
            || (mLinkTextColor != NULL && linkColor)) {
        UpdateTextColors();
    }

    Drawables* dr = mDrawables;
    if (dr != NULL) {
        ArrayOf<Int32>* state = GetDrawableState();

        Boolean isStateful = FALSE;
        if (dr->mDrawableTop != NULL && (dr->mDrawableTop->IsStateful(&isStateful), isStateful)) {
            dr->mDrawableTop->SetState(state, &isStateful);
        }

        if (dr->mDrawableBottom != NULL && (dr->mDrawableBottom->IsStateful(&isStateful), isStateful)) {
            dr->mDrawableBottom->SetState(state, &isStateful);
        }

        if (dr->mDrawableLeft != NULL && (dr->mDrawableLeft->IsStateful(&isStateful), isStateful)) {
            dr->mDrawableLeft->SetState(state, &isStateful);
        }

        if (dr->mDrawableRight != NULL && (dr->mDrawableRight->IsStateful(&isStateful), isStateful)) {
            dr->mDrawableRight->SetState(state, &isStateful);
        }
    }
    return NOERROR;
}

AutoPtr<IParcelable> TextView::OnSaveInstanceState()
{
    AutoPtr<IParcelable> superState = View::OnSaveInstanceState();

    // Save state if we are forced to
    Boolean save = mFreezesText;
    Int32 start = 0;
    Int32 end = 0;

    if (mText != NULL) {
        start = GetSelectionStart();
        end = GetSelectionEnd();
        if (start >= 0 || end >= 0) {
            // Or save state if there is a selection
            save = TRUE;
        }
    }

   if (save) {
       AutoPtr<CTextViewSavedState> ss;
       CTextViewSavedState::NewByFriend(superState, (CTextViewSavedState**)&ss);
       // XXX Should also save the current scroll position!
       ss->mSelStart = start;
       ss->mSelEnd = end;

       if (ISpanned::Probe(mText)) {
           /*
               * Calling setText() strips off any ChangeWatchers;
               * strip them now to avoid leaking references.
               * But do it to a copy so that if there are any
               * further changes to the text of this view, it
               * won't get into an inconsistent state.
               */

           AutoPtr<ISpannable> sp;
           CSpannableString::New(mText, (ISpannable**)&sp);

           // for (ChangeWatcher cw :
           //         sp.getSpans(0, sp.length(), ChangeWatcher.class)) {
           //     sp.removeSpan(cw);
           // }

           ss->mText = sp;
       }
       else {
           ss->mText = mText;
       }

       if (IsFocused() && start >= 0 && end >= 0) {
           ss->mFrozenWithFocus = TRUE;
       }

       ss->mError = mError;

       return ss;
   }

    return superState;
}

void TextView::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    if (!ITextViewSavedState::Probe(state)) {
       View::OnRestoreInstanceState(state);
       return;
    }

    CTextViewSavedState* ss = (CTextViewSavedState*)ITextViewSavedState::Probe(state);

    AutoPtr<IParcelable> superState;
    ss->GetSuperState((IParcelable**)&superState);
    View::OnRestoreInstanceState(superState);

    // XXX restore buffer type too, as well as lots of other stuff
    if (ss->mText != NULL) {
       SetText(ss->mText);
    }

    if (ss->mSelStart >= 0 && ss->mSelEnd >= 0) {
       if (ISpannable::Probe(mText)) {
           Int32 len;
           mText->GetLength(&len);

           if (ss->mSelStart > len || ss->mSelEnd > len) {
               String restored("");

               if (ss->mText != NULL) {
                   restored = String("(restored) ");
               }

               // Log.e(LOG_TAG, "Saved cursor position " + ss.selStart +
               //         "/" + ss.selEnd + " out of range for " + restored +
               //         "text " + mText);
           }
           else {
                Selection::SetSelection(
                ISpannable::Probe(mText), ss->mSelStart, ss->mSelEnd);

                if (ss->mFrozenWithFocus) {
                    mFrozenWithFocus = TRUE;
                }
           }
       }
    }

    if (ss->mError != NULL) {
       AutoPtr<ICharSequence> error = ss->mError;
       // Display the error later, after the first layout pass
       // post(new Runnable() {
       //     public void run() {
       //         setError(error);
       //     }
       // });
    }
}

/**
 * Control whether this text view saves its entire text contents when
 * freezing to an icicle, in addition to dynamic state such as cursor
 * position.  By default this is FALSE, not saving the text.  Set to TRUE
 * if the text in the text view is not being saved somewhere else in
 * persistent storage (such as in a content provider) so that if the
 * view is later thawed the user will not lose their data.
 *
 * @param freezesText Controls whether a frozen icicle should include the
 * entire text data: TRUE to include it, FALSE to not.
 *
 * @attr ref android.R.styleable#TextView_freezesText
 */
ECode TextView::SetFreezesText(
    /* [in] */ Boolean freezesText)
{
    mFreezesText = freezesText;
    return NOERROR;
}

/**
 * Return whether this text view is including its entire text contents
 * in frozen icicles.
 *
 * @return Returns TRUE if text is included, FALSE if it isn't.
 *
 * @see #setFreezesText
 */
Boolean TextView::GetFreezesText()
{
    return mFreezesText;
}

///////////////////////////////////////////////////////////////////////////

/**
 * Sets the Factory used to create new Editables.
 */
ECode TextView::SetEditableFactory(
    /* [in] */ IEditableFactory* factory)
{
    mEditableFactory = factory;
    return SetText(mText);
}

/**
 * Sets the Factory used to create new Spannables.
 */
ECode TextView::SetSpannableFactory(
    /* [in] */ ISpannableFactory* factory)
{
    mSpannableFactory = factory;
    return SetText(mText);
}

/**
 * Sets the string value of the TextView. TextView <em>does not</em> accept
 * HTML-like formatting, which you can do with text strings in XML resource files.
 * To style your strings, attach android.text.style.* objects to a
 * {@link android.text.SpannableString SpannableString}, or see the
 * <a href="{@docRoot}guide/topics/resources/available-resources.html#stringresources">
 * Available Resource Types</a> documentation for an example of setting
 * formatted text in the XML resource file.
 *
 * @attr ref android.R.styleable#TextView_text
 */
ECode TextView::SetText(
    /* [in] */ ICharSequence* text)
{
    return SetText(text, mBufferType);
}

/**
 * Like {@link #setText(CharSequence)},
 * except that the cursor position (if any) is retained in the new text.
 *
 * @param text The new text to place in the text view.
 *
 * @see #setText(CharSequence)
 */
ECode TextView::SetTextKeepState(
    /* [in] */ ICharSequence* text)
{
    return SetTextKeepState(text, mBufferType);
}

/**
 * Sets the text that this TextView is to display (see
 * {@link #setText(CharSequence)}) and also sets whether it is stored
 * in a styleable/spannable buffer and whether it is editable.
 *
 * @attr ref android.R.styleable#TextView_text
 * @attr ref android.R.styleable#TextView_bufferType
 */
ECode TextView::SetText(
    /* [in] */ ICharSequence* text,
    /* [in] */ BufferType type)
{
    SetText(text, type, TRUE, 0);

    if (mCharWrapper != NULL) {
        mCharWrapper->mChars = NULL;
    }
    return NOERROR;
}

ECode TextView::SetText(
    /* [in] */ ICharSequence* _text,
    /* [in] */ BufferType type,
    /* [in] */ Boolean notifyBefore,
    /* [in] */ Int32 oldlen)
{
    AutoPtr<ICharSequence> text = _text;

    if (text == NULL) {
        CStringWrapper::New(String(""), (ICharSequence**)&text);
    }

    if (!mUserSetTextScaleX) mTextPaint->SetTextScaleX(1.0f);

    //Int32 spanStart;
    // if (ISpanned::Probe(text) &&
    //     (ISpanned::Probe(text)->GetSpanStart(
    //     TextUtilsTruncateAt_MARQUEE, &spanStart), spanStart) >= 0) {
    //     SetHorizontalFadingEdgeEnabled(TRUE);
    //     SetEllipsize(TextUtilsTruncateAt_MARQUEE);
    // }

//    Int32 n = mFilters.length;
//    for (Int32 i = 0; i < n; i++) {
//        CharSequence out = mFilters[i].filter(text, 0, text.length(),
//                                                EMPTY_SPANNED, 0, 0);
//        if (out != NULL) {
//            text = out;
//        }
//    }

    if (notifyBefore) {
        Int32 len;
        text->GetLength(&len);
        if (mText != NULL) {
            mText->GetLength(&oldlen);
            SendBeforeTextChanged(mText, 0, oldlen, len);
        }
        else {
            AutoPtr<ICharSequence> cs;
            CStringWrapper::New(String(""), (ICharSequence**)&cs);
            SendBeforeTextChanged(cs, 0, 0, len);
        }
    }

    Boolean needEditableForNotification = FALSE;

    if (mListeners.GetSize() != 0) {
        needEditableForNotification = TRUE;
    }

    if (type == BufferType_EDITABLE || mInput != NULL ||
        needEditableForNotification) {
        AutoPtr<IEditable> t;
        mEditableFactory->NewEditable(text, (IEditable**)&t);
        text = t;
        SetFilters(t, mFilters);
        AutoPtr<ILocalInputMethodManager> imm = CLocalInputMethodManager::PeekInstance();
        if (imm != NULL) {
            imm->RestartInput((IView*)this->Probe(EIID_IView));
        }
    }
    else if (type == BufferType_SPANNABLE || mMovement != NULL) {
        AutoPtr<ISpannable> t;
        mSpannableFactory->NewSpannable(text, (ISpannable**)&t);
        text = t;
    }
//    else if (!(text instanceof CharWrapper)) {
//        text = TextUtils.stringOrSpannedString(text);
//    }

//    if (mAutoLinkMask != 0) {
//        Spannable s2;
//
//        if (type == BufferType.EDITABLE || text instanceof Spannable) {
//            s2 = (Spannable) text;
//        }
//        else {
//            s2 = mSpannableFactory.newSpannable(text);
//        }
//
//        if (Linkify.addLinks(s2, mAutoLinkMask)) {
//            text = s2;
//            type = (type == BufferType.EDITABLE) ? BufferType.EDITABLE : BufferType.SPANNABLE;
//
//            /*
//                * We must go ahead and set the text before changing the
//                * movement method, because setMovementMethod() may call
//                * setText() again to try to upgrade the buffer type.
//                */
//            mText = text;
//
//            if (mLinksClickable) {
//                setMovementMethod(LinkMovementMethod.getInstance());
//            }
//        }
//    }

    mBufferType = type;
    mText = text;

    if (mTransformation == NULL) {
        mTransformed = text;
    }
    else {
        mTransformation->GetTransformation(text, (IView*)this->Probe(EIID_IView),
                (ICharSequence**)&mTransformed);
    }

    Int32 textLength;
    text->GetLength(&textLength);

    if (ISpannable::Probe(text)) {
        AutoPtr<ISpannable> sp = ISpannable::Probe(text);

        // Remove any ChangeWatchers that might have come
        // from other TextViews.
        ArrayOf<IInterface*>* watchers;
        sp->GetSpans(0, textLength, EIID_ChangeWatcher, &watchers);
        Int32 count = watchers->GetLength();
        for (Int32 i = 0; i < count; i++) {
            sp->RemoveSpan((*watchers)[i]);
        }
        FreeArray(watchers);

        if (mChangeWatcher == NULL) {
            mChangeWatcher = new ChangeWatcher(this);
        }

        sp->SetSpan(
            mChangeWatcher->Probe(EIID_IInterface), 0, textLength,
            Spanned_SPAN_INCLUSIVE_INCLUSIVE |
            (PRIORITY << Spanned_SPAN_PRIORITY_SHIFT));

        if (mInput != NULL) {
            sp->SetSpan(mInput, 0, textLength, Spanned_SPAN_INCLUSIVE_INCLUSIVE);
        }

        if (mTransformation != NULL) {
            sp->SetSpan(mTransformation, 0, textLength, Spanned_SPAN_INCLUSIVE_INCLUSIVE);
        }

        if (mMovement != NULL) {
            mMovement->Initialize(
                (ITextView*)this->Probe(EIID_ITextView), ISpannable::Probe(text));

            /*
                * Initializing the movement method will have set the
                * selection, so reset mSelectionMoved to keep that from
                * interfering with the normal on-focus selection-setting.
                */
            mSelectionMoved = FALSE;
        }
    }

    if (mLayout != NULL) {
        CheckForRelayout();
    }

    SendOnTextChanged(text, 0, oldlen, textLength);
    OnTextChanged(text, 0, oldlen, textLength);

    if (needEditableForNotification) {
        SendAfterTextChanged(IEditable::Probe(text));
    }

    // SelectionModifierCursorController depends on textCanBeSelected, which depends on text
    PrepareCursorControllers();

    return NOERROR;
}

/**
 * Sets the TextView to display the specified slice of the specified
 * char array.  You must promise that you will not change the contents
 * of the array except for right before another call to setText(),
 * since the TextView has no way to know that the text
 * has changed and that it needs to invalidate and re-layout.
 */
ECode TextView::SetText(
    /* [in] */ const ArrayOf<Char8>& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 len)
{
    Int32 oldlen = 0;

    if (start < 0 || len < 0 || start + len > (Int32)String((const char*)text.GetPayload()).GetCharCount()) {
//        throw new IndexOutOfBoundsException(start + ", " + len);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    /*
     * We must do the before-notification here ourselves because if
     * the old text is a CharWrapper we destroy it before calling
     * into the normal path.
     */
    if (mText != NULL) {
        mText->GetLength(&oldlen);
        SendBeforeTextChanged(mText, 0, oldlen, len);
    }
    else {
        AutoPtr<ICharSequence> temp;
        CStringWrapper::New(String(""), (ICharSequence**)&temp);
        SendBeforeTextChanged(temp, 0, 0, len);
    }

    if (mCharWrapper == NULL) {
        mCharWrapper = new CharWrapper(text, start, len);
    }
    else {
        mCharWrapper->Set(text, start, len);
    }

    return SetText(mCharWrapper, mBufferType, FALSE, oldlen);
}

/**
 * Like {@link #setText(CharSequence, android.widget.TextView.BufferType)},
 * except that the cursor position (if any) is retained in the new text.
 *
 * @see #setText(CharSequence, android.widget.TextView.BufferType)
 */
ECode TextView::SetTextKeepState(
    /* [in] */ ICharSequence* text,
    /* [in] */ BufferType type)
{
    Int32 start, end, len;
    start = GetSelectionStart();
    end = GetSelectionEnd();
    text->GetLength(&len);

    FAIL_RETURN(SetText(text, type));

    if (start >= 0 || end >= 0) {
        if (ISpannable::Probe(mText)) {
            Selection::SetSelection(
                ISpannable::Probe(mText), Math::Max(0, Math::Min(start, len)),
                Math::Max(0, Math::Min(end, len)));
        }
    }
    return NOERROR;
}

ECode TextView::SetText(
    /* [in] */ Int32 resid)
{
    AutoPtr<IResources> resources;
    GetContext()->GetResources((IResources**)&resources);

    AutoPtr<ICharSequence> csq;
    resources->GetText(resid, (ICharSequence**)&csq);

    return SetText(csq);
}

ECode TextView::SetText(
    /* [in] */ Int32 resid,
    /* [in] */ BufferType type)
{
    AutoPtr<IResources> resources;
    GetContext()->GetResources((IResources**)&resources);

    AutoPtr<ICharSequence> csq;
    resources->GetText(resid, (ICharSequence**)&csq);

    return SetText(csq, type);
}

/**
 * Sets the text to be displayed when the text of the TextView is empty.
 * Null means to use the normal empty text. The hint does not currently
 * participate in determining the size of the view.
 *
 * @attr ref android.R.styleable#TextView_hint
 */
ECode TextView::SetHint(
    /* [in] */ ICharSequence* hint)
{
//    mHint = TextUtils.stringOrSpannedString(hint);

    if (mLayout != NULL) {
        CheckForRelayout();
    }

    Int32 len;
    mText->GetLength(&len);
    if (len == 0) {
        Invalidate();
    }
    return NOERROR;
}

/**
 * Sets the text to be displayed when the text of the TextView is empty,
 * from a resource.
 *
 * @attr ref android.R.styleable#TextView_hint
 */
ECode TextView::SetHint(
    /* [in] */ Int32 resid)
{
    AutoPtr<IResources> resources;
    GetContext()->GetResources((IResources**)&resources);

    AutoPtr<ICharSequence> csq;
    resources->GetText(resid, (ICharSequence**)&csq);

    return SetHint(csq);
}

/**
 * Returns the hint that is displayed when the text of the TextView
 * is empty.
 *
 * @attr ref android.R.styleable#TextView_hint
 */
AutoPtr<ICharSequence> TextView::GetHint()
{
    return mHint;
}

/**
 * Set the type of the content with a constant as defined for
 * {@link EditorInfo#inputType}.  This will take care of changing
 * the key listener, by calling {@link #setKeyListener(KeyListener)}, to
 * match the given content type.  If the given content type is
 * {@link EditorInfo#TYPE_NULL} then a soft keyboard will
 * not be displayed for this text view.
 *
 * @see #getInputType()
 * @see #setRawInputType(Int32)
 * @see android.text.InputType
 * @attr ref android.R.styleable#TextView_inputType
 */
ECode TextView::SetInputType(
    /* [in] */ Int32 type)
{
    const Boolean wasPassword = IsPasswordInputType(mInputType);
    const Boolean wasVisiblePassword = IsVisiblePasswordInputType(mInputType);
    SetInputType(type, FALSE);
    const Boolean isPassword = IsPasswordInputType(type);
    const Boolean isVisiblePassword = IsVisiblePasswordInputType(type);
    Boolean forceUpdate = FALSE;
    if (isPassword) {
        assert(0);
        // SetTransformationMethod(PasswordTransformationMethod.getInstance());
        // SetTypefaceByIndex(MONOSPACE, 0);
    }
    else if (isVisiblePassword) {
        assert(0);
        // if (mTransformation == PasswordTransformationMethod.getInstance()) {
        //     forceUpdate = TRUE;
        // }
        SetTypefaceByIndex(MONOSPACE, 0);
    }
    else if (wasPassword || wasVisiblePassword) {
        assert(0);
        // not in password mode, clean up typeface and transformation
        // SetTypefaceByIndex(-1, -1);
        // if (mTransformation == PasswordTransformationMethod.getInstance()) {
        //     forceUpdate = TRUE;
        // }
    }

    Boolean multiLine = (type& (InputType_TYPE_MASK_CLASS
                   | InputType_TYPE_TEXT_FLAG_MULTI_LINE)) ==
           (InputType_TYPE_CLASS_TEXT
                   | InputType_TYPE_TEXT_FLAG_MULTI_LINE);

    // We need to update the single line mode if it has changed or we
    // were previously in password mode.
    if (mSingleLine == multiLine || forceUpdate) {
       // Change single line mode, but only change the transformation if
       // we are not in password mode.
       ApplySingleLine(!multiLine, !isPassword);
    }

    AutoPtr<ILocalInputMethodManager> imm = CLocalInputMethodManager::PeekInstance();
    if (imm != NULL) {
        imm->RestartInput((IView*)this->Probe(EIID_IView));
    }
    return NOERROR;
}

/**
 * It would be better to rely on the input type for everything. A password inputType should have
 * a password transformation. We should hence use isPasswordInputType instead of this method.
 *
 * We should:
 * - Call setInputType in setKeyListener instead of changing the input type directly (which
 * would install the correct transformation).
 * - Refuse the installation of a non-password transformation in setTransformation if the input
 * type is password.
 *
 * However, this is like this for legacy reasons and we cannot break existing apps. This method
 * is useful since it matches what the user can see (obfuscated text or not).
 *
 * @return TRUE if the current transformation method is of the password type.
 */
Boolean TextView::HasPasswordTransformationMethod()
{
    //return mTransformation instanceof PasswordTransformationMethod;
    return FALSE;
}

Boolean TextView::IsPasswordInputType(
    /* [in] */ Int32 inputType)
{
//    Int32 variation = inputType & (InputType_TYPE_MASK_CLASS
//            | InputType_TYPE_MASK_VARIATION);
//    return variation
//            == (InputType_TYPE_CLASS_TEXT
//                    | InputType_TYPE_TEXT_VARIATION_PASSWORD);
    return FALSE;
}

Boolean TextView::IsVisiblePasswordInputType(
    /* [in] */ Int32 inputType)
{
//    final Int32 variation = inputType & (InputType_TYPE_MASK_CLASS
//            | InputType_TYPE_MASK_VARIATION);
//    return variation
//            == (InputType_TYPE_CLASS_TEXT
//                    | InputType_TYPE_TEXT_VARIATION_VISIBLE_PASSWORD);
    return FALSE;
}

/**
 * Directly change the content type integer of the text view, without
 * modifying any other state.
 * @see #setInputType(Int32)
 * @see android.text.InputType
 * @attr ref android.R.styleable#TextView_inputType
 */
ECode TextView::SetRawInputType(
    /* [in] */ Int32 type)
{
    mInputType = type;
    return NOERROR;
}

void TextView::SetInputType(
    /* [in] */ Int32 type,
    /* [in] */ Boolean direct)
{
    Int32 cls = type & InputType_TYPE_MASK_CLASS;
    AutoPtr<IKeyListener> input;
    if (cls == InputType_TYPE_CLASS_TEXT) {
        Boolean autotext = (type & InputType_TYPE_TEXT_FLAG_AUTO_CORRECT) != 0;
        Capitalize cap;
        if ((type & InputType_TYPE_TEXT_FLAG_CAP_CHARACTERS) != 0) {
            cap = Capitalize_CHARACTERS;
        }
        else if ((type & InputType_TYPE_TEXT_FLAG_CAP_WORDS) != 0) {
            cap = Capitalize_WORDS;
        }
        else if ((type & InputType_TYPE_TEXT_FLAG_CAP_SENTENCES) != 0) {
            cap = Capitalize_SENTENCES;
        }
        else {
            cap = Capitalize_NONE;
        }
        input = TextKeyListener::GetInstance(autotext, cap);
    }
    else if (cls == InputType_TYPE_CLASS_NUMBER) {
        // input = DigitsKeyListener.getInstance(
        //         (type & InputType_TYPE_NUMBER_FLAG_SIGNED) != 0,
        //         (type & InputType_TYPE_NUMBER_FLAG_DECIMAL) != 0);
    }
    else if (cls == InputType_TYPE_CLASS_DATETIME) {
        // switch (type & InputType_TYPE_MASK_VARIATION) {
        //     case InputType_TYPE_DATETIME_VARIATION_DATE:
        //         input = DateKeyListener.getInstance();
        //         break;
        //     case InputType_TYPE_DATETIME_VARIATION_TIME:
        //         input = TimeKeyListener.getInstance();
        //         break;
        //     default:
        //         input = DateTimeKeyListener.getInstance();
        //         break;
        // }
    }
    else if (cls == InputType_TYPE_CLASS_PHONE) {
        //input = DialerKeyListener.getInstance();
    }
    else {
        input = TextKeyListener::GetInstance();
    }
    SetRawInputType(type);
    if (direct)
        mInput = input;
    else {
        SetKeyListenerOnly(input);
    }
}

/**
 * Get the type of the content.
 *
 * @see #setInputType(Int32)
 * @see android.text.InputType
 */
Int32 TextView::GetInputType()
{
    return mInputType;
}

/**
 * Change the editor type integer associated with the text view, which
 * will be reported to an IME with {@link EditorInfo#imeOptions} when it
 * has focus.
 * @see #getImeOptions
 * @see android.view.inputmethod.EditorInfo
 * @attr ref android.R.styleable#TextView_imeOptions
 */
ECode TextView::SetImeOptions(
    /* [in] */ Int32 imeOptions)
{
//    if (mInputContentType == NULL) {
//        mInputContentType = new InputContentType();
//    }
//    mInputContentType.imeOptions = imeOptions;
    return E_NOT_IMPLEMENTED;
}

/**
 * Get the type of the IME editor.
 *
 * @see #setImeOptions(Int32)
 * @see android.view.inputmethod.EditorInfo
 */
Int32 TextView::GetImeOptions()
{
//    return mInputContentType != NULL
//            ? mInputContentType.imeOptions : EditorInfo_IME_NULL;
    return 0x00000000;
}

/**
 * Change the custom IME action associated with the text view, which
 * will be reported to an IME with {@link EditorInfo#actionLabel}
 * and {@link EditorInfo#actionId} when it has focus.
 * @see #getImeActionLabel
 * @see #getImeActionId
 * @see android.view.inputmethod.EditorInfo
 * @attr ref android.R.styleable#TextView_imeActionLabel
 * @attr ref android.R.styleable#TextView_imeActionId
 */
ECode TextView::SetImeActionLabel(
    /* [in] */ ICharSequence* label,
    /* [in] */ Int32 actionId)
{
//    if (mInputContentType == NULL) {
//        mInputContentType = new InputContentType();
//    }
//    mInputContentType.imeActionLabel = label;
//    mInputContentType.imeActionId = actionId;
    return E_NOT_IMPLEMENTED;
}

/**
 * Get the IME action label previous set with {@link #setImeActionLabel}.
 *
 * @see #setImeActionLabel
 * @see android.view.inputmethod.EditorInfo
 */
AutoPtr<ICharSequence> TextView::GetImeActionLabel()
{
//    return mInputContentType != NULL
//            ? mInputContentType.imeActionLabel : NULL;
    return NULL;
}

/**
 * Get the IME action ID previous set with {@link #setImeActionLabel}.
 *
 * @see #setImeActionLabel
 * @see android.view.inputmethod.EditorInfo
 */
Int32 TextView::GetImeActionId()
{
//    return mInputContentType != NULL
//            ? mInputContentType.imeActionId : 0;
    return 0;
}

/**
 * Set a special listener to be called when an action is performed
 * on the text view.  This will be called when the enter key is pressed,
 * or when an action supplied to the IME is selected by the user.  Setting
 * this means that the normal hard key event will not insert a newline
 * into the text view, even if it is multi-line; holding down the ALT
 * modifier will, however, allow the user to insert a newline character.
 */
//void TextView::SetOnEditorActionListener(OnEditorActionListener l) {
//    if (mInputContentType == NULL) {
//        mInputContentType = new InputContentType();
//    }
//    mInputContentType.onEditorActionListener = l;
//}

/**
 * Called when an attached input method calls
 * {@link InputConnection#performEditorAction(Int32)
 * InputConnection.performEditorAction()}
 * for this text view.  The default implementation will call your action
 * listener supplied to {@link #setOnEditorActionListener}, or perform
 * a standard operation for {@link EditorInfo#IME_ACTION_NEXT
 * InputType_IME_ACTION_NEXT} or {@link EditorInfo#IME_ACTION_DONE
 * InputType_IME_ACTION_DONE}.
 *
 * <p>For backwards compatibility, if no IME options have been set and the
 * text view would not normally advance focus on enter, then
 * the NEXT and DONE actions received here will be turned into an enter
 * key down/up pair to go through the normal key handling.
 *
 * @param actionCode The code of the action being performed.
 *
 * @see #setOnEditorActionListener
 */
ECode TextView::OnEditorAction(
    /* [in] */ Int32 actionCode)
{
    if (mInputContentType != NULL) {
        if (mInputContentType->mOnEditorActionListener != NULL) {
            Boolean state = FALSE;
            mInputContentType->mOnEditorActionListener->OnEditorAction(
                 (ITextView*)this->Probe(EIID_ITextView), actionCode, NULL, &state);
            if (state) {
                return NOERROR;
            }
        }

        // This is the handling for some default action.
        // Note that for backwards compatibility we don't do this
        // default handling if explicit ime options have not been given,
        // instead turning this into the normal enter key codes that an
        // app may be expecting.
        if (actionCode == EditorInfo_IME_ACTION_NEXT) {
            AutoPtr<IView> v = FocusSearch(FOCUS_DOWN);
            if (v != NULL) {
                Boolean focus = FALSE;
                if (v->RequestFocusEx(FOCUS_DOWN, &focus), !focus) {
                    // throw new IllegalStateException("focus search returned a view " +
                    //         "that wasn't able to take focus!");
                    return E_ILLEGAL_STATE_EXCEPTION;
                }
            }
            return NOERROR;
        }
        else if (actionCode == EditorInfo_IME_ACTION_DONE) {
            AutoPtr<ILocalInputMethodManager> imm = CLocalInputMethodManager::PeekInstance();
            if (imm != NULL) {
                Boolean ret = FALSE;
                imm->HideSoftInputFromWindow(GetWindowToken(), 0, &ret);
            }
            return NOERROR;
        }
    }

    // Handler h = getHandler();
    // if (h != NULL) {
    //    Int64 eventTime = SystemClock::UptimeMillis();
    //    h.sendMessage(h.obtainMessage(ViewRoot.DISPATCH_KEY_FROM_IME,
    //            new KeyEvent(eventTime, eventTime,
    //            KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_ENTER, 0, 0, 0, 0,
    //            KeyEvent.FLAG_SOFT_KEYBOARD | KeyEvent.FLAG_KEEP_TOUCH_MODE
    //            | KeyEvent.FLAG_EDITOR_ACTION)));
    //    h.sendMessage(h.obtainMessage(ViewRoot.DISPATCH_KEY_FROM_IME,
    //            new KeyEvent(SystemClock.uptimeMillis(), eventTime,
    //            KeyEvent.ACTION_UP, KeyEvent.KEYCODE_ENTER, 0, 0, 0, 0,
    //            KeyEvent.FLAG_SOFT_KEYBOARD | KeyEvent.FLAG_KEEP_TOUCH_MODE
    //            | KeyEvent.FLAG_EDITOR_ACTION)));
    //}
    return NOERROR;
}

/**
 * Set the private content type of the text, which is the
 * {@link EditorInfo#privateImeOptions InputType_privateImeOptions}
 * field that will be filled in when creating an input connection.
 *
 * @see #getPrivateImeOptions()
 * @see EditorInfo#privateImeOptions
 * @attr ref android.R.styleable#TextView_privateImeOptions
 */
ECode TextView::SetPrivateImeOptions(
    /* [in] */ const String& type)
{
//    if (mInputContentType == NULL) mInputContentType = new InputContentType();
//    mInputContentType.privateImeOptions = type;
    return E_NOT_IMPLEMENTED;
}

/**
 * Get the private type of the content.
 *
 * @see #setPrivateImeOptions(String)
 * @see EditorInfo#privateImeOptions
 */
String TextView::GetPrivateImeOptions()
{
    /*return mInputContentType != NULL
            ? mInputContentType.privateImeOptions : NULL;*/
    return String(NULL);
}

/**
 * Set the extra input data of the text, which is the
 * {@link EditorInfo#extras TextBoxAttribute.extras}
 * Bundle that will be filled in when creating an input connection.  The
 * given integer is the resource ID of an XML resource holding an
 * {@link android.R.styleable#InputExtras &lt;input-extras&gt;} XML tree.
 *
 * @see #getInputExtras(Boolean)
 * @see EditorInfo#extras
 * @attr ref android.R.styleable#TextView_editorExtras
 */
ECode TextView::SetInputExtras(
    /* [in] */ Int32 xmlResId)
{
//    XmlResourceParser parser = getResources().getXml(xmlResId);
//    if (mInputContentType == NULL) mInputContentType = new InputContentType();
//    mInputContentType.extras = new Bundle();
//    getResources().parseBundleExtras(parser, mInputContentType.extras);
    return E_NOT_IMPLEMENTED;
}

/**
 * Retrieve the input extras currently associated with the text view, which
 * can be viewed as well as modified.
 *
 * @param create If TRUE, the extras will be created if they don't already
 * exist.  Otherwise, NULL will be returned if none have been created.
 * @see #setInputExtras(Int32)
 * @see EditorInfo#extras
 * @attr ref android.R.styleable#TextView_editorExtras
 */
AutoPtr<IBundle> TextView::GetInputExtras(
    /* [in] */ Boolean create)
{
//    if (mInputContentType == NULL) {
//        if (!create) return NULL;
//        mInputContentType = new InputContentType();
//    }
//    if (mInputContentType.extras == NULL) {
//        if (!create) return NULL;
//        mInputContentType.extras = new Bundle();
//    }
//    return mInputContentType.extras;
    return NULL;
}

/**
 * Returns the error message that was set to be displayed with
 * {@link #setError}, or <code>NULL</code> if no error was set
 * or if it the error was cleared by the widget after user input.
 */
AutoPtr<ICharSequence> TextView::GetError()
{
    return mError;
}

/**
 * Sets the right-hand compound drawable of the TextView to the "error"
 * icon and sets an error message that will be displayed in a popup when
 * the TextView has focus.  The icon and error message will be reset to
 * NULL when any key events cause changes to the TextView's text.  If the
 * <code>error</code> is <code>NULL</code>, the error message and icon
 * will be cleared.
 */
ECode TextView::SetError(
    /* [in] */ ICharSequence* error)
{
    if (error == NULL) {
        return SetError(NULL, NULL);
    }
    else {
        AutoPtr<IResources> resources;
        GetContext()->GetResources((IResources**)&resources);

        AutoPtr<IDrawable> dr;
        resources->GetDrawable(0x010801dd/*com.android.internal.R.drawable.indicator_input_error*/,
                (IDrawable**)&dr);

        Int32 width, height;
        dr->GetIntrinsicWidth(&width);
        dr->GetIntrinsicHeight(&height);
        dr->SetBounds(0, 0, width, height);
        return SetError(error, dr);
    }
}

/**
 * Sets the right-hand compound drawable of the TextView to the specified
 * icon and sets an error message that will be displayed in a popup when
 * the TextView has focus.  The icon and error message will be reset to
 * NULL when any key events cause changes to the TextView's text.  The
 * drawable must already have had {@link IDrawable*#setBounds} set on it.
 * If the <code>error</code> is <code>NULL</code>, the error message will
 * be cleared (and you should provide a <code>NULL</code> icon as well).
 */
ECode TextView::SetError(
    /* [in] */ ICharSequence* error,
    /* [in] */ IDrawable* icon)
{
//    error = TextUtils.stringOrSpannedString(error);

    mError = error;
    mErrorWasChanged = TRUE;
    Drawables* dr = mDrawables;
    if (dr != NULL) {
        SetCompoundDrawables(dr->mDrawableLeft, dr->mDrawableTop,
                icon, dr->mDrawableBottom);
    }
    else {
        SetCompoundDrawables(NULL, NULL, icon, NULL);
    }

//    if (error == NULL) {
//        if (mPopup != NULL) {
//            if (mPopup.isShowing()) {
//                mPopup.dismiss();
//            }
//
//            mPopup = NULL;
//        }
//    }
//    else {
//        if (isFocused()) {
//            showError();
//        }
//    }

    return NOERROR;
}

void TextView::ShowError()
{
    if (GetWindowToken() == NULL) {
        mShowErrorAfterAttach = TRUE;
        return;
    }

    //if (mPopup == NULL) {
    //    LayoutInflater inflater = LayoutInflater.from(getContext());
    //    final TextView err = (TextView) inflater.inflate(com.android.internal.R.layout.textview_hint,
    //            NULL);

    //    final Float scale = getResources().getDisplayMetrics().density;
    //    mPopup = new ErrorPopup(err, (Int32) (200 * scale + 0.5f),
    //            (Int32) (50 * scale + 0.5f));
    //    mPopup.setFocusable(FALSE);
    //    // The user is entering text, so the input method is needed.  We
    //    // don't want the popup to be displayed on top of it.
    //    mPopup.setInputMethodMode(PopupWindow.INPUT_METHOD_NEEDED);
    //}

    //TextView tv = (TextView) mPopup.getContentView();
    //chooseSize(mPopup, mError, tv);
    //tv.setText(mError);

    //mPopup.showAsDropDown(this, getErrorX(), getErrorY());
    //mPopup.fixDirection(mPopup.isAboveAnchor());
}

/**
 * Returns the Y offset to make the pointy top of the error point
 * at the middle of the error icon.
 */
Int32 TextView::GetErrorX()
{
    /*
     * The "25" is the distance between the point and the right edge
     * of the background
     */
    AutoPtr<IResources> resources;
    GetContext()->GetResources((IResources**)&resources);

    AutoPtr<IDisplayMetrics> metrics;
    resources->GetDisplayMetrics((IDisplayMetrics**)&metrics);

    Float scale = ((CDisplayMetrics*)metrics.Get())->mDensity;

    Drawables* dr = mDrawables;
    return GetWidth()/* - mPopup.getWidth()*/
            - GetPaddingRight()
            - (dr != NULL ? dr->mDrawableSizeRight : 0) / 2 + (Int32) (25 * scale + 0.5f);
}

/**
 * Returns the Y offset to make the pointy top of the error point
 * at the bottom of the error icon.
 */
Int32 TextView::GetErrorY()
{
    /*
     * Compound, not extended, because the icon is not clipped
     * if the text height is smaller.
     */
    Int32 vspace = mBottom - mTop -
                    GetCompoundPaddingBottom() - GetCompoundPaddingTop();

    Drawables* dr = mDrawables;
    Int32 icontop = GetCompoundPaddingTop()
            + (vspace - (dr != NULL ? dr->mDrawableHeightRight : 0)) / 2;

    /*
     * The "2" is the distance between the point and the top edge
     * of the background.
     */

    return icontop + (dr != NULL ? dr->mDrawableHeightRight : 0)
            - GetHeight() - 2;
}

void TextView::HideError()
{
    /*if (mPopup != NULL) {
        if (mPopup.isShowing()) {
            mPopup.dismiss();
        }
    }*/

    mShowErrorAfterAttach = FALSE;
}

//void TextView::ChooseSize(
//    /* [in] */ PopupWindow pop,
//    /* [in] */ CharSequence text,
//    /* [in] */ TextView tv)
//{
//    Int32 wid = tv.getPaddingLeft() + tv.getPaddingRight();
//    Int32 ht = tv.getPaddingTop() + tv.getPaddingBottom();
//
//    /*
//        * Figure out how big the text would be if we laid it out to the
//        * full width of this view minus the border.
//        */
//    Int32 cap = getWidth() - wid;
//    if (cap < 0) {
//        cap = 200; // We must not be measured yet -- setFrame() will fix it.
//    }
//
//    Layout l = new StaticLayout(text, tv.getPaint(), cap,
//                                Alignment_ALIGN_NORMAL, 1, 0, TRUE);
//    Float max = 0;
//    for (Int32 i = 0; i < l.getLineCount(); i++) {
//        max = Math.max(max, l.getLineWidth(i));
//    }
//
//    /*
//        * Now set the popup size to be big enough for the text plus the border.
//        */
//    pop.setWidth(wid + (Int32) Math.ceil(max));
//    pop.setHeight(ht + l.getHeight());
//}

Boolean TextView::SetFrame(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    Boolean result = View::SetFrame(l, t, r, b);

    /*if (mPopup != NULL) {
        TextView tv = (TextView) mPopup.getContentView();
        chooseSize(mPopup, mError, tv);
        mPopup.update(this, getErrorX(), getErrorY(),
                        mPopup.getWidth(), mPopup.getHeight());
    }*/

    RestartMarqueeIfNeeded();

    return result;
}

void TextView::RestartMarqueeIfNeeded()
{
//    if (mRestartMarquee && mEllipsize == TextUtils.TruncateAt.MARQUEE) {
//        mRestartMarquee = FALSE;
//        startMarquee();
//    }
}

ECode TextView::SetFilters(
    /* [in] */ IObjectContainer* filters)
{
    if (filters == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException();
    }
    mFilters = filters;
    if (mText && mText->Probe(EIID_IEditable) != NULL) {
        return SetFilters(IEditable::Probe(mText), filters);
    }

    return NOERROR;
}

ECode TextView::SetFilters(
    /* [in] */ IEditable* e,
    /* [in] */ IObjectContainer* filters)
{
    if (mInput && mInput->Probe(EIID_IInputFilter) != NULL) {
        AutoPtr<IObjectContainer> nf;
        CObjectContainer::New((IObjectContainer**)&nf);
        AutoPtr<IObjectEnumerator> enumerator;
        filters->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
        Boolean hasNext;
        while(enumerator->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInputFilter> obj;
            enumerator->Current((IInterface**)&obj);
            nf->Add(obj);
        }
        nf->Add((IInputFilter*)mInput->Probe(EIID_IInputFilter));
        return e->SetFilters(nf);
    }
    else {
        return e->SetFilters(filters);
    }
}

AutoPtr<IObjectContainer> TextView::GetFilters()
{
   return mFilters;
}

/////////////////////////////////////////////////////////////////////////

Int32 TextView::GetVerticalOffset(
    /* [in] */ Boolean forceNormal)
{
    Int32 voffset = 0;
    Int32 gravity = mGravity & Gravity_VERTICAL_GRAVITY_MASK;

    Int32 len;
    mText->GetLength(&len);
    AutoPtr<ILayout> l = mLayout;
    if (!forceNormal && len == 0 && mHintLayout != NULL) {
        l = mHintLayout;
    }

    if (gravity != Gravity_TOP) {
        Int32 boxht;

        if (l == mHintLayout) {
            boxht = GetMeasuredHeight() - GetCompoundPaddingTop() -
                    GetCompoundPaddingBottom();
        }
        else {
            boxht = GetMeasuredHeight() - GetExtendedPaddingTop() -
                    GetExtendedPaddingBottom();
        }
        Int32 textht;
        l->GetHeight(&textht);

        if (textht < boxht) {
            if (gravity == Gravity_BOTTOM) {
                voffset = boxht - textht;
            }
            else { // (gravity == Gravity.CENTER_VERTICAL)
                voffset = (boxht - textht) >> 1;
            }
        }
    }
    return voffset;
}

Int32 TextView::GetBottomVerticalOffset(
    /* [in] */ Boolean forceNormal)
{
    Int32 voffset = 0;
    Int32 gravity = mGravity & Gravity_VERTICAL_GRAVITY_MASK;

    Int32 len;
    mText->GetLength(&len);
    AutoPtr<ILayout> l = mLayout;
    if (!forceNormal && len == 0 && mHintLayout != NULL) {
        l = mHintLayout;
    }

    if (gravity != Gravity_BOTTOM) {
        Int32 boxht;

        if (l == mHintLayout) {
            boxht = GetMeasuredHeight() - GetCompoundPaddingTop() -
                    GetCompoundPaddingBottom();
        }
        else {
            boxht = GetMeasuredHeight() - GetExtendedPaddingTop() -
                    GetExtendedPaddingBottom();
        }
        Int32 textht;
        l->GetHeight(&textht);

        if (textht < boxht) {
            if (gravity == Gravity_TOP) {
                voffset = boxht - textht;
            }
            else { // (gravity == Gravity.CENTER_VERTICAL)
                voffset = (boxht - textht) >> 1;
            }
        }
    }
    return voffset;
}

void TextView::InvalidateCursorPath()
{
    if (mHighlightPathBogus) {
        InvalidateCursor();
    }
    else {
        {
            Mutex::Autolock lock(&sTempRectLock);

            /*
                * The reason for this concern about the thickness of the
                * cursor and doing the floor/ceil on the coordinates is that
                * some EditTexts (notably textfields in the Browser) have
                * anti-aliased text where not all the characters are
                * necessarily at integer-multiple locations.  This should
                * make sure the entire cursor gets invalidated instead of
                * sometimes missing half a pixel.
                */

            Float thick;
            mTextPaint->GetStrokeWidth(&thick);
            thick = Math::Ceil(thick);
            if (thick < 1.0f) {
                thick = 1.0f;
            }

            thick /= 2;

            mHighlightPath->ComputeBounds(sTempRect, FALSE);

            Int32 left = GetCompoundPaddingLeft();
            Int32 top = GetExtendedPaddingTop() + GetVerticalOffset(TRUE);

            Invalidate((Int32)Math::Floor(left + sTempRect->mLeft - thick),
                        (Int32)Math::Floor(top + sTempRect->mTop - thick),
                        (Int32)Math::Ceil(left + sTempRect->mRight + thick),
                        (Int32)Math::Ceil(top + sTempRect->mBottom + thick));
        }
    }
}

void TextView::InvalidateCursor()
{
    Int32 where = GetSelectionEnd();

    InvalidateCursor(where, where, where);
}

void TextView::InvalidateCursor(
    /* [in] */ Int32 a,
    /* [in] */ Int32 b,
    /* [in] */ Int32 c)
{
    if (mLayout == NULL) {
        Invalidate();
    }
    else {
        if (a >= 0 || b >= 0 || c >= 0) {
            Int32 first = Math::Min(Math::Min(a, b), c);
            Int32 last = Math::Max(Math::Max(a, b), c);

            Int32 line, top;
            mLayout->GetLineForOffset(first, &line);
            mLayout->GetLineTop(line, &top);

            // This is ridiculous, but the descent from the line above
            // can hang down into the line we really want to redraw,
            // so we have to invalidate part of the line above to make
            // sure everything that needs to be redrawn really is.
            // (But not the whole line above, because that would cause
            // the same problem with the descenders on the line above it!)
            if (line > 0) {
                Int32 descent;
                mLayout->GetLineDescent(line - 1, &descent);
                top -= descent;
            }

            Int32 line2;

            if (first == last) {
                line2 = line;
            }
            else {
                mLayout->GetLineForOffset(last, &line2);
            }

            Int32 bottom, voffset;
            mLayout->GetLineTop(line2 + 1, &bottom);
            voffset = GetVerticalOffset(TRUE);

            Int32 left = GetCompoundPaddingLeft() + mScrollX;
            Invalidate(left, top + voffset + GetExtendedPaddingTop(),
                    left + GetWidth() - GetCompoundPaddingLeft() -
                    GetCompoundPaddingRight(),
                    bottom + voffset + GetExtendedPaddingTop());
        }
    }
}

void TextView::RegisterForPreDraw()
{
    AutoPtr<IViewTreeObserver> observer = GetViewTreeObserver();
    if (observer == NULL) {
        return;
    }

    if (mPreDrawState == PREDRAW_NOT_REGISTERED) {
        observer->AddOnPreDrawListener((IOnPreDrawListener*)this->Probe(EIID_IOnPreDrawListener));
        mPreDrawState = PREDRAW_PENDING;
    }
    else if (mPreDrawState == PREDRAW_DONE) {
        mPreDrawState = PREDRAW_PENDING;
    }

    // else state is PREDRAW_PENDING, so keep waiting.
}

/**
 * {@inheritDoc}
 */
Boolean TextView::OnPreDraw()
{
    if (mPreDrawState != PREDRAW_PENDING) {
        return TRUE;
    }

    if (mLayout == NULL) {
        AssumeLayout();
    }

    Boolean changed = FALSE;

    SelectionModifierCursorController* selectionController = NULL;
    if (mSelectionModifierCursorController != NULL) {
        selectionController = (SelectionModifierCursorController*)
            mSelectionModifierCursorController.Get();
    }


    if (mMovement != NULL) {
        /* This code also provides auto-scrolling when a cursor is moved using a
         * CursorController (insertion point or selection limits).
         * For selection, ensure start or end is visible depending on controller's state.
         */
        Int32 curs = GetSelectionEnd();
        if (selectionController != NULL &&
            selectionController->IsSelectionStartDragged()) {
            curs = GetSelectionStart();
        }

        /*
         * TODO: This should really only keep the end in view if
         * it already was before the text changed.  I'm not sure
         * of a good way to tell from here if it was.
         */
        if (curs < 0 && (mGravity & Gravity_VERTICAL_GRAVITY_MASK) == Gravity_BOTTOM) {
            mText->GetLength(&curs);
        }

        if (curs >= 0) {
            changed = BringPointIntoView(curs);
        }
    }
    else {
        changed = BringTextIntoView();
    }

    // This has to be checked here since:
    // - onFocusChanged cannot start it when focus is given to a view with selected text (after
    //   a screen rotation) since layout is not yet initialized at that point.
    // - ExtractEditText does not call onFocus when it is displayed. Fixing this issue would
    //   allow to test for hasSelection in onFocusChanged, which would trigger a
    //   startTextSelectionMode here. TODO
    if (mCreatedWithASelection /*||
        (this instanceof ExtractEditText && selectionController != NULL && HasSelection())*/) {
        StartTextSelectionMode();
        mCreatedWithASelection = FALSE;
    }

    mPreDrawState = PREDRAW_DONE;
    return !changed;
}

void TextView::OnAttachedToWindow()
{
    View::OnAttachedToWindow();

    mTemporaryDetach = FALSE;

    if (mShowErrorAfterAttach) {
        ShowError();
        mShowErrorAfterAttach = FALSE;
    }

    AutoPtr<IViewTreeObserver> observer = GetViewTreeObserver();
    if (observer != NULL) {
        // if (mInsertionPointCursorController != NULL) {
        //     observer->AddOnTouchModeChangeListener(
        //         (IOnTouchModeChangeListener*)(InsertionPointCursorController*)
        //         mInsertionPointCursorController);
        // }
        // if (mSelectionModifierCursorController != NULL) {
        //     observer->AddOnTouchModeChangeListener(
        //         (IOnTouchModeChangeListener*)(SelectionModifierCursorController*)
        //         mSelectionModifierCursorController);
        // }
    }
}

void TextView::OnDetachedFromWindow()
{
    View::OnDetachedFromWindow();

    AutoPtr<IViewTreeObserver> observer = GetViewTreeObserver();
    if (observer.Get() != NULL) {
        if (mPreDrawState != PREDRAW_NOT_REGISTERED) {
            observer->RemoveOnPreDrawListener((IOnPreDrawListener*)this->Probe(EIID_IOnPreDrawListener));
            mPreDrawState = PREDRAW_NOT_REGISTERED;
        }
//        if (mInsertionPointCursorController != NULL) {
//            observer.removeOnTouchModeChangeListener(mInsertionPointCursorController);
//        }
//        if (mSelectionModifierCursorController != NULL) {
//            observer.removeOnTouchModeChangeListener(mSelectionModifierCursorController);
//        }
    }

    if (mError != NULL) {
        HideError();
    }

    if (mBlink != NULL) {
        mBlink->Cancel();
    }

    if (mInsertionPointCursorController != NULL) {
        mInsertionPointCursorController->OnDetached();
    }

    if (mSelectionModifierCursorController != NULL) {
        mSelectionModifierCursorController->OnDetached();
    }

    HideControllers();
}

Boolean TextView::IsPaddingOffsetRequired()
{
    return mShadowRadius != 0 || mDrawables != NULL;
}

Int32 TextView::GetLeftPaddingOffset()
{
    return GetCompoundPaddingLeft() - mPaddingLeft +
            (Int32)Math::Min(0.f, mShadowDx - mShadowRadius);
}

Int32 TextView::GetTopPaddingOffset()
{
    return (Int32)Math::Min(0.f, mShadowDy - mShadowRadius);
}

Int32 TextView::GetBottomPaddingOffset()
{
    return (Int32)Math::Max(0.f, mShadowDy + mShadowRadius);
}

Int32 TextView::GetRightPaddingOffset()
{
    return -(GetCompoundPaddingRight() - mPaddingRight) +
            (Int32)Math::Max(0.f, mShadowDx + mShadowRadius);
}

Boolean TextView::VerifyDrawable(
    /* [in] */ IDrawable* who)
{
    Boolean verified = View::VerifyDrawable(who);
    if (!verified && mDrawables != NULL) {
        return who == mDrawables->mDrawableLeft || who == mDrawables->mDrawableTop ||
                who == mDrawables->mDrawableRight || who == mDrawables->mDrawableBottom;
    }
    return verified;
}

ECode TextView::InvalidateDrawable(
    /* [in] */ IDrawable* drawable)
{
    if (VerifyDrawable(drawable)) {
        AutoPtr<IRect> dirty;
        drawable->GetBounds((IRect**)&dirty);

        Int32 scrollX = mScrollX;
        Int32 scrollY = mScrollY;

        // IMPORTANT: The coordinates below are based on the coordinates computed
        // for each compound drawable in onDraw(). Make sure to update each section
        // accordingly.
        Drawables* drawables = mDrawables;
        if (drawables != NULL) {
            if (drawable == drawables->mDrawableLeft) {
                Int32 compoundPaddingTop = GetCompoundPaddingTop();
                Int32 compoundPaddingBottom = GetCompoundPaddingBottom();
                Int32 vspace = mBottom - mTop - compoundPaddingBottom - compoundPaddingTop;

                scrollX += mPaddingLeft;
                scrollY += compoundPaddingTop + (vspace - drawables->mDrawableHeightLeft) / 2;
            }
            else if (drawable == drawables->mDrawableRight) {
                Int32 compoundPaddingTop = GetCompoundPaddingTop();
                Int32 compoundPaddingBottom = GetCompoundPaddingBottom();
                Int32 vspace = mBottom - mTop - compoundPaddingBottom - compoundPaddingTop;

                scrollX += (mRight - mLeft - mPaddingRight - drawables->mDrawableSizeRight);
                scrollY += compoundPaddingTop + (vspace - drawables->mDrawableHeightRight) / 2;
            }
            else if (drawable == drawables->mDrawableTop) {
                Int32 compoundPaddingLeft = GetCompoundPaddingLeft();
                Int32 compoundPaddingRight = GetCompoundPaddingRight();
                Int32 hspace = mRight - mLeft - compoundPaddingRight - compoundPaddingLeft;

                scrollX += compoundPaddingLeft + (hspace - drawables->mDrawableWidthTop) / 2;
                scrollY += mPaddingTop;
            }
            else if (drawable == drawables->mDrawableBottom) {
                Int32 compoundPaddingLeft = GetCompoundPaddingLeft();
                Int32 compoundPaddingRight = GetCompoundPaddingRight();
                Int32 hspace = mRight - mLeft - compoundPaddingRight - compoundPaddingLeft;

                scrollX += compoundPaddingLeft + (hspace - drawables->mDrawableWidthBottom) / 2;
                scrollY += (mBottom - mTop - mPaddingBottom - drawables->mDrawableSizeBottom);
            }
        }

        CRect* _dirty = (CRect*)dirty.Get();
        Invalidate(_dirty->mLeft + scrollX, _dirty->mTop + scrollY,
                _dirty->mRight + scrollX, _dirty->mBottom + scrollY);
    }
    return NOERROR;
}

void TextView::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    RestartMarqueeIfNeeded();

    // Draw the background for this view
    View::OnDraw(canvas);

    Int32 compoundPaddingLeft = GetCompoundPaddingLeft();
    Int32 compoundPaddingTop = GetCompoundPaddingTop();
    Int32 compoundPaddingRight = GetCompoundPaddingRight();
    Int32 compoundPaddingBottom = GetCompoundPaddingBottom();
    Int32 scrollX = mScrollX;
    Int32 scrollY = mScrollY;
    Int32 right = mRight;
    Int32 left = mLeft;
    Int32 bottom = mBottom;
    Int32 top = mTop;
    Int32 state;

    Drawables* dr = mDrawables;
    if (dr != NULL) {
        /*
         * Compound, not extended, because the icon is not clipped
         * if the text height is smaller.
         */

        Int32 vspace = bottom - top - compoundPaddingBottom - compoundPaddingTop;
        Int32 hspace = right - left - compoundPaddingRight - compoundPaddingLeft;

        // IMPORTANT: The coordinates computed are also used in invalidateDrawable()
        // Make sure to update invalidateDrawable() when changing this code.
        if (dr->mDrawableLeft != NULL) {
            canvas->Save(&state);
            canvas->Translate(scrollX + mPaddingLeft,
                                scrollY + compoundPaddingTop +
                                (vspace - dr->mDrawableHeightLeft) / 2);
            dr->mDrawableLeft->Draw(canvas);
            canvas->Restore();
        }

        // IMPORTANT: The coordinates computed are also used in invalidateDrawable()
        // Make sure to update invalidateDrawable() when changing this code.
        if (dr->mDrawableRight != NULL) {
            canvas->Save(&state);
            canvas->Translate(scrollX + right - left - mPaddingRight - dr->mDrawableSizeRight,
                        scrollY + compoundPaddingTop + (vspace - dr->mDrawableHeightRight) / 2);
            dr->mDrawableRight->Draw(canvas);
            canvas->Restore();
        }

        // IMPORTANT: The coordinates computed are also used in invalidateDrawable()
        // Make sure to update invalidateDrawable() when changing this code.
        if (dr->mDrawableTop != NULL) {
            canvas->Save(&state);
            canvas->Translate(scrollX + compoundPaddingLeft + (hspace - dr->mDrawableWidthTop) / 2,
                    scrollY + mPaddingTop);
            dr->mDrawableTop->Draw(canvas);
            canvas->Restore();
        }

        // IMPORTANT: The coordinates computed are also used in invalidateDrawable()
        // Make sure to update invalidateDrawable() when changing this code.
        if (dr->mDrawableBottom != NULL) {
            canvas->Save(&state);
            canvas->Translate(scrollX + compoundPaddingLeft +
                    (hspace - dr->mDrawableWidthBottom) / 2,
                        scrollY + bottom - top - mPaddingBottom - dr->mDrawableSizeBottom);
            dr->mDrawableBottom->Draw(canvas);
            canvas->Restore();
        }
    }

    if (mPreDrawState == PREDRAW_DONE) {
        AutoPtr<IViewTreeObserver> observer = GetViewTreeObserver();
        if (observer != NULL) {
            observer->RemoveOnPreDrawListener((IOnPreDrawListener*)this->Probe(EIID_IOnPreDrawListener));
            mPreDrawState = PREDRAW_NOT_REGISTERED;
        }
    }

    Int32 color = mCurTextColor;

    if (mLayout == NULL) {
        AssumeLayout();
    }

    AutoPtr<ILayout> layout = mLayout;
    Int32 cursorcolor = color;

    Int32 len;
    if (mHint != NULL && (mText->GetLength(&len), len) == 0) {
        if (mHintTextColor != NULL) {
            color = mCurHintTextColor;
        }

        layout = mHintLayout;
    }

    mTextPaint->SetColor(color);
    ((CTextPaint*)mTextPaint.Get())->mDrawableState = GetDrawableState()->Clone();

    canvas->Save(&state);
    /*  Would be faster if we didn't have to do this. Can we chop the
        (displayable) text so that we don't need to do this ever?
    */

    Int32 extendedPaddingTop = GetExtendedPaddingTop();
    Int32 extendedPaddingBottom = GetExtendedPaddingBottom();

    Float clipLeft = compoundPaddingLeft + scrollX;
    Float clipTop = extendedPaddingTop + scrollY;
    Float clipRight = right - left - compoundPaddingRight + scrollX;
    Float clipBottom = bottom - top - extendedPaddingBottom + scrollY;

    if (mShadowRadius != 0) {
        clipLeft += Math::Min(0.f, mShadowDx - mShadowRadius);
        clipRight += Math::Max(0.f, mShadowDx + mShadowRadius);

        clipTop += Math::Min(0.f, mShadowDy - mShadowRadius);
        clipBottom += Math::Max(0.f, mShadowDy + mShadowRadius);
    }

    Boolean isNonEmpty;
    canvas->ClipRectEx5(clipLeft, clipTop, clipRight, clipBottom, &isNonEmpty);

    Int32 voffsetText = 0;
    Int32 voffsetCursor = 0;

    // translate in by our padding
    {
        /* shortcircuit calling getVerticaOffset() */
        if ((mGravity & Gravity_VERTICAL_GRAVITY_MASK) != Gravity_TOP) {
            voffsetText = GetVerticalOffset(FALSE);
            voffsetCursor = GetVerticalOffset(TRUE);
        }
        canvas->Translate(compoundPaddingLeft, extendedPaddingTop + voffsetText);
    }

//    if (mEllipsize == TextUtils.TruncateAt.MARQUEE) {
//        if (!mSingleLine && getLineCount() == 1 && canMarquee() &&
//                (mGravity & Gravity.HORIZONTAL_GRAVITY_MASK) != Gravity.LEFT) {
//            canvas.translate(mLayout->getLineRight(0) - (mRight - mLeft -
//                    getCompoundPaddingLeft() - getCompoundPaddingRight()), 0.0f);
//        }
//
//        if (mMarquee != NULL && mMarquee.isRunning()) {
//            canvas.translate(-mMarquee.mScroll, 0.0f);
//        }
//    }

    AutoPtr<IPath> highlight;
    Int32 selStart = -1, selEnd = -1;

    //  If there is no movement method, then there can be no selection.
    //  Check that first and attempt to skip everything having to do with
    //  the cursor.
    //  XXX This is not strictly TRUE -- a program could set the
    //  selection manually if it really wanted to.
    if (mMovement != NULL && (IsFocused() || IsPressed())) {
        selStart = GetSelectionStart();
        selEnd = GetSelectionEnd();

        if (mCursorVisible && selStart >= 0 && IsEnabled()) {
            if (mHighlightPath == NULL)
                CPath::New((IPath**)&mHighlightPath);

            if (selStart == selEnd) {
                if ((SystemClock::GetUptimeMillis() - mShowCursor) % (2 * BLINK) < BLINK) {
                    if (mHighlightPathBogus) {
                        mHighlightPath->Reset();
                        mLayout->GetCursorPath(selStart, mHighlightPath, mText);
                        mHighlightPathBogus = FALSE;
                    }

                    // XXX should pass to skin instead of drawing directly
                    mHighlightPaint->SetColor(cursorcolor);
                    mHighlightPaint->SetStyle(PaintStyle_STROKE);

                    highlight = mHighlightPath;
                }
            }
            else {
                if (mHighlightPathBogus) {
                    mHighlightPath->Reset();
                    mLayout->GetSelectionPath(selStart, selEnd, mHighlightPath);
                    mHighlightPathBogus = FALSE;
                }

                // XXX should pass to skin instead of drawing directly
                mHighlightPaint->SetColor(mHighlightColor);
                mHighlightPaint->SetStyle(PaintStyle_FILL);

                highlight = mHighlightPath;
            }
        }
    }

    /*  Comment out until we decide what to do about animations
    Boolean isLinearTextOn = FALSE;
    if (currentTransformation != NULL) {
        isLinearTextOn = mTextPaint.isLinearTextOn();
        Matrix m = currentTransformation.getMatrix();
        if (!m.isIdentity()) {
            // mTextPaint.setLinearTextOn(TRUE);
        }
    }
    */

    InputMethodState* ims = mInputMethodState;
    if (ims != NULL && ims->mBatchEditNesting == 0) {
        AutoPtr<ILocalInputMethodManager> imm = CLocalInputMethodManager::PeekInstance();
        if (imm != NULL) {
            Boolean ret = FALSE;
            imm->IsActive((IView*)this->Probe(EIID_IView), &ret);
            if (ret) {
                Boolean reported = FALSE;
                if (ims->mContentChanged || ims->mSelectionModeChanged) {
                    // We are in extract mode and the content has changed
                    // in some way... just report complete new text to the
                    // input method.
                    reported = ReportExtractedText();
                }
                if (!reported && highlight != NULL) {
                    Int32 candStart = -1;
                    Int32 candEnd = -1;
                    if (ISpannable::Probe(mText)) {
                       AutoPtr<ISpannable> sp = ISpannable::Probe(mText);
                       candStart = BaseInputConnection::GetComposingSpanStart(sp);
                       candEnd = BaseInputConnection::GetComposingSpanEnd(sp);
                    }
                    imm->UpdateSelection((IView*)this->Probe(EIID_IView),
                            selStart, selEnd, candStart, candEnd);
                }
            }

            Boolean isWatching = FALSE;
            imm->IsWatchingCursor((IView*)this->Probe(EIID_IView), &isWatching);
            if (isWatching && highlight != NULL) {
                highlight->ComputeBounds(ims->mTmpRectF, TRUE);
                (*ims->mTmpOffset)[0] = (*ims->mTmpOffset)[1] = 0;

                AutoPtr<IMatrix> mtx;
                canvas->GetMatrixEx((IMatrix**)&mtx);
                mtx->MapPointsEx2(*ims->mTmpOffset);
                ims->mTmpRectF->Offset((*ims->mTmpOffset)[0], (*ims->mTmpOffset)[1]);

                ims->mTmpRectF->Offset(0, voffsetCursor - voffsetText);

                CRectF* tmpRectF = (CRectF*)ims->mTmpRectF.Get();
                ims->mCursorRectInWindow->Set((Int32)(tmpRectF->mLeft + 0.5),
                       (Int32)(tmpRectF->mTop + 0.5),
                       (Int32)(tmpRectF->mRight + 0.5),
                       (Int32)(tmpRectF->mBottom + 0.5));

                CRect* tmpRect = (CRect*)ims->mCursorRectInWindow.Get();
                imm->UpdateCursor((IView*)this->Probe(EIID_IView),
                       tmpRect->mLeft, tmpRect->mTop,
                       tmpRect->mRight, tmpRect->mBottom);
            }
        }
    }

    layout->DrawEx(canvas, highlight, mHighlightPaint, voffsetCursor - voffsetText);

    //if (mMarquee != NULL && mMarquee.shouldDrawGhost()) {
    //    canvas.translate((Int32) mMarquee.getGhostOffset(), 0.0f);
    //    layout.draw(canvas, highlight, mHighlightPaint, voffsetCursor - voffsetText);
    //}

    /*  Comment out until we decide what to do about animations
    if (currentTransformation != NULL) {
        mTextPaint.setLinearTextOn(isLinearTextOn);
    }
    */

    canvas->Restore();

    UpdateCursorControllerPositions();
}

/**
 * Update the positions of the CursorControllers.  Needed by WebTextView,
 * which does not draw.
 * @hide
 */
void TextView::UpdateCursorControllerPositions()
{
    if (mInsertionPointCursorController != NULL &&
        mInsertionPointCursorController->IsShowing()) {
        mInsertionPointCursorController->UpdatePosition();
    }

    if (mSelectionModifierCursorController != NULL &&
        mSelectionModifierCursorController->IsShowing()) {
        mSelectionModifierCursorController->UpdatePosition();
    }
}

ECode TextView::GetFocusedRect(
    /* [in] */ IRect* r)
{
    if (mLayout == NULL) {
        return View::GetFocusedRect(r);
    }

    Int32 sel = GetSelectionEnd();
    if (sel < 0) {
        GetFocusedRect(r);
        return NOERROR;
    }

    Int32 line;
    CRect* _r = (CRect*)r;
    mLayout->GetLineForOffset(sel, &line);
    mLayout->GetLineTop(line, &_r->mTop);
    mLayout->GetLineBottom(line, &_r->mBottom);

    Float pos;
    mLayout->GetPrimaryHorizontal(sel, &pos);
    _r->mLeft = (Int32)pos;
    _r->mRight = _r->mLeft + 1;

    // Adjust for padding and gravity.
    Int32 paddingLeft = GetCompoundPaddingLeft();
    Int32 paddingTop = GetExtendedPaddingTop();
    if ((mGravity & Gravity_VERTICAL_GRAVITY_MASK) != Gravity_TOP) {
        paddingTop += GetVerticalOffset(FALSE);
    }
    r->Offset(paddingLeft, paddingTop);

    return NOERROR;
}

/**
 * Return the number of lines of text, or 0 if the internal Layout has not
 * been built.
 */
Int32 TextView::GetLineCount()
{
    Int32 count = 0;
    if (mLayout != NULL) mLayout->GetLineCount(&count);
    return count;
}

/**
 * Return the baseline for the specified line (0...getLineCount() - 1)
 * If bounds is not NULL, return the top, left, right, bottom extents
 * of the specified line in it. If the internal Layout has not been built,
 * return 0 and set bounds to (0, 0, 0, 0)
 * @param line which line to examine (0..getLineCount() - 1)
 * @param bounds Optional. If not NULL, it returns the extent of the line
 * @return the Y-coordinate of the baseline
 */
Int32 TextView::GetLineBounds(
    /* [in] */ Int32 line,
    /* [in] */ IRect* bounds)
{
    if (mLayout == NULL) {
        if (bounds != NULL) {
            bounds->Set(0, 0, 0, 0);
        }
        return 0;
    }
    else {
        Int32 baseline;
        mLayout->GetLineBounds(line, bounds, &baseline);

        Int32 voffset = GetExtendedPaddingTop();
        if ((mGravity & Gravity_VERTICAL_GRAVITY_MASK) != Gravity_TOP) {
            voffset += GetVerticalOffset(TRUE);
        }
        if (bounds != NULL) {
            bounds->Offset(GetCompoundPaddingLeft(), voffset);
        }
        return baseline + voffset;
    }
}

ECode TextView::GetBaseline(
    /* [out] */ Int32* baseline)
{
    assert(baseline != NULL);

    if (mLayout == NULL) {
        return View::GetBaseline(baseline);
    }

    Int32 voffset = 0;
    if ((mGravity & Gravity_VERTICAL_GRAVITY_MASK) != Gravity_TOP) {
        voffset = GetVerticalOffset(TRUE);
    }

    mLayout->GetLineBaseline(0, baseline);
    *baseline += GetExtendedPaddingTop() + voffset;

    return NOERROR;
}

Boolean TextView::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Int32 which = DoKeyDown(keyCode, event, NULL);
    if (which == 0) {
        // Go through default dispatching.
        return View::OnKeyDown(keyCode, event);
    }

    return TRUE;
}

Boolean TextView::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 repeatCount,
    /* [in] */ IKeyEvent* event)
{
    AutoPtr<IKeyEvent> down;
    CKeyEvent::ChangeAction(event, KeyEvent_ACTION_DOWN, (IKeyEvent**)&down);

    Int32 which = DoKeyDown(keyCode, down, event);
    if (which == 0) {
        // Go through default dispatching.
        return View::OnKeyMultiple(keyCode, repeatCount, event);
    }
    if (which == -1) {
        // Consumed the whole thing.
        return TRUE;
    }

    repeatCount--;

    // We are going to dispatch the remaining events to either the input
    // or movement method.  To do this, we will just send a repeated stream
    // of down and up events until we have done the complete repeatCount.
    // It would be nice if those interfaces had an onKeyMultiple() method,
    // but adding that is a more complicated change.
    AutoPtr<IKeyEvent> up;
    CKeyEvent::ChangeAction(event, KeyEvent_ACTION_UP, (IKeyEvent**)&up);

    if (which == 1) {
        Boolean res;
        mInput->OnKeyUp(
            (IView*)this->Probe(EIID_IView), IEditable::Probe(mText),
            keyCode, up, &res);
        while (--repeatCount > 0) {
            mInput->OnKeyDown(
                (IView*)this->Probe(EIID_IView), IEditable::Probe(mText),
                keyCode, down, &res);
            mInput->OnKeyUp(
                (IView*)this->Probe(EIID_IView), IEditable::Probe(mText),
                keyCode, up, &res);
        }
        if (mError != NULL && !mErrorWasChanged) {
            SetError(NULL, NULL);
        }

    }
    else if (which == 2) {
        Boolean res;
        mMovement->OnKeyUp(
            (ITextView*)this->Probe(EIID_ITextView), ISpannable::Probe(mText),
            keyCode, up, &res);
        while (--repeatCount > 0) {
            mMovement->OnKeyDown(
                (ITextView*)this->Probe(EIID_ITextView), ISpannable::Probe(mText),
                keyCode, down, &res);
            mMovement->OnKeyUp(
                (ITextView*)this->Probe(EIID_ITextView), ISpannable::Probe(mText),
                keyCode, up, &res);
        }
    }

    return TRUE;
}

/**
 * Returns TRUE if pressing ENTER in this field advances focus instead
 * of inserting the character.  This is TRUE mostly in single-line fields,
 * but also in mail addresses and subjects which will display on multiple
 * lines but where it doesn't make sense to insert newlines.
 */
Boolean TextView::ShouldAdvanceFocusOnEnter()
{
    if (mInput == NULL) {
        return FALSE;
    }

    if (mSingleLine) {
        return TRUE;
    }

//    if ((mInputType & InputType_TYPE_MASK_CLASS) == InputType_TYPE_CLASS_TEXT) {
//        Int32 variation = mInputType & InputType_TYPE_MASK_VARIATION;
//
//        if (variation == InputType_TYPE_TEXT_VARIATION_EMAIL_ADDRESS ||
//            variation == InputType_TYPE_TEXT_VARIATION_EMAIL_SUBJECT) {
//            return TRUE;
//        }
//    }

    return FALSE;
}

Int32 TextView::DoKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ IKeyEvent* otherEvent)
{
    if (!IsEnabled()) {
        return 0;
    }

    switch (keyCode) {
        case KeyEvent_KEYCODE_ENTER:
            mEnterKeyIsDown = TRUE;
            // If ALT modifier is held, then we always insert a
            // newline character.
            Int32 metaState;
            event->GetMetaState(&metaState);
            if ((metaState & KeyEvent_META_ALT_ON) == 0) {

                // When mInputContentType is set, we know that we are
                // running in a "modern" cupcake environment, so don't need
                // to worry about the application trying to capture
                // enter key events.
                //if (mInputContentType != NULL) {
                //
                //    // If there is an action listener, given them a
                //    // chance to consume the event.
                //    if (mInputContentType.onEditorActionListener != NULL &&
                //            mInputContentType.onEditorActionListener.onEditorAction(
                //            this, EditorInfo_IME_NULL, event)) {
                //        mInputContentType.enterDown = TRUE;
                //        // We are consuming the enter key for them.
                //        return -1;
                //    }
                //}

                // If our editor should move focus when enter is pressed, or
                // this is a generated event from an IME action button, then
                // don't let it be inserted into the text.
                Int32 flag;
                event->GetFlags(&flag);
                if ((flag & KeyEvent_FLAG_EDITOR_ACTION) != 0
                        || ShouldAdvanceFocusOnEnter()) {
                    return -1;
                }
            }
            break;

        case KeyEvent_KEYCODE_DPAD_CENTER:
            mDPadCenterIsDown = TRUE;
            if (ShouldAdvanceFocusOnEnter()) {
                return 0;
            }
            break;

            // Has to be done on key down (and not on key up) to correctly be intercepted.
        case KeyEvent_KEYCODE_BACK:
            if (mIsInTextSelectionMode) {
                StopTextSelectionMode();
                return -1;
            }
            break;
    }

    if (mInput != NULL) {
        /*
         * Keep track of what the error was before doing the input
         * so that if an input filter changed the error, we leave
         * that error showing.  Otherwise, we take down whatever
        * error was showing when the user types something.
        */
        mErrorWasChanged = FALSE;

        Boolean doDown = TRUE;
        if (otherEvent != NULL) {
            BeginBatchEdit();
            Boolean handled;
            mInput->OnKeyOther(
                (IView*)this->Probe(EIID_IView), IEditable::Probe(mText),
                otherEvent, &handled);
            if (mError != NULL && !mErrorWasChanged) {
                SetError(NULL, NULL);
            }
            doDown = FALSE;
            if (handled) {
                EndBatchEdit();
                return -1;
            }

            EndBatchEdit();
        }

        if (doDown) {
            BeginBatchEdit();
            Boolean res;
            mInput->OnKeyDown(
                (IView*)this->Probe(EIID_IView), IEditable::Probe(mText),
                keyCode, event, &res);
            if (res) {
                EndBatchEdit();
                if (mError != NULL && !mErrorWasChanged) {
                    SetError(NULL, NULL);
                }
                return 1;
            }
            EndBatchEdit();
        }
    }

    // bug 650865: sometimes we get a key event before a layout.
    // don't try to move around if we don't know the layout.

    if (mMovement != NULL && mLayout != NULL) {
        Boolean doDown = TRUE;
        if (otherEvent != NULL) {
            Boolean handled;
            mMovement->OnKeyOther(
                (ITextView*)this->Probe(EIID_ITextView), ISpannable::Probe(mText),
                otherEvent, &handled);
            doDown = FALSE;
            if (handled) {
                return -1;
            }
        }

        if (doDown) {
            Boolean res;
            mMovement->OnKeyDown(
                (ITextView*)this->Probe(EIID_ITextView), ISpannable::Probe(mText),
                keyCode, event, &res);
            if (res) {
                return 2;
            }
        }
    }

    return 0;
}

Boolean TextView::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (!IsEnabled()) {
        return View::OnKeyUp(keyCode, event);
    }

    HideControllers();
    StopTextSelectionMode();

    switch (keyCode) {
        case KeyEvent_KEYCODE_DPAD_CENTER:
            mDPadCenterIsDown = FALSE;
            /*
                * If there is a click listener, just call through to
                * super, which will invoke it.
                *
                * If there isn't a click listener, try to show the soft
                * input method.  (It will also
                * call performClick(), but that won't do anything in
                * this case.)
                */
            if (mOnClickListener == NULL) {
                if (mMovement != NULL && IEditable::Probe(mText) != NULL
                        && mLayout != NULL && OnCheckIsTextEditor()) {
                    AutoPtr<ILocalInputMethodManager> imm;
                    GetContext()->GetSystemService(Context_INPUT_METHOD_SERVICE,
                            (IInterface**)&imm);
                    Boolean ret = FALSE;
                    imm->ShowSoftInput((IView*)this->Probe(EIID_IView), 0, &ret);
                }
            }
            return View::OnKeyUp(keyCode, event);

        case KeyEvent_KEYCODE_ENTER:
            mEnterKeyIsDown = FALSE;
            /*if (mInputContentType != NULL
                    && mInputContentType.onEditorActionListener != NULL
                    && mInputContentType.enterDown) {
                mInputContentType.enterDown = FALSE;
                if (mInputContentType.onEditorActionListener.onEditorAction(
                        this, EditorInfo_IME_NULL, event)) {
                    return TRUE;
                }
            }*/

            Int32 flags;
            event->GetFlags(&flags);
            if ((flags & KeyEvent_FLAG_EDITOR_ACTION) != 0
                    || ShouldAdvanceFocusOnEnter()) {
                /*
                    * If there is a click listener, just call through to
                    * super, which will invoke it.
                    *
                    * If there isn't a click listener, try to advance focus,
                    * but still call through to super, which will reset the
                    * pressed state and longpress state.  (It will also
                    * call performClick(), but that won't do anything in
                    * this case.)
                    */
                if (mOnClickListener == NULL) {
                    AutoPtr<IView> v = FocusSearch(FOCUS_DOWN);

                    if (v != NULL) {
                        Boolean focus = FALSE;
                        v->RequestFocusEx(FOCUS_DOWN, &focus);
                        if (!focus) {
                            assert(0);
                            // throw new IllegalStateException("focus search returned a view " +
                            //         "that wasn't able to take focus!");
                        }

                        /*
                           * Return TRUE because we handled the key; super
                           * will return FALSE because there was no click
                           * listener.
                           */
                        View::OnKeyUp(keyCode, event);
                        return TRUE;
                    }
                    else if ((flags & KeyEvent_FLAG_EDITOR_ACTION) != 0) {
                        // No target for next focus, but make sure the IME
                        // if this came from it.
                        AutoPtr<ILocalInputMethodManager> imm =
                                CLocalInputMethodManager::PeekInstance();
                        if (imm != NULL) {
                            Boolean ret = FALSE;
                            imm->HideSoftInputFromWindow(GetWindowToken(), 0, &ret);
                        }
                    }
                }

                return View::OnKeyUp(keyCode, event);
            }
            break;
    }

    if (mInput != NULL) {
        Boolean res;
        mInput->OnKeyUp(
            (IView*)this->Probe(EIID_IView), IEditable::Probe(mText),
            keyCode, event, &res);
        if (res)
            return TRUE;
    }

    if (mMovement != NULL && mLayout != NULL) {
        Boolean res;
        mMovement->OnKeyUp(
            (ITextView*)this->Probe(EIID_ITextView), ISpannable::Probe(mText),
            keyCode, event, &res);
        if (res)
            return TRUE;
    }

    return View::OnKeyUp(keyCode, event);
}

Boolean TextView::OnCheckIsTextEditor()
{
    return mInputType != InputType_TYPE_NULL;
}

AutoPtr<IInputConnection> TextView::OnCreateInputConnection(
   /* [in] */ IEditorInfo* outEditorInfoAttrs)
{
    if (OnCheckIsTextEditor()) {
        CEditorInfo* outAttrs = (CEditorInfo*)outEditorInfoAttrs;
        assert(outAttrs != NULL);
        if (mInputMethodState == NULL) {
            mInputMethodState = new InputMethodState();
        }
        outAttrs->mInputType = mInputType;
        if (mInputContentType != NULL) {
           outAttrs->mImeOptions = mInputContentType->mImeOptions;
           outAttrs->mPrivateImeOptions = mInputContentType->mPrivateImeOptions;
           outAttrs->mActionLabel = mInputContentType->mImeActionLabel;
           outAttrs->mActionId = mInputContentType->mImeActionId;
           outAttrs->mExtras = mInputContentType->mExtras;
        }
        else {
            outAttrs->mImeOptions = EditorInfo_IME_NULL;
        }
        if ((outAttrs->mImeOptions & EditorInfo_IME_MASK_ACTION)
               == EditorInfo_IME_ACTION_UNSPECIFIED) {
            if (FocusSearch(FOCUS_DOWN) != NULL) {
               // An action has not been set, but the enter key will move to
               // the next focus, so set the action to that.
               outAttrs->mImeOptions |= EditorInfo_IME_ACTION_NEXT;
            }
            else {
               // An action has not been set, and there is no focus to move
               // to, so let's just supply a "done" action.
               outAttrs->mImeOptions |= EditorInfo_IME_ACTION_DONE;
            }
            if (!ShouldAdvanceFocusOnEnter()) {
               outAttrs->mImeOptions |= EditorInfo_IME_FLAG_NO_ENTER_ACTION;
            }
        }
        if ((outAttrs->mInputType & (InputType_TYPE_MASK_CLASS
               | InputType_TYPE_TEXT_FLAG_MULTI_LINE))
               == (InputType_TYPE_CLASS_TEXT
                       | InputType_TYPE_TEXT_FLAG_MULTI_LINE)) {
            // Multi-line text editors should always show an enter key.
            outAttrs->mImeOptions |= EditorInfo_IME_FLAG_NO_ENTER_ACTION;
        }
        outAttrs->mHintText = mHint;
        if (IEditable::Probe(mText) != NULL) {
            AutoPtr<IInputConnection> ic;
            CEditableInputConnection::New((ITextView*)this->Probe(EIID_ITextView), (IEditableInputConnection**)&ic);
            outAttrs->mInitialSelStart = GetSelectionStart();
            outAttrs->mInitialSelEnd = GetSelectionEnd();
            ic->GetCursorCapsMode(mInputType, &outAttrs->mInitialCapsMode);
            return ic;
        }
    }
    return NULL;
}

/**
 * If this TextView contains editable content, extract a portion of it
 * based on the information in <var>request</var> in to <var>outText</var>.
 * @return Returns TRUE if the text was successfully extracted, else FALSE.
 */
Boolean TextView::ExtractText(
    /* [in] */ IExtractedTextRequest* request,
    /* [in] */ IExtractedText* outText)
{
    return ExtractTextInternal(request, EXTRACT_UNKNOWN, EXTRACT_UNKNOWN,
            EXTRACT_UNKNOWN, outText);
}

Boolean TextView::ExtractTextInternal(
    /* [in] */ IExtractedTextRequest* request,
    /* [in] */ Int32 partialStartOffset,
    /* [in] */ Int32 partialEndOffset,
    /* [in] */ Int32 delta,
    /* [in] */ IExtractedText* outText)
{
    AutoPtr<ICharSequence> content = mText;
    if (content != NULL) {
        if (partialStartOffset != EXTRACT_NOTHING) {
            Int32 N;
            content->GetLength(&N);
            if (partialStartOffset < 0) {
//                outText.partialStartOffset = outText.partialEndOffset = -1;
                partialStartOffset = 0;
                partialEndOffset = N;
            }
            else {
                // Adjust offsets to ensure we contain full spans.
//                if (content instanceof Spanned) {
//                    Spanned spanned = (Spanned)content;
//                    Object[] spans = spanned.getSpans(partialStartOffset,
//                            partialEndOffset, ParcelableSpan.class);
//                    Int32 i = spans.length;
//                    while (i > 0) {
//                        i--;
//                        Int32 j = spanned.getSpanStart(spans[i]);
//                        if (j < partialStartOffset) partialStartOffset = j;
//                        j = spanned.getSpanEnd(spans[i]);
//                        if (j > partialEndOffset) partialEndOffset = j;
//                    }
//                }
//                outText.partialStartOffset = partialStartOffset;
//                outText.partialEndOffset = partialEndOffset;
                // Now use the delta to determine the actual amount of text
                // we need.
                partialEndOffset += delta;
                if (partialStartOffset > N) {
                    partialStartOffset = N;
                }
                else if (partialStartOffset < 0) {
                    partialStartOffset = 0;
                }
                if (partialEndOffset > N) {
                    partialEndOffset = N;
                }
                else if (partialEndOffset < 0) {
                    partialEndOffset = 0;
                }
            }
//            if ((request.flags&InputConnection.GET_TEXT_WITH_STYLES) != 0) {
//                outText.text = content.subSequence(partialStartOffset,
//                        partialEndOffset);
//            } else {
//                outText.text = TextUtils.substring(content, partialStartOffset,
//                        partialEndOffset);
//            }
        }
        else {
//            outText.partialStartOffset = 0;
//            outText.partialEndOffset = 0;
//            outText.text = "";
        }
//        outText.flags = 0;
//        if (MetaKeyKeyListener.getMetaState(mText, MetaKeyKeyListener.META_SELECTING) != 0) {
//            outText.flags |= ExtractedText.FLAG_SELECTING;
//        }
//        if (mSingleLine) {
//            outText.flags |= ExtractedText.FLAG_SINGLE_LINE;
//        }
//        outText.startOffset = 0;
//        outText.selectionStart = getSelectionStart();
//        outText.selectionEnd = getSelectionEnd();
        return TRUE;
    }
    return FALSE;
}

Boolean TextView::ReportExtractedText()
{
    InputMethodState* ims = mInputMethodState;
    if (ims != NULL) {
        Boolean contentChanged = ims->mContentChanged;
        if (contentChanged || ims->mSelectionModeChanged) {
            ims->mContentChanged = FALSE;
            ims->mSelectionModeChanged = FALSE;
            AutoPtr<IExtractedTextRequest> req = ims->mExtracting;
           if (req != NULL) {
                AutoPtr<ILocalInputMethodManager> imm = CLocalInputMethodManager::PeekInstance();
                if (imm != NULL) {
                    // if (DEBUG_EXTRACT) Log.v(LOG_TAG, "Retrieving extracted start="
                    //        + ims->mChangedStart + " end=" + ims->mChangedEnd
                    //        + " delta=" + ims->mChangedDelta);
                    if (ims->mChangedStart < 0 && !contentChanged) {
                        ims->mChangedStart = EXTRACT_NOTHING;
                    }
                    if (ExtractTextInternal(req, ims->mChangedStart, ims->mChangedEnd,
                           ims->mChangedDelta, ims->mTmpExtracted)) {
                        // if (DEBUG_EXTRACT) Log.v(LOG_TAG, "Reporting extracted start="
                        //        + ims->mTmpExtracted.partialStartOffset
                        //        + " end=" + ims->mTmpExtracted.partialEndOffset
                        //        + ": " + ims->mTmpExtracted.text);
                        imm->UpdateExtractedText((IView*)this->Probe(EIID_IView),
                               ((CExtractedTextRequest*)req.Get())->mToken,
                               mInputMethodState->mTmpExtracted);
                        return TRUE;
                    }
                }
            }
        }
    }
    return FALSE;
}

/**
 * This is used to remove all style-impacting spans from text before new
 * extracted text is being replaced into it, so that we don't have any
 * lingering spans applied during the replace.
 */
//void removeParcelableSpans(Spannable spannable, Int32 start, Int32 end) {
//    Object[] spans = spannable.getSpans(start, end, ParcelableSpan.class);
//    Int32 i = spans.length;
//    while (i > 0) {
//        i--;
//        spannable.removeSpan(spans[i]);
//    }
//}

/**
 * Apply to this text view the given extracted text, as previously
 * returned by {@link #extractText(ExtractedTextRequest, ExtractedText)}.
 */
ECode TextView::SetExtractedText(
    /* [in] */ IExtractedText* text)
{
    //Editable content = getEditableText();
    //if (text.text != NULL) {
    //    if (content == NULL) {
    //        setText(text.text, TextView.BufferType.EDITABLE);
    //    } else if (text.partialStartOffset < 0) {
    //        removeParcelableSpans(content, 0, content.length());
    //        content.replace(0, content.length(), text.text);
    //    } else {
    //        final Int32 N = content.length();
    //        Int32 start = text.partialStartOffset;
    //        if (start > N) start = N;
    //        Int32 end = text.partialEndOffset;
    //        if (end > N) end = N;
    //        removeParcelableSpans(content, start, end);
    //        content.replace(start, end, text.text);
    //    }
    //}
    //
    //// Now set the selection position...  make sure it is in range, to
    //// avoid crashes.  If this is a partial update, it is possible that
    //// the underlying text may have changed, causing us problems here.
    //// Also we just don't want to trust clients to do the right thing.
    //Spannable sp = (Spannable)getText();
    //final Int32 N = sp.length();
    //Int32 start = text.selectionStart;
    //if (start < 0) start = 0;
    //else if (start > N) start = N;
    //Int32 end = text.selectionEnd;
    //if (end < 0) end = 0;
    //else if (end > N) end = N;
    //Selection::SetSelection(sp, start, end);
    //
    //// Finally, update the selection mode.
    //if ((text.flags&ExtractedText.FLAG_SELECTING) != 0) {
    //    MetaKeyKeyListener.startSelecting(this, sp);
    //} else {
    //    MetaKeyKeyListener.stopSelecting(this, sp);
    //}
    return E_NOT_IMPLEMENTED;
}

/**
 * @hide
 */
ECode TextView::SetExtracting(
    /* [in] */ IExtractedTextRequest* req)
{
    if (mInputMethodState != NULL) {
        mInputMethodState->mExtracting = req;
    }
    HideControllers();
    return NOERROR;
}

/**
 * Called by the framework in response to a text completion from
 * the current input method, provided by it calling
 * {@link InputConnection#commitCompletion
 * InputConnection.commitCompletion()}.  The default implementation does
 * nothing; text views that are supporting auto-completion should override
 * this to do their desired behavior.
 *
 * @param text The auto complete text the user has selected.
 */
ECode TextView::OnCommitCompletion(
    /* [in] */ ICompletionInfo* text)
{
    return NOERROR;
}

ECode TextView::BeginBatchEdit()
{
    mInBatchEditControllers = TRUE;
    InputMethodState* ims = mInputMethodState;
    if (ims != NULL) {
        Int32 nesting = ++ims->mBatchEditNesting;
        if (nesting == 1) {
            ims->mCursorChanged = FALSE;
            ims->mChangedDelta = 0;
            if (ims->mContentChanged) {
                // We already have a pending change from somewhere else,
                // so turn this into a full update.
                ims->mChangedStart = 0;
                mText->GetLength(&ims->mChangedEnd);
            }
            else {
                ims->mChangedStart = EXTRACT_UNKNOWN;
                ims->mChangedEnd = EXTRACT_UNKNOWN;
                ims->mContentChanged = FALSE;
            }
            OnBeginBatchEdit();
        }
    }
    return NOERROR;
}

ECode TextView::EndBatchEdit()
{
    mInBatchEditControllers = FALSE;
    InputMethodState* ims = mInputMethodState;
    if (ims != NULL) {
        Int32 nesting = --ims->mBatchEditNesting;
        if (nesting == 0) {
            FinishBatchEdit(ims);
        }
    }
    return NOERROR;
}

void TextView::EnsureEndedBatchEdit()
{
    InputMethodState* ims = mInputMethodState;
    if (ims != NULL && ims->mBatchEditNesting != 0) {
        ims->mBatchEditNesting = 0;
        FinishBatchEdit(ims);
    }
}

void TextView::FinishBatchEdit(
    /* [in] */ InputMethodState* ims)
{
    OnEndBatchEdit();

    if (ims->mContentChanged || ims->mSelectionModeChanged) {
        UpdateAfterEdit();
        ReportExtractedText();
    }
    else if (ims->mCursorChanged) {
        // Cheezy way to get us to report the current cursor location.
        InvalidateCursor();
    }
}

void TextView::UpdateAfterEdit()
{
    Invalidate();
    Int32 curs = GetSelectionStart();

    if (curs >= 0 || (mGravity & Gravity_VERTICAL_GRAVITY_MASK) ==
                            Gravity_BOTTOM) {
        RegisterForPreDraw();
    }

    if (curs >= 0) {
        mHighlightPathBogus = TRUE;

        if (IsFocused()) {
            mShowCursor = SystemClock::GetUptimeMillis();
            MakeBlink();
        }
    }

    CheckForResize();
}

/**
 * Called by the framework in response to a request to begin a batch
 * of edit operations through a call to link {@link #beginBatchEdit()}.
 */
ECode TextView::OnBeginBatchEdit()
{
    return NOERROR;
}

/**
 * Called by the framework in response to a request to end a batch
 * of edit operations through a call to link {@link #endBatchEdit}.
 */
ECode TextView::OnEndBatchEdit()
{
    return NOERROR;
}

/**
 * Called by the framework in response to a private command from the
 * current method, provided by it calling
 * {@link InputConnection#performPrivateCommand
 * InputConnection.performPrivateCommand()}.
 *
 * @param action The action name of the command.
 * @param data Any additional data for the command.  This may be NULL.
 * @return Return TRUE if you handled the command, else FALSE.
 */
Boolean TextView::OnPrivateIMECommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    return FALSE;
}

void TextView::NullLayouts()
{
    if (mLayout != NULL &&IBoringLayout::Probe(mLayout) != NULL
        && mSavedLayout == NULL) {
        mSavedLayout = IBoringLayout::Probe(mLayout);
    }

    if (mHintLayout != NULL && IBoringLayout::Probe(mHintLayout) != NULL
        && mSavedHintLayout == NULL) {
        mSavedHintLayout = IBoringLayout::Probe(mHintLayout);
    }

    mLayout = NULL;
    mHintLayout = NULL;
}

/**
 * Make a new Layout based on the already-measured size of the view,
 * on the assumption that it was measured correctly at some point.
 */
void TextView::AssumeLayout()
{
    Int32 width = mRight - mLeft - GetCompoundPaddingLeft() - GetCompoundPaddingRight();

    if (width < 1) {
        width = 0;
    }

    Int32 physicalWidth = width;

    if (mHorizontallyScrolling) {
        width = VERY_WIDE;
    }

    MakeNewLayout(width, physicalWidth, UNKNOWN_BORING, UNKNOWN_BORING,
            physicalWidth, FALSE);
}

/**
 * The width passed in is now the desired layout width,
 * not the full view width with padding.
 * {@hide}
 */
void TextView::MakeNewLayout(
    /* [in] */ Int32 w,
    /* [in] */ Int32 hintWidth,
    /* [in] */ IBoringLayoutMetrics* _boring,
    /* [in] */ IBoringLayoutMetrics* _hintBoring,
    /* [in] */ Int32 ellipsisWidth,
    /* [in] */ Boolean bringIntoView)
{
    AutoPtr<IBoringLayoutMetrics> boring = _boring;
    AutoPtr<IBoringLayoutMetrics> hintBoring = _hintBoring;

    StopMarquee();

    mHighlightPathBogus = TRUE;

    if (w < 0) {
        w = 0;
    }
    if (hintWidth < 0) {
        hintWidth = 0;
    }

    LayoutAlignment alignment;
    switch (mGravity & Gravity_HORIZONTAL_GRAVITY_MASK) {
        case Gravity_CENTER_HORIZONTAL:
            alignment = LayoutAlignment_ALIGN_CENTER;
            break;

        case Gravity_RIGHT:
            alignment = LayoutAlignment_ALIGN_OPPOSITE;
            break;

        default:
            alignment = LayoutAlignment_ALIGN_NORMAL;
    }

    Boolean shouldEllipsize = mEllipsize != -1 && mInput == NULL;

    if (ISpannable::Probe(mText.Get()) != NULL) {
        ASSERT_SUCCEEDED(CDynamicLayout::New(
            mText, mTransformed, mTextPaint, w, alignment, mSpacingMult,
            mSpacingAdd, mIncludePad, mInput == NULL ? mEllipsize : NULL,
            ellipsisWidth, (IDynamicLayout**)&mLayout));
    }
    else {
        if (boring == UNKNOWN_BORING) {
            boring = CBoringLayout::IsBoring(mTransformed, mTextPaint, mBoring);

            if (boring != NULL) {
                mBoring = boring;
            }
        }

        if (boring != NULL) {
            Int32 width;
            boring->GetWidth(&width);
            if (width <= w &&
                (mEllipsize == -1 || width <= ellipsisWidth)) {
                if (mSavedLayout != NULL) {
                    mLayout = NULL;
                    mSavedLayout->ReplaceOrMake(mTransformed, mTextPaint,
                            w, alignment, mSpacingMult, mSpacingAdd,
                            boring, mIncludePad, (IBoringLayout**)&mLayout);
                }
                else {
                    mLayout = CBoringLayout::Make(mTransformed, mTextPaint,
                            w, alignment, mSpacingMult, mSpacingAdd,
                            boring, mIncludePad);
                }

                mSavedLayout = (IBoringLayout*)mLayout.Get();
            }
            else if (shouldEllipsize && width <= w) {
                if (mSavedLayout != NULL) {
                    mLayout = NULL;
                    mSavedLayout->ReplaceOrMakeEx(mTransformed, mTextPaint,
                            w, alignment, mSpacingMult, mSpacingAdd,
                            boring, mIncludePad, mEllipsize,
                            ellipsisWidth, (IBoringLayout**)&mLayout);
                }
                else {
                    mLayout = CBoringLayout::Make(mTransformed, mTextPaint,
                            w, alignment, mSpacingMult, mSpacingAdd,
                            boring, mIncludePad, mEllipsize,
                            ellipsisWidth);
                }
            }
            else if (shouldEllipsize) {
                Int32 length;
                mTransformed->GetLength(&length);
                ASSERT_SUCCEEDED(CStaticLayout::New(
                    mTransformed, 0, length, mTextPaint, w, alignment,
                    mSpacingMult, mSpacingAdd, mIncludePad, mEllipsize,
                    ellipsisWidth, (IStaticLayout**)&mLayout));
            }
            else {
                ASSERT_SUCCEEDED(CStaticLayout::New(
                    mTransformed, mTextPaint, w, alignment, mSpacingMult,
                    mSpacingAdd, mIncludePad, (IStaticLayout**)&mLayout));
            }
        }
        else if (shouldEllipsize) {
            Int32 length;
            mTransformed->GetLength(&length);
            ASSERT_SUCCEEDED(CStaticLayout::New(
                mTransformed, 0, length, mTextPaint, w, alignment,
                mSpacingMult, mSpacingAdd, mIncludePad, mEllipsize,
                ellipsisWidth, (IStaticLayout**)&mLayout));
        }
        else {
            ASSERT_SUCCEEDED(CStaticLayout::New(mTransformed, mTextPaint,
                    w, alignment, mSpacingMult, mSpacingAdd,
                    mIncludePad, (IStaticLayout**)&mLayout));
        }
    }

    shouldEllipsize = mEllipsize != -1;
    mHintLayout = NULL;

    if (mHint != NULL) {
        if (shouldEllipsize) hintWidth = w;

        if (hintBoring == UNKNOWN_BORING) {
            hintBoring = CBoringLayout::IsBoring(mHint, mTextPaint, mHintBoring);

            if (hintBoring != NULL) {
                mHintBoring = hintBoring;
            }
        }

        if (hintBoring != NULL) {
            Int32 hbwidth;
            hintBoring->GetWidth(&hbwidth);
            if (hbwidth <= hintWidth &&
                (!shouldEllipsize || hbwidth <= ellipsisWidth)) {
                if (mSavedHintLayout != NULL) {
                    mHintLayout = NULL;
                    mSavedHintLayout->ReplaceOrMake(mHint, mTextPaint,
                            hintWidth, alignment, mSpacingMult, mSpacingAdd,
                            hintBoring, mIncludePad, (IBoringLayout**)&mHintLayout);
                }
                else {
                    mHintLayout = CBoringLayout::Make(mHint, mTextPaint,
                            hintWidth, alignment, mSpacingMult, mSpacingAdd,
                            hintBoring, mIncludePad);
                }

                mSavedHintLayout = (IBoringLayout*)mHintLayout.Get();
            }
            else if (shouldEllipsize && hbwidth <= hintWidth) {
                if (mSavedHintLayout != NULL) {
                    mHintLayout = NULL;
                    mSavedHintLayout->ReplaceOrMakeEx(mHint, mTextPaint,
                            hintWidth, alignment, mSpacingMult, mSpacingAdd,
                            hintBoring, mIncludePad, mEllipsize,
                            ellipsisWidth, (IBoringLayout**)&mHintLayout);
                }
                else {
                    mHintLayout = CBoringLayout::Make(mHint, mTextPaint,
                            hintWidth, alignment, mSpacingMult, mSpacingAdd,
                            hintBoring, mIncludePad, mEllipsize,
                            ellipsisWidth);
                }
            }
            else if (shouldEllipsize) {
                Int32 length;
                mHint->GetLength(&length);
                ASSERT_SUCCEEDED(CStaticLayout::New(
                    mHint, 0, length, mTextPaint, hintWidth, alignment,
                    mSpacingMult, mSpacingAdd, mIncludePad, mEllipsize,
                    ellipsisWidth, (IStaticLayout**)&mHintLayout));
            }
            else {
                ASSERT_SUCCEEDED(CStaticLayout::New(
                    mHint, mTextPaint, hintWidth, alignment, mSpacingMult,
                    mSpacingAdd, mIncludePad, (IStaticLayout**)&mHintLayout));
            }
        }
        else if (shouldEllipsize) {
            Int32 length;
            mHint->GetLength(&length);
            ASSERT_SUCCEEDED(CStaticLayout::New(
                mHint, 0, length, mTextPaint, hintWidth, alignment,
                mSpacingMult, mSpacingAdd, mIncludePad, mEllipsize,
                ellipsisWidth, (IStaticLayout**)&mHintLayout));
        }
        else {
            ASSERT_SUCCEEDED(CStaticLayout::New(
                mHint, mTextPaint, hintWidth, alignment, mSpacingMult,
                mSpacingAdd, mIncludePad, (IStaticLayout**)&mHintLayout));
        }
    }

    if (bringIntoView) {
        RegisterForPreDraw();
    }

    if (mEllipsize == TextUtilsTruncateAt_MARQUEE) {
        if (!CompressText(ellipsisWidth)) {
            Int32 height;
            mLayoutParams->GetHeight(&height);
            // If the size of the view does not depend on the size of the text, try to
            // start the marquee immediately
            if (height != ViewGroupLayoutParams_WRAP_CONTENT
                && height != ViewGroupLayoutParams_MATCH_PARENT) {
                StartMarquee();
            }
            else {
                // Defer the start of the marquee until we know our width (see setFrame())
                mRestartMarquee = TRUE;
            }
        }
    }

    // CursorControllers need a non-NULL mLayout
    PrepareCursorControllers();
}

Boolean TextView::CompressText(
    /* [in] */ Float width)
{
    // Only compress the text if it hasn't been compressed by the previous pass
    /*if (width > 0.0f && mLayout != NULL && getLineCount() == 1 && !mUserSetTextScaleX &&
            mTextPaint.getTextScaleX() == 1.0f) {
        final Float textWidth = mLayout->getLineWidth(0);
        final Float overflow = (textWidth + 1.0f - width) / width;
        if (overflow > 0.0f && overflow <= Marquee.MARQUEE_DELTA_MAX) {
            mTextPaint.setTextScaleX(1.0f - overflow - 0.005f);
            post(new Runnable() {
                public void run() {
                    requestLayout();
                }
            });
            return TRUE;
        }
    }*/

    return FALSE;
}

Int32 TextView::Desired(
    /* [in] */ ILayout* layout)
{
    Int32 N;
    layout->GetLineCount(&N);
    AutoPtr<ICharSequence> text;
    layout->GetText((ICharSequence**)&text);
    Float max = 0;

    // if any line was wrapped, we can't use it.
    // but it's ok for the last line not to have a newline

    for (Int32 i = 0; i < N - 1; i++) {
        Int32 offset;
        Char32 c;
        layout->GetLineEnd(i, &offset);
        text->GetCharAt(offset - 1, &c);
        if (c != '\n')
            return -1;
    }

    for (Int32 i = 0; i < N; i++) {
        Float extent;
        layout->GetLineWidth(i, &extent);
        max = Math::Max(max, extent);
    }

    return (Int32)Math::Ceil(max);
}

/**
 * Set whether the TextView includes extra top and bottom padding to make
 * room for accents that go above the normal ascent and descent.
 * The default is TRUE.
 *
 * @attr ref android.R.styleable#TextView_includeFontPadding
 */
ECode TextView::SetIncludeFontPadding(
    /* [in] */ Boolean includepad)
{
    mIncludePad = includepad;

    if (mLayout != NULL) {
        NullLayouts();
        RequestLayout();
        Invalidate();
    }
    return NOERROR;
}

void TextView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    Int32 heightMode = MeasureSpec::GetMode(heightMeasureSpec);
    Int32 widthSize = MeasureSpec::GetSize(widthMeasureSpec);
    Int32 heightSize = MeasureSpec::GetSize(heightMeasureSpec);

    Int32 width;
    Int32 height;

    AutoPtr<IBoringLayoutMetrics> boring = UNKNOWN_BORING;
    AutoPtr<IBoringLayoutMetrics> hintBoring = UNKNOWN_BORING;

    Int32 des = -1;
    Boolean fromexisting = FALSE;

    if (widthMode == MeasureSpec::EXACTLY) {
        // Parent has told us how big to be. So be it.
        width = widthSize;
    }
    else {
        if (mLayout != NULL && mEllipsize == -1) {
            des = Desired(mLayout);
        }

        if (des < 0) {
            boring = CBoringLayout::IsBoring(mTransformed, mTextPaint, mBoring);
            if (boring != NULL) {
                mBoring = boring;
            }
        }
        else {
            fromexisting = TRUE;
        }

        if (boring == NULL || boring == UNKNOWN_BORING) {
            if (des < 0) {
                des = (Int32)Math::Ceil(Layout::GetDesiredWidth(mTransformed, mTextPaint));
            }

            width = des;
        }
        else {
            boring->GetWidth(&width);
        }

        Drawables* dr = mDrawables;
        if (dr != NULL) {
            width = Math::Max(width, dr->mDrawableWidthTop);
            width = Math::Max(width, dr->mDrawableWidthBottom);
        }

        if (mHint != NULL) {
            Int32 hintDes = -1;
            Int32 hintWidth;

            if (mHintLayout != NULL && mEllipsize == -1) {
                hintDes = Desired(mHintLayout);
            }

            if (hintDes < 0) {
                hintBoring = CBoringLayout::IsBoring(mHint, mTextPaint, mHintBoring);
                if (hintBoring != NULL) {
                    mHintBoring = hintBoring;
                }
            }

            if (hintBoring == NULL || hintBoring == UNKNOWN_BORING) {
                if (hintDes < 0) {
                    hintDes = (Int32)Math::Ceil(
                            Layout::GetDesiredWidth(mHint, mTextPaint));
                }

                hintWidth = hintDes;
            }
            else {
                hintBoring->GetWidth(&hintWidth);
            }

            if (hintWidth > width) {
                width = hintWidth;
            }
        }

        width += GetCompoundPaddingLeft() + GetCompoundPaddingRight();

        if (mMaxWidthMode == EMS) {
            width = Math::Min(width, mMaxWidth * GetLineHeight());
        }
        else {
            width = Math::Min(width, mMaxWidth);
        }

        if (mMinWidthMode == EMS) {
            width = Math::Max(width, mMinWidth * GetLineHeight());
        }
        else {
            width = Math::Max(width, mMinWidth);
        }

        // Check against our minimum width
        width = Math::Max(width, GetSuggestedMinimumWidth());

        if (widthMode == MeasureSpec::AT_MOST) {
            width = Math::Min(widthSize, width);
        }
    }

    Int32 want = width - GetCompoundPaddingLeft() - GetCompoundPaddingRight();
    Int32 unpaddedWidth = want;
    Int32 hintWant = want;

    if (mHorizontallyScrolling) want = VERY_WIDE;

    Int32 hintWidth;
    if (mHintLayout == NULL) {
        hintWidth = hintWant;
    }
    else {
        mHintLayout->GetWidth(&hintWidth);
    }

    if (mLayout == NULL) {
        MakeNewLayout(want, hintWant, boring, hintBoring,
                width - GetCompoundPaddingLeft() - GetCompoundPaddingRight(), FALSE);
    }
    else {
        Int32 _width, _eWidth;
        mLayout->GetWidth(&_width);
        mLayout->GetEllipsizedWidth(&_eWidth);
        if ((_width != want) || (hintWidth != hintWant) ||
                (_eWidth != width - GetCompoundPaddingLeft() - GetCompoundPaddingRight())) {
            if (mHint == NULL && mEllipsize == -1 &&
                    want > _width &&
                    (IBoringLayout::Probe(mLayout.Get()) != NULL ||
                            (fromexisting && des >= 0 && des <= want))) {
                mLayout->IncreaseWidthTo(want);
            }
            else {
                MakeNewLayout(want, hintWant, boring, hintBoring,
                        width - GetCompoundPaddingLeft() - GetCompoundPaddingRight(), FALSE);
            }
        }
        else {
            // Width has not changed.
        }
    }

    if (heightMode == MeasureSpec::EXACTLY) {
        // Parent has told us how big to be. So be it.
        height = heightSize;
        mDesiredHeightAtMeasure = -1;
    }
    else {
        Int32 desired = GetDesiredHeight();

        height = desired;
        mDesiredHeightAtMeasure = desired;

        if (heightMode == MeasureSpec::AT_MOST) {
            height = Math::Min(desired, heightSize);
        }
    }

    Int32 count;
    assert(mLayout.Get());
    mLayout->GetLineCount(&count);

    Int32 unpaddedHeight = height - GetCompoundPaddingTop() - GetCompoundPaddingBottom();
    if (mMaxMode == LINES && count > mMaximum) {
        Int32 pos;
        mLayout->GetLineTop(mMaximum, &pos);
        unpaddedHeight = Math::Min(unpaddedHeight, pos);
    }

    ///*
    //    * We didn't let makeNewLayout() register to bring the cursor into view,
    //    * so do it here if there is any possibility that it is needed.
    //    */
    Int32 w, h;
    if (mMovement != NULL ||
        (mLayout->GetWidth(&w), w) > unpaddedWidth ||
        (mLayout->GetHeight(&h), h) > unpaddedHeight) {
        RegisterForPreDraw();
    }
    else {
        ScrollTo(0, 0);
    }

    SetMeasuredDimension(width, height);
}

Int32 TextView::GetDesiredHeight()
{
    return Math::Max(
            GetDesiredHeight(mLayout, TRUE),
            GetDesiredHeight(mHintLayout, mEllipsize != -1));
}

Int32 TextView::GetDesiredHeight(
    /* [in] */ ILayout* layout,
    /* [in] */ Boolean cap)
{
    if (layout == NULL) {
        return 0;
    }

    Int32 linecount, pad, desired;
    layout->GetLineCount(&linecount);
    pad = GetCompoundPaddingTop() + GetCompoundPaddingBottom();
    layout->GetLineTop(linecount, &desired);

    Drawables* dr = mDrawables;
    if (dr != NULL) {
        desired = Math::Max(desired, dr->mDrawableHeightLeft);
        desired = Math::Max(desired, dr->mDrawableHeightRight);
    }

    desired += pad;

    if (mMaxMode == LINES) {
        /*
         * Don't cap the hint to a certain number of lines.
         * (Do cap it, though, if we have a maximum pixel height.)
         */
        if (cap) {
            if (linecount > mMaximum) {
                Int32 pos1, pos2;
                layout->GetLineTop(mMaximum, &pos1);
                layout->GetBottomPadding(&pos2);
                desired = pos1 + pos2;

                if (dr != NULL) {
                    desired = Math::Max(desired, dr->mDrawableHeightLeft);
                    desired = Math::Max(desired, dr->mDrawableHeightRight);
                }

                desired += pad;
                linecount = mMaximum;
            }
        }
    }
    else {
        desired = Math::Min(desired, mMaximum);
    }

    if (mMinMode == LINES) {
        if (linecount < mMinimum) {
            desired += GetLineHeight() * (mMinimum - linecount);
        }
    }
    else {
        desired = Math::Max(desired, mMinimum);
    }

    // Check against our minimum height
    desired = Math::Max(desired, GetSuggestedMinimumHeight());

    return desired;
}

/**
 * Check whether a change to the existing text layout requires a
 * new view layout.
 */
void TextView::CheckForResize()
{
    Boolean sizeChanged = FALSE;

    CViewGroupLayoutParams* params = (CViewGroupLayoutParams*)mLayoutParams.Get();
    if (mLayout != NULL) {
        // Check if our width changed
        if (params->mWidth == ViewGroupLayoutParams_WRAP_CONTENT) {
            sizeChanged = TRUE;
            Invalidate();
        }

        // Check if our height changed
        if (params->mHeight == ViewGroupLayoutParams_WRAP_CONTENT) {
            Int32 desiredHeight = GetDesiredHeight();

            if (desiredHeight != GetHeight()) {
                sizeChanged = TRUE;
            }
        }
        else if (params->mHeight == ViewGroupLayoutParams_MATCH_PARENT) {
            if (mDesiredHeightAtMeasure >= 0) {
                Int32 desiredHeight = GetDesiredHeight();

                if (desiredHeight != mDesiredHeightAtMeasure) {
                    sizeChanged = TRUE;
                }
            }
        }
    }

    if (sizeChanged) {
        RequestLayout();
        // caller will have already invalidated
    }
}

/**
 * Check whether entirely new text requires a new view layout
 * or merely a new text layout.
 */
void TextView::CheckForRelayout()
{
    // If we have a fixed width, we can just swap in a new text layout
    // if the text height stays the same or if the view height is fixed.

    CViewGroupLayoutParams* params = (CViewGroupLayoutParams*)mLayoutParams.Get();
    if ((params->mWidth != ViewGroupLayoutParams_WRAP_CONTENT ||
        (mMaxWidthMode == mMinWidthMode && mMaxWidth == mMinWidth)) &&
        (mHint == NULL || mHintLayout != NULL) &&
        (mRight - mLeft - GetCompoundPaddingLeft() - GetCompoundPaddingRight() > 0)) {
        // Static width, so try making a new text layout.

        Int32 oldht;
        mLayout->GetHeight(&oldht);
        Int32 want;
        mLayout->GetWidth(&want);
        Int32 hintWant = 0;
        if (mHintLayout != NULL) {
            mHintLayout->GetWidth(&hintWant);
        }

        /*
         * No need to bring the text into view, since the size is not
         * changing (unless we do the requestLayout(), in which case it
         * will happen at measure).
         */
        MakeNewLayout(
            want, hintWant, UNKNOWN_BORING, UNKNOWN_BORING,
            mRight - mLeft - GetCompoundPaddingLeft() - GetCompoundPaddingRight(),
            FALSE);

        if (mEllipsize != TextUtilsTruncateAt_MARQUEE) {
            // In a fixed-height view, so use our new text layout.
            if (params->mHeight != ViewGroupLayoutParams_WRAP_CONTENT &&
                params->mHeight != ViewGroupLayoutParams_MATCH_PARENT) {
                Invalidate();
                return;
            }

            // Dynamic height, but height has stayed the same,
            // so use our new text layout.
            Int32 ht1, ht2;
            mLayout->GetHeight(&ht1);
            if (mHintLayout != NULL) {
                mHintLayout->GetHeight(&ht2);
            }

            if (ht1 == oldht && (mHintLayout == NULL || ht2 == oldht)) {
                Invalidate();
                return;
            }
        }

        // We lose: the height has changed and we have a dynamic height.
        // Request a new view layout using our new text layout.
        RequestLayout();
        Invalidate();
    }
    else {
        // Dynamic width, so we have no choice but to request a new
        // view layout with a new text layout.

        NullLayouts();
        RequestLayout();
        Invalidate();
    }
}


/**
 * Returns TRUE if anything changed.
 */
Boolean TextView::BringTextIntoView()
{
    Int32 line = 0;
    if ((mGravity & Gravity_VERTICAL_GRAVITY_MASK) == Gravity_BOTTOM) {
        mLayout->GetLineCount(&line);
        line--;
    }

    LayoutAlignment a;
    mLayout->GetParagraphAlignment(line, &a);
    Int32 dir;
    mLayout->GetParagraphDirection(line, &dir);
    Int32 hspace = mRight - mLeft - GetCompoundPaddingLeft() - GetCompoundPaddingRight();
    Int32 vspace = mBottom - mTop - GetExtendedPaddingTop() - GetExtendedPaddingBottom();
    Int32 ht;
    mLayout->GetHeight(&ht);

    Int32 scrollx, scrolly;

    if (a == LayoutAlignment_ALIGN_CENTER) {
        /*
         * Keep centered if possible, or, if it is too wide to fit,
         * keep leading edge in view.
         */

        Float fLeft, fRight;
        mLayout->GetLineLeft(line, &fLeft);
        mLayout->GetLineRight(line, &fRight);
        Int32 left = (Int32)Math::Floor(fLeft);
        Int32 right = (Int32)Math::Ceil(fRight);

        if (right - left < hspace) {
            scrollx = (right + left) / 2 - hspace / 2;
        }
        else {
            if (dir < 0) {
                scrollx = right - hspace;
            }
            else {
                scrollx = left;
            }
        }
    }
    else if (a == LayoutAlignment_ALIGN_NORMAL) {
        /*
         * Keep leading edge in view.
         */

        if (dir < 0) {
            Float fRight;
            mLayout->GetLineRight(line, &fRight);
            Int32 right = (Int32)Math::Ceil(fRight);
            scrollx = right - hspace;
        }
        else {
            Float fLeft;
            mLayout->GetLineLeft(line, &fLeft);
            scrollx = (Int32)Math::Floor(fLeft);
        }
    }
    else /* a == LayoutAlignment_ALIGN_OPPOSITE */ {
        /*
         * Keep trailing edge in view.
         */

        if (dir < 0) {
            Float fLeft;
            mLayout->GetLineLeft(line, &fLeft);
            scrollx = (Int32)Math::Floor(fLeft);
        }
        else {
            Float fRight;
            mLayout->GetLineRight(line, &fRight);
            Int32 right = (Int32)Math::Ceil(fRight);
            scrollx = right - hspace;
        }
    }

    if (ht < vspace) {
        scrolly = 0;
    }
    else {
        if ((mGravity & Gravity_VERTICAL_GRAVITY_MASK) == Gravity_BOTTOM) {
            scrolly = ht - vspace;
        }
        else {
            scrolly = 0;
        }
    }

    if (scrollx != mScrollX || scrolly != mScrollY) {
        ScrollTo(scrollx, scrolly);
        return TRUE;
    }
    else {
        return FALSE;
    }
    return FALSE;
}

/**
 * Move the point, specified by the offset, into the view if it is needed.
 * This has to be called after layout. Returns TRUE if anything changed.
 */
Boolean TextView::BringPointIntoView(
    /* [in] */ Int32 offset)
{
    Boolean changed = FALSE;

    Int32 line;
    mLayout->GetLineForOffset(offset, &line);

    // FIXME: Is it okay to truncate this, or should we round?
    Float fx;
    mLayout->GetPrimaryHorizontal(offset, &fx);
    Int32 x = (Int32)fx;
    Int32 top;
    mLayout->GetLineTop(line, &top);
    Int32 bottom;
    mLayout->GetLineTop(line + 1, &bottom);

    Float fLeft, fRight;
    mLayout->GetLineLeft(line, &fLeft);
    mLayout->GetLineRight(line, &fRight);
    Int32 left = (Int32)Math::Floor(fLeft);
    Int32 right = (Int32)Math::Floor(fRight);
    Int32 ht;
    mLayout->GetHeight(&ht);

    Int32 grav;

    LayoutAlignment alignment;
    mLayout->GetParagraphAlignment(line, &alignment);
    switch (alignment) {
        case LayoutAlignment_ALIGN_NORMAL:
            grav = 1;
            break;

        case LayoutAlignment_ALIGN_OPPOSITE:
            grav = -1;
            break;

        default:
            grav = 0;
    }

    Int32 dir;
    mLayout->GetParagraphDirection(line, &dir);
    grav *= dir;

    Int32 hspace = mRight - mLeft - GetCompoundPaddingLeft() - GetCompoundPaddingRight();
    Int32 vspace = mBottom - mTop - GetExtendedPaddingTop() - GetExtendedPaddingBottom();

    Int32 hslack = (bottom - top) / 2;
    Int32 vslack = hslack;

    if (vslack > vspace / 4)
        vslack = vspace / 4;
    if (hslack > hspace / 4)
        hslack = hspace / 4;

    Int32 hs = mScrollX;
    Int32 vs = mScrollY;

    if (top - vs < vslack)
        vs = top - vslack;
    if (bottom - vs > vspace - vslack)
        vs = bottom - (vspace - vslack);
    if (ht - vs < vspace)
        vs = ht - vspace;
    if (0 - vs > 0)
        vs = 0;

    if (grav != 0) {
        if (x - hs < hslack) {
            hs = x - hslack;
        }
        if (x - hs > hspace - hslack) {
            hs = x - (hspace - hslack);
        }
    }

    if (grav < 0) {
        if (left - hs > 0)
            hs = left;
        if (right - hs < hspace)
            hs = right - hspace;
    }
    else if (grav > 0) {
        if (right - hs < hspace)
            hs = right - hspace;
        if (left - hs > 0)
            hs = left;
    }
    else /* grav == 0 */ {
        if (right - left <= hspace) {
            /*
             * If the entire text fits, center it exactly.
             */
            hs = left - (hspace - (right - left)) / 2;
        }
        else if (x > right - hslack) {
            /*
             * If we are near the right edge, keep the right edge
             * at the edge of the view.
             */
            hs = right - hspace;
        }
        else if (x < left + hslack) {
            /*
             * If we are near the left edge, keep the left edge
             * at the edge of the view.
             */
            hs = left;
        }
        else if (left > hs) {
            /*
             * Is there whitespace visible at the left?  Fix it if so.
             */
            hs = left;
        }
        else if (right < hs + hspace) {
            /*
             * Is there whitespace visible at the right?  Fix it if so.
             */
            hs = right - hspace;
        }
        else {
            /*
             * Otherwise, Float as needed.
             */
            if (x - hs < hslack) {
                hs = x - hslack;
            }
            if (x - hs > hspace - hslack) {
                hs = x - (hspace - hslack);
            }
        }
    }

    if (hs != mScrollX || vs != mScrollY) {
        if (mScroller == NULL) {
            ScrollTo(hs, vs);
        }
        else {
            Int64 duration = AnimationUtils::CurrentAnimationTimeMillis() - mLastScroll;
            Int32 dx = hs - mScrollX;
            Int32 dy = vs - mScrollY;

            if (duration > ANIMATED_SCROLL_GAP) {
                mScroller->StartScroll(mScrollX, mScrollY, dx, dy);
                AwakenScrollBars(mScroller->GetDuration());
                Invalidate();
            }
            else {
                if (!mScroller->IsFinished()) {
                    mScroller->AbortAnimation();
                }

                ScrollBy(dx, dy);
            }

            mLastScroll = AnimationUtils::CurrentAnimationTimeMillis();
        }

        changed = TRUE;
    }

    if (IsFocused()) {
        //This offsets because getInterestingRect() is in terms of
        //viewport coordinates, but requestRectangleOnScreen()
        //is in terms of content coordinates.

        AutoPtr<IRect> r;
        CRect::New(x, top, x + 1, bottom, (IRect**)&r);
        GetInterestingRect(r, line);
        r->Offset(mScrollX, mScrollY);

        if (RequestRectangleOnScreen(r)) {
            changed = TRUE;
        }
    }

    return changed;
}

/**
 * Move the cursor, if needed, so that it is at an offset that is visible
 * to the user.  This will not move the cursor if it represents more than
 * one character (a selection range).  This will only work if the
 * TextView contains spannable text; otherwise it will do nothing.
 *
 * @return True if the cursor was actually moved, FALSE otherwise.
 */
Boolean TextView::MoveCursorToVisibleOffset()
{
    if (!(ISpannable::Probe(mText))) {
        return FALSE;
    }
    Int32 start = GetSelectionStart();
    Int32 end = GetSelectionEnd();
    if (start != end) {
        return FALSE;
    }

    //First: make sure the line is visible on screen:

    Int32 line;
    mLayout->GetLineForOffset(start, &line);

    Int32 top;
    mLayout->GetLineTop(line, &top);
    Int32 bottom;
    mLayout->GetLineTop(line + 1, &bottom);
    Int32 vspace = mBottom - mTop - GetExtendedPaddingTop() - GetExtendedPaddingBottom();
    Int32 vslack = (bottom - top) / 2;
    if (vslack > vspace / 4)
        vslack = vspace / 4;
    Int32 vs = mScrollY;

    if (top < (vs+vslack)) {
        mLayout->GetLineForVertical(vs+vslack+(bottom-top), &line);
    }
    else if (bottom > (vspace+vs-vslack)) {
        mLayout->GetLineForVertical(vspace+vs-vslack-(bottom-top), &line);
    }

    //Next: make sure the character is visible on screen:

    Int32 hspace = mRight - mLeft - GetCompoundPaddingLeft() - GetCompoundPaddingRight();
    Int32 hs = mScrollX;
    Int32 leftChar;
    mLayout->GetOffsetForHorizontal(line, hs, &leftChar);
    Int32 rightChar;
    mLayout->GetOffsetForHorizontal(line, hspace+hs, &rightChar);

    Int32 newStart = start;
    if (newStart < leftChar) {
        newStart = leftChar;
    }
    else if (newStart > rightChar) {
        newStart = rightChar;
    }

    if (newStart != start) {
        Selection::SetSelection(ISpannable::Probe(mText), newStart);
        return TRUE;
    }
    return FALSE;
}

ECode TextView::ComputeScroll()
{
    if (mScroller != NULL) {
        if (mScroller->ComputeScrollOffset()) {
            mScrollX = mScroller->GetCurrX();
            mScrollY = mScroller->GetCurrY();
            PostInvalidate();  // So we draw again
        }
    }
    return NOERROR;
}

void TextView::GetInterestingRect(
    /* [in] */ IRect* r,
    /* [in] */ Int32 line)
{
    ConvertFromViewportToContentCoordinates(r);

    // Rectangle can can be expanded on first and last line to take
    // padding into account.
    // TODO Take left/right padding into account too?
    Int32 count;
    mLayout->GetLineCount(&count);
    if (line == 0) ((CRect*)r)->mTop -= GetExtendedPaddingTop();
    if (line == count - 1) ((CRect*)r)->mBottom += GetExtendedPaddingBottom();
}

void TextView::ConvertFromViewportToContentCoordinates(
    /* [in] */ IRect* r)
{
    Int32 horizontalOffset = ViewportToContentHorizontalOffset();
    ((CRect*)r)->mLeft += horizontalOffset;
    ((CRect*)r)->mRight += horizontalOffset;

    Int32 verticalOffset = ViewportToContentVerticalOffset();
    ((CRect*)r)->mTop += verticalOffset;
    ((CRect*)r)->mBottom += verticalOffset;
}

Int32 TextView::ViewportToContentHorizontalOffset()
{
    return GetCompoundPaddingLeft() - mScrollX;
}

Int32 TextView::ViewportToContentVerticalOffset()
{
    Int32 offset = GetExtendedPaddingTop() - mScrollY;
    if ((mGravity & Gravity_VERTICAL_GRAVITY_MASK) != Gravity_TOP) {
        offset += GetVerticalOffset(FALSE);
    }
    return offset;
}

void TextView::Debug(
    /* [in] */ Int32 depth)
{
    /*super.debug(depth);

    String output = debugIndent(depth);
    output += "frame={" + mLeft + ", " + mTop + ", " + mRight
            + ", " + mBottom + "} scroll={" + mScrollX + ", " + mScrollY
            + "} ";

    if (mText != NULL) {

        output += "mText=\"" + mText + "\" ";
        if (mLayout != NULL) {
            output += "mLayout width=" + mLayout->getWidth()
                    + " height=" + mLayout->getHeight();
        }
    } else {
        output += "mText=NULL";
    }
    Log::d(VIEW_LOG_TAG, output);*/
}

/**
 * Convenience for {@link Selection#getSelectionStart}.
 */
Int32 TextView::GetSelectionStart()
{
    return Selection::GetSelectionStart(ISpannable::Probe(GetText().Get()));
}

/**
 * Convenience for {@link Selection#getSelectionEnd}.
 */
Int32 TextView::GetSelectionEnd()
{
    return Selection::GetSelectionEnd(ISpannable::Probe(GetText().Get()));
}

/**
 * Return TRUE iff there is a selection inside this text view.
 */
Boolean TextView::HasSelection()
{
    Int32 selectionStart = GetSelectionStart();
    Int32 selectionEnd = GetSelectionEnd();

    return selectionStart >= 0 && selectionStart != selectionEnd;
}

/**
 * Sets the properties of this field (lines, horizontally scrolling,
 * transformation method) to be for a single-line input.
 *
 * @attr ref android.R.styleable#TextView_singleLine
 */
ECode TextView::SetSingleLine()
{
    return SetSingleLine(TRUE);
}

/**
 * If TRUE, sets the properties of this field (lines, horizontally
 * scrolling, transformation method) to be for a single-line input;
 * if FALSE, restores these to the default conditions.
 * Note that calling this with FALSE restores default conditions,
 * not necessarily those that were in effect prior to calling
 * it with TRUE.
 *
 * @attr ref android.R.styleable#TextView_singleLine
 */
ECode TextView::SetSingleLine(
    /* [in] */ Boolean singleLine)
{
//    if ((mInputType&InputType_TYPE_MASK_CLASS)
//            == InputType_TYPE_CLASS_TEXT) {
//        if (singleLine) {
//            mInputType &= ~InputType_TYPE_TEXT_FLAG_MULTI_LINE;
//        } else {
//            mInputType |= InputType_TYPE_TEXT_FLAG_MULTI_LINE;
//        }
//    }
    return ApplySingleLine(singleLine, TRUE);
}

ECode TextView::ApplySingleLine(
    /* [in] */ Boolean singleLine,
    /* [in] */ Boolean applyTransformation)
{
    mSingleLine = singleLine;
    if (singleLine) {
        SetLines(1);
        SetHorizontallyScrolling(TRUE);
        if (applyTransformation) {
            //SetTransformationMethod(SingleLineTransformationMethod::GetInstance());
        }
    }
    else {
        SetMaxLines(Math::INT32_MAX_VALUE);
        SetHorizontallyScrolling(FALSE);
        if (applyTransformation) {
            SetTransformationMethod(NULL);
        }
    }

    return NOERROR;
}

/**
 * Causes words in the text that are longer than the view is wide
 * to be ellipsized instead of broken in the middle.  You may also
 * want to {@link #setSingleLine} or {@link #setHorizontallyScrolling}
 * to constrain the text to a single line.  Use <code>NULL</code>
 * to turn off ellipsizing.
 *
 * @attr ref android.R.styleable#TextView_ellipsize
 */
ECode TextView::SetEllipsize(
    /* [in] */ TextUtilsTruncateAt where)
{
    mEllipsize = where;

    if (mLayout != NULL) {
        NullLayouts();
        RequestLayout();
        Invalidate();
    }
    return NOERROR;
}

/**
 * Sets how many times to repeat the marquee animation. Only applied if the
 * TextView has marquee enabled. Set to -1 to repeat indefinitely.
 *
 * @attr ref android.R.styleable#TextView_marqueeRepeatLimit
 */
ECode TextView::SetMarqueeRepeatLimit(
    /* [in] */ Int32 marqueeLimit)
{
    mMarqueeRepeatLimit = marqueeLimit;
    return NOERROR;
}

/**
 * Returns where, if anywhere, words that are longer than the view
 * is wide should be ellipsized.
 */
TextUtilsTruncateAt TextView::GetEllipsize()
{
    return mEllipsize;
}

/**
 * Set the TextView so that when it takes focus, all the text is
 * selected.
 *
 * @attr ref android.R.styleable#TextView_selectAllOnFocus
 */
ECode TextView::SetSelectAllOnFocus(
    /* [in] */ Boolean selectAllOnFocus)
{
    mSelectAllOnFocus = selectAllOnFocus;

    /*if (selectAllOnFocus && !(ISpannable::Probe(mText))) {
        setText(mText, BufferType.SPANNABLE);
    }*/
    return NOERROR;
}

/**
 * Set whether the cursor is visible.  The default is TRUE.
 *
 * @attr ref android.R.styleable#TextView_cursorVisible
 */
ECode TextView::SetCursorVisible(
    /* [in] */ Boolean visible)
{
    mCursorVisible = visible;
    Invalidate();

    if (visible) {
        MakeBlink();
    }
    else if (mBlink != NULL) {
        mBlink->RemoveCallbacks(mBlink);
    }

    // InsertionPointCursorController depends on mCursorVisible
    PrepareCursorControllers();
    return NOERROR;
}

Boolean TextView::CanMarquee()
{
    Float extent;
    mLayout->GetLineWidth(0, &extent);
    Int32 width = (mRight - mLeft - GetCompoundPaddingLeft() - GetCompoundPaddingRight());
    return width > 0 && extent > width;
}

void TextView::StartMarquee()
{
    // Do not ellipsize EditText
    /*if (mInput != NULL) return;

    if (compressText(getWidth() - getCompoundPaddingLeft() - getCompoundPaddingRight())) {
        return;
    }

    if ((mMarquee == NULL || mMarquee.isStopped()) && (isFocused() || isSelected()) &&
            getLineCount() == 1 && canMarquee()) {

        if (mMarquee == NULL) mMarquee = new Marquee(this);
        mMarquee.start(mMarqueeRepeatLimit);
    }*/
}

void TextView::StopMarquee()
{
    /*if (mMarquee != NULL && !mMarquee.isStopped()) {
        mMarquee.stop();
    }*/
}

void TextView::StartStopMarquee(
    /* [in] */ Boolean start)
{
    /*if (mEllipsize == TextUtils.TruncateAt.MARQUEE) {
        if (start) {
            startMarquee();
        } else {
            stopMarquee();
        }
    }*/
}

/**
 * This method is called when the text is changed, in case any
 * subclasses would like to know.
 *
 * @param text The text the TextView is displaying.
 * @param start The offset of the start of the range of the text
 *              that was modified.
 * @param before The offset of the former end of the range of the
 *               text that was modified.  If text was simply inserted,
 *               this will be the same as <code>start</code>.
 *               If text was replaced with new text or deleted, the
 *               length of the old text was <code>before-start</code>.
 * @param after The offset of the end of the range of the text
 *              that was modified.  If text was simply deleted,
 *              this will be the same as <code>start</code>.
 *              If text was replaced with new text or inserted,
 *              the length of the new text is <code>after-start</code>.
 */
void TextView::OnTextChanged(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
}

/**
 * This method is called when the selection has changed, in case any
 * subclasses would like to know.
 *
 * @param selStart The new selection start location.
 * @param selEnd The new selection end location.
 */
void TextView::OnSelectionChanged(
    /* [in] */ Int32 selStart,
    /* [in] */ Int32 selEnd)
{
}

/**
 * Adds a TextWatcher to the list of those whose methods are called
 * whenever this TextView's text changes.
 * <p>
 * In 1.0, the {@link TextWatcher#afterTextChanged} method was erroneously
 * not called after {@link #setText} calls.  Now, doing {@link #setText}
 * if there are any text changed listeners forces the buffer type to
 * Editable if it would not otherwise be and does call this method.
 */
void TextView::AddTextChangedListener(
    /* [in] */ ITextWatcher* watcher)
{
    mListeners.PushBack(watcher);
}

/**
 * Removes the specified TextWatcher from the list of those whose
 * methods are called
 * whenever this TextView's text changes.
 */
void TextView::RemoveTextChangedListener(
    /* [in] */ ITextWatcher* watcher)
{
    List<AutoPtr<ITextWatcher> >::Iterator iter = mListeners.Begin();
    for (; iter != mListeners.End(); ++iter) {
        if (iter->Get() == watcher) {
            mListeners.Erase(iter);
            break;
        }
    }
}

void TextView::SendBeforeTextChanged(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    List<AutoPtr<ITextWatcher> >::Iterator iter = mListeners.Begin();
    for (; iter != mListeners.End(); ++iter) {
        (*iter)->BeforeTextChanged(text, start, before, after);
    }
}

/**
 * Not private so it can be called from an inner class without going
 * through a thunk.
 */
void TextView::SendOnTextChanged(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    List<AutoPtr<ITextWatcher> >::Iterator iter = mListeners.Begin();
    for (; iter != mListeners.End(); ++iter) {
        (*iter)->OnTextChanged(text, start, before, after);
    }
}

/**
 * Not private so it can be called from an inner class without going
 * through a thunk.
 */
void TextView::SendAfterTextChanged(
    /* [in] */ IEditable* text)
{
    List<AutoPtr<ITextWatcher> >::Iterator iter = mListeners.Begin();
    for (; iter != mListeners.End(); ++iter) {
        (*iter)->AfterTextChanged(text);
    }
}

/**
 * Not private so it can be called from an inner class without going
 * through a thunk.
 */
void TextView::HandleTextChanged(
    /* [in] */ ICharSequence* buffer,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 after)
{
    InputMethodState* ims = mInputMethodState;
    if (ims == NULL || ims->mBatchEditNesting == 0) {
        UpdateAfterEdit();
    }
    if (ims != NULL) {
        ims->mContentChanged = TRUE;
        if (ims->mChangedStart < 0) {
            ims->mChangedStart = start;
            ims->mChangedEnd = start+before;
        }
        else {
            if (ims->mChangedStart > start) ims->mChangedStart = start;
            if (ims->mChangedEnd < (start+before)) ims->mChangedEnd = start+before;
        }
        ims->mChangedDelta += after-before;
    }

    SendOnTextChanged(buffer, start, before, after);
    OnTextChanged(buffer, start, before, after);

    // Hide the controller if the amount of content changed
    if (before != after) {
        HideControllers();
    }
}

/**
    * Not private so it can be called from an inner class without going
    * through a thunk.
    */
ECode TextView::SpanChange(
    /* [in] */ ISpanned* buf,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 oldStart,
    /* [in] */ Int32 newStart,
    /* [in] */ Int32 oldEnd,
    /* [in] */ Int32 newEnd)
{
    // XXX Make the start and end move together if this ends up
    // spending too much time invalidating.

    Boolean selChanged = FALSE;
    Int32 newSelStart=-1, newSelEnd=-1;

    InputMethodState* ims = mInputMethodState;

    if (what == Selection::SELECTION_END) {
        mHighlightPathBogus = TRUE;
        selChanged = TRUE;
        newSelEnd = newStart;

        if (!IsFocused()) {
            mSelectionMoved = TRUE;
        }

        if (oldStart >= 0 || newStart >= 0) {
            InvalidateCursor(Selection::GetSelectionStart(buf), oldStart, newStart);
            RegisterForPreDraw();

            if (IsFocused()) {
                mShowCursor = SystemClock::GetUptimeMillis();
                MakeBlink();
            }
        }
    }

    if (what == Selection::SELECTION_START) {
        mHighlightPathBogus = TRUE;
        selChanged = TRUE;
        newSelStart = newStart;

        if (!IsFocused()) {
            mSelectionMoved = TRUE;
        }

        if (oldStart >= 0 || newStart >= 0) {
            Int32 end = Selection::GetSelectionEnd(buf);
            InvalidateCursor(end, oldStart, newStart);
        }
    }

    if (selChanged) {
        Int32 flags;
        buf->GetSpanFlags(what, &flags);
        if ((flags & Spanned_SPAN_INTERMEDIATE) == 0) {
            if (newSelStart < 0) {
                newSelStart = Selection::GetSelectionStart(buf);
            }
            if (newSelEnd < 0) {
                newSelEnd = Selection::GetSelectionEnd(buf);
            }
            OnSelectionChanged(newSelStart, newSelEnd);
        }
    }

    // if (what instanceof UpdateAppearance ||
    //     what instanceof ParagraphStyle) {
    //     if (ims == NULL || ims->mBatchEditNesting == 0) {
    //         invalidate();
    //         mHighlightPathBogus = TRUE;
    //         checkForResize();
    //     } else {
    //         ims->mContentChanged = TRUE;
    //     }
    // }

    if (MetaKeyKeyListener::IsMetaTracker(buf, what)) {
        mHighlightPathBogus = TRUE;
        if (ims != NULL && MetaKeyKeyListener::IsSelectingMetaTracker(buf, what)) {
            ims->mSelectionModeChanged = TRUE;
        }

        if (Selection::GetSelectionStart(buf) >= 0) {
            if (ims == NULL || ims->mBatchEditNesting == 0) {
                InvalidateCursor();
            }
            else {
                ims->mCursorChanged = TRUE;
            }
        }
    }

//   if (what instanceof ParcelableSpan) {
//       // If this is a span that can be sent to a remote process,
//        // the current extract editor would be interested in it.
//        if (ims != NULL && ims->mExtracting != NULL) {
//            if (ims->mBatchEditNesting != 0) {
//                if (oldStart >= 0) {
//                    if (ims->mChangedStart > oldStart) {
//                        ims->mChangedStart = oldStart;
//                    }
//                    if (ims->mChangedStart > oldEnd) {
//                        ims->mChangedStart = oldEnd;
//                    }
//                }
//                if (newStart >= 0) {
//                    if (ims->mChangedStart > newStart) {
//                        ims->mChangedStart = newStart;
//                    }
//                    if (ims->mChangedStart > newEnd) {
//                        ims->mChangedStart = newEnd;
//                    }
//                }
//            } else {
//                if (DEBUG_EXTRACT) Log.v(LOG_TAG, "Span change outside of batch: "
//                        + oldStart + "-" + oldEnd + ","
//                        + newStart + "-" + newEnd + what);
//                ims->mContentChanged = TRUE;
//            }
//        }
//    }
    return NOERROR;
}

void TextView::MakeBlink()
{
    if (!mCursorVisible || !IsTextEditable()) {
        if (mBlink != NULL) {
            mBlink->RemoveCallbacks(mBlink);
        }

        return;
    }

    if (mBlink == NULL)
        mBlink = new Blink(this);

    mBlink->RemoveCallbacks(mBlink);
    mBlink->PostAtTime(mBlink, mShowCursor + BLINK);
}

/**
 * @hide
 */
ECode TextView::DispatchFinishTemporaryDetach()
{
    mDispatchTemporaryDetach = TRUE;
    ECode ec = View::DispatchFinishTemporaryDetach();
    mDispatchTemporaryDetach = FALSE;
    return ec;
}

ECode TextView::OnStartTemporaryDetach()
{
    ECode ec = View::OnStartTemporaryDetach();
    // Only track when onStartTemporaryDetach() is called directly,
    // usually because this instance is an editable field in a list
    if (!mDispatchTemporaryDetach) mTemporaryDetach = TRUE;
    return ec;
}

ECode TextView::OnFinishTemporaryDetach()
{
    ECode ec = View::OnFinishTemporaryDetach();
    // Only track when onStartTemporaryDetach() is called directly,
    // usually because this instance is an editable field in a list
    if (!mDispatchTemporaryDetach) mTemporaryDetach = FALSE;
    return ec;
}

void TextView::OnFocusChanged(
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    if (mTemporaryDetach) {
        // If we are temporarily in the detach state, then do nothing.
        View::OnFocusChanged(focused, direction, previouslyFocusedRect);
        return;
    }

    mShowCursor = SystemClock::GetUptimeMillis();

    EnsureEndedBatchEdit();

    if (focused) {
        Int32 selStart = GetSelectionStart();
        Int32 selEnd = GetSelectionEnd();

        // SelectAllOnFocus fields are highlighted and not selected. Do not start text selection
        // mode for these, unless there was a specific selection already started.
        Int32 len;
        mText->GetLength(&len);
        Boolean isFocusHighlighted =
            mSelectAllOnFocus && selStart == 0 && selEnd == len;
        mCreatedWithASelection = mFrozenWithFocus && HasSelection() && !isFocusHighlighted;

        if (!mFrozenWithFocus || (selStart < 0 || selEnd < 0)) {
            // If a tap was used to give focus to that view, move cursor at tap position.
            // Has to be done before onTakeFocus, which can be overloaded.
            Int32 lastTapPosition = GetLastTapPosition();
            if (lastTapPosition >= 0) {
                Selection::SetSelection(ISpannable::Probe(mText), lastTapPosition);
            }

            if (mMovement != NULL) {
                mMovement->OnTakeFocus(
                    (ITextView*)this->Probe(EIID_ITextView),
                    ISpannable::Probe(mText), direction);
            }

            // The DecorView does not have focus when the 'Done' ExtractEditText button is
            // pressed. Since it is the ViewRoot's mView, it requests focus before
            // ExtractEditText clears focus, which gives focus to the ExtractEditText.
            // This special case ensure that we keep current selection in that case.
            // It would be better to know why the DecorView does not have focus at that time.
            if ((/*IExtractEditText::Probe(this) ||*/ mSelectionMoved) &&
                    selStart >= 0 && selEnd >= 0) {
                /*
                    * Someone intentionally set the selection, so let them
                    * do whatever it is that they wanted to do instead of
                    * the default on-focus behavior.  We reset the selection
                    * here instead of just skipping the onTakeFocus() call
                    * because some movement methods do something other than
                    * just setting the selection in theirs and we still
                    * need to go through that path.
                    */
                Selection::SetSelection(ISpannable::Probe(mText), selStart, selEnd);
            }

            if (mSelectAllOnFocus) {
                Selection::SetSelection(ISpannable::Probe(mText), 0, len);
            }

            mTouchFocusSelected = TRUE;
        }

        mFrozenWithFocus = FALSE;
        mSelectionMoved = FALSE;

        if (ISpannable::Probe(mText)) {
            AutoPtr<ISpannable> sp = ISpannable::Probe(mText);
            MetaKeyKeyListener::ResetMetaState(sp);
        }

        MakeBlink();

        if (mError != NULL) {
            ShowError();
        }
    }
    else {
        if (mError != NULL) {
            HideError();
        }
        // Don't leave us in the middle of a batch edit.
        OnEndBatchEdit();

        HideInsertionPointCursorController();
        // if (IExtractEditText::Probe(this)) {
        //     // terminateTextSelectionMode would remove selection, which we want to keep when
        //     // ExtractEditText goes out of focus.
        //     mIsInTextSelectionMode = FALSE;
        // }
        // else {
            StopTextSelectionMode();
        //}

        if (mSelectionModifierCursorController != NULL) {
            ((SelectionModifierCursorController*)mSelectionModifierCursorController.Get())
                ->ResetTouchOffsets();
        }
    }

    StartStopMarquee(focused);

    if (mTransformation != NULL) {
        mTransformation->OnFocusChanged(
            (IView*)this->Probe(EIID_IView), mText,
            focused, direction, previouslyFocusedRect);
    }

    View::OnFocusChanged(focused, direction, previouslyFocusedRect);
}

Int32 TextView::GetLastTapPosition()
{
    if (mSelectionModifierCursorController != NULL) {
        Int32 lastTapPosition = ((SelectionModifierCursorController*)
                mSelectionModifierCursorController.Get())->GetMinTouchOffset();
        if (lastTapPosition >= 0) {
            // Safety check, should not be possible.
            Int32 len;
            mText->GetLength(&len);
            if (lastTapPosition > len) {
                // Log.e(LOG_TAG, "Invalid tap focus position (" + lastTapPosition + " vs "
                //         + mText.length() + ")");
                lastTapPosition = len;
            }
            return lastTapPosition;
        }
    }

    return -1;
}

ECode TextView::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    View::OnWindowFocusChanged(hasWindowFocus);

    if (hasWindowFocus) {
        if (mBlink != NULL) {
            mBlink->Uncancel();

            if (IsFocused()) {
                mShowCursor = SystemClock::GetUptimeMillis();
                MakeBlink();
            }
        }
    }
     else {
        if (mBlink != NULL) {
            mBlink->Cancel();
        }
        // Don't leave us in the middle of a batch edit.
        OnEndBatchEdit();
        // if (mInputContentType != NULL) {
        //     mInputContentType.enterDown = FALSE;
        // }
        HideControllers();
    }

    StartStopMarquee(hasWindowFocus);
    return NOERROR;
}

void TextView::OnVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    View::OnVisibilityChanged(changedView, visibility);
    if (visibility != VISIBLE) {
        HideControllers();
    }
}

/**
 * Use {@link BaseInputConnection#removeComposingSpans
 * BaseInputConnection.removeComposingSpans()} to remove any IME composing
 * state from this text view.
 */
ECode TextView::ClearComposingText()
{
    /*if (ISpannable::Probe(mText)) {
        BaseInputConnection.removeComposingSpans(ISpannable::Probe(mText));
    }*/
    return NOERROR;
}

ECode TextView::SetSelected(
    /* [in] */ Boolean selected)
{
    Boolean wasSelected = IsSelected();

    View::SetSelected(selected);

    if (selected != wasSelected && mEllipsize == TextUtilsTruncateAt_MARQUEE) {
        if (selected) {
            StartMarquee();
        }
        else {
            StopMarquee();
        }
    }
    return NOERROR;
}

void TextView::OnTapUpEvent(
    /* [in] */ Int32 prevStart,
    /* [in] */ Int32 prevEnd)
{
    Int32 start = GetSelectionStart();
    Int32 end = GetSelectionEnd();

    if (start == end) {
        Int32 len;
        mText->GetLength(&len);
        Boolean tapInsideSelectAllOnFocus =
            mSelectAllOnFocus && prevStart == 0 && prevEnd == len;
        if (start >= prevStart && start < prevEnd && !tapInsideSelectAllOnFocus) {
            // Restore previous selection
            Selection::SetSelection(ISpannable::Probe(mText), prevStart, prevEnd);

            // Tapping inside the selection displays the cut/copy/paste context menu, unless
            // this is a double tap that should simply trigger text selection mode.
            if (!mNoContextMenuOnUp)
                ShowContextMenu();
        }
        else {
            // Tapping outside stops selection mode, if any
            StopTextSelectionMode();

            Boolean selectAllGotFocus = mSelectAllOnFocus && mTouchFocusSelected;
            if (HasInsertionController() && !selectAllGotFocus) {
                GetInsertionController()->Show();
            }
        }
    }
}

Boolean TextView::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Int32 action;
    event->GetActionMasked(&action);

    if (HasInsertionController()) {
        GetInsertionController()->OnTouchEvent(event);
    }

    if (HasSelectionController()) {
        GetSelectionController()->OnTouchEvent(event);
    }

    if (action == MotionEvent_ACTION_DOWN) {
    // Reset this state; it will be re-set if super.onTouchEvent
    // causes focus to move to the view.
        mTouchFocusSelected = FALSE;
        mScrolled = FALSE;
    }

    Boolean result = View::OnTouchEvent(event);

    /*
     * Don't handle the release after a Int64 press, because it will
     * move the selection away from whatever the menu action was
     * trying to affect.
     */
    if (mEatTouchRelease && action == MotionEvent_ACTION_UP) {
        mEatTouchRelease = FALSE;
    }
    else if ((mMovement != NULL || OnCheckIsTextEditor()) &&
        ISpannable::Probe(mText) && mLayout != NULL) {
        Boolean handled = FALSE;

        // Save previous selection, in case this event is used to show the IME.
        Int32 oldSelStart = GetSelectionStart();
        Int32 oldSelEnd = GetSelectionEnd();

        Int32 oldScrollX = mScrollX;
        Int32 oldScrollY = mScrollY;

        if (mMovement != NULL) {
            Boolean res;
            mMovement->OnTouchEvent(
                (ITextView*)this->Probe(EIID_IView), ISpannable::Probe(mText),
                event, &res);
            handled |= res;
        }

        if (IsTextEditable()) {
            if (mScrollX != oldScrollX || mScrollY != oldScrollY) {
                // Hide insertion anchor while scrolling. Leave selection.
                HideInsertionPointCursorController();
                if (mSelectionModifierCursorController != NULL &&
                    mSelectionModifierCursorController->IsShowing()) {
                    mSelectionModifierCursorController->UpdatePosition();
                }
            }
            if (action == MotionEvent_ACTION_UP && IsFocused() && !mScrolled) {
                AutoPtr<ILocalInputMethodManager> imm;
                GetContext()->GetSystemService(Context_INPUT_METHOD_SERVICE, (IInterface**)&imm);

                AutoPtr<CTextViewCommitSelectionReceiver> csr;
                if (GetSelectionStart() != oldSelStart || GetSelectionEnd() != oldSelEnd ||
                        DidTouchFocusSelect()) {
                    CTextViewCommitSelectionReceiver::NewByFriend(oldSelStart, oldSelEnd, (Handle32)this,
                            (CTextViewCommitSelectionReceiver**)&csr);
                }

                Boolean ret = FALSE;
                imm->ShowSoftInputEx((IView*)this->Probe(EIID_IView), 0,
                        (ILocalResultReceiver*)csr.Get(), &ret);
                handled |= ret && (csr != NULL);

                // Cannot be done by CommitSelectionReceiver, which might not always be called,
                // for instance when dealing with an ExtractEditText.
                OnTapUpEvent(oldSelStart, oldSelEnd);
            }
        }

        if (handled) result = TRUE;
    }

    if (action == MotionEvent_ACTION_UP || action == MotionEvent_ACTION_CANCEL) {
        mNoContextMenuOnUp = FALSE;
    }

    return result;
}

void TextView::PrepareCursorControllers()
{
    Boolean windowSupportsHandles = FALSE;

    AutoPtr<IViewGroupLayoutParams> params;
    GetRootView()->GetLayoutParams((IViewGroupLayoutParams**)&params);
    if (params != NULL && IWindowManagerLayoutParams::Probe(params)) {
        CWindowManagerLayoutParams* windowParams =
         (CWindowManagerLayoutParams*)IWindowManagerLayoutParams::Probe(params.Get());
        windowSupportsHandles =
            windowParams->mType < WindowManagerLayoutParams_FIRST_SUB_WINDOW
            || windowParams->mType > WindowManagerLayoutParams_LAST_SUB_WINDOW;
    }

    // TODO Add an extra android:cursorController flag to disable the controller?
    mInsertionControllerEnabled = windowSupportsHandles && mCursorVisible && mLayout != NULL;
    mSelectionControllerEnabled = windowSupportsHandles && TextCanBeSelected() &&
            mLayout != NULL;

    if (!mInsertionControllerEnabled) {
        mInsertionPointCursorController = NULL;
    }

    if (!mSelectionControllerEnabled) {
        // Stop selection mode if the controller becomes unavailable.
        StopTextSelectionMode();
        mSelectionModifierCursorController = NULL;
    }
}

/**
 * @return True iff this TextView contains a text that can be edited.
 */
Boolean TextView::IsTextEditable()
{
    return mText != NULL && IEditable::Probe(mText) && OnCheckIsTextEditor() && IsEnabled();
}

/**
 * Returns TRUE, only while processing a touch gesture, if the initial
 * touch down event caused focus to move to the text view and as a result
 * its selection changed.  Only valid while processing the touch gesture
 * of interest.
 */
Boolean TextView::DidTouchFocusSelect()
{
    return mTouchFocusSelected;
}

ECode TextView::CancelLongPress()
{
    ECode ec = View::CancelLongPress();
    mScrolled = TRUE;
    return ec;
}

Boolean TextView::OnTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    if (mMovement != NULL && ISpannable::Probe(mText) && mLayout != NULL) {
        Boolean res;
        mMovement->OnTrackballEvent(
            (ITextView*)this->Probe(EIID_ITextView), ISpannable::Probe(mText),
            event, &res);
        if (res) {
            return TRUE;
        }
    }

    return View::OnTrackballEvent(event);
}

void TextView::SetScroller(
    /* [in] */ Scroller* s)
{
    mScroller = s;
}

Float TextView::GetLeftFadingEdgeStrength()
{
//    if (mEllipsize == TextUtils.TruncateAt.MARQUEE) {
//        if (mMarquee != NULL && !mMarquee.isStopped()) {
//            final Marquee marquee = mMarquee;
//            if (marquee.shouldDrawLeftFade()) {
//                return marquee.mScroll / getHorizontalFadingEdgeLength();
//            } else {
//                return 0.0f;
//            }
//        } else if (getLineCount() == 1) {
//            switch (mGravity & Gravity.HORIZONTAL_GRAVITY_MASK) {
//                case Gravity.LEFT:
//                    return 0.0f;
//                case Gravity.RIGHT:
//                    return (mLayout->getLineRight(0) - (mRight - mLeft) -
//                            getCompoundPaddingLeft() - getCompoundPaddingRight() -
//                            mLayout->getLineLeft(0)) / getHorizontalFadingEdgeLength();
//                case Gravity.CENTER_HORIZONTAL:
//                    return 0.0f;
//            }
//        }
//    }
    return View::GetLeftFadingEdgeStrength();
}

Float TextView::GetRightFadingEdgeStrength()
{
//    if (mEllipsize == TextUtils.TruncateAt.MARQUEE) {
//        if (mMarquee != NULL && !mMarquee.isStopped()) {
//            final Marquee marquee = mMarquee;
//            return (marquee.mMaxFadeScroll - marquee.mScroll) / getHorizontalFadingEdgeLength();
//        } else if (getLineCount() == 1) {
//            switch (mGravity & Gravity.HORIZONTAL_GRAVITY_MASK) {
//                case Gravity.LEFT:
//                    final Int32 textWidth = (mRight - mLeft) - getCompoundPaddingLeft() -
//                            getCompoundPaddingRight();
//                    final Float lineWidth = mLayout->getLineWidth(0);
//                    return (lineWidth - textWidth) / getHorizontalFadingEdgeLength();
//                case Gravity.RIGHT:
//                    return 0.0f;
//                case Gravity.CENTER_HORIZONTAL:
//                    return (mLayout->getLineWidth(0) - ((mRight - mLeft) -
//                            getCompoundPaddingLeft() - getCompoundPaddingRight())) /
//                            getHorizontalFadingEdgeLength();
//            }
//        }
//    }
    return View::GetRightFadingEdgeStrength();
}

Int32 TextView::ComputeHorizontalScrollRange()
{
    if (mLayout != NULL) {
        Int32 width;
        mLayout->GetWidth(&width);
        return width;
    }

    return View::ComputeHorizontalScrollRange();
}

Int32 TextView::ComputeVerticalScrollRange()
{
    if (mLayout != NULL) {
        Int32 height;
        mLayout->GetHeight(&height);
        return height;
    }

    return View::ComputeVerticalScrollRange();
}

Int32 TextView::ComputeVerticalScrollExtent()
{
    return GetHeight() - GetCompoundPaddingTop() - GetCompoundPaddingBottom();
}

/**
 * Returns the TextView_textColor attribute from the
 * Resources.StyledAttributes, if set, or the TextAppearance_textColor
 * from the TextView_textAppearance attribute, if TextView_textColor
 * was not set directly.
 */
AutoPtr<IColorStateList> TextView::GetTextColors(
    /* [in] */ IContext* context,
    /* [in] */ ITypedArray* attrs)
{
    AutoPtr<IColorStateList> colors;
    attrs->GetColorStateList(5/*com.android.internal.R.styleable.TextView_textColor*/,
            (IColorStateList**)&colors);

    if (colors == NULL) {
        Int32 ap;
        attrs->GetResourceId(1/*com.android.internal.R.styleable.TextView_textAppearance*/,
                -1, &ap);
        if (ap != -1) {
            AutoPtr<ITypedArray> appearance;
            ASSERT_SUCCEEDED(context->ObtainStyledAttributesEx(ap,
                    ArrayOf<Int32>(R_Styleable_TextAppearance, sizeof(R_Styleable_TextAppearance) / sizeof(Int32)),/*com.android.internal.R.styleable.TextAppearance*/
                    (ITypedArray**)&appearance));
            appearance->GetColorStateList(3/*com.android.internal.R.styleable.TextAppearance_textColor*/,
                    (IColorStateList**)&colors);
            appearance->Recycle();
        }
    }

    return colors;
}

/**
 * Returns the default color from the TextView_textColor attribute
 * from the AttributeSet, if set, or the default color from the
 * TextAppearance_textColor from the TextView_textAppearance attribute,
 * if TextView_textColor was not set directly.
 */
Int32 TextView::GetTextColor(
    /* [in] */ IContext* context,
    /* [in] */ ITypedArray* attrs,
    /* [in] */ Int32 def)
{
    AutoPtr<IColorStateList> colors = GetTextColors(context, attrs);

    if (colors == NULL) {
        return def;
    } else {
        Int32 color;
        colors->GetDefaultColor(&color);
        return color;
    }
}

Boolean TextView::OnKeyShortcut(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
//    switch (keyCode) {
//    case KeyEvent.KEYCODE_A:
//        if (canSelectText()) {
//            return onTextContextMenuItem(ID_SELECT_ALL);
//        }
//
//        break;
//
//    case KeyEvent.KEYCODE_X:
//        if (canCut()) {
//            return onTextContextMenuItem(ID_CUT);
//        }
//
//        break;
//
//    case KeyEvent.KEYCODE_C:
//        if (canCopy()) {
//            return onTextContextMenuItem(ID_COPY);
//        }
//
//        break;
//
//    case KeyEvent.KEYCODE_V:
//        if (canPaste()) {
//            return onTextContextMenuItem(ID_PASTE);
//        }
//
//        break;
//    }

    return View::OnKeyShortcut(keyCode, event);
}

Boolean TextView::CanSelectText()
{
    Int32 len;
    mText->GetLength(&len);
    return HasSelectionController() && len != 0;
}

Boolean TextView::TextCanBeSelected()
{
    // prepareCursorController() relies on this method.
    // If you change this condition, make sure prepareCursorController is called anywhere
    // the value of this condition might be changed.
    Boolean can;
    return (ISpannable::Probe(mText) &&
            mMovement != NULL &&
            (mMovement->CanSelectArbitrarily(&can), can));
}

Boolean TextView::CanCut()
{
    if (HasPasswordTransformationMethod()) {
        return FALSE;
    }

    Int32 len;
    mText->GetLength(&len);
    if (len > 0 && HasSelection()) {
        if (IEditable::Probe(mText) && mInput != NULL) {
            return TRUE;
        }
    }

    return FALSE;
}

Boolean TextView::CanCopy()
{
    if (HasPasswordTransformationMethod()) {
        return FALSE;
    }

    Int32 len;
    mText->GetLength(&len);
    if (len > 0 && HasSelection()) {
        return TRUE;
    }

    return FALSE;
}

Boolean TextView::CanPaste()
{
    /*return (mText instanceof Editable &&
            mInput != NULL &&
            getSelectionStart() >= 0 &&
            getSelectionEnd() >= 0 &&
            ((ClipboardManager)getContext().getSystemService(Context.CLIPBOARD_SERVICE)).
            hasText());*/
    return FALSE;
}

/**
 * Returns the offsets delimiting the 'word' located at position offset.
 *
 * @param offset An offset in the text.
 * @return The offsets for the start and end of the word located at <code>offset</code>.
 * The two ints offsets are packed in a Int64, with the starting offset shifted by 32 bits.
 * Returns a negative value if no valid word was found.
 */
Int64 TextView::GetWordLimitsAt(
    /* [in] */ Int32 offset)
{
    /*
     * Quick return if the input types one where adding words
     * to the dictionary doesn't make any sense.
     */
    Int32 klass = mInputType & InputType_TYPE_MASK_CLASS;
    if (klass == InputType_TYPE_CLASS_NUMBER ||
        klass == InputType_TYPE_CLASS_PHONE ||
        klass == InputType_TYPE_CLASS_DATETIME) {
        return -1;
    }

    Int32 variation = mInputType & InputType_TYPE_MASK_VARIATION;
    if (variation == InputType_TYPE_TEXT_VARIATION_URI ||
        variation == InputType_TYPE_TEXT_VARIATION_PASSWORD ||
        variation == InputType_TYPE_TEXT_VARIATION_VISIBLE_PASSWORD ||
        variation == InputType_TYPE_TEXT_VARIATION_EMAIL_ADDRESS ||
        variation == InputType_TYPE_TEXT_VARIATION_FILTER) {
        return -1;
    }

    Int32 len, end;
    mText->GetLength(&len);
    end = Math::Min(offset, len);

    if (end < 0) {
        return -1;
    }

    Int32 start = end;

    for (; start > 0; start--) {
        Char32 c;
        mTransformed->GetCharAt(start - 1, &c);
        Int32 type = Character::GetType(c);

        if (c != '\'' &&
            type != Character::UPPERCASE_LETTER &&
            type != Character::LOWERCASE_LETTER &&
            type != Character::TITLECASE_LETTER &&
            type != Character::MODIFIER_LETTER &&
            type != Character::DECIMAL_DIGIT_NUMBER) {
            break;
        }
    }

    for (; end < len; end++) {
        Char32 c;
        mTransformed->GetCharAt(end, &c);
        Int32 type = Character::GetType(c);

        if (c != '\'' &&
            type != Character::UPPERCASE_LETTER &&
            type != Character::LOWERCASE_LETTER &&
            type != Character::TITLECASE_LETTER &&
            type != Character::MODIFIER_LETTER &&
            type != Character::DECIMAL_DIGIT_NUMBER) {
            break;
        }
    }

    if (start == end) {
        return -1;
    }

    if (end - start > 48) {
        return -1;
    }

    Boolean hasLetter = FALSE;
    for (Int32 i = start; i < end; i++) {
        Char32 c;
        mTransformed->GetCharAt(i, &c);
        if (Character::IsLetter(c)) {
            hasLetter = TRUE;
            break;
        }
    }

    if (!hasLetter) {
        return -1;
    }

    // Two ints packed in a Int64
    return PackRangeInLong(start, end);
}

Int64 TextView::PackRangeInLong(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return (((Int64)start) << 32) | end;
}

Int32 TextView::ExtractRangeStartFromLong(
    /* [in] */ Int64 range)
{
    return (Int32)(range >> 32);
}

Int32 TextView::ExtractRangeEndFromLong(
    /* [in] */ Int64 range) {
    return (Int32)(range & 0x00000000FFFFFFFFLL);
}

void TextView::SelectCurrentWord()
{
    // In case selection mode is started after an orientation change or after a select all,
    // use the current selection instead of creating one
    if (HasSelection()) {
        return;
    }

    Int32 minOffset, maxOffset;

    if (mContextMenuTriggeredByKey) {
        minOffset = GetSelectionStart();
        maxOffset = GetSelectionEnd();
    }
    else {
        // hasSelectionController is TRUE since we canSelectText.
        SelectionModifierCursorController* selectionModifierCursorController =
            (SelectionModifierCursorController*)GetSelectionController().Get();
        minOffset = selectionModifierCursorController->GetMinTouchOffset();
        maxOffset = selectionModifierCursorController->GetMaxTouchOffset();
    }

    Int32 selectionStart, selectionEnd;

    Int64 wordLimits = GetWordLimitsAt(minOffset);
    if (wordLimits >= 0) {
        selectionStart = ExtractRangeStartFromLong(wordLimits);
    }
    else {
        selectionStart = Math::Max(minOffset - 5, 0);
    }

    wordLimits = GetWordLimitsAt(maxOffset);
    if (wordLimits >= 0) {
        selectionEnd = ExtractRangeEndFromLong(wordLimits);
    }
    else {
        Int32 len;
        mText->GetLength(&len);
        selectionEnd = Math::Min(maxOffset + 5, len);
    }

    Selection::SetSelection(ISpannable::Probe(mText), selectionStart, selectionEnd);
}

void TextView::GetWordForDictionary(
    /* [out] */ String* word)
{
    Int32 seedPosition = mContextMenuTriggeredByKey ?
        GetSelectionStart() : GetLastTapPosition();
    Int64 wordLimits = GetWordLimitsAt(seedPosition);
    if (wordLimits >= 0) {
        Int32 start = ExtractRangeStartFromLong(wordLimits);
        Int32 end = ExtractRangeEndFromLong(wordLimits);
        AutoPtr<ICharSequence> sub;
        mTransformed->SubSequence(start, end, (ICharSequence**)&sub);
        sub->ToString(word);
    }
    else {
        word = NULL;
    }
}

Boolean TextView::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    /*if (!isShown()) {
        return FALSE;
    }

    final Boolean isPassword = isPasswordInputType(mInputType);

    if (!isPassword) {
        CharSequence text = getText();
        if (TextUtils.isEmpty(text)) {
            text = getHint();
        }
        if (!TextUtils.isEmpty(text)) {
            if (text.length() > AccessibilityEvent.MAX_TEXT_LENGTH) {
                text = text.subSequence(0, AccessibilityEvent.MAX_TEXT_LENGTH + 1);
            }
            event.getText().add(text);
        }
    } else {
        event.setPassword(isPassword);
    }*/
    return FALSE;
}

void TextView::SendAccessibilityEventTypeViewTextChanged(
    /* [in] */ ICharSequence* beforeText,
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 removedCount,
    /* [in] */ Int32 addedCount)
{
    /*AccessibilityEvent event =
        AccessibilityEvent.obtain(AccessibilityEvent.TYPE_VIEW_TEXT_CHANGED);
    event.setFromIndex(fromIndex);
    event.setRemovedCount(removedCount);
    event.setAddedCount(addedCount);
    event.setBeforeText(beforeText);
    sendAccessibilityEventUnchecked(event);*/
}

void TextView::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu)
{
    View::OnCreateContextMenu(menu);
    Boolean added = FALSE;
    mContextMenuTriggeredByKey = mDPadCenterIsDown || mEnterKeyIsDown;
    // Problem with context menu on Int64 press: the menu appears while the key in down and when
    // the key is released, the view does not receive the key_up event. This ensures that the
    // state is reset whenever the context menu action is displayed.
    // mContextMenuTriggeredByKey saved that state so that it is available in
    // onTextContextMenuItem. We cannot simply clear these flags in onTextContextMenuItem since
    // it may not be called (if the user/ discards the context menu with the back key).
    mDPadCenterIsDown = mEnterKeyIsDown = FALSE;

    if (mIsInTextSelectionMode) {
        AutoPtr<MenuHandler> handler = new MenuHandler(this);

        if (CanCut()) {
            AutoPtr<IMenuItem> tmp;
            menu->AddEx3(0, ID_CUT, 0, 0x01040003/*com.android.internal.R.string.cut*/, (IMenuItem**)&tmp);
            tmp->SetOnMenuItemClickListener((IOnMenuItemClickListener*)handler->Probe(EIID_IOnMenuItemClickListener));
            tmp->SetAlphabeticShortcut('x');
            added = TRUE;
        }

        if (CanCopy()) {
            AutoPtr<IMenuItem> tmp;
            menu->AddEx3(0, ID_COPY, 0, 0x01040001/*com.android.internal.R.string.copy*/, (IMenuItem**)&tmp);
            tmp->SetOnMenuItemClickListener((IOnMenuItemClickListener*)handler->Probe(EIID_IOnMenuItemClickListener));
            tmp->SetAlphabeticShortcut('c');
            added = TRUE;
        }

        if (CanPaste()) {
            AutoPtr<IMenuItem> tmp;
            menu->AddEx3(0, ID_PASTE, 0, 0x0104000b/*com.android.internal.R.string.paste*/, (IMenuItem**)&tmp);
            tmp->SetOnMenuItemClickListener((IOnMenuItemClickListener*)handler->Probe(EIID_IOnMenuItemClickListener));
            tmp->SetAlphabeticShortcut('v');
            added = TRUE;
        }
    }
    else {
        AutoPtr<MenuHandler> handler = new MenuHandler(this);

        if (CanSelectText()) {
            AutoPtr<IMenuItem> tmp;
            if (!HasPasswordTransformationMethod()) {
                // selectCurrentWord is not available on a password field and would return an
                // arbitrary 10-charater selection around pressed position. Discard it.
                // SelectAll is still useful to be able to clear the field using the delete key.
                menu->AddEx3(0, ID_START_SELECTING_TEXT, 0, 0x010402ec/*com.android.internal.R.string.selectText*/, (IMenuItem**)&tmp);
                tmp->SetOnMenuItemClickListener((IOnMenuItemClickListener*)handler->Probe(EIID_IOnMenuItemClickListener));
            }
            tmp = NULL;
            menu->AddEx3(0, ID_SELECT_ALL, 0, 0x0104000d/*com.android.internal.R.string.selectAll*/, (IMenuItem**)&tmp);
            tmp->SetOnMenuItemClickListener((IOnMenuItemClickListener*)handler->Probe(EIID_IOnMenuItemClickListener));
            tmp->SetAlphabeticShortcut('a');
            added = TRUE;
        }

        if (mText != NULL && mText->Probe(EIID_ISpanned) != NULL) {
            Int32 selStart = GetSelectionStart();
            Int32 selEnd = GetSelectionEnd();

            Int32 min = Math::Min(selStart, selEnd);
            Int32 max = Math::Max(selStart, selEnd);

            ArrayOf<IInterface*>* urls = NULL;
            ISpanned::Probe(mText)->GetSpans(min, max, EIID_IURLSpan, &urls);
            if (urls->GetLength() == 1) {
                AutoPtr<IMenuItem> tmp;
                menu->AddEx3(0, ID_COPY_URL, 0, 0x01040002/*com.android.internal.R.string.copyUrl*/, (IMenuItem**)&tmp);
                tmp->SetOnMenuItemClickListener((IOnMenuItemClickListener*)handler->Probe(EIID_IOnMenuItemClickListener));
                added = TRUE;
            }
            for (Int32 i = 0; i < urls->GetLength(); ++i) {
                (*urls)[i]->Release();
                (*urls)[i] = NULL;
            }
            ArrayOf<IInterface*>::Free(urls);
        }

        if (CanPaste()) {
            AutoPtr<IMenuItem> tmp;
            menu->AddEx3(0, ID_PASTE, 0, 0x0104000b/*com.android.internal.R.string.paste*/, (IMenuItem**)&tmp);
            tmp->SetOnMenuItemClickListener((IOnMenuItemClickListener*)handler->Probe(EIID_IOnMenuItemClickListener));
            tmp->SetAlphabeticShortcut('v');
            added = TRUE;
        }

        if (IsInputMethodTarget()) {
            AutoPtr<IMenuItem> tmp;
            menu->AddEx3(1, ID_SWITCH_INPUT_METHOD, 0, 0x010402ed/*com.android.internal.R.string.inputMethod*/, (IMenuItem**)&tmp);
            tmp->SetOnMenuItemClickListener((IOnMenuItemClickListener*)handler->Probe(EIID_IOnMenuItemClickListener));
            added = TRUE;
        }

        String word;
        GetWordForDictionary(&word);
        if (!word.IsNull()) {
            AutoPtr<IResources> resources;
            GetContext()->GetResources((IResources**)&resources);

            String dic;
            resources->GetString(0x010402ee/*com.android.internal.R.string.addToDictionary*/, &dic);
            AutoPtr<ICharSequence> csq;
            CStringWrapper::New(dic, (ICharSequence**)&csq);

            AutoPtr<IMenuItem> tmp;
            menu->AddEx2(1, ID_ADD_TO_DICTIONARY, 0, csq, (IMenuItem**)&tmp);
            tmp->SetOnMenuItemClickListener((IOnMenuItemClickListener*)handler->Probe(EIID_IOnMenuItemClickListener));
            added = TRUE;
        }
    }

    if (added) {
       HideControllers();
       menu->SetHeaderTitle(0x010402ef/*com.android.internal.R.string.editTextMenuTitle*/);
    }
}

/**
 * Returns whether this text view is a current input method target.  The
 * default implementation just checks with {@link InputMethodManager}.
 */
Boolean TextView::IsInputMethodTarget()
{
    AutoPtr<ILocalInputMethodManager> imm = CLocalInputMethodManager::PeekInstance();
    Boolean isTarget = FALSE;
    imm->IsActive((IView*)this->Probe(EIID_IView), &isTarget);
    return imm != NULL && isTarget;
}

/**
 * Called when a context menu option for the text view is selected.  Currently
 * this will be one of: {@link android.R.id#selectAll},
 * {@link android.R.id#startSelectingText},
 * {@link android.R.id#cut}, {@link android.R.id#copy},
 * {@link android.R.id#paste}, {@link android.R.id#copyUrl},
 * or {@link android.R.id#switchInputMethod}.
 */
Boolean TextView::OnTextContextMenuItem(
    /* [in] */ Int32 id)
{
    assert(0);
    // Int32 min = 0;
    // Int32 max = 0;
    // mText->GetLength(&max);

    // if (IsFocused()) {
    //     Int32 selStart = GetSelectionStart();
    //     Int32 selEnd = GetSelectionEnd();


    //     min = Math::Max(0, Math::Min(selStart, selEnd));
    //     max = Math::Max(0, Math::Max(selStart, selEnd));
    // }

    // ClipboardManager clip = (ClipboardManager)getContext()
    //         .getSystemService(Context.CLIPBOARD_SERVICE);

    switch (id) {
    //     case ID_SELECT_ALL:
    //         Selection::SetSelection(ISpannable::Probe(mText), 0, mText.length());
    //         startTextSelectionMode();
    //         getSelectionController().show();
    //         return TRUE;

    //     case ID_START_SELECTING_TEXT:
    //         startTextSelectionMode();
    //         getSelectionController().show();
    //         return TRUE;

    //     case ID_CUT:
    //         clip.setText(mTransformed.subSequence(min, max));
    //         (IEditable::Probe(mText)).delete(min, max);
    //         stopTextSelectionMode();
    //         return TRUE;

    //     case ID_COPY:
    //         clip.setText(mTransformed.subSequence(min, max));
    //         stopTextSelectionMode();
    //         return TRUE;

    //     case ID_PASTE:
    //         CharSequence paste = clip.getText();

    //         if (paste != NULL && paste.length() > 0) {
    //             Int64 minMax = prepareSpacesAroundPaste(min, max, paste);
    //             min = extractRangeStartFromLong(minMax);
    //             max = extractRangeEndFromLong(minMax);
    //             Selection::SetSelection(ISpannable::Probe(mText), max);
    //             (IEditable::Probe(mText)).replace(min, max, paste);
    //             stopTextSelectionMode();
    //         }
    //         return TRUE;

    //     case ID_COPY_URL:
    //         URLSpan[] urls = ((Spanned) mText).getSpans(min, max, URLSpan.class);
    //         if (urls.length == 1) {
    //             clip.setText(urls[0].getURL());
    //         }
    //         return TRUE;

            case ID_SWITCH_INPUT_METHOD: {
                AutoPtr<ILocalInputMethodManager> imm = CLocalInputMethodManager::PeekInstance();
                if (imm != NULL) {
                    imm->ShowInputMethodPicker();
                }
                return TRUE;
            }

    //     case ID_ADD_TO_DICTIONARY:
    //         String word = getWordForDictionary();
    //         if (word != NULL) {
    //             Intent i = new Intent("com.android.settings.USER_DICTIONARY_INSERT");
    //             i.putExtra("word", word);
    //             i.setFlags(i.getFlags() | Intent.FLAG_ACTIVITY_NEW_TASK);
    //             getContext().startActivity(i);
    //         }
    //         return TRUE;
         }

    return FALSE;
}

/**
 * Prepare text so that there are not zero or two spaces at beginning and end of region defined
 * by [min, max] when replacing this region by paste.
 */
Int64 TextView::PrepareSpacesAroundPaste(
    /* [in] */ Int32 min,
    /* [in] */ Int32 max,
    /* [in] */ ICharSequence* paste)
{
    // Paste adds/removes spaces before or after insertion as needed.
    //if (Character.isSpaceChar(paste.charAt(0))) {
    //    if (min > 0 && Character.isSpaceChar(mTransformed.charAt(min - 1))) {
    //        // Two spaces at beginning of paste: remove one
    //        final Int32 originalLength = mText.length();
    //        (IEditable::Probe(mText)).replace(min - 1, min, "");
    //        // Due to filters, there is no garantee that exactly one character was
    //        // removed. Count instead.
    //        final Int32 delta = mText.length() - originalLength;
    //        min += delta;
    //        max += delta;
    //    }
    //} else {
    //    if (min > 0 && !Character.isSpaceChar(mTransformed.charAt(min - 1))) {
    //        // No space at beginning of paste: add one
    //        final Int32 originalLength = mText.length();
    //        (IEditable::Probe(mText)).replace(min, min, " ");
    //        // Taking possible filters into account as above.
    //        final Int32 delta = mText.length() - originalLength;
    //        min += delta;
    //        max += delta;
    //    }
    //}

    //if (Character.isSpaceChar(paste.charAt(paste.length() - 1))) {
    //    if (max < mText.length() && Character.isSpaceChar(mTransformed.charAt(max))) {
    //        // Two spaces at end of paste: remove one
    //        (IEditable::Probe(mText)).replace(max, max + 1, "");
    //    }
    //} else {
    //    if (max < mText.length() && !Character.isSpaceChar(mTransformed.charAt(max))) {
    //        // No space at end of paste: add one
    //        (IEditable::Probe(mText)).replace(max, max, " ");
    //    }
    //}
    return PackRangeInLong(min, max);
}

Boolean TextView::PerformLongClick()
{
    if (View::PerformLongClick()) {
        mEatTouchRelease = TRUE;
        return TRUE;
    }

    return FALSE;
}

void TextView::StartTextSelectionMode()
{
    if (!mIsInTextSelectionMode) {
        if (!HasSelectionController()) {
            Logger::W(TEXT_VIEW_LOG_TAG,
                "TextView has no selection controller. Action mode cancelled.");
            return;
        }

        if (!CanSelectText() || !RequestFocus()) {
            return;
        }

        SelectCurrentWord();
        GetSelectionController()->Show();
        AutoPtr<ILocalInputMethodManager> imm;
        GetContext()->GetSystemService(Context_INPUT_METHOD_SERVICE,
                (IInterface**)&imm);
        Boolean ret = FALSE;
        imm->ShowSoftInputEx((IView*)this->Probe(EIID_IView), 0, NULL, &ret);
        mIsInTextSelectionMode = TRUE;
    }
}

void TextView::StopTextSelectionMode()
{
    if (mIsInTextSelectionMode) {
        Selection::SetSelection(ISpannable::Probe(mText), GetSelectionEnd());
        HideSelectionModifierCursorController();
        mIsInTextSelectionMode = FALSE;
    }
}

void TextView::HideInsertionPointCursorController()
{
    if (mInsertionPointCursorController != NULL) {
        mInsertionPointCursorController->Hide();
    }
}

void TextView::HideSelectionModifierCursorController()
{
    if (mSelectionModifierCursorController != NULL) {
        mSelectionModifierCursorController->Hide();
    }
}

void TextView::HideControllers()
{
    HideInsertionPointCursorController();
    HideSelectionModifierCursorController();
}

Int32 TextView::GetOffsetForHorizontal(
    /* [in] */ Int32 line,
    /* [in] */ Int32 x)
{
    Int32 offset;
    x -= GetTotalPaddingLeft();
    // Clamp the position to inside of the view.
    x = Math::Max(0, x);
    x = Math::Min(GetWidth() - GetTotalPaddingRight() - 1, x);
    x += GetScrollX();
    GetLayout()->GetOffsetForHorizontal(line, x, &offset);
    return offset;
}

/**
 * Get the offset character closest to the specified absolute position.
 *
 * @param x The horizontal absolute position of a point on screen
 * @param y The vertical absolute position of a point on screen
 * @return the character offset for the character whose position is closest to the specified
 *  position. Returns -1 if there is no layout.
 *
 * @hide
 */
Int32 TextView::GetOffset(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (GetLayout() == NULL) return -1;

    y -= GetTotalPaddingTop();
    // Clamp the position to inside of the view.
    y = Math::Max(0, y);
    y = Math::Min(GetHeight() - GetTotalPaddingBottom() - 1, y);
    y += GetScrollY();

    Int32 line, offset;
    GetLayout()->GetLineForVertical(y, &line);
    offset = GetOffsetForHorizontal(line, x);
    return offset;
}

Int32 TextView::GetHysteresisOffset(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 previousOffset)
{
    AutoPtr<ILayout> layout = GetLayout();
    if (layout == NULL) return -1;

    y -= GetTotalPaddingTop();
    // Clamp the position to inside of the view.
    y = Math::Max(0, y);
    y = Math::Min(GetHeight() - GetTotalPaddingBottom() - 1, y);
    y += GetScrollY();

    Int32 line, previousLine, previousLineTop, previousLineBottom;
    Int32 hysteresisThreshold;

    layout->GetLineForVertical(y, &line);
    layout->GetLineForOffset(previousOffset, &previousLine);
    layout->GetLineTop(previousLine, &previousLineTop);
    layout->GetLineBottom(previousLine, &previousLineBottom);
    hysteresisThreshold = (previousLineBottom - previousLineTop) / 8;

    // If new line is just before or after previous line and y position is less than
    // hysteresisThreshold away from previous line, keep cursor on previous line.
    if (((line == previousLine + 1) && ((y - previousLineBottom) < hysteresisThreshold)) ||
        ((line == previousLine - 1) && ((previousLineTop - y)    < hysteresisThreshold))) {
        line = previousLine;
    }

    return GetOffsetForHorizontal(line, x);
}

/**
 * @return True if this view supports insertion handles.
 */
Boolean TextView::HasInsertionController()
{
    return mInsertionControllerEnabled;
}

/**
 * @return True if this view supports selection handles.
 */
Boolean TextView::HasSelectionController()
{
    return mSelectionControllerEnabled;
}

AutoPtr<TextView::CursorController> TextView::GetInsertionController()
{
    if (!mInsertionControllerEnabled) {
        return NULL;
    }

    if (mInsertionPointCursorController == NULL) {
        mInsertionPointCursorController = new InsertionPointCursorController(this);

        // final ViewTreeObserver observer = getViewTreeObserver();
        // if (observer != NULL) {
        //     observer.addOnTouchModeChangeListener(mInsertionPointCursorController);
        // }
    }

    return mInsertionPointCursorController;
}

AutoPtr<TextView::CursorController> TextView::GetSelectionController()
{
    if (!mSelectionControllerEnabled) {
        return NULL;
    }

    if (mSelectionModifierCursorController == NULL) {
        mSelectionModifierCursorController = new SelectionModifierCursorController(this);

        // final ViewTreeObserver observer = getViewTreeObserver();
        // if (observer != NULL) {
        //     observer.addOnTouchModeChangeListener(mSelectionModifierCursorController);
        // }
    }

    return mSelectionModifierCursorController;
}

Boolean TextView::IsInBatchEditMode()
{
    InputMethodState* ims = mInputMethodState;
    if (ims != NULL) {
        return ims->mBatchEditNesting > 0;
    }
    return mInBatchEditControllers;
}

ECode TextView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(View::Init(context, attrs, defStyle));
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
    return NOERROR;
}

static Int32 R_Styleable_TextView[] = {
    0x0101000e, 0x01010034, 0x01010095, 0x01010096,
    0x01010097, 0x01010098, 0x01010099, 0x0101009a,
    0x0101009b, 0x010100ab, 0x010100af, 0x010100b0,
    0x010100b1, 0x0101011f, 0x01010120, 0x0101013f,
    0x01010140, 0x0101014e, 0x0101014f, 0x01010150,
    0x01010151, 0x01010152, 0x01010153, 0x01010154,
    0x01010155, 0x01010156, 0x01010157, 0x01010158,
    0x01010159, 0x0101015a, 0x0101015b, 0x0101015c,
    0x0101015d, 0x0101015e, 0x0101015f, 0x01010160,
    0x01010161, 0x01010162, 0x01010163, 0x01010164,
    0x01010165, 0x01010166, 0x01010167, 0x01010168,
    0x01010169, 0x0101016a, 0x0101016b, 0x0101016c,
    0x0101016d, 0x0101016e, 0x0101016f, 0x01010170,
    0x01010171, 0x01010217, 0x01010218, 0x0101021d,
    0x01010220, 0x01010223, 0x01010224, 0x01010264,
    0x01010265, 0x01010266, 0x010102c5, 0x010102c6,
    0x010102c7
};

static Int32 R_Styleable_View[] = {
    0x01010063, 0x01010064, 0x01010065, 0x01010066,
    0x01010067, 0x01010068, 0x01010069, 0x0101007f,
    0x010100d0, 0x010100d1, 0x010100d2, 0x010100d3,
    0x010100d4, 0x010100d5, 0x010100d6, 0x010100d7,
    0x010100d8, 0x010100d9, 0x010100da, 0x010100db,
    0x010100dc, 0x010100dd, 0x010100de, 0x010100df,
    0x010100e0, 0x010100e1, 0x010100e2, 0x010100e3,
    0x010100e4, 0x010100e5, 0x010100e6, 0x010100e7,
    0x010100e8, 0x010100e9, 0x0101013f, 0x01010140,
    0x01010215, 0x01010216, 0x0101024e, 0x0101025e,
    0x0101026f, 0x01010273, 0x010102a8, 0x010102a9,
    0x010102aa, 0x010102c1, 0x010102c4
};

ECode TextView::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(CSpannableFactory::AcquireSingleton(
        (ISpannableFactory**)&mSpannableFactory));
    ASSERT_SUCCEEDED(CEditableFactory::AcquireSingleton(
        (IEditableFactory**)&mEditableFactory));

    CStringWrapper::New(String(""), (ICharSequence**)&mText);

    AutoPtr<IDisplayMetrics> dm;
    AutoPtr<ICompatibilityInfo> ci;
    GetResources()->GetDisplayMetrics((IDisplayMetrics**)&dm);
    GetResources()->GetCompatibilityInfo((ICompatibilityInfo**)&ci);
    CTextPaint::NewByFriend(Paint_ANTI_ALIAS_FLAG, (CTextPaint**)&mTextPaint);
    mTextPaint->mDensity = ((CDisplayMetrics*)dm.Get())->mDensity;
    mTextPaint->SetCompatibilityScaling(
            ((CCompatibilityInfo*)ci.Get())->mApplicationScale);

    // If we get the paint from the skin, we should set it to left, since
    // the layout always wants it to be left.
    // mTextPaint.setTextAlign(Paint.Align.LEFT);

    ASSERT_SUCCEEDED(CPaint::New(Paint_ANTI_ALIAS_FLAG,
            (IPaint**)&mHighlightPaint));
    AutoPtr<ICompatibilityInfo> compatibilityInfo;
    ASSERT_SUCCEEDED(GetResources()->GetCompatibilityInfo(
            (ICompatibilityInfo**)&compatibilityInfo));
    mHighlightPaint->SetCompatibilityScaling(
            ((CCompatibilityInfo*)compatibilityInfo.Get())->mApplicationScale);

    mMovement = GetDefaultMovementMethod();
    mTransformation = NULL;

    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributesEx3(attrs,
            ArrayOf<Int32>(R_Styleable_TextView, sizeof(R_Styleable_TextView) / sizeof(Int32)),/*com.android.internal.R.styleable.TextView*/
            defStyle, 0, (ITypedArray**)&a));

    Int32 textColorHighlight = 0;
    AutoPtr<IColorStateList> textColor;
    AutoPtr<IColorStateList> textColorHint;
    AutoPtr<IColorStateList> textColorLink;
    Int32 textSize = 15;
    Int32 typefaceIndex = -1;
    Int32 styleIndex = -1;

    /*
     * Look the appearance up without checking first if it exists because
     * almost every TextView has one and it greatly simplifies the logic
     * to be able to parse the appearance first and then let specific tags
     * for this View override it.
     */
    AutoPtr<ITypedArray> appearance;
    Int32 ap;
    a->GetResourceId(1/*com.android.internal.R.styleable.TextView_textAppearance*/,
            -1, &ap);
    if (ap != -1) {
        ASSERT_SUCCEEDED(context->ObtainStyledAttributesEx(ap,
                ArrayOf<Int32>(R_Styleable_TextAppearance,
                sizeof(R_Styleable_TextAppearance) / sizeof(Int32)),/*com.android.internal.R.styleable.TextAppearance*/
                (ITypedArray**)&appearance));
    }
    if (appearance != NULL) {
        Int32 N;
        appearance->GetIndexCount(&N);
        for (Int32 i = 0; i < N; i++) {
            Int32 attr;
            appearance->GetIndex(i, &attr);

            //printf("TextView::Init---appearance---Index = %d, attr = %d\n", i, attr);
            switch (attr) {
            case 4/*com.android.internal.R.styleable.TextAppearance_textColorHighlight*/:
                appearance->GetColor(attr, textColorHighlight, &textColorHighlight);
                break;

            case 3/*com.android.internal.R.styleable.TextAppearance_textColor*/:
                appearance->GetColorStateList(attr, (IColorStateList**)&textColor);
                //printf("textColor = 0x%08x\n", (Int32)textColor.Get());
                break;

            case 5/*com.android.internal.R.styleable.TextAppearance_textColorHint*/:
                appearance->GetColorStateList(attr, (IColorStateList**)&textColorHint);
                break;

            case 6/*com.android.internal.R.styleable.TextAppearance_textColorLink*/:
                appearance->GetColorStateList(attr, (IColorStateList**)&textColorLink);
                break;

            case 0/*com.android.internal.R.styleable.TextAppearance_textSize*/:
                appearance->GetDimensionPixelSize(attr, textSize, &textSize);
                break;

            case 1/*com.android.internal.R.styleable.TextAppearance_typeface*/:
                appearance->GetInt32(attr, -1, &typefaceIndex);
                break;

            case 2/*com.android.internal.R.styleable.TextAppearance_textStyle*/:
                appearance->GetInt32(attr, -1, &styleIndex);
                break;
            }
        }

        appearance->Recycle();
    }

    Boolean editable = GetDefaultEditable();
    AutoPtr<ICharSequence> inputMethod;
    Int32 numeric = 0;
    AutoPtr<ICharSequence> digits;
    Boolean phone = FALSE;
    Boolean autotext = FALSE;
    Int32 autocap = -1;
    Int32 buffertype = 0;
    Boolean selectallonfocus = FALSE;
    AutoPtr<IDrawable> drawableLeft, drawableTop, drawableRight, drawableBottom;
    Int32 drawablePadding = 0;
    Int32 ellipsize = -1;
    Boolean singleLine = FALSE;
    Int32 maxlength = -1;
    AutoPtr<ICharSequence> text;
    CStringWrapper::New(String(""), (ICharSequence**)&text);
    AutoPtr<ICharSequence> hint;
    Int32 shadowcolor = 0;
    Float dx = 0, dy = 0, r = 0;
    Boolean password = FALSE;
    Int32 inputType = InputType_TYPE_NULL;

    Int32 N;
    a->GetIndexCount(&N);
    for (Int32 i = 0; i < N; i++) {
        Int32 attr;
        a->GetIndex(i, &attr);

        //printf("TextView::Init---Index = %d, attr = %d\n", i, attr);
        Int32 value;
        Boolean bValue;
        Float fValue;
        switch (attr) {
        case 46/*com.android.internal.R.styleable.TextView_editable*/:
            a->GetBoolean(attr, editable, &editable);
            break;

        case 43/*com.android.internal.R.styleable.TextView_inputMethod*/:
            a->GetText(attr, (ICharSequence**)&inputMethod);
            break;

        case 40/*com.android.internal.R.styleable.TextView_numeric*/:
            a->GetInt32(attr, numeric, &numeric);
            break;

        case 41/*com.android.internal.R.styleable.TextView_digits*/:
            a->GetText(attr, (ICharSequence**)&digits);
            break;

        case 42/*com.android.internal.R.styleable.TextView_phoneNumber*/:
            a->GetBoolean(attr, phone, &phone);
            break;

        case 45/*com.android.internal.R.styleable.TextView_autoText*/:
            a->GetBoolean(attr, autotext, &autotext);
            break;

        case 44/*com.android.internal.R.styleable.TextView_capitalize*/:
            a->GetInt32(attr, autocap, &autocap);
            break;

        case 17/*com.android.internal.R.styleable.TextView_bufferType*/:
            a->GetInt32(attr, buffertype, &buffertype);
            break;

        case 33/*com.android.internal.R.styleable.TextView_selectAllOnFocus*/:
            a->GetBoolean(attr, selectallonfocus, &selectallonfocus);
            break;

        case 11/*com.android.internal.R.styleable.TextView_autoLink*/:
            a->GetInt32(attr, 0, &mAutoLinkMask);
            break;

        case 12/*com.android.internal.R.styleable.TextView_linksClickable*/:
            a->GetBoolean(attr, TRUE, &mLinksClickable);
            break;

        case 50/*com.android.internal.R.styleable.TextView_drawableLeft*/:
            a->GetDrawable(attr, (IDrawable**)&drawableLeft);
            break;

        case 48/*com.android.internal.R.styleable.TextView_drawableTop*/:
            a->GetDrawable(attr, (IDrawable**)&drawableTop);
            break;

        case 51/*com.android.internal.R.styleable.TextView_drawableRight*/:
            a->GetDrawable(attr, (IDrawable**)&drawableRight);
            break;

        case 49/*com.android.internal.R.styleable.TextView_drawableBottom*/:
            a->GetDrawable(attr, (IDrawable**)&drawableBottom);
            break;

        case 52/*com.android.internal.R.styleable.TextView_drawablePadding*/:
            a->GetDimensionPixelSize(attr, drawablePadding, &drawablePadding);
            break;

        case 22/*com.android.internal.R.styleable.TextView_maxLines*/:
            a->GetInt32(attr, -1, &value);
            SetMaxLines(value);
            break;
        case 14/*com.android.internal.R.styleable.TextView_maxHeight*/:
            a->GetDimensionPixelSize(attr, -1, &value);
            SetMaxHeight(value);
            break;

        case 23/*com.android.internal.R.styleable.TextView_lines*/:
            a->GetInt32(attr, -1, &value);
            SetLines(value);
            break;

        case 24/*com.android.internal.R.styleable.TextView_height*/:
            a->GetDimensionPixelSize(attr, -1, &value);
            SetHeight(value);
            break;

        case 25/*com.android.internal.R.styleable.TextView_minLines*/:
            a->GetInt32(attr, -1, &value);
            SetMinLines(value);
            break;

        case 16/*com.android.internal.R.styleable.TextView_minHeight*/:
            a->GetDimensionPixelSize(attr, -1, &value);
            SetMinHeight(value);
            break;

        case 26/*com.android.internal.R.styleable.TextView_maxEms*/:
            a->GetInt32(attr, -1, &value);
            SetMaxEms(value);
            break;

        case 13/*com.android.internal.R.styleable.TextView_maxWidth*/:
            a->GetDimensionPixelSize(attr, -1, &value);
            SetMaxWidth(value);
            break;

        case 27/*com.android.internal.R.styleable.TextView_ems*/:
            a->GetInt32(attr, -1, &value);
            SetEms(value);
            break;

        case 28/*com.android.internal.R.styleable.TextView_width*/:
            a->GetDimensionPixelSize(attr, -1, &value);
            SetWidth(value);
            break;

        case 29/*com.android.internal.R.styleable.TextView_minEms*/:
            a->GetInt32(attr, -1, &value);
            SetMinEms(value);
            break;

        case 15/*com.android.internal.R.styleable.TextView_minWidth*/:
            a->GetDimensionPixelSize(attr, -1, &value);
            SetMinWidth(value);
            break;

        case 10/*com.android.internal.R.styleable.TextView_gravity*/:
            a->GetInt32(attr, -1, &value);
            SetGravity(value);
            break;

        case 19/*com.android.internal.R.styleable.TextView_hint*/:
            a->GetText(attr, (ICharSequence**)&hint);
            break;

        case 18/*com.android.internal.R.styleable.TextView_text*/:
            text = NULL;
            a->GetText(attr, (ICharSequence**)&text);
            break;

        case 30/*com.android.internal.R.styleable.TextView_scrollHorizontally*/:
            a->GetBoolean(attr, FALSE, &bValue);
            if (bValue) {
                SetHorizontallyScrolling(TRUE);
            }
            break;

        case 32/*com.android.internal.R.styleable.TextView_singleLine*/:
            a->GetBoolean(attr, singleLine, &singleLine);
            break;

        case 9/*com.android.internal.R.styleable.TextView_ellipsize*/:
            a->GetInt32(attr, ellipsize, &ellipsize);
            break;

        case 55/*com.android.internal.R.styleable.TextView_marqueeRepeatLimit*/:
            a->GetInt32(attr, mMarqueeRepeatLimit, &value);
            SetMarqueeRepeatLimit(value);
            break;

        case 34/*com.android.internal.R.styleable.TextView_includeFontPadding*/:
            a->GetBoolean(attr, TRUE, &bValue);
            if (!bValue) {
                SetIncludeFontPadding(FALSE);
            }
            break;

        case 21/*com.android.internal.R.styleable.TextView_cursorVisible*/:
            a->GetBoolean(attr, TRUE, &bValue);
            if (!bValue) {
                SetCursorVisible(FALSE);
            }
            break;

        case 35/*com.android.internal.R.styleable.TextView_maxLength*/:
            a->GetInt32(attr, -1, &maxlength);
            break;

        case 20/*com.android.internal.R.styleable.TextView_textScaleX*/:
            a->GetFloat(attr, 1.0f, &fValue);
            SetTextScaleX(fValue);
            break;

        case 47/*com.android.internal.R.styleable.TextView_freezesText*/:
            a->GetBoolean(attr, FALSE, &mFreezesText);
            break;

        case 36/*com.android.internal.R.styleable.TextView_shadowColor*/:
            a->GetInt32(attr, 0, &shadowcolor);
            break;

        case 37/*com.android.internal.R.styleable.TextView_shadowDx*/:
            a->GetFloat(attr, 0, &dx);
            break;

        case 38/*com.android.internal.R.styleable.TextView_shadowDy*/:
            a->GetFloat(attr, 0, &dy);
            break;

        case 39/*com.android.internal.R.styleable.TextView_shadowRadius*/:
            a->GetFloat(attr, 0, &r);
            break;

        case 0/*com.android.internal.R.styleable.TextView_enabled*/:
            a->GetBoolean(attr, IsEnabled(), &bValue);
            SetEnabled(bValue);
            break;

        case 6/*com.android.internal.R.styleable.TextView_textColorHighlight*/:
            a->GetColor(attr, textColorHighlight, &textColorHighlight);
            break;

        case 5/*com.android.internal.R.styleable.TextView_textColor*/:
            a->GetColorStateList(attr, (IColorStateList**)&textColor);
            //printf("2---textColor = 0x%08x\n", (Int32)textColor.Get());
            break;

        case 7/*com.android.internal.R.styleable.TextView_textColorHint*/:
            a->GetColorStateList(attr, (IColorStateList**)&textColorHint);
            break;

        case 8/*com.android.internal.R.styleable.TextView_textColorLink*/:
            a->GetColorStateList(attr, (IColorStateList**)&textColorLink);
            break;

        case 2/*com.android.internal.R.styleable.TextView_textSize*/:
            a->GetDimensionPixelSize(attr, textSize, &textSize);
            break;

        case 3/*com.android.internal.R.styleable.TextView_typeface*/:
            a->GetInt32(attr, typefaceIndex, &typefaceIndex);
            break;

        case 4/*com.android.internal.R.styleable.TextView_textStyle*/:
            a->GetInt32(attr, styleIndex, &styleIndex);
            break;

        case 31/*com.android.internal.R.styleable.TextView_password*/:
            a->GetBoolean(attr, password, &password);
            break;

        case 53/*com.android.internal.R.styleable.TextView_lineSpacingExtra*/:
            a->GetDimensionPixelSize(attr, (Int32)mSpacingAdd, &value);
            mSpacingAdd = value;
            break;

        case 54/*com.android.internal.R.styleable.TextView_lineSpacingMultiplier*/:
            a->GetFloat(attr, mSpacingMult, &mSpacingMult);
            break;

        case 56/*com.android.internal.R.styleable.TextView_inputType*/:
            a->GetInt32(attr, mInputType, &inputType);
            break;

        case 59/*com.android.internal.R.styleable.TextView_imeOptions*/:
//            if (mInputContentType == NULL) {
//                mInputContentType = new InputContentType();
//            }
//            mInputContentType.imeOptions = a.getInt(attr,
//                    mInputContentType.imeOptions);
            break;

        case 60/*com.android.internal.R.styleable.TextView_imeActionLabel*/:
//            if (mInputContentType == NULL) {
//                mInputContentType = new InputContentType();
//            }
//            mInputContentType.imeActionLabel = a.getText(attr);
            break;

        case 61/*com.android.internal.R.styleable.TextView_imeActionId*/:
//            if (mInputContentType == NULL) {
//                mInputContentType = new InputContentType();
//            }
//            mInputContentType.imeActionId = a.getInt(attr,
//                    mInputContentType.imeActionId);
            break;

        case 57/*com.android.internal.R.styleable.TextView_privateImeOptions*/:
        {
            String str;
            a->GetString(attr, &str);
            SetPrivateImeOptions(str);
            break;
        }
        case 58/*com.android.internal.R.styleable.TextView_editorExtras*/:
        {
//            try {
            a->GetResourceId(attr, 0, &value);
            ECode ec = SetInputExtras(value);
            if (ec == (Int32)E_XML_PULL_PARSER_EXCEPTION) {

            }
            else if (ec == (Int32)E_IO_EXCEPTION) {

            }
//            } catch (XmlPullParserException e) {
//                Log.w(LOG_TAG, "Failure reading input extras", e);
//            } catch (IOException e) {
//                Log.w(LOG_TAG, "Failure reading input extras", e);
//            }
            break;
        }
        case 62/*com.android.internal.R.styleable.TextView_textSelectHandleLeft*/:
            a->GetResourceId(attr, 0, &mTextSelectHandleLeftRes);
            break;

        case 63/*com.android.internal.R.styleable.TextView_textSelectHandleRight*/:
            a->GetResourceId(attr, 0, &mTextSelectHandleRightRes);
            break;

        case 64/*com.android.internal.R.styleable.TextView_textSelectHandle*/:
            a->GetResourceId(attr, 0, &mTextSelectHandleRes);
            break;
        }
    }
    a->Recycle();

    BufferType bufferType = BufferType_EDITABLE;

    if ((inputType & (InputType_TYPE_MASK_CLASS | InputType_TYPE_MASK_VARIATION))
            == (InputType_TYPE_CLASS_TEXT | InputType_TYPE_TEXT_VARIATION_PASSWORD)) {
        password = TRUE;
    }

    if (inputMethod != NULL) {
        assert(0);
//        Class<?> c;
//
//        try {
//            c = Class.forName(inputMethod.toString());
//        } catch (ClassNotFoundException ex) {
//            throw new RuntimeException(ex);
//        }
//
//        try {
//            mInput = (KeyListener) c.newInstance();
//        } catch (InstantiationException ex) {
//            throw new RuntimeException(ex);
//        } catch (IllegalAccessException ex) {
//            throw new RuntimeException(ex);
//        }
//        try {
//            mInputType = inputType != InputType_TYPE_NULL
//                    ? inputType
//                    : mInput.getInputType();
//        } catch (IncompatibleClassChangeError e) {
//            mInputType = InputType_TYPE_CLASS_TEXT;
//        }
    }
    else if (digits != NULL) {
        assert(0);
//        mInput = DigitsKeyListener.getInstance(digits.toString());
//        // If no input type was specified, we will default to generic
//        // text, since we can't tell the IME about the set of digits
//        // that was selected.
//        mInputType = inputType != InputType_TYPE_NULL
//                ? inputType : InputType_TYPE_CLASS_TEXT;
    }
    else if (inputType != InputType_TYPE_NULL) {
        SetInputType(inputType, TRUE);
        singleLine = (inputType&(InputType_TYPE_MASK_CLASS
                | InputType_TYPE_TEXT_FLAG_MULTI_LINE)) !=
                (InputType_TYPE_CLASS_TEXT
                | InputType_TYPE_TEXT_FLAG_MULTI_LINE);
    }
    else if (phone) {
        assert(0);
//        mInput = DialerKeyListener.getInstance();
//        mInputType = inputType = InputType_TYPE_CLASS_PHONE;
    }
    else if (numeric != 0) {
        assert(0);
//        mInput = DigitsKeyListener.getInstance((numeric & SIGNED) != 0,
//                                                (numeric & DECIMAL) != 0);
//        inputType = InputType_TYPE_CLASS_NUMBER;
//        if ((numeric & SIGNED) != 0) {
//            inputType |= InputType_TYPE_NUMBER_FLAG_SIGNED;
//        }
//        if ((numeric & DECIMAL) != 0) {
//            inputType |= InputType_TYPE_NUMBER_FLAG_DECIMAL;
//        }
//        mInputType = inputType;
    }
    else if (autotext || autocap != -1) {
        Capitalize cap;

        inputType = InputType_TYPE_CLASS_TEXT;
        if (!singleLine) {
            inputType |= InputType_TYPE_TEXT_FLAG_MULTI_LINE;
        }

        switch (autocap) {
        case 1:
            cap = Capitalize_SENTENCES;
            inputType |= InputType_TYPE_TEXT_FLAG_CAP_SENTENCES;
            break;

        case 2:
            cap = Capitalize_WORDS;
            inputType |= InputType_TYPE_TEXT_FLAG_CAP_WORDS;
            break;

        case 3:
            cap = Capitalize_CHARACTERS;
            inputType |= InputType_TYPE_TEXT_FLAG_CAP_CHARACTERS;
            break;

        default:
            cap = Capitalize_NONE;
            break;
        }

        mInput = TextKeyListener::GetInstance(autotext, cap);
        mInputType = inputType;
    }
    else if (editable) {
        mInput = TextKeyListener::GetInstance();
        mInputType = InputType_TYPE_CLASS_TEXT;
        if (!singleLine) {
            mInputType |= InputType_TYPE_TEXT_FLAG_MULTI_LINE;
        }
    }
    else {
        mInput = NULL;

        switch (buffertype) {
            case 0:
                bufferType = BufferType_NORMAL;
                break;
            case 1:
                bufferType = BufferType_SPANNABLE;
                break;
            case 2:
                bufferType = BufferType_EDITABLE;
                break;
        }
    }

    if (password && (mInputType&InputType_TYPE_MASK_CLASS)
            == InputType_TYPE_CLASS_TEXT) {
        mInputType = (mInputType & ~(InputType_TYPE_MASK_VARIATION))
            | InputType_TYPE_TEXT_VARIATION_PASSWORD;
    }

    if (selectallonfocus) {
        mSelectAllOnFocus = TRUE;

        if (bufferType == BufferType_NORMAL)
            bufferType = BufferType_SPANNABLE;
    }

    SetCompoundDrawablesWithIntrinsicBounds(
            drawableLeft, drawableTop, drawableRight, drawableBottom);
    SetCompoundDrawablePadding(drawablePadding);

    if (singleLine) {
        SetSingleLine();

        if (mInput == NULL && ellipsize < 0) {
            ellipsize = 3; // END
        }
    }

//    switch (ellipsize) {
//        case 1:
//            setEllipsize(TextUtils.TruncateAt.START);
//            break;
//        case 2:
//            setEllipsize(TextUtils.TruncateAt.MIDDLE);
//            break;
//        case 3:
//            setEllipsize(TextUtils.TruncateAt.END);
//            break;
//        case 4:
//            setHorizontalFadingEdgeEnabled(TRUE);
//            setEllipsize(TextUtils.TruncateAt.MARQUEE);
//            break;
//    }

    if (textColor == NULL) {
        CColorStateList::ValueOf(0xFF000000, (IColorStateList**)&textColor);
    }
    SetTextColor(textColor);
    SetHintTextColor(textColorHint);
    SetLinkTextColor(textColorLink);
    if (textColorHighlight != 0) {
        SetHighlightColor(textColorHighlight);
    }
    SetRawTextSize(textSize);

//    if (password) {
//        setTransformationMethod(PasswordTransformationMethod.getInstance());
//        typefaceIndex = MONOSPACE;
//    }
//    else if ((mInputType&(InputType_TYPE_MASK_CLASS
//            |InputType_TYPE_MASK_VARIATION))
//            == (InputType_TYPE_CLASS_TEXT
//                    |InputType_TYPE_TEXT_VARIATION_PASSWORD)) {
//        typefaceIndex = MONOSPACE;
//    }

    SetTypefaceByIndex(typefaceIndex, styleIndex);

    if (shadowcolor != 0) {
        SetShadowLayer(r, dx, dy, shadowcolor);
    }

    if (maxlength >= 0) {
        AutoPtr<IInputFilter> lenFilter;
        CLengthFilter::New(maxlength, (IInputFilter**)&lenFilter);
        AutoPtr<IObjectContainer> container;
        ASSERT_SUCCEEDED(CObjectContainer::New((IObjectContainer**)&container));
        container->Add(lenFilter);
        SetFilters(container);
    }
    else {
        AutoPtr<IObjectContainer> container;
        ASSERT_SUCCEEDED(CObjectContainer::New((IObjectContainer**)&container));
        SetFilters(container);
    }

    SetText(text, bufferType);
    if (hint != NULL) SetHint(hint);

    /*
     * Views are not normally focusable unless specified to be.
     * However, TextViews that have input or movement methods *are*
     * focusable by default.
     */
    ASSERT_SUCCEEDED(context->ObtainStyledAttributesEx3(
            attrs, ArrayOf<Int32>(R_Styleable_View, sizeof(R_Styleable_View) / sizeof(Int32))/*com.android.internal.R.styleable.View*/,
            defStyle, 0, (ITypedArray**)&a));

    Boolean focusable = mMovement != NULL || mInput != NULL;
    Boolean clickable = focusable;
    Boolean longClickable = focusable;

    a->GetIndexCount(&N);
    for (Int32 i = 0; i < N; i++) {
        Int32 attr;
        a->GetIndex(i, &attr);

        switch (attr) {
        case 18/*com.android.internal.R.styleable.View_focusable*/:
            a->GetBoolean(attr, focusable, &focusable);
            break;

        case 29/*com.android.internal.R.styleable.View_clickable*/:
            a->GetBoolean(attr, clickable, &clickable);
            break;

        case 30/*com.android.internal.R.styleable.View_longClickable*/:
            a->GetBoolean(attr, longClickable, &longClickable);
            break;
        }
    }
    a->Recycle();

    SetFocusable(focusable);
    SetClickable(clickable);
    SetLongClickable(longClickable);

    PrepareCursorControllers();

    return NOERROR;
}
