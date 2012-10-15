
#ifndef __CMENUTYPE_H__
#define __CMENUTYPE_H__

#include "_CMenuType.h"
#include <elastos/AutoPtr.h>

using namespace Elastos;

class CMenuBuilder;

CarClass(CMenuType) {
    private:
        Int32 mMenuType;
        /** The layout inflater that uses the menu type's theme */
        AutoPtr<ILayoutInflater> mInflater;

        //IMutex* mMenuLock;
        CMenuBuilder* m_cmb;

        /** The lazily loaded {@link MenuView} */
        AutoPtr<IMenuView> mMenuView;

    public:
        CMenuType() {}

        ~CMenuType(){}

        CMenuType(
            /* [in] */ Int32 menuType,
            /* [in] */ IMenuBuilder* cmb) {
            mMenuType = menuType;
            m_cmb = (CMenuBuilder*)cmb;
        }

        CARAPI constructor (
            /* [in] */ Int32 menuType,
            /* [in] */ IMenuBuilder* cmb) {
            mMenuType = menuType;
            m_cmb = (CMenuBuilder*)cmb;
            return NOERROR;
        }

        virtual CARAPI GetInflater(
            /* [out] */ ILayoutInflater** layoutInflater);

        virtual CARAPI SetMenuView(
            /* [out] */ IMenuView* menuView);

        virtual CARAPI GetMenuView(
            /* [in] */ IViewGroup* parent,
            /* [out] */ IMenuView** menuView);

        virtual CARAPI GetMenuViewEx(
            /* [out] */ IMenuView** menuView);

        virtual CARAPI HasMenuView(
            /* [out] */ Boolean* has);
};

#endif  //__CMENUTYPE_H__
