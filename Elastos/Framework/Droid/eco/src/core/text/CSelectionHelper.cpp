
#include "ext/frameworkdef.h"
#include "text/CSelectionHelper.h"
#include "text/Selection.h"

ECode CSelectionHelper::GetSelectionStart(
    /* [in] */ ICharSequence* text,
    /* [out] */ Int32* start)
{
    VALIDATE_NOT_NULL(start);

    *start = Selection::GetSelectionStart(text);

    return NOERROR;
}

ECode CSelectionHelper::GetSelectionEnd(
    /* [in] */ ICharSequence* text,
    /* [out] */ Int32* end)
{
    VALIDATE_NOT_NULL(text);

    *end = Selection::GetSelectionEnd(text);

    return NOERROR;
}

ECode CSelectionHelper::SetSelection(
    /* [in] */ ISpannable* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 stop)
{
    Selection::SetSelection(text, start, stop);

    return NOERROR;
}

ECode CSelectionHelper::SetSelection2(
    /* [in] */ ISpannable* text,
    /* [in] */ Int32 index)
{
    Selection::SetSelection(text, index);

    return NOERROR;
}

ECode CSelectionHelper::SelectAll(
    /* [in] */ ISpannable* text)
{
    Selection::SelectAll(text);

    return NOERROR;
}

ECode CSelectionHelper::ExtendSelection(
    /* [in] */ ISpannable* text,
    /* [in] */ Int32 index)
{
    Selection::ExtendSelection(text, index);

    return NOERROR;
}

ECode CSelectionHelper::RemoveSelection(
    /* [in] */ ISpannable* text)
{
    Selection::RemoveSelection(text);

    return NOERROR;
}

ECode CSelectionHelper::MoveUp(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::MoveUp(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelectionHelper::MoveDown(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::MoveDown(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelectionHelper::MoveLeft(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::MoveLeft(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelectionHelper::MoveRight(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::MoveRight(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelectionHelper::ExtendUp(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::ExtendUp(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelectionHelper::ExtendDown(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::ExtendDown(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelectionHelper::ExtendLeft(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::ExtendLeft(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelectionHelper::ExtendRight(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::ExtendRight(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelectionHelper::ExtendToLeftEdge(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::ExtendToLeftEdge(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelectionHelper::ExtendToRightEdge(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::ExtendToRightEdge(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelectionHelper::MoveToLeftEdge(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::MoveToLeftEdge(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelectionHelper::MoveToRightEdge(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::MoveToRightEdge(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelectionHelper::GetSelectionStartObject(
    /* [out] */ IInterface** start)
{
    VALIDATE_NOT_NULL(start);

    *start = Selection::SELECTION_START;
    if (*start) (*start)->AddRef();

    return NOERROR;
}

ECode CSelectionHelper::GetSelectionEndObject(
    /* [out] */ IInterface** end)
{
    VALIDATE_NOT_NULL(end);

    *end = Selection::SELECTION_START;
    if (*end) (*end)->AddRef();

    return NOERROR;
}

