
#ifndef __CEXPANDABLELISTCONNECTOR_H__
#define __CEXPANDABLELISTCONNECTOR_H__

#include "_CExpandableListConnector.h"
#include "widget/ExpandableListConnector.h"

CarClass(CExpandableListConnector), public ExpandableListConnector
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

    CARAPI SetExpandableListAdapter(
        /* [in] */ IExpandableListAdapter * pExpandableListAdapter);

    CARAPI GetUnflattenedPos(
        /* [in] */ Int32 flPos,
        /* [out] */ IPositionMetadata ** ppPositionMetadata);

    CARAPI IsGroupExpanded(
        /* [in] */ Int32 groupPosition,
        /* [out] */ Boolean * pExpanded);

    CARAPI SetMaxExpGroupCount(
        /* [in] */ Int32 maxExpGroupCount);

    CARAPI GetFilter(
        /* [out] */ IFilter ** ppFilter);

    CARAPI constructor(
        /* [in] */ IExpandableListAdapter * pExpandableListAdapter);

private:
    // TODO: Add your private member variables here.
};

#endif // __CEXPANDABLELISTCONNECTOR_H__
