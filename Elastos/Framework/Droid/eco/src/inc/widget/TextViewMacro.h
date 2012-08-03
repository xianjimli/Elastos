
#ifndef __TEXTVIEWMACRO_H__
#define __TEXTVIEWMACRO_H__

#define ITEXTVIEW_METHODS_DECL()                                            \
    CARAPI SetTypeface(                                                     \
        /* [in] */ ITypeface * pTf,                                         \
        /* [in] */ Int32 style);                                            \
                                                                            \
    CARAPI GetText(                                                         \
        /* [out] */ ICharSequence ** ppText);                               \
                                                                            \
    CARAPI GetLength(                                                       \
        /* [out] */ Int32 * pLength);                                       \
                                                                            \
    CARAPI GetLineHeight(                                                   \
        /* [out] */ Int32 * pHeight);                                       \
                                                                            \
    CARAPI GetCompoundPaddingTop(                                           \
        /* [out] */ Int32 * pTop);                                          \
                                                                            \
    CARAPI GetCompoundPaddingBottom(                                        \
        /* [out] */ Int32 * pBottom);                                       \
                                                                            \
    CARAPI GetCompoundPaddingLeft(                                          \
        /* [out] */ Int32 * pLeft);                                         \
                                                                            \
    CARAPI GetCompoundPaddingRight(                                         \
        /* [out] */ Int32 * pRight);                                        \
                                                                            \
    CARAPI GetExtendedPaddingTop(                                           \
        /* [out] */ Int32 * pTop);                                          \
                                                                            \
    CARAPI GetExtendedPaddingBottom(                                        \
        /* [out] */ Int32 * pBottom);                                       \
                                                                            \
    CARAPI GetTotalPaddingLeft(                                             \
        /* [out] */ Int32 * pLeft);                                         \
                                                                            \
    CARAPI GetTotalPaddingRight(                                            \
        /* [out] */ Int32 * pRight);                                        \
                                                                            \
    CARAPI GetTotalPaddingTop(                                              \
        /* [out] */ Int32 * pTop);                                          \
                                                                            \
    CARAPI GetTotalPaddingBottom(                                           \
        /* [out] */ Int32 * pBottom);                                       \
                                                                            \
    CARAPI SetCompoundDrawables(                                            \
        /* [in] */ IDrawable * pLeft,                                       \
        /* [in] */ IDrawable * pTop,                                        \
        /* [in] */ IDrawable * pRight,                                      \
        /* [in] */ IDrawable * pBottom);                                    \
                                                                            \
    CARAPI SetCompoundDrawablesWithIntrinsicBounds(                         \
        /* [in] */ Int32 left,                                              \
        /* [in] */ Int32 top,                                               \
        /* [in] */ Int32 right,                                             \
        /* [in] */ Int32 bottom);                                           \
                                                                            \
    CARAPI SetCompoundDrawablesWithIntrinsicBoundsEx(                       \
        /* [in] */ IDrawable * pLeft,                                       \
        /* [in] */ IDrawable * pTop,                                        \
        /* [in] */ IDrawable * pRight,                                      \
        /* [in] */ IDrawable * pBottom);                                    \
                                                                            \
    CARAPI GetCompoundDrawables(                                            \
        /* [out, callee] */ ArrayOf<IDrawable *> ** ppDrawables);           \
                                                                            \
    CARAPI SetCompoundDrawablePadding(                                      \
        /* [in] */ Int32 pad);                                              \
                                                                            \
    CARAPI GetCompoundDrawablePadding(                                      \
        /* [out] */ Int32 * pPad);                                          \
                                                                            \
    CARAPI GetAutoLinkMask(                                                 \
        /* [out] */ Int32 * pMask);                                         \
                                                                            \
    CARAPI SetTextAppearance(                                               \
        /* [in] */ IContext * pContxt,                                    \
        /* [in] */ Int32 resid);                                            \
                                                                            \
    CARAPI GetTextSize(                                                     \
        /* [out] */ Float * pSize);                                         \
                                                                            \
    CARAPI SetTextSize(                                                     \
        /* [in] */ Float size);                                             \
                                                                            \
    CARAPI SetTextSizeEx(                                                   \
        /* [in] */ Int32 unit,                                              \
        /* [in] */ Float size);                                             \
                                                                            \
    CARAPI GetTextScaleX(                                                   \
        /* [out] */ Float * pSize);                                         \
                                                                            \
    CARAPI SetTextScaleX(                                                   \
        /* [in] */ Float size);                                             \
                                                                            \
    CARAPI SetTypefaceEx(                                                   \
        /* [in] */ ITypeface * pTf);                                        \
                                                                            \
    CARAPI GetTypeface(                                                     \
        /* [out] */ ITypeface ** ppFace);                                   \
                                                                            \
    CARAPI SetTextColor(                                                    \
        /* [in] */ Int32 color);                                            \
                                                                            \
    CARAPI SetTextColorEx(                                                  \
        /* [in] */ IColorStateList * pColors);                              \
                                                                            \
    CARAPI GetTextColors(                                                   \
        /* [out] */ IColorStateList ** ppColors);                           \
                                                                            \
    CARAPI GetCurrentTextColor(                                             \
        /* [out] */ Int32 * pColor);                                        \
                                                                            \
    CARAPI SetHighlightColor(                                               \
        /* [in] */ Int32 color);                                            \
                                                                            \
    CARAPI SetShadowLayer(                                                  \
        /* [in] */ Float radius,                                            \
        /* [in] */ Float dx,                                                \
        /* [in] */ Float dy,                                                \
        /* [in] */ Int32 color);                                            \
                                                                            \
    CARAPI SetAutoLinkMask(                                                 \
        /* [in] */ Int32 mask);                                             \
                                                                            \
    CARAPI SetLinksClickable(                                               \
        /* [in] */ Boolean whether);                                        \
                                                                            \
    CARAPI GetLinksClickable(                                               \
        /* [out] */ Boolean * pWhether);                                    \
                                                                            \
    CARAPI SetHintTextColor(                                                \
        /* [in] */ Int32 color);                                            \
                                                                            \
    CARAPI SetHintTextColorEx(                                              \
        /* [in] */ IColorStateList * pColors);                              \
                                                                            \
    CARAPI GetHintTextColors(                                               \
        /* [out] */ IColorStateList ** ppColors);                           \
                                                                            \
    CARAPI GetCurrentHintTextColor(                                         \
        /* [out] */ Int32 * pColor);                                        \
                                                                            \
    CARAPI SetLinkTextColor(                                                \
        /* [in] */ Int32 color);                                            \
                                                                            \
    CARAPI SetLinkTextColorEx(                                              \
        /* [in] */ IColorStateList * pColors);                              \
                                                                            \
    CARAPI GetLinkTextColors(                                               \
        /* [out] */ IColorStateList ** ppColors);                           \
                                                                            \
    CARAPI SetGravity(                                                      \
        /* [in] */ Int32 gravity);                                          \
                                                                            \
    CARAPI GetGravity(                                                      \
        /* [out] */ Int32 * pGravity);                                      \
                                                                            \
    CARAPI GetPaintFlags(                                                   \
        /* [out] */ Int32 * pFlags);                                        \
                                                                            \
    CARAPI GetPaint(                                                        \
        /* [out] */ ITextPaint** pFlags);                                   \
                                                                            \
    CARAPI SetPaintFlags(                                                   \
        /* [in] */ Int32 flags);                                            \
                                                                            \
    CARAPI SetHorizontallyScrolling(                                        \
        /* [in] */ Boolean whether);                                        \
                                                                            \
    CARAPI SetMinLines(                                                     \
        /* [in] */ Int32 minlines);                                         \
                                                                            \
    CARAPI SetMinHeight(                                                    \
        /* [in] */ Int32 minHeight);                                        \
                                                                            \
    CARAPI SetMaxLines(                                                     \
        /* [in] */ Int32 maxlines);                                         \
                                                                            \
    CARAPI SetMaxHeight(                                                    \
        /* [in] */ Int32 maxHeight);                                        \
                                                                            \
    CARAPI SetLines(                                                        \
        /* [in] */ Int32 lines);                                            \
                                                                            \
    CARAPI SetHeight(                                                       \
        /* [in] */ Int32 pixels);                                           \
                                                                            \
    CARAPI SetMinEms(                                                       \
        /* [in] */ Int32 minems);                                           \
                                                                            \
    CARAPI SetMinWidth(                                                     \
        /* [in] */ Int32 minpixels);                                        \
                                                                            \
    CARAPI SetMaxEms(                                                       \
        /* [in] */ Int32 maxems);                                           \
                                                                            \
    CARAPI SetMaxWidth(                                                     \
        /* [in] */ Int32 maxpixels);                                        \
                                                                            \
    CARAPI SetEms(                                                          \
        /* [in] */ Int32 ems);                                              \
                                                                            \
    CARAPI SetWidth(                                                        \
        /* [in] */ Int32 pixels);                                           \
                                                                            \
    CARAPI SetLineSpacing(                                                  \
        /* [in] */ Float add,                                               \
        /* [in] */ Float mult);                                             \
                                                                            \
    CARAPI Append(                                                          \
        /* [in] */ ICharSequence * pText);                                  \
                                                                            \
    CARAPI AppendEx(                                                        \
        /* [in] */ ICharSequence * pText,                                   \
        /* [in] */ Int32 start,                                             \
        /* [in] */ Int32 end);                                              \
                                                                            \
    CARAPI SetFreezesText(                                                  \
        /* [in] */ Boolean freezesText);                                    \
                                                                            \
    CARAPI GetFreezesText(                                                  \
        /* [out] */ Boolean * pText);                                       \
                                                                            \
    CARAPI SetText(                                                         \
        /* [in] */ ICharSequence * pText);                                  \
                                                                            \
    CARAPI SetTextKeepState(                                                \
        /* [in] */ ICharSequence * pText);                                  \
                                                                            \
    CARAPI SetTextEx(                                                       \
        /* [in] */ ICharSequence * pText,                                   \
        /* [in] */ BufferType type);                                        \
                                                                            \
    CARAPI SetTextEx2(                                                      \
        /* [in] */ const ArrayOf<Char8> & text,                             \
        /* [in] */ Int32 start,                                             \
        /* [in] */ Int32 len);                                              \
                                                                            \
    CARAPI SetTextKeepStateEx(                                              \
        /* [in] */ ICharSequence * pText,                                   \
        /* [in] */ BufferType type);                                        \
                                                                            \
    CARAPI SetTextEx3(                                                      \
        /* [in] */ Int32 resid);                                            \
                                                                            \
    CARAPI SetTextEx4(                                                      \
        /* [in] */ Int32 resid,                                             \
        /* [in] */ BufferType type);                                        \
                                                                            \
    CARAPI SetHint(                                                         \
        /* [in] */ ICharSequence * pHint);                                  \
                                                                            \
    CARAPI SetHintEx(                                                       \
        /* [in] */ Int32 resid);                                            \
                                                                            \
    CARAPI GetHint(                                                         \
        /* [out] */ ICharSequence ** ppHint);                               \
                                                                            \
    CARAPI SetInputType(                                                    \
        /* [in] */ Int32 type);                                             \
                                                                            \
    CARAPI SetRawInputType(                                                 \
        /* [in] */ Int32 type);                                             \
                                                                            \
    CARAPI GetInputType(                                                    \
        /* [out] */ Int32 * pType);                                         \
                                                                            \
    CARAPI SetImeOptions(                                                   \
        /* [in] */ Int32 imeOptions);                                       \
                                                                            \
    CARAPI GetImeOptions(                                                   \
        /* [out] */ Int32 * pOptions);                                      \
                                                                            \
    CARAPI SetImeActionLabel(                                               \
        /* [in] */ ICharSequence * pLabel,                                  \
        /* [in] */ Int32 actionId);                                         \
                                                                            \
    CARAPI GetImeActionLabel(                                               \
        /* [out] */ ICharSequence ** ppLabel);                              \
                                                                            \
    CARAPI GetImeActionId(                                                  \
        /* [out] */ Int32 * pId);                                           \
                                                                            \
    CARAPI OnEditorAction(                                                  \
        /* [in] */ Int32 actionCode);                                       \
                                                                            \
    CARAPI SetPrivateImeOptions(                                            \
        /* [in] */ const String& type);                                            \
                                                                            \
    CARAPI GetPrivateImeOptions(                                            \
        /* [out] */ String * pOptions);                                     \
                                                                            \
    CARAPI SetInputExtras(                                                  \
        /* [in] */ Int32 xmlResId);                                         \
                                                                            \
    CARAPI GetInputExtras(                                                  \
        /* [in] */ Boolean create,                                          \
        /* [out] */ IBundle ** ppBundle);                                   \
                                                                            \
    CARAPI GetError(                                                        \
        /* [out] */ ICharSequence ** ppError);                              \
                                                                            \
    CARAPI SetError(                                                        \
        /* [in] */ ICharSequence * pError);                                 \
                                                                            \
    CARAPI SetErrorEx(                                                      \
        /* [in] */ ICharSequence * pError,                                  \
        /* [in] */ IDrawable * pIcon);                                      \
                                                                            \
    CARAPI GetLineCount(                                                    \
        /* [out] */ Int32 * pCount);                                        \
                                                                            \
    CARAPI GetLineBounds(                                                   \
        /* [in] */ Int32 line,                                              \
        /* [in] */ IRect * pBounds,                                         \
        /* [out] */ Int32 * pY);                                            \
                                                                            \
    CARAPI ExtractText(                                                     \
        /* [in] */ IExtractedTextRequest * pRequest,                        \
        /* [in] */ IExtractedText * pOutText,                               \
        /* [out] */ Boolean * pResult);                                     \
                                                                            \
    CARAPI SetExtractedText(                                                \
        /* [in] */ IExtractedText * pText);                                 \
                                                                            \
    CARAPI SetExtracting(                                                   \
        /* [in] */ IExtractedTextRequest * pReq);                           \
                                                                            \
    CARAPI OnCommitCompletion(                                              \
        /* [in] */ ICompletionInfo * pText);                                \
                                                                            \
    CARAPI BeginBatchEdit();                                                \
                                                                            \
    CARAPI EndBatchEdit();                                                  \
                                                                            \
    CARAPI OnBeginBatchEdit();                                              \
                                                                            \
    CARAPI OnEndBatchEdit();                                                \
                                                                            \
    CARAPI OnPrivateIMECommand(                                             \
        /* [in] */ const String& action,                                           \
        /* [in] */ IBundle * pData,                                         \
        /* [out] */ Boolean * pResult);                                     \
                                                                            \
    CARAPI SetIncludeFontPadding(                                           \
        /* [in] */ Boolean includepad);                                     \
                                                                            \
    CARAPI BringPointIntoView(                                              \
        /* [in] */ Int32 offset,                                            \
        /* [out] */ Boolean * pResult);                                     \
                                                                            \
    CARAPI MoveCursorToVisibleOffset(                                       \
        /* [out] */ Boolean * pResult);                                     \
                                                                            \
    CARAPI GetSelectionStart(                                               \
        /* [out] */ Int32 * pStart);                                        \
                                                                            \
    CARAPI GetSelectionEnd(                                                 \
        /* [out] */ Int32 * pEnd);                                          \
                                                                            \
    CARAPI HasSelection(                                                    \
        /* [out] */ Boolean * pResult);                                     \
                                                                            \
    CARAPI SetSingleLine();                                                 \
                                                                            \
    CARAPI SetSingleLineEx(                                                 \
        /* [in] */ Boolean singleLine);                                     \
                                                                            \
    CARAPI SetMarqueeRepeatLimit(                                           \
        /* [in] */ Int32 marqueeLimit);                                     \
                                                                            \
    CARAPI SetSelectAllOnFocus(                                             \
        /* [in] */ Boolean selectAllOnFocus);                               \
                                                                            \
    CARAPI SetCursorVisible(                                                \
        /* [in] */ Boolean visible);                                        \
                                                                            \
    CARAPI ClearComposingText();                                            \
                                                                            \
    CARAPI DidTouchFocusSelect(                                             \
        /* [out] */ Boolean * pResult);                                     \
                                                                            \
    CARAPI IsInputMethodTarget(                                             \
        /* [out] */ Boolean * pResult);                                     \
                                                                            \
    CARAPI OnTextContextMenuItem(                                           \
        /* [in] */ Int32 id,                                                \
        /* [out] */ Boolean * pResult);                                     \
                                                                            \
    CARAPI GetOffset(                                                       \
        /* [in] */ Int32 x,                                                 \
        /* [in] */ Int32 y,                                                 \
        /* [out] */ Int32 * pOffset);                                       \

#define ITEXTVIEW_METHODS_IMPL(className, superClass, overRideClass)        \
ECode className::SetTypeface(                                                                   \
    /* [in] */ ITypeface * pTf,                                                                 \
    /* [in] */ Int32 style)                                                                     \
{                                                                                               \
    return superClass::SetTypeface(pTf, style);                                                 \
}                                                                                               \
                                                                                                \
ECode className::GetText(                                                                       \
    /* [out] */ ICharSequence ** ppText)                                                        \
{                                                                                               \
    VALIDATE_NOT_NULL(ppText);                                                                  \
    *ppText = superClass::GetText();                                                            \
    if (*ppText) {                                                                              \
        (*ppText)->AddRef();                                                                    \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetLength(                                                                     \
    /* [out] */ Int32 * pLength)                                                                \
{                                                                                               \
    VALIDATE_NOT_NULL(pLength);                                                                 \
    *pLength = superClass::GetLength();                                                         \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetLineHeight(                                                                 \
    /* [out] */ Int32 * pHeight)                                                                \
{                                                                                               \
    VALIDATE_NOT_NULL(pHeight);                                                                 \
    *pHeight = superClass::GetLineHeight();                                                     \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetCompoundPaddingTop(                                                         \
    /* [out] */ Int32 * pTop)                                                                   \
{                                                                                               \
    VALIDATE_NOT_NULL(pTop);                                                                    \
    *pTop = superClass::GetCompoundPaddingTop();                                                \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetCompoundPaddingBottom(                                                      \
    /* [out] */ Int32 * pBottom)                                                                \
{                                                                                               \
    VALIDATE_NOT_NULL(pBottom);                                                                 \
    *pBottom = superClass::GetCompoundPaddingBottom();                                          \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetCompoundPaddingLeft(                                                        \
    /* [out] */ Int32 * pLeft)                                                                  \
{                                                                                               \
    VALIDATE_NOT_NULL(pLeft);                                                                   \
    *pLeft = superClass::GetCompoundPaddingLeft();                                              \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetCompoundPaddingRight(                                                       \
    /* [out] */ Int32 * pRight)                                                                 \
{                                                                                               \
    VALIDATE_NOT_NULL(pRight);                                                                  \
    *pRight = superClass::GetCompoundPaddingRight();                                            \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetExtendedPaddingTop(                                                         \
    /* [out] */ Int32 * pTop)                                                                   \
{                                                                                               \
    VALIDATE_NOT_NULL(pTop);                                                                    \
    *pTop = superClass::GetExtendedPaddingTop();                                                \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetExtendedPaddingBottom(                                                      \
    /* [out] */ Int32 * pBottom)                                                                \
{                                                                                               \
    VALIDATE_NOT_NULL(pBottom);                                                                 \
    *pBottom = superClass::GetExtendedPaddingBottom();                                          \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetTotalPaddingLeft(                                                           \
    /* [out] */ Int32 * pLeft)                                                                  \
{                                                                                               \
    VALIDATE_NOT_NULL(pLeft);                                                                   \
    *pLeft = superClass::GetTotalPaddingLeft();                                                 \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetTotalPaddingRight(                                                          \
    /* [out] */ Int32 * pRight)                                                                 \
{                                                                                               \
    VALIDATE_NOT_NULL(pRight);                                                                  \
    *pRight = superClass::GetTotalPaddingRight();                                               \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetTotalPaddingTop(                                                            \
    /* [out] */ Int32 * pTop)                                                                   \
{                                                                                               \
    VALIDATE_NOT_NULL(pTop);                                                                    \
    *pTop = superClass::GetTotalPaddingTop();                                                   \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetTotalPaddingBottom(                                                         \
    /* [out] */ Int32 * pBottom)                                                                \
{                                                                                               \
    VALIDATE_NOT_NULL(pBottom);                                                                 \
    *pBottom = superClass::GetTotalPaddingBottom();                                             \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetCompoundDrawables(                                                          \
    /* [in] */ IDrawable * pLeft,                                                               \
    /* [in] */ IDrawable * pTop,                                                                \
    /* [in] */ IDrawable * pRight,                                                              \
    /* [in] */ IDrawable * pBottom)                                                             \
{                                                                                               \
    return superClass::SetCompoundDrawables(pLeft, pTop, pRight, pBottom);                      \
}                                                                                               \
                                                                                                \
ECode className::SetCompoundDrawablesWithIntrinsicBounds(                                       \
    /* [in] */ Int32 left,                                                                      \
    /* [in] */ Int32 top,                                                                       \
    /* [in] */ Int32 right,                                                                     \
    /* [in] */ Int32 bottom)                                                                    \
{                                                                                               \
    return superClass::SetCompoundDrawablesWithIntrinsicBounds(left, top, right, bottom);       \
}                                                                                               \
                                                                                                \
ECode className::SetCompoundDrawablesWithIntrinsicBoundsEx(                                     \
    /* [in] */ IDrawable * pLeft,                                                               \
    /* [in] */ IDrawable * pTop,                                                                \
    /* [in] */ IDrawable * pRight,                                                              \
    /* [in] */ IDrawable * pBottom)                                                             \
{                                                                                               \
    return superClass::SetCompoundDrawablesWithIntrinsicBounds(pLeft, pTop, pRight, pBottom); \
}                                                                                               \
                                                                                                \
ECode className::GetCompoundDrawables(                                                          \
    /* [out, callee] */ ArrayOf<IDrawable *> ** ppDrawables)                                    \
{                                                                                               \
    return superClass::GetCompoundDrawables(ppDrawables);                                       \
}                                                                                               \
                                                                                                \
ECode className::SetCompoundDrawablePadding(                                                    \
    /* [in] */ Int32 pad)                                                                       \
{                                                                                               \
    return superClass::SetCompoundDrawablePadding(pad);                                         \
}                                                                                               \
                                                                                                \
ECode className::GetCompoundDrawablePadding(                                                    \
    /* [out] */ Int32 * pPad)                                                                   \
{                                                                                               \
    VALIDATE_NOT_NULL(pPad);                                                                    \
    *pPad = superClass::GetCompoundDrawablePadding();                                           \
                                                                                                \
    return  NOERROR;                                                                            \
}                                                                                               \
                                                                                                \
ECode className::GetAutoLinkMask(                                                               \
    /* [out] */ Int32 * pMask)                                                                  \
{                                                                                               \
    VALIDATE_NOT_NULL(pMask);                                                                   \
    *pMask = superClass::GetAutoLinkMask();                                                     \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetTextAppearance(                                                             \
    /* [in] */ IContext * pContxt,                                                              \
    /* [in] */ Int32 resid)                                                                     \
{                                                                                               \
    return superClass::SetTextAppearance(pContxt, resid);                                       \
}                                                                                               \
                                                                                                \
ECode className::GetTextSize(                                                                   \
    /* [out] */ Float * pSize)                                                                  \
{                                                                                               \
    VALIDATE_NOT_NULL(pSize);                                                                   \
    *pSize = superClass::GetTextSize();                                                         \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetTextSize(                                                                   \
    /* [in] */ Float size)                                                                      \
{                                                                                               \
    return superClass::SetTextSize(size);                                                       \
}                                                                                               \
                                                                                                \
ECode className::SetTextSizeEx(                                                                 \
    /* [in] */ Int32 unit,                                                                      \
    /* [in] */ Float size)                                                                      \
{                                                                                               \
    return superClass::SetTextSize(unit, size);                                                 \
}                                                                                               \
                                                                                                \
ECode className::GetTextScaleX(                                                                 \
    /* [out] */ Float * pSize)                                                                  \
{                                                                                               \
    VALIDATE_NOT_NULL(pSize);                                                                   \
    *pSize = superClass::GetTextScaleX();                                                       \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetTextScaleX(                                                                 \
    /* [in] */ Float size)                                                                      \
{                                                                                               \
    return superClass::SetTextScaleX(size);                                                     \
}                                                                                               \
                                                                                                \
ECode className::SetTypefaceEx(                                                                 \
    /* [in] */ ITypeface * pTf)                                                                 \
{                                                                                               \
    return superClass::SetTypeface(pTf);                                                        \
}                                                                                               \
                                                                                                \
ECode className::GetTypeface(                                                                   \
    /* [out] */ ITypeface ** ppFace)                                                            \
{                                                                                               \
    VALIDATE_NOT_NULL(ppFace);                                                                  \
                                                                                                \
    return superClass::GetTypeface(ppFace);                                                     \
}                                                                                               \
                                                                                                \
ECode className::SetTextColor(                                                                  \
    /* [in] */ Int32 color)                                                                     \
{                                                                                               \
    return superClass::SetTextColor(color);                                                     \
}                                                                                               \
                                                                                                \
ECode className::SetTextColorEx(                                                                \
    /* [in] */ IColorStateList * pColors)                                                       \
{                                                                                               \
    return superClass::SetTextColor(pColors);                                                   \
}                                                                                               \
                                                                                                \
ECode className::GetTextColors(                                                                 \
    /* [out] */ IColorStateList ** ppColors)                                                    \
{                                                                                               \
    VALIDATE_NOT_NULL(ppColors);                                                                \
                                                                                                \
    return superClass::GetTextColors(ppColors);                                                 \
                                                                                                \
}                                                                                               \
                                                                                                \
ECode className::GetCurrentTextColor(                                                           \
    /* [out] */ Int32 * pColor)                                                                 \
{                                                                                               \
    VALIDATE_NOT_NULL(pColor);                                                                  \
    *pColor = superClass::GetCurrentTextColor();                                                \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetHighlightColor(                                                             \
    /* [in] */ Int32 color)                                                                     \
{                                                                                               \
    return superClass::SetHighlightColor(color);                                                \
}                                                                                               \
                                                                                                \
ECode className::SetShadowLayer(                                                                \
    /* [in] */ Float radius,                                                                    \
    /* [in] */ Float dx,                                                                        \
    /* [in] */ Float dy,                                                                        \
    /* [in] */ Int32 color)                                                                     \
{                                                                                               \
    return superClass::SetShadowLayer(radius, dx, dy, color);                                   \
}                                                                                               \
                                                                                                \
ECode className::SetAutoLinkMask(                                                               \
    /* [in] */ Int32 mask)                                                                      \
{                                                                                               \
    return superClass::SetAutoLinkMask(mask);                                                   \
}                                                                                               \
                                                                                                \
ECode className::SetLinksClickable(                                                             \
    /* [in] */ Boolean whether)                                                                 \
{                                                                                               \
    return superClass::SetLinksClickable(whether);                                              \
}                                                                                               \
                                                                                                \
ECode className::GetLinksClickable(                                                             \
    /* [out] */ Boolean * pWhether)                                                             \
{                                                                                               \
    VALIDATE_NOT_NULL(pWhether);                                                                \
    *pWhether = superClass::GetLinksClickable();                                                \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetHintTextColor(                                                              \
    /* [in] */ Int32 color)                                                                     \
{                                                                                               \
    return superClass::SetHintTextColor(color);                                                 \
}                                                                                               \
                                                                                                \
ECode className::SetHintTextColorEx(                                                            \
    /* [in] */ IColorStateList * pColors)                                                       \
{                                                                                               \
    return superClass::SetHintTextColor(pColors);                                               \
}                                                                                               \
                                                                                                \
ECode className::GetHintTextColors(                                                             \
    /* [out] */ IColorStateList ** ppColors)                                                    \
{                                                                                               \
    VALIDATE_NOT_NULL(ppColors);                                                                \
    return superClass::GetHintTextColors(ppColors);                                             \
}                                                                                               \
                                                                                                \
ECode className::GetCurrentHintTextColor(                                                       \
    /* [out] */ Int32 * pColor)                                                                 \
{                                                                                               \
    VALIDATE_NOT_NULL(pColor);                                                                  \
    *pColor = superClass::GetCurrentHintTextColor();                                            \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetLinkTextColor(                                                              \
    /* [in] */ Int32 color)                                                                     \
{                                                                                               \
    return superClass::SetLinkTextColor(color);                                                 \
}                                                                                               \
                                                                                                \
ECode className::SetLinkTextColorEx(                                                            \
    /* [in] */ IColorStateList * pColors)                                                       \
{                                                                                               \
    return superClass::SetLinkTextColor(pColors);                                               \
}                                                                                               \
                                                                                                \
ECode className::GetLinkTextColors(                                                             \
    /* [out] */ IColorStateList ** ppColors)                                                    \
{                                                                                               \
    VALIDATE_NOT_NULL(ppColors);                                                                \
                                                                                                \
    return superClass::GetLinkTextColors(ppColors);                                             \
}                                                                                               \
                                                                                                \
ECode className::SetGravity(                                                                    \
    /* [in] */ Int32 gravity)                                                                   \
{                                                                                               \
    return superClass::SetGravity(gravity);                                                     \
}                                                                                               \
                                                                                                \
ECode className::GetGravity(                                                                    \
    /* [out] */ Int32 * pGravity)                                                               \
{                                                                                               \
    VALIDATE_NOT_NULL(pGravity);                                                                \
    *pGravity = superClass::GetGravity();                                                       \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetPaintFlags(                                                                 \
    /* [out] */ Int32 * pFlags)                                                                 \
{                                                                                               \
    VALIDATE_NOT_NULL(pFlags);                                                                  \
    *pFlags = superClass::GetPaintFlags();                                                      \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
																								\
ECode className::GetPaint( 																        \
	/* [out] */ ITextPaint** textPaint) 															\
{																								\
	VALIDATE_NOT_NULL(textPaint);																\
	*textPaint = superClass::GetPaint();														\
	if (*textPaint) {																			\
		(*textPaint)->AddRef();																	\
	}																							\
																								\
	return NOERROR; 																			\
}																								\
																								\
ECode className::SetPaintFlags(                                                                 \
    /* [in] */ Int32 flags)                                                                     \
{                                                                                               \
    return superClass::SetPaintFlags(flags);                                                    \
}                                                                                               \
                                                                                                \
ECode className::SetHorizontallyScrolling(                                                      \
    /* [in] */ Boolean whether)                                                                 \
{                                                                                               \
    return superClass::SetHorizontallyScrolling(whether);                                       \
}                                                                                               \
                                                                                                \
ECode className::SetMinLines(                                                                   \
    /* [in] */ Int32 minlines)                                                                  \
{                                                                                               \
    return superClass::SetMinLines(minlines);                                                   \
}                                                                                               \
                                                                                                \
ECode className::SetMinHeight(                                                                  \
    /* [in] */ Int32 minHeight)                                                                 \
{                                                                                               \
    return superClass::SetMinHeight(minHeight);                                                 \
}                                                                                               \
                                                                                                \
ECode className::SetMaxLines(                                                                   \
    /* [in] */ Int32 maxlines)                                                                  \
{                                                                                               \
    return superClass::SetMaxLines(maxlines);                                                   \
}                                                                                               \
                                                                                                \
ECode className::SetMaxHeight(                                                                  \
    /* [in] */ Int32 maxHeight)                                                                 \
{                                                                                               \
    return superClass::SetMaxHeight(maxHeight);                                                 \
}                                                                                               \
                                                                                                \
ECode className::SetLines(                                                                      \
    /* [in] */ Int32 lines)                                                                     \
{                                                                                               \
    return superClass::SetLines(lines);                                                         \
}                                                                                               \
                                                                                                \
ECode className::SetHeight(                                                                     \
    /* [in] */ Int32 pixels)                                                                    \
{                                                                                               \
    return superClass::SetHeight(pixels);                                                       \
}                                                                                               \
                                                                                                \
ECode className::SetMinEms(                                                                     \
    /* [in] */ Int32 minems)                                                                    \
{                                                                                               \
    return superClass::SetMinEms(minems);                                                       \
}                                                                                               \
                                                                                                \
ECode className::SetMinWidth(                                                                   \
    /* [in] */ Int32 minpixels)                                                                 \
{                                                                                               \
    return superClass::SetMinWidth(minpixels);                                                  \
}                                                                                               \
                                                                                                \
ECode className::SetMaxEms(                                                                     \
    /* [in] */ Int32 maxems)                                                                    \
{                                                                                               \
    return superClass::SetMinWidth(maxems);                                                     \
}                                                                                               \
                                                                                                \
ECode className::SetMaxWidth(                                                                   \
    /* [in] */ Int32 maxpixels)                                                                 \
{                                                                                               \
    return superClass::SetMaxWidth(maxpixels);                                                  \
}                                                                                               \
                                                                                                \
ECode className::SetEms(                                                                        \
    /* [in] */ Int32 ems)                                                                       \
{                                                                                               \
    return superClass::SetEms(ems);                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetWidth(                                                                      \
    /* [in] */ Int32 pixels)                                                                    \
{                                                                                               \
    return superClass::SetWidth(pixels);                                                        \
}                                                                                               \
                                                                                                \
ECode className::SetLineSpacing(                                                                \
    /* [in] */ Float add,                                                                       \
    /* [in] */ Float mult)                                                                      \
{                                                                                               \
    return superClass::SetLineSpacing(add, mult);                                               \
}                                                                                               \
                                                                                                \
ECode className::Append(                                                                        \
    /* [in] */ ICharSequence * pText)                                                           \
{                                                                                               \
    return superClass::Append(pText);                                                           \
}                                                                                               \
                                                                                                \
ECode className::AppendEx(                                                                      \
    /* [in] */ ICharSequence * pText,                                                           \
    /* [in] */ Int32 start,                                                                     \
    /* [in] */ Int32 end)                                                                       \
{                                                                                               \
    return superClass::Append(pText, start, end);                                               \
}                                                                                               \
                                                                                                \
ECode className::SetFreezesText(                                                                \
    /* [in] */ Boolean freezesText)                                                             \
{                                                                                               \
    return superClass::SetFreezesText(freezesText);                                             \
}                                                                                               \
                                                                                                \
ECode className::GetFreezesText(                                                                \
    /* [out] */ Boolean * pText)                                                                \
{                                                                                               \
    VALIDATE_NOT_NULL(pText);                                                                   \
    *pText = superClass::GetFreezesText();                                                      \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetText(                                                                       \
    /* [in] */ ICharSequence * pText)                                                           \
{                                                                                               \
    return superClass::SetText(pText);                                                          \
}                                                                                               \
                                                                                                \
ECode className::SetTextKeepState(                                                              \
    /* [in] */ ICharSequence * pText)                                                           \
{                                                                                               \
    return superClass::SetTextKeepState(pText);                                                 \
}                                                                                               \
                                                                                                \
ECode className::SetTextEx(                                                                     \
    /* [in] */ ICharSequence * pText,                                                           \
    /* [in] */ BufferType type)                                                                 \
{                                                                                               \
    return superClass::SetText(pText, type);                                                    \
}                                                                                               \
                                                                                                \
ECode className::SetTextEx2(                                                                    \
    /* [in] */ const ArrayOf<Char8> & text,                                                     \
    /* [in] */ Int32 start,                                                                     \
    /* [in] */ Int32 len)                                                                       \
{                                                                                               \
    return superClass::SetText(text, start, len);                                               \
}                                                                                               \
                                                                                                \
ECode className::SetTextKeepStateEx(                                                            \
    /* [in] */ ICharSequence * pText,                                                           \
    /* [in] */ BufferType type)                                                                 \
{                                                                                               \
    return superClass::SetTextKeepState(pText, type);                                           \
}                                                                                               \
                                                                                                \
ECode className::SetTextEx3(                                                                    \
    /* [in] */ Int32 resid)                                                                     \
{                                                                                               \
    return superClass::SetText(resid);                                                          \
}                                                                                               \
                                                                                                \
ECode className::SetTextEx4(                                                                    \
    /* [in] */ Int32 resid,                                                                     \
    /* [in] */ BufferType type)                                                                 \
{                                                                                               \
    return superClass::SetText(resid, type);                                                    \
}                                                                                               \
                                                                                                \
ECode className::SetHint(                                                                       \
    /* [in] */ ICharSequence * pHint)                                                           \
{                                                                                               \
    return superClass::SetHint(pHint);                                                          \
}                                                                                               \
                                                                                                \
ECode className::SetHintEx(                                                                     \
    /* [in] */ Int32 resid)                                                                     \
{                                                                                               \
    return superClass::SetHint(resid);                                                          \
}                                                                                               \
                                                                                                \
ECode className::GetHint(                                                                       \
    /* [out] */ ICharSequence ** ppHint)                                                        \
{                                                                                               \
    VALIDATE_NOT_NULL(ppHint);                                                                  \
    *ppHint = superClass::GetHint();                                                            \
    if (*ppHint) {                                                                              \
        (*ppHint)->AddRef();                                                                    \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetInputType(                                                                  \
    /* [in] */ Int32 type)                                                                      \
{                                                                                               \
    return superClass::SetInputType(type);                                                      \
}                                                                                               \
                                                                                                \
ECode className::SetRawInputType(                                                               \
    /* [in] */ Int32 type)                                                                      \
{                                                                                               \
    return superClass::SetRawInputType(type);                                                   \
}                                                                                               \
                                                                                                \
ECode className::GetInputType(                                                                  \
    /* [out] */ Int32 * pType)                                                                  \
{                                                                                               \
    VALIDATE_NOT_NULL(pType);                                                                   \
    *pType = superClass::GetInputType();                                                        \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetImeOptions(                                                                 \
    /* [in] */ Int32 imeOptions)                                                                \
{                                                                                               \
    return superClass::SetImeOptions(imeOptions);                                               \
}                                                                                               \
                                                                                                \
ECode className::GetImeOptions(                                                                 \
    /* [out] */ Int32 * pOptions)                                                               \
{                                                                                               \
    VALIDATE_NOT_NULL(pOptions);                                                                \
    *pOptions = superClass::GetImeOptions();                                                    \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetImeActionLabel(                                                             \
    /* [in] */ ICharSequence * pLabel,                                                          \
    /* [in] */ Int32 actionId)                                                                  \
{                                                                                               \
    return superClass::SetImeActionLabel(pLabel, actionId);                                     \
}                                                                                               \
                                                                                                \
ECode className::GetImeActionLabel(                                                             \
    /* [out] */ ICharSequence ** ppLabel)                                                       \
{                                                                                               \
    VALIDATE_NOT_NULL(ppLabel);                                                                 \
    *ppLabel = superClass::GetImeActionLabel();                                                 \
    if (*ppLabel) {                                                                             \
        (*ppLabel)->AddRef();                                                                   \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetImeActionId(                                                                \
    /* [out] */ Int32 * pId)                                                                    \
{                                                                                               \
    VALIDATE_NOT_NULL(pId);                                                                     \
    *pId = superClass::GetImeActionId();                                                        \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::OnEditorAction(                                                                \
    /* [in] */ Int32 actionCode)                                                                \
{                                                                                               \
    return superClass::OnEditorAction(actionCode);                                              \
}                                                                                               \
                                                                                                \
ECode className::SetPrivateImeOptions(                                                          \
    /* [in] */ const String& type)                                                                     \
{                                                                                               \
    return superClass::SetPrivateImeOptions(type);                                              \
}                                                                                               \
                                                                                                \
ECode className::GetPrivateImeOptions(                                                          \
    /* [out] */ String * pOptions)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pOptions);                                                                \
    *pOptions = superClass::GetPrivateImeOptions();                                             \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetInputExtras(                                                                \
    /* [in] */ Int32 xmlResId)                                                                  \
{                                                                                               \
    return superClass::SetInputExtras(xmlResId);                                                \
}                                                                                               \
                                                                                                \
ECode className::GetInputExtras(                                                                \
    /* [in] */ Boolean create,                                                                  \
    /* [out] */ IBundle ** ppBundle)                                                            \
{                                                                                               \
    VALIDATE_NOT_NULL(ppBundle);                                                                \
    *ppBundle = superClass::GetInputExtras(create);                                             \
    if (*ppBundle) {                                                                            \
        (*ppBundle)->AddRef();                                                                  \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetError(                                                                      \
    /* [out] */ ICharSequence ** ppError)                                                       \
{                                                                                               \
    VALIDATE_NOT_NULL(ppError);                                                                 \
    *ppError = superClass::GetError();                                                          \
    if (*ppError) {                                                                             \
        (*ppError)->AddRef();                                                                   \
    }                                                                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetError(                                                                      \
    /* [in] */ ICharSequence * pError)                                                          \
{                                                                                               \
    return superClass::SetError(pError);                                                        \
}                                                                                               \
                                                                                                \
ECode className::SetErrorEx(                                                                    \
    /* [in] */ ICharSequence * pError,                                                          \
    /* [in] */ IDrawable * pIcon)                                                               \
{                                                                                               \
    return superClass::SetError(pError, pIcon);                                                 \
}                                                                                               \
                                                                                                \
ECode className::GetLineCount(                                                                  \
    /* [out] */ Int32 * pCount)                                                                 \
{                                                                                               \
    VALIDATE_NOT_NULL(pCount);                                                                  \
    *pCount = superClass::GetLineCount();                                                       \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetLineBounds(                                                                 \
    /* [in] */ Int32 line,                                                                      \
    /* [in] */ IRect * pBounds,                                                                 \
    /* [out] */ Int32 * pY)                                                                     \
{                                                                                               \
    VALIDATE_NOT_NULL(pY);                                                                      \
    *pY = superClass::GetLineBounds(line, pBounds);                                             \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::ExtractText(                                                                   \
    /* [in] */ IExtractedTextRequest * pRequest,                                                \
    /* [in] */ IExtractedText * pOutText,                                                       \
    /* [out] */ Boolean * pResult)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pResult);                                                                 \
    *pResult = superClass::ExtractText(pRequest, pOutText);                                     \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetExtractedText(                                                              \
    /* [in] */ IExtractedText * pText)                                                          \
{                                                                                               \
    return superClass::SetExtractedText(pText);                                                 \
}                                                                                               \
                                                                                                \
ECode className::SetExtracting(                                                                 \
    /* [in] */ IExtractedTextRequest * pReq)                                                    \
{                                                                                               \
    return superClass::SetExtracting(pReq);                                                     \
}                                                                                               \
                                                                                                \
ECode className::OnCommitCompletion(                                                            \
    /* [in] */ ICompletionInfo * pText)                                                         \
{                                                                                               \
    return superClass::OnCommitCompletion(pText);                                               \
}                                                                                               \
                                                                                                \
ECode className::BeginBatchEdit()                                                               \
{                                                                                               \
    return superClass::BeginBatchEdit();                                                        \
}                                                                                               \
                                                                                                \
ECode className::EndBatchEdit()                                                                 \
{                                                                                               \
    return superClass::EndBatchEdit();                                                          \
}                                                                                               \
                                                                                                \
ECode className::OnBeginBatchEdit()                                                             \
{                                                                                               \
    return superClass::OnBeginBatchEdit();                                                      \
}                                                                                               \
                                                                                                \
ECode className::OnEndBatchEdit()                                                               \
{                                                                                               \
    return superClass::OnEndBatchEdit();                                                        \
}                                                                                               \
                                                                                                \
ECode className::OnPrivateIMECommand(                                                           \
    /* [in] */ const String& action,                                                                   \
    /* [in] */ IBundle * pData,                                                                 \
    /* [out] */ Boolean * pResult)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pResult);                                                                 \
    *pResult = superClass::OnPrivateIMECommand(action, pData);                                  \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetIncludeFontPadding(                                                         \
    /* [in] */ Boolean includepad)                                                              \
{                                                                                               \
    return superClass::SetIncludeFontPadding(includepad);                                       \
}                                                                                               \
                                                                                                \
ECode className::BringPointIntoView(                                                            \
    /* [in] */ Int32 offset,                                                                    \
    /* [out] */ Boolean * pResult)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pResult);                                                                 \
    *pResult = superClass::BringPointIntoView(offset);                                          \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::MoveCursorToVisibleOffset(                                                     \
    /* [out] */ Boolean * pResult)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pResult);                                                                 \
    *pResult = superClass::MoveCursorToVisibleOffset();                                         \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetSelectionStart(                                                             \
    /* [out] */ Int32 * pStart)                                                                 \
{                                                                                               \
    VALIDATE_NOT_NULL(pStart);                                                                  \
    *pStart = superClass::GetSelectionStart();                                                  \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetSelectionEnd(                                                               \
    /* [out] */ Int32 * pEnd)                                                                   \
{                                                                                               \
    VALIDATE_NOT_NULL(pEnd);                                                                    \
    *pEnd = superClass::GetSelectionEnd();                                                      \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::HasSelection(                                                                  \
    /* [out] */ Boolean * pResult)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pResult);                                                                 \
    *pResult = superClass::HasSelection();                                                      \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::SetSingleLine()                                                                \
{                                                                                               \
    return superClass::SetSingleLine();                                                         \
}                                                                                               \
                                                                                                \
ECode className::SetSingleLineEx(                                                               \
    /* [in] */ Boolean singleLine)                                                              \
{                                                                                               \
    return superClass::SetSingleLine(singleLine);                                               \
}                                                                                               \
                                                                                                \
ECode className::SetMarqueeRepeatLimit(                                                         \
    /* [in] */ Int32 marqueeLimit)                                                              \
{                                                                                               \
    return superClass::SetMarqueeRepeatLimit(marqueeLimit);                                     \
}                                                                                               \
                                                                                                \
ECode className::SetSelectAllOnFocus(                                                           \
    /* [in] */ Boolean selectAllOnFocus)                                                        \
{                                                                                               \
    return superClass::SetSelectAllOnFocus(selectAllOnFocus);                                   \
}                                                                                               \
                                                                                                \
ECode className::SetCursorVisible(                                                              \
    /* [in] */ Boolean visible)                                                                 \
{                                                                                               \
    return superClass::SetCursorVisible(visible);                                               \
}                                                                                               \
                                                                                                \
ECode className::ClearComposingText()                                                           \
{                                                                                               \
    return superClass::ClearComposingText();                                                    \
}                                                                                               \
                                                                                                \
ECode className::DidTouchFocusSelect(                                                           \
    /* [out] */ Boolean * pResult)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pResult);                                                                 \
    *pResult = superClass::DidTouchFocusSelect();                                               \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::IsInputMethodTarget(                                                           \
    /* [out] */ Boolean * pResult)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pResult);                                                                 \
    *pResult = superClass::IsInputMethodTarget();                                               \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::OnTextContextMenuItem(                                                         \
    /* [in] */ Int32 id,                                                                        \
    /* [out] */ Boolean * pResult)                                                              \
{                                                                                               \
    VALIDATE_NOT_NULL(pResult);                                                                 \
    *pResult = superClass::OnTextContextMenuItem(id);                                           \
                                                                                                \
    return NOERROR;                                                                             \
}                                                                                               \
                                                                                                \
ECode className::GetOffset(                                                                     \
    /* [in] */ Int32 x,                                                                         \
    /* [in] */ Int32 y,                                                                         \
    /* [out] */ Int32 * pOffset)                                                                \
{                                                                                               \
    VALIDATE_NOT_NULL(pOffset);                                                                 \
    *pOffset = superClass::GetOffset(x, y);                                                     \
                                                                                                \
    return NOERROR;                                                                             \
}

#endif  //__TEXTVIEWMACRO_H__
