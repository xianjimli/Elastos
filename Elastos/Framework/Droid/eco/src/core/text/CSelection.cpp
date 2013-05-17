
#include "ext/frameworkdef.h"
#include "text/CSelection.h"
#include "text/Selection.h"

ECode CSelection::GetSelectionStart(
    /* [in] */ ICharSequence* text,
    /* [out] */ Int32* start)
{
    VALIDATE_NOT_NULL(start);

    *start = Selection::GetSelectionStart(text);

    return NOERROR;
}

ECode CSelection::GetSelectionEnd(
    /* [in] */ ICharSequence* text,
    /* [out] */ Int32* end)
{
    VALIDATE_NOT_NULL(text);

    *end = Selection::GetSelectionEnd(text);

    return NOERROR;
}

ECode CSelection::SetSelection(
    /* [in] */ ISpannable* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 stop)
{
    Selection::SetSelection(text, start, stop);

    return NOERROR;
}

ECode CSelection::SetSelection2(
    /* [in] */ ISpannable* text,
    /* [in] */ Int32 index)
{
    Selection::SetSelection(text, index);

    return NOERROR;
}

ECode CSelection::SelectAll(
    /* [in] */ ISpannable* text)
{
    Selection::SelectAll(text);

    return NOERROR;
}

ECode CSelection::ExtendSelection(
    /* [in] */ ISpannable* text,
    /* [in] */ Int32 index)
{
    Selection::ExtendSelection(text, index);

    return NOERROR;
}

ECode CSelection::RemoveSelection(
    /* [in] */ ISpannable* text)
{
    Selection::RemoveSelection(text);

    return NOERROR;
}

ECode CSelection::MoveUp(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::MoveUp(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelection::MoveDown(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::MoveDown(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelection::MoveLeft(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::MoveLeft(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelection::MoveRight(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::MoveRight(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelection::ExtendUp(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::ExtendUp(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelection::ExtendDown(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::ExtendDown(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelection::ExtendLeft(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::ExtendLeft(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelection::ExtendRight(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::ExtendRight(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelection::ExtendToLeftEdge(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::ExtendToLeftEdge(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelection::ExtendToRightEdge(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::ExtendToRightEdge(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelection::MoveToLeftEdge(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::MoveToLeftEdge(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelection::MoveToRightEdge(
    /* [in] */ ISpannable* text,
    /* [in] */ ILayout* layout,
    /* [out] */ Boolean* result)
{
    Boolean ret = Selection::MoveToRightEdge(text, layout);
    if (result) *result = ret;

    return NOERROR;
}

ECode CSelection::GetSelectionStartObject(
    /* [out] */ IInterface** start)
{
    VALIDATE_NOT_NULL(start);

    *start = Selection::SELECTION_START;
    if (*start) (*start)->AddRef();

    return NOERROR;
}

ECode CSelection::GetSelectionEndObject(
    /* [out] */ IInterface** end)
{
    VALIDATE_NOT_NULL(end);

    *end = Selection::SELECTION_START;
    if (*end) (*end)->AddRef();

    return NOERROR;
}

