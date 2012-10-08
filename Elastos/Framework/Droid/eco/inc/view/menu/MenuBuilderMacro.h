
#ifndef __MENUBUILDERMACRO_H__
#define __MENUBUILDERMACRO_H__


#define IMENUBUILDER_METHODS_DECL()                                     \
    CARAPI GetMenuView(													\
        /* [in] */ Int32 menuType,										\
        /* [in] */ IViewGroup* parent,									\
        /* [out] */ IMenuView** menuView);								\
    																	\
    CARAPI ClearMenuViews();											\
    																	\
    CARAPI Add(															\
        /* [in] */ ICharSequence* title,								\
        /* [out] */ IMenuItem** item);									\
    																	\
    CARAPI AddEx(														\
        /* [in] */ Int32 titleRes,										\
        /* [out] */ IMenuItem** item);									\
    																	\
    CARAPI AddEx2(														\
        /* [in] */ Int32 groupId,										\
        /* [in] */ Int32 itemId,										\
        /* [in] */ Int32 order,											\
        /* [in] */ ICharSequence* title,									\
        /* [out] */ IMenuItem** item);									\
    																	\
    CARAPI AddEx3(														\
        /* [in] */ Int32 groupId,										\
        /* [in] */ Int32 itemId,										\
        /* [in] */ Int32 order,											\
        /* [in] */ Int32 titleRes,										\
        /* [out] */ IMenuItem** item);									\
    																	\
    CARAPI AddSubMenu(													\
        /* [in] */ ICharSequence* title,									\
        /* [out] */ ISubMenu** subMenu);								\
    																	\
    CARAPI AddSubMenuEx(												\
        /* [in] */ Int32 titleRes,										\
        /* [out] */ ISubMenu** subMenu);								\
    																	\
    CARAPI AddSubMenuEx2(												\
        /* [in] */ Int32 groupId,										\
        /* [in] */ Int32 itemId,										\
        /* [in] */ Int32 order,											\
        /* [in] */ ICharSequence* title,									\
        /* [out] */ ISubMenu** subMenu);								\
    																	\
    CARAPI AddSubMenuEx3(												\
        /* [in] */ Int32 groupId,										\
        /* [in] */ Int32 itemId,										\
        /* [in] */ Int32 order,											\
        /* [in] */ Int32 titleRes,										\
        /* [out] */ ISubMenu** subMenu);								\
    																	\
    CARAPI AddIntentOptions(											\
        /* [in] */ Int32 groupId,										\
        /* [in] */ Int32 itemId,										\
        /* [in] */ Int32 order,											\
        /* [in] */ IComponentName* caller,								\
        /* [in] */ const ArrayOf<IIntent*>& specifics,					\
        /* [in] */ IIntent* intent,										\
        /* [in] */ Int32 flags,											\
        /* [in] */ const ArrayOf<IMenuItem*>& outSpecificItems,			\
        /* [out] */ Int32* count);										\
    																	\
    CARAPI RemoveItem(													\
        /* [in] */ Int32 id);											\
    																	\
    CARAPI RemoveGroup(													\
        /* [in] */ Int32 groupId);										\
    																	\
    CARAPI Clear();														\
    																	\
    CARAPI SetGroupCheckable(											\
        /* [in] */ Int32 group,											\
        /* [in] */ Boolean checkable,									\
        /* [in] */ Boolean exclusive);									\
    																	\
    CARAPI SetGroupVisible(												\
        /* [in] */ Int32 group,											\
        /* [in] */ Boolean visible);									\
    																	\
    CARAPI SetGroupEnabled(												\
        /* [in] */ Int32 group,											\
        /* [in] */ Boolean enabled);									\
    																	\
    CARAPI HasVisibleItems(												\
        /* [out] */ Boolean* result);									\
    																	\
    CARAPI FindItem(													\
        /* [in] */ Int32 id,											\
        /* [out] */ IMenuItem** item);									\
    																	\
    CARAPI FindItemIndex(												\
        /* [in] */ Int32 id,											\
        /* [out] */ Int32* index);										\
    																	\
    CARAPI FindGroupIndex(												\
        /* [in] */ Int32 group,											\
        /* [out] */ Int32* index);										\
    																	\
    CARAPI FindGroupIndexEx(											\
        /* [in] */ Int32 group,											\
        /* [in] */ Int32 start,											\
        /* [out] */ Int32* index);										\
    																	\
    CARAPI Size(														\
        /* [out] */ Int32* size);										\
    																	\
    CARAPI GetItem(														\
        /* [in] */ Int32 index,											\
        /* [out] */ IMenuItem** item);									\
    																	\
    CARAPI IsShortcutKey(												\
        /* [in] */ Int32 keyCode,										\
        /* [in] */ IKeyEvent* event,									\
        /* [out] */ Boolean* isShortcutKey);							\
    																	\
    CARAPI PerformIdentifierAction(										\
        /* [in] */ Int32 id,											\
        /* [in] */ Int32 flags,											\
        /* [out] */ Boolean* isShown);									\
    																	\
    CARAPI SetQwertyMode(												\
        /* [in] */ Boolean isQwerty);									\
    																	\
    CARAPI IsQwertyMode(												\
        /* [out] */ Boolean* qwertyMode);								\
    																	\
    CARAPI SetShortcutsVisible(											\
        /* [in] */ Boolean shortcutsVisible);							\
    																	\
    CARAPI IsShortcutsVisible(											\
        /* [out] */ Boolean* qwertyMode);								\
    																	\
    CARAPI GetResources(												\
        /* [out] */ IResources** resource);								\
    																	\
    CARAPI GetCallback(													\
        /* [out] */ IMenuBuilderCallback** callback);							\
    																	\
    CARAPI GetContext(													\
        /* [out] */ IContext** context);								\
    																	\
    CARAPI PerformShortcut(												\
        /* [in] */ Int32 keyCode,										\
        /* [in] */ IKeyEvent* event,									\
        /* [in] */ Int32 flags,											\
        /* [out] */ Boolean* handled);									\
    																	\
    CARAPI FindItemsWithShortcutForKey(									\
        /* [in] */ Int32 keyCode,										\
        /* [in] */ IKeyEvent* event,									\
        /* [out] */ IObjectContainer** list);							\
    																	\
    CARAPI FindItemWithShortcutForKey(									\
        /* [in] */ Int32 keyCode,										\
        /* [in] */ IKeyEvent* event,									\
        /* [out] */ IMenuItem** menuItem);								\
    																	\
    CARAPI PerformItemAction(											\
        /* [in] */ IMenuItem* item,										\
        /* [in] */ Int32 flags,											\
        /* [out] */ Boolean* state);									\
    																	\
    CARAPI CloseEx(														\
        /* [in] */ Boolean allMenusAreClosing);							\
    																	\
    CARAPI Close();														\
    																	\
    CARAPI OnItemVisibleChanged(										\
        /* [in] */ IMenuItem* item);									\
    																	\
    CARAPI GetVisibleItems(												\
        /* [out] */ IObjectContainer** list);							\
    																	\
    CARAPI ClearHeader();												\
    																	\
    CARAPI GetHeaderTitle(												\
        /* [out] */ ICharSequence** title);										\
    																	\
    CARAPI GetHeaderIcon(												\
        /* [out] */ IDrawable** drawable);								\
    																	\
    CARAPI GetHeaderView(												\
        /* [out] */ IView** headerView);								\
    																	\
    CARAPI GetRootMenu(													\
        /* [out] */ IMenuBuilder** menu);										\
    																	\
    CARAPI SetCurrentMenuInfo(											\
        /* [in] */ IContextMenuInfo* menuInfo);							\
																		\
    CARAPI GetMenuAdapter(												\
        /* [in] */ Int32 menuType,										\
        /* [out] */ IMenuAdapter** menuAdapter);						\
																		\
    CARAPI SetOptionalIconsVisible(										\
        /* [in] */ Boolean visible);									\
    																	\
    CARAPI GetOptionalIconsVisible(										\
        /* [out] */ Boolean* visible);									\
																		\
    CARAPI SaveHierarchyState(											\
        /* [in] */ IBundle* outState);									\
																		\
    CARAPI RestoreHierarchyState(										\
        /* [in] */ IBundle* inState);									\
    																	\
    CARAPI SetExclusiveItemChecked(										\
        /* [in] */ IMenuItem* item);									\
																		\
    CARAPI SetHeaderTitleInt(											\
        /* [in] */ ICharSequence* title,								\
        /* [out] */ IMenuBuilder** menu);								\
																		\
    CARAPI SetHeaderTitleIntEx(											\
        /* [in] */ Int32 titleRes,										\
        /* [out] */ IMenuBuilder** menu);								\
																		\
    CARAPI SetHeaderIconInt(											\
        /* [in] */ IDrawable* icon,										\
        /* [out] */ IMenuBuilder** menu);								\
																		\
    CARAPI SetHeaderIconIntEx(											\
        /* [in] */ Int32 iconRes,										\
        /* [out] */ IMenuBuilder** menu);								\
																		\
    CARAPI SetHeaderViewInt(											\
        /* [in] */ IView* view,											\
        /* [out] */ IMenuBuilder** menu);								\
																		\
    CARAPI GetNumIconMenuItemsShown(									\
        /* [out] */ Int32* state);										\
																		\
    CARAPI GetMenuType(													\
        /* [in] */ Int32 menuType,										\
        /* [out] */ IMenuBuilderType** type);									\
																		\
    CARAPI RefreshShortcuts(											\
        /* [in] */ Boolean shortcutsVisible,							\
        /* [in] */ Boolean qwertyMode);									\
																		\
    CARAPI SetCallback(													\
        /* [in] */ IMenuBuilderCallback* callback);							\
																		\
    CARAPI SetHeaderInternal(											\
        /* [in] */ Int32 titleRes,										\
        /* [in] */ ICharSequence* title,									\
        /* [in] */ Int32 iconRes,										\
        /* [in] */ IDrawable* icon,										\
        /* [in] */ IView* view);										\
    																	\
    CARAPI ClearAll();


#define IMENUBUILDER_METHODS_IMPL(className, superClass, overRideClass) \
    CARAPI className::GetMenuView(										\
        /* [in] */ Int32 menuType,										\
        /* [in] */ IViewGroup* parent,									\
        /* [out] */ IMenuView** menuView) {								\
    	return superClass::GetMenuView(menuType, parent, menuView);		\
	}																	\
																		\
    CARAPI className::ClearMenuViews() {								\
    	return superClass::ClearMenuViews();							\
    }																	\
																		\
    CARAPI className::Add(												\
        /* [in] */ ICharSequence* title,								\
        /* [out] */ IMenuItem** item) {									\
    	return superClass::Add(title, item);							\
	}																	\
																		\
    CARAPI className::AddEx(											\
        /* [in] */ Int32 titleRes,										\
        /* [out] */ IMenuItem** item) {									\
    	return superClass::Add(titleRes, item);				    		\
	}																	\
																		\
    CARAPI className::AddEx2(											\
        /* [in] */ Int32 groupId,										\
        /* [in] */ Int32 itemId,										\
        /* [in] */ Int32 order,											\
        /* [in] */ ICharSequence* title,									\
        /* [out] */ IMenuItem** item) {									\
    	return superClass::Add(groupId, itemId, order, title, item); \
	}																	\
																		\
    CARAPI className::AddEx3(											\
        /* [in] */ Int32 groupId,										\
        /* [in] */ Int32 itemId,										\
        /* [in] */ Int32 order,											\
        /* [in] */ Int32 titleRes,										\
        /* [out] */ IMenuItem** item) {									\
    	return superClass::Add(groupId, itemId, order, titleRes, item);\
	}																	\
																		\
    CARAPI className::AddSubMenu(										\
        /* [in] */ ICharSequence* title,									\
        /* [out] */ ISubMenu** subMenu) {								\
    	return superClass::AddSubMenu(title, subMenu);					\
	}																	\
																		\
    CARAPI className::AddSubMenuEx(										\
        /* [in] */ Int32 titleRes,										\
        /* [out] */ ISubMenu** subMenu) {								\
    	return superClass::AddSubMenu(titleRes, subMenu);				\
	}																	\
																		\
    CARAPI className::AddSubMenuEx2(									\
        /* [in] */ Int32 groupId,										\
        /* [in] */ Int32 itemId,										\
        /* [in] */ Int32 order,											\
        /* [in] */ ICharSequence* title,									\
        /* [out] */ ISubMenu** subMenu) {								\
    	return superClass::AddSubMenu(groupId, itemId, order, title, subMenu);	\
	}																	\
																		\
    CARAPI className::AddSubMenuEx3(									\
        /* [in] */ Int32 groupId,										\
        /* [in] */ Int32 itemId,										\
        /* [in] */ Int32 order,											\
        /* [in] */ Int32 titleRes,										\
        /* [out] */ ISubMenu** subMenu) {								\
    	return superClass::AddSubMenu(groupId, itemId, order, titleRes, subMenu);	\
	}																	\
																		\
    CARAPI className::AddIntentOptions(									\
        /* [in] */ Int32 groupId,										\
        /* [in] */ Int32 itemId,										\
        /* [in] */ Int32 order,											\
        /* [in] */ IComponentName* caller,								\
        /* [in] */ const ArrayOf<IIntent*>& specifics,					\
        /* [in] */ IIntent* intent,										\
        /* [in] */ Int32 flags,											\
        /* [in] */ const ArrayOf<IMenuItem*>& outSpecificItems,			\
        /* [out] */ Int32* count) {										\
    	return superClass::AddIntentOptions(groupId, itemId, order, caller, specifics    \
    		, intent, flags, outSpecificItems, count);					\
	}																	\
																		\
    CARAPI className::RemoveItem(										\
        /* [in] */ Int32 id) {											\
    	return superClass::RemoveItem(id);								\
	}																	\
																		\
    CARAPI className::RemoveGroup(										\
        /* [in] */ Int32 groupId) {										\
    	return superClass::RemoveGroup(groupId);						\
	}																	\
																		\
    CARAPI className::Clear() {											\
    	return superClass::Clear();										\
    }																	\
																		\
    CARAPI className::SetGroupCheckable(								\
        /* [in] */ Int32 group,											\
        /* [in] */ Boolean checkable,									\
        /* [in] */ Boolean exclusive) {									\
    	return superClass::SetGroupCheckable(group, checkable, exclusive);   \
	}																	\
																		\
    CARAPI className::SetGroupVisible(									\
        /* [in] */ Int32 group,											\
        /* [in] */ Boolean visible) {									\
    	return superClass::SetGroupVisible(group, visible);				\
	}																	\
																		\
    CARAPI className::SetGroupEnabled(									\
        /* [in] */ Int32 group,											\
        /* [in] */ Boolean enabled) {									\
    	return superClass::SetGroupEnabled(group, enabled);				\
	}																	\
																		\
    CARAPI className::HasVisibleItems(									\
        /* [out] */ Boolean* result) {									\
    	return superClass::HasVisibleItems(result);						\
	}																	\
																		\
    CARAPI className::FindItem(											\
        /* [in] */ Int32 id,											\
        /* [out] */ IMenuItem** item) {									\
    	return superClass::FindItem(id, item);							\
	}																	\
																		\
    CARAPI className::FindItemIndex(									\
        /* [in] */ Int32 id,											\
        /* [out] */ Int32* index) {										\
    	return superClass::FindItemIndex(id, index);					\
	}																	\
																		\
    CARAPI className::FindGroupIndex(									\
        /* [in] */ Int32 group,											\
        /* [out] */ Int32* index) {										\
    	return superClass::FindGroupIndex(group, index);				\
	}																	\
																		\
    CARAPI className::FindGroupIndexEx(									\
        /* [in] */ Int32 group,											\
        /* [in] */ Int32 start,											\
        /* [out] */ Int32* index) {										\
    	return superClass::FindGroupIndex(group, start, index);		    \
	}																	\
																		\
    CARAPI className::Size(												\
        /* [out] */ Int32* size) {										\
    	return superClass::Size(size);									\
	}																	\
																		\
    CARAPI className::GetItem(											\
        /* [in] */ Int32 index,											\
        /* [out] */ IMenuItem** item) {									\
    	return superClass::GetItem(index, item);						\
	}																	\
																		\
    CARAPI className::IsShortcutKey(									\
        /* [in] */ Int32 keyCode,										\
        /* [in] */ IKeyEvent* event,									\
        /* [out] */ Boolean* isShortcutKey) {							\
    	return superClass::IsShortcutKey(keyCode, event, isShortcutKey);	\
	}																	\
																		\
    CARAPI className::PerformIdentifierAction(							\
        /* [in] */ Int32 id,											\
        /* [in] */ Int32 flags,											\
        /* [out] */ Boolean* isShown) {									\
    	return superClass::PerformIdentifierAction(id, flags, isShown);	\
	}																	\
																		\
    CARAPI className::SetQwertyMode(									\
        /* [in] */ Boolean isQwerty) {									\
    	return superClass::SetQwertyMode(isQwerty);						\
	}																	\
																		\
    CARAPI className::IsQwertyMode(										\
        /* [out] */ Boolean* qwertyMode) {								\
    	return superClass::IsQwertyMode(qwertyMode);					\
	}																	\
																		\
    CARAPI className::SetShortcutsVisible(								\
        /* [in] */ Boolean shortcutsVisible) {							\
    	return superClass::SetShortcutsVisible(shortcutsVisible);		\
	}																	\
																		\
    CARAPI className::IsShortcutsVisible(								\
        /* [out] */ Boolean* qwertyMode) {								\
    	return superClass::IsShortcutsVisible(qwertyMode);				\
	}																	\
																		\
    CARAPI className::GetResources(										\
        /* [out] */ IResources** resource) {							\
    	return superClass::GetResources(resource);						\
	}																	\
																		\
    CARAPI className::GetCallback(										\
        /* [out] */ IMenuBuilderCallback** callback) {					\
    	return superClass::GetCallback(callback);						\
	}																	\
																		\
    CARAPI className::GetContext(										\
        /* [out] */ IContext** context) {								\
    	return superClass::GetContext(context);							\
	}																	\
																		\
    CARAPI className::PerformShortcut(									\
        /* [in] */ Int32 keyCode,										\
        /* [in] */ IKeyEvent* event,									\
        /* [in] */ Int32 flags,											\
        /* [out] */ Boolean* handled) {									\
    	return superClass::PerformShortcut(keyCode, event, flags, handled);   \
	}																	\
																		\
    CARAPI className::FindItemsWithShortcutForKey(						\
        /* [in] */ Int32 keyCode,										\
        /* [in] */ IKeyEvent* event,									\
        /* [out] */ IObjectContainer** list) {							\
    	return superClass::FindItemsWithShortcutForKey(keyCode, event, list);   \
	}																	\
																		\
    CARAPI className::FindItemWithShortcutForKey(						\
        /* [in] */ Int32 keyCode,										\
        /* [in] */ IKeyEvent* event,									\
        /* [out] */ IMenuItem** menuItem) {								\
    	return superClass::FindItemWithShortcutForKey(keyCode, event, menuItem);  \
	}																	\
																		\
    CARAPI className::PerformItemAction(								\
        /* [in] */ IMenuItem* item,										\
        /* [in] */ Int32 flags,											\
        /* [out] */ Boolean* state) {									\
    	return superClass::PerformItemAction(item, flags, state);		\
	}																	\
																		\
    CARAPI className::CloseEx(											\
        /* [in] */ Boolean allMenusAreClosing) {						\
    	return superClass::CloseEx(allMenusAreClosing);					\
	}																	\
																		\
    CARAPI className::Close() {											\
    	return superClass::Close();										\
    }																	\
																		\
    CARAPI className::OnItemVisibleChanged(								\
        /* [in] */ IMenuItem* item) {									\
    	return superClass::OnItemVisibleChanged(item);					\
	}																	\
																		\
    CARAPI className::GetVisibleItems(									\
        /* [out] */ IObjectContainer** list) {							\
    	return superClass::GetVisibleItems(list);						\
	}																	\
																		\
    CARAPI className::ClearHeader() {									\
    	return superClass::ClearHeader();								\
    }																	\
																		\
    CARAPI className::GetHeaderTitle(									\
        /* [out] */ ICharSequence** title) {									\
    	return superClass::GetHeaderTitle(title);						\
	}																	\
																		\
    CARAPI className::GetHeaderIcon(									\
        /* [out] */ IDrawable** drawable) {								\
    	return superClass::GetHeaderIcon(drawable);						\
	}																	\
																		\
    CARAPI className::GetHeaderView(									\
        /* [out] */ IView** headerView) {								\
    	return superClass::GetHeaderView(headerView);					\
	}																	\
																		\
    CARAPI className::GetRootMenu(										\
        /* [out] */ IMenuBuilder** menu) {										\
    	return superClass::GetRootMenu(menu);							\
	}																	\
																		\
    CARAPI className::SetCurrentMenuInfo(								\
        /* [in] */ IContextMenuInfo* menuInfo) {						\
    	return superClass::SetCurrentMenuInfo(menuInfo);				\
	}																	\
																		\
    CARAPI className::GetMenuAdapter(									\
        /* [in] */ Int32 menuType,										\
        /* [out] */ IMenuAdapter** menuAdapter) {						\
    	return superClass::GetMenuAdapter(menuType, menuAdapter);		\
	}																	\
																		\
    CARAPI className::SetOptionalIconsVisible(							\
        /* [in] */ Boolean visible) {									\
    	return superClass::SetOptionalIconsVisible(visible);			\
	}																	\
																		\
    CARAPI className::GetOptionalIconsVisible(							\
        /* [out] */ Boolean* visible) {									\
    	return superClass::GetOptionalIconsVisible(visible);			\
	}																	\
																		\
    CARAPI className::SaveHierarchyState(								\
        /* [in] */ IBundle* outState) {									\
    	return superClass::SaveHierarchyState(outState);				\
	}																	\
																		\
    CARAPI className::RestoreHierarchyState(							\
        /* [in] */ IBundle* inState) {									\
    	return superClass::RestoreHierarchyState(inState);				\
	}																	\
																		\
    CARAPI className::SetExclusiveItemChecked(							\
        /* [in] */ IMenuItem* item) {									\
    	return superClass::SetExclusiveItemChecked(item);				\
	}																	\
																		\
    CARAPI className::SetHeaderTitleInt(								\
        /* [in] */ ICharSequence* title,								\
        /* [out] */ IMenuBuilder** menu) {								\
    	return superClass::SetHeaderTitleInt(title, menu);				\
	}																	\
																		\
    CARAPI className::SetHeaderTitleIntEx(								\
        /* [in] */ Int32 titleRes,										\
        /* [out] */ IMenuBuilder** menu) {								\
    	return superClass::SetHeaderTitleInt(titleRes, menu);			\
	}																	\
																		\
    CARAPI className::SetHeaderIconInt(									\
        /* [in] */ IDrawable* icon,										\
        /* [out] */ IMenuBuilder** menu) {								\
    	return superClass::SetHeaderIconInt(icon, menu);				\
	}																	\
																		\
    CARAPI className::SetHeaderIconIntEx(								\
        /* [in] */ Int32 iconRes,										\
        /* [out] */ IMenuBuilder** menu) {								\
    	return superClass::SetHeaderIconInt(iconRes, menu);		      	\
	}																	\
																		\
    CARAPI className::SetHeaderViewInt(									\
        /* [in] */ IView* view,											\
        /* [out] */ IMenuBuilder** menu) {								\
    	return superClass::SetHeaderViewInt(view, menu);				\
	}																	\
																		\
    CARAPI className::GetNumIconMenuItemsShown(							\
        /* [out] */ Int32* state) {										\
    	return superClass::GetNumIconMenuItemsShown(state);				\
	}																	\
																		\
    CARAPI className::GetMenuType(													\
        /* [in] */ Int32 menuType,										\
        /* [out] */ IMenuBuilderType** type) {									\
    	return superClass::GetMenuType(menuType, type);					\
	}																	\
																		\
    CARAPI className::RefreshShortcuts(									\
        /* [in] */ Boolean shortcutsVisible,							\
        /* [in] */ Boolean qwertyMode) {								\
    	return superClass::RefreshShortcuts(shortcutsVisible, qwertyMode);  \
	}																	\
																		\
    CARAPI className::SetCallback(										\
        /* [in] */ IMenuBuilderCallback* callback) {							\
    	return superClass::SetCallback(callback);						\
	}																	\
																		\
    CARAPI className::SetHeaderInternal(								\
        /* [in] */ Int32 titleRes,										\
        /* [in] */ ICharSequence* title,									\
        /* [in] */ Int32 iconRes,										\
        /* [in] */ IDrawable* icon,										\
        /* [in] */ IView* view) {										\
    	return superClass::SetHeaderInternal(titleRes, title, iconRes, icon, view); \
	}																	\
																		\
	CARAPI className::ClearAll() {										\
		return superClass::ClearAll();									\
	}

#endif //__MENUBUILDERMACRO_H__