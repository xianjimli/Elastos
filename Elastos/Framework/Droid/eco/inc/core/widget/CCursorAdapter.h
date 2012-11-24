
#ifndef __CCURSORADAPTER_H__
#define __CCURSORADAPTER_H__

#include "_CCursorAdapter.h"
#include "widget/CursorAdapter.h"

CarClass(CCursorAdapter), public CursorAdapter
{
public:
    CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver * pObserver);

    CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver * pObserver);

    CARAPI GetCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetItem(
        /* [in] */ Int32 position,
        /* [out] */ IInterface ** ppItem);

    CARAPI GetItemId(
        /* [in] */ Int32 position,
        /* [out] */ Int64 * pItemId);

    CARAPI HasStableIds(
        /* [out] */ Boolean * pHasStableIds);

    CARAPI GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView * pConvertView,
        /* [in] */ IViewGroup * pParent,
        /* [out] */ IView ** ppView);

    CARAPI GetItemViewType(
        /* [in] */ Int32 position,
        /* [out] */ Int32 * pViewType);

    CARAPI GetViewTypeCount(
        /* [out] */ Int32 * pCount);

    CARAPI IsEmpty(
        /* [out] */ Boolean * pIsEmpty);

    CARAPI AreAllItemsEnabled(
        /* [out] */ Boolean * pEnabled);

    CARAPI IsEnabled(
        /* [in] */ Int32 position,
        /* [out] */ Boolean * pEnabled);

    CARAPI NotifyDataSetChanged();

    CARAPI NotifyDataSetInvalidated();

    CARAPI GetCursor(
        /* [out] */ ICursor ** ppCursor);

    CARAPI NewView(
        /* [in] */ IContext * pContext,
        /* [in] */ ICursor * pCursor,
        /* [in] */ IViewGroup * pParent,
        /* [out] */ IView ** ppView);

    CARAPI NewDropDownView(
        /* [in] */ IContext * pContext,
        /* [in] */ ICursor * pCursor,
        /* [in] */ IViewGroup * pParent,
        /* [out] */ IView ** ppView);

    CARAPI BindView(
        /* [in] */ IView * pView,
        /* [in] */ IContext * pContext,
        /* [in] */ ICursor * pCursor);

    CARAPI ChangeCursor(
        /* [in] */ ICursor * pCursor);

    CARAPI ConvertToString(
        /* [in] */ ICursor * pCursor,
        /* [out] */ ICharSequence ** ppCs);

    CARAPI RunQueryOnBackgroundThread(
        /* [in] */ ICharSequence * pConstraint,
        /* [out] */ ICursor ** ppCursor);

    CARAPI GetFilter(
        /* [out] */ IFilter ** ppFilter);

    CARAPI GetFilterQueryProvider(
        /* [out] */ IFilterQueryProvider ** ppFilterQueryProvider);

    CARAPI SetFilterQueryProvider(
        /* [in] */ IFilterQueryProvider * pFilterQueryProvider);

    CARAPI constructor(
        /*[in]*/ IContext* ctx, 
        /* [in] */ ICursor* c);

    CARAPI constructor(
        /* [in] */ IContext* ctx, 
        /* [in] */ ICursor* c, 
        /* [in] */ Boolean autoRequery);

private:
    // TODO: Add your private member variables here.
};

#endif // __CCURSORADAPTER_H__
