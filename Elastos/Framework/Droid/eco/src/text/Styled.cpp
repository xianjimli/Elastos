
#include "text/Styled.h"
#include "text/Layout.h"
#include "text/CTextPaint.h"
#include "text/TextUtils.h"

/**
 * Draws and/or measures a uniform run of text on a single line. No span of
 * interest should start or end in the middle of this run (if not
 * drawing, character spans that don't affect metrics can be ignored).
 * Neither should the run direction change in the middle of the run.
 *
 * <p>The x position is the leading edge of the text. In a right-to-left
 * paragraph, this will be to the right of the text to be drawn. Paint
 * should not have an Align value other than LEFT or positioning will get
 * confused.
 *
 * <p>On return, workPaint will reflect the original paint plus any
 * modifications made by character styles on the run.
 *
 * <p>The returned width is signed and will be < 0 if the paragraph
 * direction is right-to-left.
 */
Float Styled::DrawUniformRun(
    /* [in] */ ICanvas* canvas,
    /* [in] */ ISpanned* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 dir,
    /* [in] */ Boolean runIsRtl,
    /* [in] */ Float x,
    /* [in] */ Int32 top,
    /* [in] */ Int32 y,
    /* [in] */ Int32 bottom,
    /* [in] */ IPaintFontMetricsInt* fmi,
    /* [in] */ ITextPaint* paint,
    /* [in] */ ITextPaint* workPaint,
    /* [in] */ Boolean needWidth)
{
    Boolean haveWidth = FALSE;
    Float ret = 0;
    ArrayOf<IInterface*>* spans;
    text->GetSpans(start, end, EIID_ICharacterStyle, &spans);

    AutoPtr<IReplacementSpan> replacement;

    // XXX: This shouldn't be modifying paint, only workPaint.
    // However, the members belonging to TextPaint should have default
    // values anyway.  Better to ensure this in the Layout constructor.
    ((CTextPaint*)paint)->mBgColor = 0;
    ((CTextPaint*)paint)->mBaselineShift = 0;
    workPaint->Set(paint);

    if (spans->GetLength() > 0) {
        for (Int32 i = 0; i < spans->GetLength(); i++) {
            ICharacterStyle* span = ICharacterStyle::Probe((*spans)[i]);

            if (IReplacementSpan::Probe(span) != NULL) {
                replacement = IReplacementSpan::Probe(span);
            }
            else {
                span->UpdateDrawState(workPaint);
            }

            span->Release();
        }
    }
    ArrayOf<IInterface*>::Free(spans);

    if (replacement == NULL) {
        AutoPtr<ICharSequence> tmp;
        Int32 tmpstart, tmpend;

        if (runIsRtl) {
            tmp = TextUtils::GetReverse(text, start, end);
            tmpstart = 0;
            // XXX: assumes getReverse doesn't change the length of the text
            tmpend = end - start;
        }
        else {
            tmp = text;
            tmpstart = start;
            tmpend = end;
        }

        if (fmi != NULL) {
            Int32 spacing;
            workPaint->GetFontMetricsInt(fmi, &spacing);
        }

        if (canvas != NULL) {
            if (((CTextPaint*)workPaint)->mBgColor != 0) {
                Int32 c;
                workPaint->GetColor(&c);
                PaintStyle s;
                workPaint->GetStyle(&s);
                workPaint->SetColor(((CTextPaint*)workPaint)->mBgColor);
                workPaint->SetStyle(PaintStyle_FILL);

                if (!haveWidth) {
                    workPaint->MeasureTextEx3(tmp, tmpstart, tmpend, &ret);
                    haveWidth = TRUE;
                }

                if (dir == Layout::DIR_RIGHT_TO_LEFT) {
                    canvas->DrawRectEx2(x - ret, top, x, bottom, workPaint);
                }
                else {
                    canvas->DrawRectEx2(x, top, x + ret, bottom, workPaint);
                }

                workPaint->SetStyle(s);
                workPaint->SetColor(c);
            }

            if (dir == Layout::DIR_RIGHT_TO_LEFT) {
                if (!haveWidth) {
                    workPaint->MeasureTextEx3(tmp, tmpstart, tmpend, &ret);
                    haveWidth = TRUE;
                }

                canvas->DrawTextInCharSequence(tmp, tmpstart, tmpend, x - ret,
                        y + ((CTextPaint*)workPaint)->mBaselineShift, workPaint);
            }
            else {
                if (needWidth) {
                    if (!haveWidth) {
                        workPaint->MeasureTextEx3(tmp, tmpstart, tmpend, &ret);
                        haveWidth = TRUE;
                    }
                }

                canvas->DrawTextInCharSequence(tmp, tmpstart, tmpend, x,
                        y + ((CTextPaint*)workPaint)->mBaselineShift, workPaint);
            }
        }
        else {
            if (needWidth && !haveWidth) {
                workPaint->MeasureTextEx3(tmp, tmpstart, tmpend, &ret);
                haveWidth = TRUE;
            }
        }
    }
    else {
        Int32 sz;
        replacement->GetSize(workPaint, text, start, end, fmi, &sz);
        ret = sz;

        if (canvas != NULL) {
            if (dir == Layout::DIR_RIGHT_TO_LEFT) {
                replacement->Draw(canvas, text, start, end,
                                  x - ret, top, y, bottom, workPaint);
            }
            else {
                replacement->Draw(canvas, text, start, end,
                                  x, top, y, bottom, workPaint);
            }
        }
    }

    if (dir == Layout::DIR_RIGHT_TO_LEFT) {
        return -ret;
    }
    else {
        return ret;
    }
}

/**
 * Returns the advance widths for a uniform left-to-right run of text with
 * no style changes in the middle of the run. If any style is replacement
 * text, the first character will get the width of the replacement and the
 * remaining characters will get a width of 0.
 *
 * @param paint the paint, will not be modified
 * @param workPaint a paint to modify; on return will reflect the original
 *        paint plus the effect of all spans on the run
 * @param text the text
 * @param start the start of the run
 * @param end the limit of the run
 * @param widths array to receive the advance widths of the characters. Must
 *        be at least a large as (end - start).
 * @param fmi FontMetrics information; can be null
 * @return the actual number of widths returned
 */
Int32 Styled::GetTextWidths(
    /* [in] */ ITextPaint* paint,
    /* [in] */ ITextPaint* workPaint,
    /* [in] */ ISpanned* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ArrayOf<Float>* widths,
    /* [in] */ IPaintFontMetricsInt* fmi)
{
    ArrayOf<IInterface*>* spans;
    text->GetSpans(start, end, EIID_IMetricAffectingSpan, &spans);

    AutoPtr<IReplacementSpan> replacement;
    workPaint->Set(paint);

    for (Int32 i = 0; i < spans->GetLength(); i++) {
        IMetricAffectingSpan* span = IMetricAffectingSpan::Probe((*spans)[i]);
        if (IReplacementSpan::Probe(span) != NULL) {
            replacement = IReplacementSpan::Probe(span);
        }
        else {
            span->UpdateMeasureState(workPaint);
        }

        span->Release();
    }
    ArrayOf<IInterface*>::Free(spans);

    if (replacement == NULL) {
        Int32 val;
        workPaint->GetFontMetricsInt(fmi, &val);
        workPaint->GetTextWidthsEx(text, start, end, widths, &val);
    }
    else {
        Int32 wid;
        replacement->GetSize(workPaint, text, start, end, fmi, &wid);

        if (end > start) {
            (*widths)[0] = wid;
            for (Int32 i = start + 1; i < end; i++)
                (*widths)[i - start] = 0;
        }
    }
    return end - start;
}

/**
 * Renders and/or measures a directional run of text on a single line.
 * Unlike {@link #drawUniformRun}, this can render runs that cross style
 * boundaries.  Returns the signed advance width, if requested.
 *
 * <p>The x position is the leading edge of the text. In a right-to-left
 * paragraph, this will be to the right of the text to be drawn. Paint
 * should not have an Align value other than LEFT or positioning will get
 * confused.
 *
 * <p>This optimizes for unstyled text and so workPaint might not be
 * modified by this call.
 *
 * <p>The returned advance width will be < 0 if the paragraph
 * direction is right-to-left.
 */
Float Styled::DrawDirectionalRun(
    /* [in] */ ICanvas* canvas,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 dir,
    /* [in] */ Boolean runIsRtl,
    /* [in] */ Float x,
    /* [in] */ Int32 top,
    /* [in] */ Int32 y,
    /* [in] */ Int32 bottom,
    /* [in] */ IPaintFontMetricsInt* fmi,
    /* [in] */ ITextPaint* paint,
    /* [in] */ ITextPaint* workPaint,
    /* [in] */ Boolean needWidth)
{
    // XXX: It looks like all calls to this API match dir and runIsRtl, so
    // having both parameters is redundant and confusing.

    // fast path for unstyled text
    if (ISpanned::Probe(text) == NULL) {
        Float ret = 0;

        if (runIsRtl) {
            AutoPtr<ICharSequence> tmp = TextUtils::GetReverse(text, start, end);
            // XXX: this assumes getReverse doesn't tweak the length of
            // the text
            Int32 tmpend = end - start;

            if (canvas != NULL || needWidth) {
                paint->MeasureTextEx3(tmp, 0, tmpend, &ret);
            }

            if (canvas != NULL) {
                canvas->DrawTextInCharSequence(tmp, 0, tmpend, x - ret, y, paint);
            }
        }
        else {
            if (needWidth) {
                paint->MeasureTextEx3(text, start, end, &ret);
            }

            if (canvas != NULL) {
                canvas->DrawTextInCharSequence(text, start, end, x, y, paint);
            }
        }

        if (fmi != NULL) {
            Int32 val;
            paint->GetFontMetricsInt(fmi, &val);
        }

        return ret * dir;   // Layout.DIR_RIGHT_TO_LEFT == -1
    }

    Float ox = x;
    Int32 minAscent = 0, maxDescent = 0, minTop = 0, maxBottom = 0;

    AutoPtr<ISpanned> sp = ISpanned::Probe(text);
    InterfaceID division;

    if (canvas == NULL) {
        division = EIID_IMetricAffectingSpan;
    }
    else {
        division = EIID_ICharacterStyle;
    }

    Int32 next;
    for (Int32 i = start; i < end; i = next) {
        sp->NextSpanTransition(i, end, division, &next);

        // XXX: if dir and runIsRtl were not the same, this would draw
        // spans in the wrong order, but no one appears to call it this
        // way.
        x += DrawUniformRun(canvas, sp, i, next, dir, runIsRtl,
                x, top, y, bottom, fmi, paint, workPaint,
                needWidth || next != end);

        if (fmi != NULL) {
            Int32 ascent, descent, top, bottom;
            fmi->GetAscent(&ascent);
            fmi->GetDescent(&descent);
            fmi->GetTop(&top);
            fmi->GetBottom(&bottom);

            if (ascent < minAscent) {
                minAscent = ascent;
            }
            if (descent > maxDescent) {
                maxDescent = descent;
            }

            if (top < minTop) {
                minTop = top;
            }
            if (bottom > maxBottom) {
                maxBottom = bottom;
            }
        }
    }

    if (fmi != NULL) {
        if (start == end) {
            Int32 val;
            paint->GetFontMetricsInt(fmi, &val);
        }
        else {
            fmi->SetAscent(minAscent);
            fmi->SetDescent(maxDescent);
            fmi->SetTop(minTop);
            fmi->SetBottom(maxBottom);
        }
    }

    return x - ox;
}

/**
 * Draws a unidirectional run of text on a single line, and optionally
 * returns the signed advance.  Unlike drawDirectionalRun, the paragraph
 * direction and run direction can be different.
 */
/* package */
Float Styled::DrawText(
    /* [in] */ ICanvas* canvas,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 dir,
    /* [in] */ Boolean runIsRtl,
    /* [in] */ Float x,
    /* [in] */ Int32 top,
    /* [in] */ Int32 y,
    /* [in] */ Int32 bottom,
    /* [in] */ ITextPaint* paint,
    /* [in] */ ITextPaint* workPaint,
    /* [in] */ Boolean needWidth)
{
    // XXX this logic is (dir == DIR_LEFT_TO_RIGHT) == runIsRtl
    if ((dir == Layout::DIR_RIGHT_TO_LEFT && !runIsRtl) ||
        (runIsRtl && dir == Layout::DIR_LEFT_TO_RIGHT)) {
        // TODO: this needs the real direction
        Float ch = DrawDirectionalRun(NULL, text, start, end,
                Layout::DIR_LEFT_TO_RIGHT, FALSE, 0, 0, 0, 0, NULL, paint,
                workPaint, TRUE);

        ch *= dir;  // DIR_RIGHT_TO_LEFT == -1
        DrawDirectionalRun(canvas, text, start, end, -dir,
                runIsRtl, x + ch, top, y, bottom, NULL, paint,
                workPaint, TRUE);

        return ch;
    }

    return DrawDirectionalRun(canvas, ISpanned::Probe(text), start, end, dir, runIsRtl,
                   x, top, y, bottom, NULL, paint, workPaint,
                   needWidth);
}

/**
 * Draws a run of text on a single line, with its
 * origin at (x,y), in the specified Paint. The origin is interpreted based
 * on the Align setting in the Paint.
 *
 * This method considers style information in the text (e.g. even when text
 * is an instance of {@link android.text.Spanned}, this method correctly
 * draws the text). See also
 * {@link android.graphics.Canvas#drawText(CharSequence, int, int, float,
 * float, Paint)} and
 * {@link android.graphics.Canvas#drawRect(float, float, float, float,
 * Paint)}.
 *
 * @param canvas The target canvas
 * @param text The text to be drawn
 * @param start The index of the first character in text to draw
 * @param end (end - 1) is the index of the last character in text to draw
 * @param direction The direction of the text. This must be
 *        {@link android.text.Layout#DIR_LEFT_TO_RIGHT} or
 *        {@link android.text.Layout#DIR_RIGHT_TO_LEFT}.
 * @param x The x-coordinate of origin for where to draw the text
 * @param top The top side of the rectangle to be drawn
 * @param y The y-coordinate of origin for where to draw the text
 * @param bottom The bottom side of the rectangle to be drawn
 * @param paint The main {@link TextPaint} object.
 * @param workPaint The {@link TextPaint} object used for temporal
 *        workspace.
 * @param needWidth If true, this method returns the width of drawn text
 * @return Width of the drawn text if needWidth is true
 */
Float Styled::DrawText(
    /* [in] */ ICanvas* canvas,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 direction,
    /* [in] */ Float x,
    /* [in] */ Int32 top,
    /* [in] */ Int32 y,
    /* [in] */ Int32 bottom,
    /* [in] */ ITextPaint* paint,
    /* [in] */ ITextPaint* workPaint,
    /* [in] */ Boolean needWidth)
{
    // For safety.
    direction = direction >= 0 ? Layout::DIR_LEFT_TO_RIGHT
            : Layout::DIR_RIGHT_TO_LEFT;

    // Hide runIsRtl parameter since it is meaningless for external
    // developers.
    // XXX: the runIsRtl probably ought to be the same as direction, then
    // this could draw rtl text.
    return DrawText(canvas, text, start, end, direction, FALSE,
                    x, top, y, bottom, paint, workPaint, needWidth);
}

/**
 * Returns the width of a run of left-to-right text on a single line,
 * considering style information in the text (e.g. even when text is an
 * instance of {@link android.text.Spanned}, this method correctly measures
 * the width of the text).
 *
 * @param paint the main {@link TextPaint} object; will not be modified
 * @param workPaint the {@link TextPaint} object available for modification;
 *        will not necessarily be used
 * @param text the text to measure
 * @param start the index of the first character to start measuring
 * @param end 1 beyond the index of the last character to measure
 * @param fmi FontMetrics information; can be null
 * @return The width of the text
 */
Float Styled::MeasureText(
    /* [in] */ ITextPaint* paint,
    /* [in] */ ITextPaint* workPaint,
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IPaintFontMetricsInt* fmi)
{
    return DrawDirectionalRun(NULL, text, start, end,
                       Layout::DIR_LEFT_TO_RIGHT, FALSE,
                       0, 0, 0, 0, fmi, paint, workPaint, TRUE);
}
