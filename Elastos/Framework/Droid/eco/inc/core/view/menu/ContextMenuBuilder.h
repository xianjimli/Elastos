
#ifndef __CONTEXTMENUBUILDER_H__
#define __CONTEXTMENUBUILDER_H__

#include "view/menu/ContextMenuBuilderBase.h"
#include "view/MenuMacro.h"
#include "view/menu/MenuBuilderMacro.h"
#include "view/menu/MenuDialogHelper.h"
#include <elastos/ElRefBase.h>

/**
 * Implementation of the {@link android.view.ContextMenu} interface.
 * <p>
 * Most clients of the menu framework will never need to touch this
 * class.  However, if the client has a window that
 * is not a content view of a Dialog or Activity (for example, the
 * view was added directly to the window manager) and needs to show
 * context menus, it will use this class.
 * <p>
 * To use this class, instantiate it via {@link #ContextMenuBuilder(Context)},
 * and optionally populate it with any of your custom items.  Finally,
 * call {@link #show(View, IBinder)} which will populate the menu
 * with a view's context menu items and show the context menu.
 */
class ContextMenuBuilder
    : public ElRefBase
    , public ContextMenuBuilderBase
    , public IMenuBuilder
    , public IContextMenu
{
public:
    ContextMenuBuilder(
        /* [in] */ IContext* context);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    IMENU_METHODS_DECL();
    IMENUBUILDER_METHODS_DECL();

    /**
     * Sets the context menu header's icon to the icon given in <var>icon</var>
     * {@link Drawable}.
     *
     * @param icon The {@link Drawable} used for the icon.
     * @return This ContextMenu so additional setters can be called.
     */
    CARAPI SetHeaderIconEx(
        /* [in] */ IDrawable* icon);

    /**
     * Sets the context menu header's icon to the icon given in <var>iconRes</var>
     * resource id.
     *
     * @param iconRes The resource identifier used for the icon.
     * @return This ContextMenu so additional setters can be called.
     */
    CARAPI SetHeaderIcon(
        /* [in] */ Int32 iconRes);

    /**
     * Sets the context menu header's title to the title given in <var>title</var>.
     *
     * @param title The character sequence used for the title.
     * @return This ContextMenu so additional setters can be called.
     */
    CARAPI SetHeaderTitleEx(
        /* [in] */ ICharSequence* title);

    /**
     * Sets the context menu header's title to the title given in <var>titleRes</var>
     * resource identifier.
     *
     * @param titleRes The string resource identifier used for the title.
     * @return This ContextMenu so additional setters can be called.
     */
    CARAPI SetHeaderTitle(
        /* [in] */ Int32 titleRes);

    /**
     * Sets the header of the context menu to the {@link View} given in
     * <var>view</var>. This replaces the header title and icon (and those
     * replace this).
     *
     * @param view The {@link View} used for the header.
     * @return This ContextMenu so additional setters can be called.
     */
    CARAPI SetHeaderView(
        /* [in] */ IView* view);

    CARAPI ClearHeader();
};

#endif  //__CONTEXTMENUBUILDER_H__
