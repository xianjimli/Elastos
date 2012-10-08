
#include "CMenuType.h"
#include "CMenuBuilder.h"

ECode CMenuType::GetInflater(
    /* [out] */ ILayoutInflater** layoutInflater) {

    //VALIDATE_NOT_NULL(*layoutInflater);

    // Create an inflater that uses the given theme for the Views it inflates
    if (mInflater == NULL) {
        //TODO
//        Context wrappedContext = new ContextThemeWrapper(mContext,
//                THEME_RES_FOR_TYPE[mMenuType]);
//        mInflater = (LayoutInflater) wrappedContext
//                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    }

    *layoutInflater = mInflater;
    if (*layoutInflater != NULL) {
        (*layoutInflater)->AddRef();
    }

    return NOERROR;
}

ECode CMenuType::SetMenuView(
    /* [in] */ IMenuView* menuView) {
    mMenuView = menuView;

    return NOERROR;
}

ECode CMenuType::GetMenuViewEx(
    /* [out] */ IMenuView** retMenuView) {
    *retMenuView = mMenuView;
    if (*retMenuView != NULL) {
        (*retMenuView)->AddRef();
    }

    return NOERROR;
}

ECode CMenuType::GetMenuView(
    /* [in] */ IViewGroup* parent,
    /* [out] */ IMenuView** retMenuView) {
    *retMenuView = NULL;
    if (m_cmb == NULL) {
        return NOERROR;
    }

    if (m_cmb->LAYOUT_RES_FOR_TYPE[mMenuType] == 0) {
        return NOERROR;
    }

    //TODO synchronized(this)

    AutoPtr<IMenuView> menuView = mMenuView != NULL ? mMenuView.Get() : NULL;

    if (menuView == NULL) {
        ILayoutInflater* layoutInflater = NULL;
        ECode ec = GetInflater(&layoutInflater);
        if(!FAILED(ec) && layoutInflater != NULL) {
            layoutInflater->InflateEx2(m_cmb->LAYOUT_RES_FOR_TYPE[mMenuType], parent, FALSE, (IView**)&menuView);

            if (menuView != NULL) {
                menuView->Initialize(m_cmb, mMenuType);
            }

            //TODO Cache the view
            //mMenuView = new WeakReference<MenuView>(menuView);
            mMenuView = menuView;

            if (m_cmb->mFrozenViewStates != NULL) {
                AutoPtr<IView> view = (AutoPtr<IView>) menuView;

                if (view != NULL) {
                    view->RestoreHierarchyState(m_cmb->mFrozenViewStates);

                    // Clear this menu type's frozen state, since we just restored it
                    Int32 id = 0;
                    view->GetId(&id);
                    m_cmb->mFrozenViewStates->Put(id, NULL);
                }
            }
        }
    }

    *retMenuView = menuView;
    if (*retMenuView != NULL) {
        (*retMenuView)->AddRef();
    }

    return NOERROR;
}

ECode CMenuType::HasMenuView(
    /* [out] */ Boolean* has) {
    *has = mMenuView != NULL && mMenuView.Get() != NULL;
    return NOERROR;
}
