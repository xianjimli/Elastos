
#include "view/menu/CExpandedMenuView.h"
#include "widget/CMenuAdapter.h"

static Int32 R_Styleable_MenuView[] = {
    0x010100ae, 0x0101012c, 0x0101012d, 0x0101012e,
    0x0101012f, 0x01010130, 0x01010131
};

CExpandedMenuView::CExpandedMenuView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs): ListView(context, attrs) {
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs,
        ArrayOf<Int32>(R_Styleable_MenuView, sizeof(R_Styleable_MenuView) / sizeof(Int32)), 0, 0, (ITypedArray**)&a);

    a->GetResourceId(R_Styleable_MenuView[0], 0, &mAnimations);
    //mAnimations = a.getResourceId(com.android.internal.R.styleable.MenuView_windowAnimationStyle, 0);
    a->Recycle();

    SetOnItemClickListener((IOnItemClickListener*)this->Probe(EIID_IOnItemClickListener));
}

PInterface CExpandedMenuView::Probe(
    /* [in]  */ REIID riid) {
    if (riid == EIID_IOnItemClickListener) {
        return (IOnItemClickListener*)this;
    }
    else if (riid == EIID_IMenuView) {
        return (IMenuView*)this;
    }

    return NULL;
}

UInt32 CExpandedMenuView::AddRef() {
    return ElRefBase::AddRef();
}

UInt32 CExpandedMenuView::Release() {
    return ElRefBase::Release();
}

ECode CExpandedMenuView::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID) {
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IOnItemClickListener*)this) {
        *pIID = EIID_IOnItemClickListener;
    }
    else if (pObject == (IInterface*)(IMenuView*)this) {
        *pIID = EIID_IMenuView;
    }

    return NOERROR;
}

ECode CExpandedMenuView::Initialize(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Int32 menuType) {
    mMenu = menu;

    AutoPtr<IMenuAdapter> adapter;
    CMenuAdapter::New(menuType, mMenu, (IMenuAdapter**) &adapter);

    SetAdapter(adapter);

    return NOERROR;
}

ECode CExpandedMenuView::UpdateChildren(
    /* [in] */ Boolean cleared) {
    AutoPtr<IAdapter> adapter = GetAdapter();

    // Tell adapter of the change, it will notify the mListView
    if (adapter != NULL) {
        if (cleared) {
            ((IBaseAdapter*)adapter.Get())->NotifyDataSetInvalidated();
        }
        else {
            ((IBaseAdapter*)adapter.Get())->NotifyDataSetChanged();
        }
    }

    return NOERROR;
}

void CExpandedMenuView::OnDetachedFromWindow() {
    ListView::OnDetachedFromWindow();

    // Clear the cached bitmaps of children
    SetChildrenDrawingCacheEnabled(FALSE);
}

ECode CExpandedMenuView::RecycleOnMeasure(
    /* [out] */ Boolean* recycled) {
    *recycled = FALSE;
    return NOERROR;
}

ECode CExpandedMenuView::InvokeItem(
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* state) {
    return mMenu->PerformItemAction(item, 0, state);
}

ECode CExpandedMenuView::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id) {
    AutoPtr<IAdapter> adapter = GetAdapter();

    if (adapter != NULL) {
        AutoPtr<IMenuItem> menuItem;
        adapter->GetItem(position, (IInterface**) &menuItem);
        Boolean state;
        return InvokeItem((IMenuItem*)menuItem.Get(), &state);
    }

    return NOERROR;
}

ECode CExpandedMenuView::GetWindowAnimations(
    /* [out] */ Int32* animations) {
    *animations = mAnimations;

    return NOERROR;
}
