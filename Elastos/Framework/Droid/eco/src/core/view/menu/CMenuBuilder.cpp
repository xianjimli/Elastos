
#include "view/menu/CMenuBuilder.h"
#include "view/menu/MenuBuilderMacro.h"
#include <stdio.h>

IMENUBUILDER_METHODS_IMPL(CMenuBuilder, MenuBuilder, NULL);

CMenuBuilder::CMenuBuilder(
    /* [in] */ IContext* context): MenuBuilder(context) {
	printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
}

ECode CMenuBuilder::constructor(
    /*[in]*/ IContext* context) {
	printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
	mContext = context;

    mMenuTypes = new AutoPtr<IMenuBuilderType>[NUM_TYPES];
    memset(mMenuTypes, 0, sizeof(IMenuBuilderType*) * NUM_TYPES);

    mContext = context;
    context->GetResources(&mResources);

    mItems = new List<AutoPtr<IMenuItem> >();

    mVisibleItems = new List<AutoPtr<IMenuItem> >();
    mIsVisibleItemsStale = TRUE;

    mPreventDispatchingItemsChanged = FALSE;
    mOptionalIconsVisible = FALSE;
    mHeaderTitle = NULL;

    AutoPtr<IConfiguration> config;
    mResources->GetConfiguration((IConfiguration**)&config);

    if (config != NULL) {
        //TODO mShortcutsVisible = config->keyboard != Configuration.KEYBOARD_NOKEYS);
        mShortcutsVisible = TRUE;
    }

	return NOERROR;
}

CMenuBuilder::CMenuBuilder(){
	printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
}

PInterface CMenuBuilder::Probe(
    /* [in] */ REIID riid) {
	return _CMenuBuilder::Probe(riid);
}
