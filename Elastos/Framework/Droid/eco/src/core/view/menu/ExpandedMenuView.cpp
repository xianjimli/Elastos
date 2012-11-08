
#include "view/menu/ExpandedMenuView.h"
#include "view/menu/MenuBuilderBase.h"

ExpandedMenuView::ExpandedMenuView()
{}

static Int32 R_Styleable_MenuView[] = {
    0x010100ae, 0x0101012c, 0x0101012d, 0x0101012e,
    0x0101012f, 0x01010130, 0x01010131
};

ECode ExpandedMenuView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(ListView::Init(context, attrs));

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(attrs,
            ArrayOf<Int32>(R_Styleable_MenuView, sizeof(R_Styleable_MenuView) / sizeof(Int32)),
            0, 0, (ITypedArray**)&a);

    a->GetResourceId(0/*com.android.internal.R.styleable.MenuView_windowAnimationStyle*/, 0, &mAnimations);
    a->Recycle();

    return SetOnItemClickListener((IOnItemClickListener*)this->Probe(EIID_IOnItemClickListener));
}

ECode ExpandedMenuView::Initialize(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Int32 menuType)
{
    mMenu = menu;

    AutoPtr<IMenuAdapter> adapter = new MenuBuilderBase::MenuAdapter(
            menuType, (MenuBuilderBase*)mMenu->Probe(EIID_MenuBuilderBase));
    return SetAdapter(adapter);
}

ECode ExpandedMenuView::UpdateChildren(
    /* [in] */ Boolean cleared)
{
    AutoPtr<IAdapter> adapter = GetAdapter();

    // Tell adapter of the change, it will notify the mListView
    if (adapter != NULL) {
        if (cleared) {
            return IBaseAdapter::Probe(adapter)->NotifyDataSetInvalidated();
        }
        else {
            return IBaseAdapter::Probe(adapter)->NotifyDataSetChanged();
        }
    }

    return NOERROR;
}

void ExpandedMenuView::OnDetachedFromWindow()
{
    ListView::OnDetachedFromWindow();

    // Clear the cached bitmaps of children
    SetChildrenDrawingCacheEnabled(FALSE);
}

Boolean ExpandedMenuView::RecycleOnMeasure()
{
    return FALSE;
}

Boolean ExpandedMenuView::InvokeItem(
    /* [in] */ IMenuItemImpl* item)
{
    Boolean state;
    mMenu->PerformItemAction(item, 0, &state);
    return state;
}

ECode ExpandedMenuView::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    AutoPtr<IInterface> item;
    GetAdapter()->GetItem(position, (IInterface**)&item);
    InvokeItem(IMenuItemImpl::Probe(item));
    return NOERROR;
}

Int32 ExpandedMenuView::GetWindowAnimations()
{
    return  mAnimations;
}
