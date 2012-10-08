#ifndef __CMENUADAPTER_H__
#define __CMENUADAPTER_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include "_CMenuAdapter.h"

using namespace Elastos;

CarClass(CMenuAdapter)
{
    private:
        Int32 mMenuType;
        IMenuBuilder* m_cmb;

    public:
        CMenuAdapter(){}

        CMenuAdapter(
            /* [in] */ Int32 menuType,
            /* [in] */ IMenuBuilder* cmb) {
            mMenuType = menuType;
            m_cmb = cmb;
        }

        CARAPI constructor(
            /* [in] */ Int32 menuType,
            /* [in] */ IMenuBuilder* cmb) {
            mMenuType = menuType;
            m_cmb = cmb;

            return NOERROR;
        }

        CARAPI GetOffset(
            /* [out] */ Int32* offset);

        CARAPI GetCount(
            /* [out] */ Int32* count);

        virtual CARAPI GetItem(
            /* [in] */ Int32 position,
            /* [out] */ IInterface** i);

        ECode GetItemId(
            /* [in] */ Int32 position,
            /* [out] */ Int64* id) {
            // Since a menu item's ID is optional, we'll use the position as an
            // ID for the item in the AdapterView
            *id = position;
            return NOERROR;
        }

        CARAPI GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [out] */ IView** view);

        virtual CARAPI RegisterDataSetObserver(
            /* [in] */ IDataSetObserver* dso);

        virtual CARAPI UnregisterDataSetObserver(
            /* [in] */ IDataSetObserver* dso);

        virtual CARAPI HasStableIds(
            /* [out] */ Boolean*);

        virtual CARAPI GetItemViewType(
            /* [in] */ Elastos::Int32,
            /* [out] */ Int32*);

        virtual CARAPI GetViewTypeCount(
            /* [out] */ Int32*);

        virtual CARAPI IsEmpty(
            /* [out] */ Boolean*);

        virtual CARAPI AreAllItemsEnabled(
            /* [out] */ Boolean*);

        virtual CARAPI IsEnabled(
            /* [in] */ Int32,
            /* [out] */ Boolean*);

        virtual CARAPI NotifyDataSetChanged();

        virtual CARAPI NotifyDataSetInvalidated();

};
#endif //__CMENUADAPTER_H__
