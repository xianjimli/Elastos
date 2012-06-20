#ifndef __LAYOUTMACRO_H__
#define __LAYOUTMACRO_H__

#define ILAYOUT_METHODS_DECL()                                          \
    CARAPI Draw(                                                        \
        /* [in] */ ICanvas* c);                                       \
                                                                        \
    CARAPI DrawEx(                                                      \
        /* [in] */ ICanvas* c,                                        \
        /* [in] */ IPath* highlight,                                    \
        /* [in] */ IPaint* highlightPaint,                            \
        /* [in] */ Int32 cursorOffsetVertical);                         \
                                                                        \
    CARAPI GetText(                                                     \
        /* [out] */ ICharSequence** text);                              \
                                                                        \
    CARAPI GetPaint(                                                    \
        /* [out] */ ITextPaint** paint);                                \
                                                                        \
    CARAPI GetWidth(                                                    \
        /* [out] */ Int32* width);                                      \
                                                                        \
    CARAPI GetEllipsizedWidth(                                          \
        /* [out] */ Int32* width);                                      \
                                                                        \
    CARAPI IncreaseWidthTo(                                             \
        /* [in] */ Int32 wid);                                          \
                                                                        \
    CARAPI GetHeight(                                                   \
        /* [out] */ Int32* height);                                     \
                                                                        \
    CARAPI GetAlignment(                                                \
        /* [out] */ LayoutAlignment* alignment);                        \
                                                                        \
    CARAPI GetSpacingMultiplier(                                        \
        /* [out] */ Float* spacingMultiplier);                          \
                                                                        \
    CARAPI GetSpacingAdd(                                               \
        /* [out] */ Float* spacingAdd);                                 \
                                                                        \
    CARAPI GetLineCount(                                                \
        /* [out] */ Int32* count);                                      \
                                                                        \
    CARAPI GetLineBounds(                                               \
        /* [in] */ Int32 line,                                          \
        /* [in] */ IRect* bounds,                                       \
        /* [out] */ Int32* baseline);                                   \
                                                                        \
    CARAPI GetLineTop(                                                  \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Int32* pos);                                        \
                                                                        \
    CARAPI GetLineDescent(                                              \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Int32* descent);                                    \
                                                                        \
    CARAPI GetLineStart(                                                \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Int32* offset);                                     \
                                                                        \
    CARAPI GetParagraphDirection(                                       \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Int32* direction);                                  \
                                                                        \
    CARAPI GetLineContainsTab(                                          \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI GetLineDirections(                                           \
        /* [in] */ Int32 line,                                          \
        /* [out] */ IDirections** directions);                          \
                                                                        \
    CARAPI GetTopPadding(                                               \
        /* [out] */ Int32* number);                                     \
                                                                        \
    CARAPI GetBottomPadding(                                            \
        /* [out] */ Int32* number);                                     \
                                                                        \
    CARAPI GetPrimaryHorizontal(                                        \
        /* [in] */ Int32 offset,                                        \
        /* [out] */ Float* pos);                                        \
                                                                        \
    CARAPI GetSecondaryHorizontal(                                      \
        /* [in] */ Int32 offset,                                        \
        /* [out] */ Float* pos);                                        \
                                                                        \
    CARAPI GetLineLeft(                                                 \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Float* pos);                                        \
                                                                        \
    CARAPI GetLineRight(                                                \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Float* pos);                                        \
                                                                        \
    CARAPI GetLineMax(                                                  \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Float* extent);                                     \
                                                                        \
    CARAPI GetLineWidth(                                                \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Float* extent);                                     \
                                                                        \
    CARAPI GetLineForVertical(                                          \
        /* [in] */ Int32 vertical,                                      \
        /* [out] */ Int32* number);                                     \
                                                                        \
    CARAPI GetLineForOffset(                                            \
        /* [in] */ Int32 offset,                                        \
        /* [out] */ Int32* number);                                     \
                                                                        \
    CARAPI GetOffsetForHorizontal(                                      \
        /* [in] */ Int32 line,                                          \
        /* [in] */ Float horiz,                                         \
        /* [out] */ Int32* offset);                                     \
                                                                        \
    CARAPI GetLineEnd(                                                  \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Int32* offset);                                     \
                                                                        \
    CARAPI GetLineVisibleEnd(                                           \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Int32* offset);                                     \
                                                                        \
    CARAPI GetLineBottom(                                               \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Int32* pos);                                        \
                                                                        \
    CARAPI GetLineBaseline(                                             \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Int32* pos);                                        \
                                                                        \
    CARAPI GetLineAscent(                                               \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Int32* ascent);                                     \
                                                                        \
    CARAPI GetOffsetToLeftOf(                                           \
        /* [in] */ Int32 offset,                                        \
        /* [out] */ Int32* retOffset);                                  \
                                                                        \
    CARAPI GetOffsetToRightOf(                                          \
        /* [in] */ Int32 offset,                                        \
        /* [out] */ Int32* retOffset);                                  \
                                                                        \
    CARAPI GetCursorPath(                                               \
        /* [in] */ Int32 point,                                         \
        /* [in] */ IPath* dest,                                         \
        /* [in] */ ICharSequence* editingBuffer);                       \
                                                                        \
    CARAPI GetSelectionPath(                                            \
        /* [in] */ Int32 start,                                         \
        /* [in] */ Int32 end,                                           \
        /* [in] */ IPath* dest);                                        \
                                                                        \
    CARAPI GetParagraphAlignment(                                       \
        /* [in] */ Int32 line,                                          \
        /* [out] */ LayoutAlignment* alignment);                        \
                                                                        \
    CARAPI GetParagraphLeft(                                            \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Int32* leftEdge);                                   \
                                                                        \
    CARAPI GetParagraphRight(                                           \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Int32* rightEdge);                                  \
                                                                        \
    CARAPI GetEllipsisStart(                                            \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Int32* ellipsisStart);                              \
                                                                        \
    CARAPI GetEllipsisCount(                                            \
        /* [in] */ Int32 line,                                          \
        /* [out] */ Int32* ellipsisCount);

#define ILAYOUT_METHODS_IMPL(className, superClass, overRideClass)      \
ECode className::Draw(                                                  \
    /* [in] */ ICanvas* c)                                            \
{                                                                       \
    return superClass::Draw(c);                                         \
}                                                                       \
                                                                        \
ECode className::DrawEx(                                                \
    /* [in] */ ICanvas* c,                                            \
    /* [in] */ IPath* highlight,                                        \
    /* [in] */ IPaint* highlightPaint,                                \
    /* [in] */ Int32 cursorOffsetVertical)                              \
{                                                                       \
    return superClass::Draw(c, highlight,                               \
            highlightPaint, cursorOffsetVertical);                      \
}                                                                       \
                                                                        \
ECode className::GetText(                                               \
    /* [out] */ ICharSequence** text)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(text);                                            \
    AutoPtr<ICharSequence> temp = superClass::GetText();                \
    *text = temp;                                                       \
    if (*text) {                                                        \
        (*text)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetPaint(                                              \
    /* [out] */ ITextPaint** paint)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(paint);                                           \
    AutoPtr<ITextPaint> temp = superClass::GetPaint();                  \
    *paint = temp;                                                      \
    if (*paint) {                                                       \
        (*paint)->AddRef();                                             \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetWidth(                                              \
    /* [out] */ Int32* width)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(width);                                           \
    *width = superClass::GetWidth();                                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetEllipsizedWidth(                                    \
    /* [out] */ Int32* width)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(width);                                           \
    *width = superClass::GetEllipsizedWidth();                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IncreaseWidthTo(                                       \
    /* [in] */ Int32 wid)                                               \
{                                                                       \
    return superClass::IncreaseWidthTo(wid);                            \
}                                                                       \
                                                                        \
ECode className::GetHeight(                                             \
    /* [out] */ Int32* height)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(height);                                          \
    *height = superClass::GetHeight();                                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetAlignment(                                          \
    /* [out] */ LayoutAlignment* alignment)                             \
{                                                                       \
    VALIDATE_NOT_NULL(alignment);                                       \
    *alignment = superClass::GetAlignment();                            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetSpacingMultiplier(                                  \
    /* [out] */ Float* spacingMultiplier)                               \
{                                                                       \
    VALIDATE_NOT_NULL(spacingMultiplier);                               \
    *spacingMultiplier = superClass::GetSpacingMultiplier();            \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetSpacingAdd(                                         \
    /* [out] */ Float* spacingAdd)                                      \
{                                                                       \
    VALIDATE_NOT_NULL(spacingAdd);                                      \
    *spacingAdd = superClass::GetSpacingAdd();                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineCount(                                          \
    /* [out] */ Int32* count)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(count);                                           \
    *count = superClass::GetLineCount();                                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineBounds(                                         \
    /* [in] */ Int32 line,                                              \
    /* [in] */ IRect* bounds,                                           \
    /* [out] */ Int32* baseline)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(baseline);                                        \
    *baseline = superClass::GetLineBounds(line, bounds);                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineTop(                                            \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Int32* pos)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(pos);                                             \
    *pos = superClass::GetLineTop(line);                                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineDescent(                                        \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Int32* descent)                                         \
{                                                                       \
    VALIDATE_NOT_NULL(descent);                                         \
    *descent = superClass::GetLineDescent(line);                        \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineStart(                                          \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Int32* offset)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(offset);                                          \
    *offset = superClass::GetLineStart(line);                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetParagraphDirection(                                 \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Int32* direction)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(direction);                                       \
    *direction = superClass::GetParagraphDirection(line);               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineContainsTab(                                    \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result);                                          \
    *result = superClass::GetLineContainsTab(line);                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineDirections(                                     \
    /* [in] */ Int32 line,                                              \
    /* [out] */ IDirections** directions)                               \
{                                                                       \
    VALIDATE_NOT_NULL(directions);                                      \
    AutoPtr<IDirections> temp = superClass::GetLineDirections(line);    \
    *directions = temp;                                                 \
    if (*directions) {                                                  \
        (*directions)->AddRef();                                        \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetTopPadding(                                         \
    /* [out] */ Int32* number)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(number);                                          \
    *number = superClass::GetTopPadding();                              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetBottomPadding(                                      \
    /* [out] */ Int32* number)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(number);                                          \
    *number = superClass::GetBottomPadding();                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetPrimaryHorizontal(                                  \
    /* [in] */ Int32 offset,                                            \
    /* [out] */ Float* pos)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(pos);                                             \
    *pos = superClass::GetPrimaryHorizontal(offset);                    \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetSecondaryHorizontal(                                \
    /* [in] */ Int32 offset,                                            \
    /* [out] */ Float* pos)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(pos);                                             \
    *pos = superClass::GetSecondaryHorizontal(offset);                  \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineLeft(                                           \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Float* pos)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(pos);                                             \
    *pos = superClass::GetLineLeft(line);                               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineRight(                                          \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Float* pos)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(pos);                                             \
    *pos = superClass::GetLineRight(line);                              \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineMax(                                            \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Float* extent)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(extent);                                          \
    *extent = superClass::GetLineMax(line);                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineWidth(                                          \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Float* extent)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(extent);                                          \
    *extent = superClass::GetLineWidth(line);                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineForVertical(                                    \
    /* [in] */ Int32 vertical,                                          \
    /* [out] */ Int32* number)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(number);                                          \
    *number = superClass::GetLineForVertical(vertical);                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineForOffset(                                      \
    /* [in] */ Int32 offset,                                            \
    /* [out] */ Int32* number)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(number);                                          \
    *number = superClass::GetLineForOffset(offset);                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetOffsetForHorizontal(                                \
    /* [in] */ Int32 line,                                              \
    /* [in] */ Float horiz,                                             \
    /* [out] */ Int32* offset)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(offset);                                          \
    *offset = superClass::GetOffsetForHorizontal(line, horiz);          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineEnd(                                            \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Int32* offset)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(offset);                                          \
    *offset = superClass::GetLineEnd(line);                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineVisibleEnd(                                     \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Int32* offset)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(offset);                                          \
    *offset = superClass::GetLineVisibleEnd(line);                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineBottom(                                         \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Int32* pos)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(pos);                                             \
    *pos = superClass::GetLineBottom(line);                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineBaseline(                                       \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Int32* pos)                                             \
{                                                                       \
    VALIDATE_NOT_NULL(pos);                                             \
    *pos = superClass::GetLineBaseline(line);                           \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetLineAscent(                                         \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Int32* ascent)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(ascent);                                          \
    *ascent = superClass::GetLineAscent(line);                          \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetOffsetToLeftOf(                                     \
    /* [in] */ Int32 offset,                                            \
    /* [out] */ Int32* retOffset)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(retOffset);                                       \
    *retOffset = superClass::GetOffsetToLeftOf(offset);                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetOffsetToRightOf(                                    \
    /* [in] */ Int32 offset,                                            \
    /* [out] */ Int32* retOffset)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(retOffset);                                       \
    *retOffset = superClass::GetOffsetToRightOf(offset);                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetCursorPath(                                         \
    /* [in] */ Int32 point,                                             \
    /* [in] */ IPath* dest,                                             \
    /* [in] */ ICharSequence* editingBuffer)                            \
{                                                                       \
    return superClass::GetCursorPath(point, dest, editingBuffer);       \
}                                                                       \
                                                                        \
ECode className::GetSelectionPath(                                      \
    /* [in] */ Int32 start,                                             \
    /* [in] */ Int32 end,                                               \
    /* [in] */ IPath* dest)                                             \
{                                                                       \
    return superClass::GetSelectionPath(start, end, dest);              \
}                                                                       \
                                                                        \
ECode className::GetParagraphAlignment(                                 \
    /* [in] */ Int32 line,                                              \
    /* [out] */ LayoutAlignment* alignment)                             \
{                                                                       \
    VALIDATE_NOT_NULL(alignment);                                       \
    *alignment = superClass::GetParagraphAlignment(line);               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetParagraphLeft(                                      \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Int32* leftEdge)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(leftEdge);                                        \
    *leftEdge = superClass::GetParagraphLeft(line);                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetParagraphRight(                                     \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Int32* rightEdge)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(rightEdge);                                       \
    *rightEdge = superClass::GetParagraphRight(line);                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetEllipsisStart(                                      \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Int32* ellipsisStart)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(ellipsisStart);                                   \
    *ellipsisStart = superClass::GetEllipsisStart(line);                \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetEllipsisCount(                                      \
    /* [in] */ Int32 line,                                              \
    /* [out] */ Int32* ellipsisCount)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(ellipsisCount);                                   \
    *ellipsisCount = superClass::GetEllipsisCount(line);                \
                                                                        \
    return NOERROR;                                                     \
}

#endif //__LAYOUTMACRO_H__
