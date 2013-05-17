
#ifndef __CSELECTION_H__
#define __CSELECTION_H__

#include "_CSelection.h"

CarClass(CSelection)
{
public:
    CARAPI GetSelectionStart(
        /* [in] */ ICharSequence* text,
        /* [out] */ Int32* start);

    CARAPI GetSelectionEnd(
        /* [in] */ ICharSequence* text,
        /* [out] */ Int32* end);

    CARAPI SetSelection(
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 stop);

    CARAPI SetSelection2(
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 index);

    CARAPI SelectAll(
        /* [in] */ ISpannable* text);

    CARAPI ExtendSelection(
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 index);

    CARAPI RemoveSelection(
        /* [in] */ ISpannable* text);

    CARAPI MoveUp(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI MoveDown(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI MoveLeft(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI MoveRight(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI ExtendUp(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI ExtendDown(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI ExtendLeft(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI ExtendRight(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI ExtendToLeftEdge(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI ExtendToRightEdge(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI MoveToLeftEdge(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI MoveToRightEdge(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI GetSelectionStartObject(
        /* [out] */ IInterface** start);

    CARAPI GetSelectionEndObject(
        /* [out] */ IInterface** end);

private:
    // TODO: Add your private member variables here.
};

#endif // __CSELECTIONHELPER_H__
