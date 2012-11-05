
#ifndef __EXPANDABLELISTCONNECTOR_H__
#define __EXPANDABLELISTCONNECTOR_H__

#include "ext/frameworkext.h"
#include <elastos/Math.h>
#include <elastos/AutoPtr.h>
#include <elastos/ElRefBase.h>
#include "widget/ExpandableListPosition.h"
#include "widget/BaseAdapter.h"

/*
 * Implementation notes:
 * 
 * <p>
 * Terminology:
 * <li> flPos - Flat list position, the position used by ListView
 * <li> gPos - Group position, the position of a group among all the groups
 * <li> cPos - Child position, the position of a child among all the children
 * in a group
 */

/**
 * A {@link BaseAdapter} that provides data/Views in an expandable list (offers
 * features such as collapsing/expanding groups containing children). By
 * itself, this adapter has no data and is a connector to a
 * {@link ExpandableListAdapter} which provides the data.
 * <p>
 * Internally, this connector translates the flat list position that the
 * ListAdapter expects to/from group and child positions that the ExpandableListAdapter
 * expects.
 */
class ExpandableListConnector : public BaseAdapter
{
public:
    ExpandableListConnector();
    /**
     * Constructs the connector
     */
    ExpandableListConnector(
        /* [in] */ IExpandableListAdapter* expandableListAdapter);

    /**
     * Point to the {@link ExpandableListAdapter} that will give us data/Views
     * 
     * @param expandableListAdapter the adapter that supplies us with data/Views
     */
    virtual CARAPI SetExpandableListAdapter(
        /* [in] */ IExpandableListAdapter* expandableListAdapter);

    /**
     * Translates a flat list position to either a) group pos if the specified
     * flat list position corresponds to a group, or b) child pos if it
     * corresponds to a child.  Performs a binary search on the expanded
     * groups list to find the flat list pos if it is an exp group, otherwise
     * finds where the flat list pos fits in between the exp groups.
     * 
     * @param flPos the flat list position to be translated
     * @return the group position or child position of the specified flat list
     *         position encompassed in a {@link PositionMetadata} object
     *         that contains additional useful info for insertion, etc.
     */
    virtual CARAPI_(AutoPtr<IPositionMetadata>) GetUnflattenedPos(
        /* [in] */ Int32 flPos);

    /**
     * Translates either a group pos or a child pos (+ group it belongs to) to a
     * flat list position.  If searching for a child and its group is not expanded, this will
     * return NULL since the child isn't being shown in the ListView, and hence it has no
     * position.
     * 
     * @param pos a {@link ExpandableListPosition} representing either a group position
     *        or child position
     * @return the flat list position encompassed in a {@link PositionMetadata}
     *         object that contains additional useful info for insertion, etc., or NULL.
     */
    virtual CARAPI_(AutoPtr<IPositionMetadata>) GetFlattenedPos(
        /* [in] */ IExpandableListPosition* pos);

    virtual CARAPI_(Boolean) AreAllItemsEnabled();

    virtual CARAPI_(Boolean) IsEnabled(
        /* [in] */ Int32 flatListPos);

    virtual CARAPI_(Int32) GetCount();

    virtual CARAPI_(AutoPtr<IInterface>) GetItem(
        /* [in] */ Int32 flatListPos);

    virtual CARAPI_(Int64) GetItemId(
        /* [in] */ Int32 flatListPos);

    virtual CARAPI_(AutoPtr<IView>) GetView(
        /* [in] */ Int32 flatListPos, 
        /* [in] */ IView* convertView, 
        /* [in] */ IViewGroup* parent);

    virtual CARAPI_(Int32) GetItemViewType(
        /* [in] */ Int32 flatListPos);

    virtual CARAPI_(Int32) GetViewTypeCount();
    
    virtual CARAPI_(Boolean) HasStableIds();

private:
    /**
     * Traverses the expanded group metadata list and fills in the flat list
     * positions.
     * 
     * @param forceChildrenCountRefresh Forces refreshing of the children count
     *        for all expanded groups.
     * @param syncGroupPositions Whether to search for the group positions
     *         based on the group IDs. This should only be needed when calling
     *         this from an onChanged callback.
     */
    virtual CARAPI_(void) RefreshExpGroupMetadataList(
        /* [in] */ Boolean forceChildrenCountRefresh,
        /* [in] */ Boolean syncGroupPositions);
 
public:
    /**
     * Collapse a group in the grouped list view
     * 
     * @param groupPos position of the group to collapse
     */
    virtual CARAPI_(Boolean) CollapseGroup(
        /* [in] */ Int32 groupPos);
    
    virtual CARAPI_(Boolean) CollapseGroup(
        /* [in] */ IPositionMetadata* posMetadata);

    /**
     * Expand a group in the grouped list view
     * @param groupPos the group to be expanded
     */
    virtual CARAPI_(Boolean) ExpandGroup(
        /* [in] */ Int32 groupPos);

    virtual CARAPI_(Boolean) ExpandGroup(
        /* [in] */ IPositionMetadata* posMetadata);
    
    /**
     * Whether the given group is currently expanded.
     * @param groupPosition The group to check.
     * @return Whether the group is currently expanded.
     */
    virtual CARAPI_(Boolean) IsGroupExpanded(
        /* [in] */ Int32 groupPosition);
    
    /**
     * Set the maximum number of groups that can be expanded at any given time
     */
    virtual CARAPI SetMaxExpGroupCount(
        /* [in] */ Int32 maxExpGroupCount);

    virtual CARAPI_(AutoPtr<IExpandableListAdapter>) GetAdapter();
    
    virtual CARAPI_(AutoPtr<IFilter>) GetFilter();

    virtual CARAPI_(ArrayOf<IGroupMetadata*>*) GetExpandedGroupMetadataList();
    
    virtual CARAPI SetExpandedGroupMetadataList(
        /* [in] */ ArrayOf<IGroupMetadata*>* expandedGroupMetadataList);
    
    virtual CARAPI_(Boolean) IsEmpty();

    /**
     * Searches the expandable list adapter for a group position matching the
     * given group ID. The search starts at the given seed position and then
     * alternates between moving up and moving down until 1) we find the right
     * position, or 2) we run out of time, or 3) we have looked at every
     * position
     * 
     * @return Position of the row that matches the given row ID, or
     *         {@link AdapterView#INVALID_POSITION} if it can't be found
     * @see AdapterView#findSyncPosition()
     */
    virtual CARAPI_(Int32) FindGroupPosition(
        /* [in] */ Int64 groupIdToMatch, 
        /* [in] */ Int32 seedGroupPosition);

    class MyDataSetObserver : public IDataSetObserver, public ElRefBase
    {
    public:
        MyDataSetObserver(
            /* [in] */ ExpandableListConnector* owner);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        virtual CARAPI OnChanged();

        virtual CARAPI OnInvalidated();

        ExpandableListConnector* owner;
    };
    
    /**
     * Metadata about an expanded group to help convert from a flat list
     * position to either a) group position for groups, or b) child position for
     * children
     */
    class GroupMetadata /*implements Parcelable, Comparable<GroupMetadata> */
    {

    public:
        GroupMetadata();

        static CARAPI_(AutoPtr<IGroupMetadata>) Obtain(Int32 flPos, Int32 lastChildFlPos, Int32 gPos, Int64 gId);
        
        CARAPI_(Int32) CompareTo(
            /* [in] */ IGroupMetadata* another);

        CARAPI_(Int32) DescribeContents();

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest, 
            /* [in] */ Int32 flags);
        
        /*public static final Parcelable.Creator<GroupMetadata> CREATOR =
                new Parcelable.Creator<GroupMetadata>() {
            
            public GroupMetadata createFromParcel(Parcel in) {
                GroupMetadata gm = GroupMetadata.obtain(
                        in.readInt(),
                        in.readInt(),
                        in.readInt(),
                        in.readLong());
                return gm;
            }
    
            public GroupMetadata[] newArray(Int32 size) {
                return new GroupMetadata[size];
            }
        };*/

        static const Int32 REFRESH = -1;
        
        /** This group's flat list position */
        Int32 flPos;
        
        /* firstChildFlPos isn't needed since it's (flPos + 1) */
        
        /**
         * This group's last child's flat list position, so basically
         * the range of this group in the flat list
         */
        Int32 lastChildFlPos;
        
        /**
         * This group's group position
         */
        Int32 gPos;
        
        /**
         * This group's id
         */
        Int64 gId;
        
    };

    /**
     * Data type that contains an expandable list position (can refer to either a group
     * or child) and some extra information regarding referred item (such as
     * where to insert into the flat list, etc.)
     */
    class PositionMetadata
    {
    private:
        CARAPI_(void) ResetState();
        
        /**
         * Use {@link #obtain(Int32, Int32, Int32, Int32, GroupMetadata, Int32)}
         */
        
        
        static CARAPI_(AutoPtr<IPositionMetadata>) GetRecycledOrCreate();
        
    public:
        PositionMetadata();

        static CARAPI_(AutoPtr<IPositionMetadata>) Obtain(
            /* [in] */ Int32 flatListPos, 
            /* [in] */ Int32 type, 
            /* [in] */ Int32 groupPos,
            /* [in] */ Int32 childPos, 
            /* [in] */ IGroupMetadata* groupMetadata, 
            /* [in] */ Int32 groupInsertIndex);

        CARAPI_(void) Recycle();
        
        /**
         * Checks whether the group referred to in this object is expanded,
         * or not (at the time this object was created)
         * 
         * @return whether the group at groupPos is expanded or not
         */
        CARAPI_(Boolean) IsExpanded();

    private:
        static const Int32 MAX_POOL_SIZE = 5;
        static ArrayOf<IPositionMetadata*>* sPool;/* =
                new ArrayList<PositionMetadata>(MAX_POOL_SIZE);*/
        
    public:
        /** Data type to hold the position and its type (child/group) */
        AutoPtr<IExpandableListPosition> position;
        
        /**
         * Link back to the expanded GroupMetadata for this group. Useful for
         * removing the group from the list of expanded groups inside the
         * connector when we collapse the group, and also as a check to see if
         * the group was expanded or collapsed (this will be NULL if the group
         * is collapsed since we don't keep that group's metadata)
         */
        AutoPtr<IGroupMetadata> groupMetadata;

        /**
         * For groups that are collapsed, we use this as the index (in
         * mExpGroupMetadataList) to insert this group when we are expanding
         * this group.
         */
        Int32 groupInsertIndex;
    };

private:
    /**
     * The ExpandableListAdapter to fetch the data/Views for this expandable list
     */
    AutoPtr<IExpandableListAdapter> mExpandableListAdapter;

    /**
     * List of metadata for the currently expanded groups. The metadata consists
     * of data essential for efficiently translating between flat list positions
     * and group/child positions. See {@link GroupMetadata}.
     */
    ArrayOf<IGroupMetadata*>* mExpGroupMetadataList;

    /** The number of children from all currently expanded groups */
    Int32 mTotalExpChildrenCount;
    
    /** The maximum number of allowable expanded groups. Defaults to 'no limit' */
    Int32 mMaxExpGroupCount;// = Math::INT32_MAX_VALUE;

    /** Change observer used to have ExpandableListAdapter changes pushed to us */
    AutoPtr<IDataSetObserver> mDataSetObserver;// = new MyDataSetObserver();
};

#endif
