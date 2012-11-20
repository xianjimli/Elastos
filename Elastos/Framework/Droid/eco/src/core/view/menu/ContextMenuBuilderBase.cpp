
#include "ext/frameworkext.h"
#include "view/menu/ContextMenuBuilderBase.h"
#include <stdio.h>


ContextMenuBuilderBase::ContextMenuBuilderBase(
    /* [in] */ IContext* context)
    : MenuBuilderBase(context)
{}

ECode ContextMenuBuilderBase::SetHeaderIcon(
    /* [in] */ IDrawable* icon)
{
    MenuBuilderBase::SetHeaderIconInt(icon);
    return NOERROR;
}

ECode ContextMenuBuilderBase::SetHeaderIcon(
    /* [in] */ Int32 iconRes)
{
    MenuBuilderBase::SetHeaderIconInt(iconRes);
    return NOERROR;
}

ECode ContextMenuBuilderBase::SetHeaderTitle(
    /* [in] */ ICharSequence* title)
{
    MenuBuilderBase::SetHeaderTitleInt(title);
    return NOERROR;
}

ECode ContextMenuBuilderBase::SetHeaderTitle(
    /* [in] */ Int32 titleRes)
{
    MenuBuilderBase::SetHeaderTitleInt(titleRes);
    return NOERROR;
}

ECode ContextMenuBuilderBase::SetHeaderView(
    /* [in] */ IView* view)
{
    MenuBuilderBase::SetHeaderViewInt(view);
    return NOERROR;
}

/**
 * Shows this context menu, allowing the optional original view (and its
 * ancestors) to add items.
 *
 * @param originalView Optional, the original view that triggered the
 *        context menu.
 * @param token Optional, the window token that should be set on the context
 *        menu's window.
 * @return If the context menu was shown, the {@link MenuDialogHelper} for
 *         dismissing it. Otherwise, null.
 */
AutoPtr<MenuDialogHelper> ContextMenuBuilderBase::Show(
    /* [in] */ IView* originalView,
    /* [in] */ IBinder* token)
{
    if (originalView != NULL) {
        // Let relevant views and their populate context listeners populate
        // the context menu
        originalView->CreateContextMenu((IContextMenu*)this->Probe(EIID_IContextMenu));
    }

    if (GetVisibleItems()->GetSize() > 0) {
        //EventLog.writeEvent(50001. 1);
        AutoPtr<MenuDialogHelper> helper = new MenuDialogHelper((IMenuBuilder*)this);
        //AutoPtr<MenuDialogHelper> helper = new MenuDialogHelper((IMenuBuilder*)this->Probe(EIID_IMenuBuilder));
        helper->Show(token);

        return helper;
    }

    return NULL;
}
