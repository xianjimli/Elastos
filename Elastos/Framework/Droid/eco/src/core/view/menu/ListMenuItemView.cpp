
#include "view/menu/ListMenuItemView.h"

ListMenuItemView::ListMenuItemView()
{

}

ListMenuItemView::~ListMenuItemView()
{

}

UInt32 ListMenuItemView::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ListMenuItemView::Release()
{
    return ElRefBase::Release();
}

ECode ListMenuItemView::OnFinishInflate()
{
    LinearLayout::OnFinishInflate();

    SetBackgroundDrawable(mBackground);

    AutoPtr<IView> view = FindViewById(0x01020016 /*com.android.internal.R.id.title*/);
    mTitleView = (ITextView*)view->Probe(EIID_ITextView);
    if (mTextAppearance != -1) {
        mTitleView->SetTextAppearance(mTextAppearanceContext,
                                     mTextAppearance);
    }

    view = FindViewById(0x010201ef /*com.android.internal.R.id.shortcut*/);
    mShortcutView = (ITextView*)view->Probe(EIID_ITextView);
    return NOERROR;
}
