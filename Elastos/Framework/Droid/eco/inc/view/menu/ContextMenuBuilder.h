
#ifndef __CONTEXTMENUBUILDER_H__
#define __CONTEXTMENUBUILDER_H__

#include "CMenuBuilder.h"
#include <elastos/ElRefBase.h>
#include "MenuBuilderMacro.h"

class CMenuDialogHelper;

//public class ContextMenuBuilder extends MenuBuilder implements ContextMenu {
class ContextMenuBuilder:
    public ElRefBase,
    public MenuBuilder,
    public IContextMenu {
public:

    IMENUBUILDER_METHODS_DECL();

    ContextMenuBuilder(
        // /* [in] */ IContext* context): CMenuBuilder(context) {
        /* [in] */ IContext* context): MenuBuilder(context) {
    }

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    /**
     * Sets the context menu header's title to the title given in <var>titleRes</var>
     * resource identifier.
     *
     * @param titleRes The string resource identifier used for the title.
     * @return This ContextMenu so additional setters can be called.
     */
    CARAPI SetHeaderTitle(
        /* [in] */ Int32 titleRes,
        /* [out] */ IContextMenu** contextMenu);

    /**
     * Sets the context menu header's title to the title given in <var>title</var>.
     *
     * @param title The character sequence used for the title.
     * @return This ContextMenu so additional setters can be called.
     */
    CARAPI SetHeaderTitleEx(
        /* [in] */ ICharSequence* title,
        /* [out] */ IContextMenu** contextMenu);

    /**
     * Sets the context menu header's icon to the icon given in <var>iconRes</var>
     * resource id.
     *
     * @param iconRes The resource identifier used for the icon.
     * @return This ContextMenu so additional setters can be called.
     */
    CARAPI SetHeaderIcon(
        /* [in] */ Int32 iconRes,
        /* [out] */ IContextMenu** contextMenu);

    /**
     * Sets the context menu header's icon to the icon given in <var>icon</var>
     * {@link Drawable}.
     *
     * @param icon The {@link Drawable} used for the icon.
     * @return This ContextMenu so additional setters can be called.
     */
    CARAPI SetHeaderIconEx(
        /* [in] */ IDrawable* icon,
        /* [out] */ IContextMenu** contextMenu);

    /**
     * Sets the header of the context menu to the {@link View} given in
     * <var>view</var>. This replaces the header title and icon (and those
     * replace this).
     *
     * @param view The {@link View} used for the header.
     * @return This ContextMenu so additional setters can be called.
     */
    CARAPI SetHeaderView(
        /* [in] */ IView* view,
        /* [out] */ IContextMenu** contextMenu);

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
    CARAPI Show(
        /* [in] */ IView* originalView,
        /* [in] */ IBinder* token,
        /* [out] */ CMenuDialogHelper** helper);

};

#endif  //__CONTEXTMENUBUILDER_H__
