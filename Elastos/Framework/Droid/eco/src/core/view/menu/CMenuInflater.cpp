
#include "ext/frameworkext.h"
#include "view/menu/CMenuInflater.h"
#include "utils/Xml.h"

const Int32 CMenuInflater::MenuState::mDefaultGroupId = CMenuInflater::NO_ID;
const Int32 CMenuInflater::MenuState::mDefaultItemId = CMenuInflater::NO_ID;
const Int32 CMenuInflater::MenuState::mDefaultItemCategory;
const Int32 CMenuInflater::MenuState::mDefaultItemOrder;
const Int32 CMenuInflater::MenuState::mDefaultItemCheckable;
const Boolean CMenuInflater::MenuState::mDefaultItemChecked;
const Boolean CMenuInflater::MenuState::mDefaultItemVisible;
const Boolean CMenuInflater::MenuState::mDefaultItemEnabled;

static Int32 R_Styleable_MenuGroup[] = {
    0x0101000e, 0x010100d0, 0x01010194, 0x010101de,
    0x010101df, 0x010101e0
};

static Int32 R_Styleable_MenuItem[] = {
    0x01010002, 0x0101000e, 0x010100d0, 0x01010106,
    0x01010194, 0x010101de, 0x010101df, 0x010101e1,
    0x010101e2, 0x010101e3, 0x010101e4, 0x010101e5
};

CMenuInflater::MenuState::MenuState(
    /* [in] */ IMenu* menu,
    /* [in] */ CMenuInflater* host)
    : mHost(host)
    , mMenu(menu)
{
    ResetGroup();
}

void CMenuInflater::MenuState::ResetGroup()
{
    mGroupId = mDefaultGroupId;
    mGroupCategory = mDefaultItemCategory;
    mGroupOrder = mDefaultItemOrder;
    mGroupCheckable = mDefaultItemCheckable;
    mGroupVisible = mDefaultItemVisible;
    mGroupEnabled = mDefaultItemEnabled;
}

void CMenuInflater::MenuState::ReadGroup(
	/* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    mHost->mContext->ObtainStyledAttributesEx2(attrs,
        ArrayOf<Int32>(R_Styleable_MenuGroup/*com.android.internal.R.styleable.MenuGroup*/,
        sizeof(R_Styleable_MenuGroup) / sizeof(Int32)),
        (ITypedArray**)&a);

    a->GetResourceId(1/*com.android.internal.R.styleable.MenuGroup_id*/, mDefaultGroupId, &mGroupId);
    a->GetInt32(3/*com.android.internal.R.styleable.MenuGroup_menuCategory*/, mDefaultItemCategory, &mGroupCategory);
    a->GetInt32(4/*com.android.internal.R.styleable.MenuGroup_orderInCategory*/, mDefaultItemOrder, &mGroupOrder);
    a->GetInt32(5/*com.android.internal.R.styleable.MenuGroup_checkableBehavior*/, mDefaultItemCheckable, &mGroupCheckable);
    a->GetBoolean(2/*com.android.internal.R.styleable.MenuGroup_visible*/, mDefaultItemVisible, &mGroupVisible);
    a->GetBoolean(0/*com.android.internal.R.styleable.MenuGroup_enabled*/, mDefaultItemEnabled, &mGroupEnabled);

    a->Recycle();
}

void CMenuInflater::MenuState::ReadItem(
	/* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    mHost->mContext->ObtainStyledAttributesEx2(attrs,
        ArrayOf<Int32>(R_Styleable_MenuItem/*com.android.internal.R.styleable.MenuItem*/,
        sizeof(R_Styleable_MenuItem) / sizeof(Int32)),
        (ITypedArray**)&a);

    // Inherit attributes from the group as default value
    a->GetResourceId(2/*com.android.internal.R.styleable.MenuItem_id*/, mDefaultItemId, &mItemId);
    Int32 category = 0;
    a->GetInt32(5/*com.android.internal.R.styleable.MenuItem_menuCategory*/, mGroupCategory, &category);
    Int32 order = 0;
    a->GetInt32(6/*com.android.internal.R.styleable.MenuItem_orderInCategory*/, mGroupOrder, &order);
    mItemCategoryOrder = (category & MENU_CATEGORY_MASK) | (order & MENU_USER_MASK);
    a->GetString(7/*com.android.internal.R.styleable.MenuItem_title*/, &mItemTitle);
    a->GetString(8/*com.android.internal.R.styleable.MenuItem_titleCondensed*/, &mItemTitleCondensed);
    a->GetResourceId(0/*com.android.internal.R.styleable.MenuItem_icon*/, 0, &mItemIconResId);
    String alphabeticShortcut, numericShortcut;
    a->GetString(9/*com.android.internal.R.styleable.MenuItem_alphabeticShortcut*/, &alphabeticShortcut);
    a->GetString(10/*com.android.internal.R.styleable.MenuItem_numericShortcut*/, &numericShortcut);
    mItemAlphabeticShortcut = GetShortcut(alphabeticShortcut);
    mItemNumericShortcut = GetShortcut(numericShortcut);

    Boolean has = FALSE;
    a->HasValue(11/*com.android.internal.R.styleable.MenuItem_checkable*/, &has);
    if (has) {
        // Item has attribute checkable, use it
        Boolean temp = FALSE;
        a->GetBoolean(11/*com.android.internal.R.styleable.MenuItem_checkable*/, FALSE, &temp);
        mItemCheckable = temp ? 1 : 0;
    }
    else {
        // Item does not have attribute, use the group's (group can have one more state
        // for checkable that represents the exclusive checkable)
        mItemCheckable = mGroupCheckable;
    }
    a->GetBoolean(3/*com.android.internal.R.styleable.MenuItem_checked*/, mDefaultItemChecked, &mItemChecked);
    a->GetBoolean(4/*com.android.internal.R.styleable.MenuItem_visible*/, mGroupVisible, &mItemVisible);
    a->GetBoolean(1/*com.android.internal.R.styleable.MenuItem_enabled*/, mGroupEnabled, &mItemEnabled);

    a->Recycle();

    mItemAdded = FALSE;
}

Char32 CMenuInflater::MenuState::GetShortcut(
    /* [in] */ const String& shortcutString)
{
    if (shortcutString.IsNull()) {
        return 0;
    }
    else {
        return shortcutString.GetChar(0);
    }
}

void CMenuInflater::MenuState::SetItem(
    /* [in] */ IMenuItem* item)
{
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(mItemTitleCondensed, (ICharSequence**)&csq);

    item->SetChecked(mItemChecked);
    item->SetVisible(mItemVisible);
    item->SetEnabled(mItemEnabled);
    item->SetCheckable(mItemCheckable >= 1);
    item->SetTitleCondensed(csq);
    item->SetIconEx(mItemIconResId);
    item->SetAlphabeticShortcut(mItemAlphabeticShortcut);
    item->SetNumericShortcut(mItemNumericShortcut);

    if (mItemCheckable >= 2) {
        IMenuItemImpl::Probe(item)->SetExclusiveCheckable(TRUE);
    }
}

void CMenuInflater::MenuState::AddItem()
{
    mItemAdded = TRUE;
    AutoPtr<IMenuItem> item;
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(mItemTitle, (ICharSequence**)&csq);
    mMenu->AddEx2(mGroupId, mItemId, mItemCategoryOrder, csq, (IMenuItem**)&item);
    SetItem(item);
}

AutoPtr<ISubMenu> CMenuInflater::MenuState::AddSubMenuItem()
{
    mItemAdded = TRUE;
    AutoPtr<ISubMenu> subMenu;
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(mItemTitle, (ICharSequence**)&csq);
    mMenu->AddSubMenuEx2(mGroupId, mItemId, mItemCategoryOrder, csq, (ISubMenu**)&subMenu);
    AutoPtr<IMenuItem> item;
    subMenu->GetItemEx((IMenuItem**)&item);
    SetItem(item);

    return subMenu;
}

Boolean CMenuInflater::MenuState::HasAddedItem()
{
    return mItemAdded;
}


const CString CMenuInflater::XML_MENU = "menu";
const CString CMenuInflater::XML_GROUP = "group";
const CString CMenuInflater::XML_ITEM = "item";
const Int32 CMenuInflater::NO_ID;

CMenuInflater::CMenuInflater()
{}

ECode CMenuInflater::constructor(
    /* [in] */ IContext* ctx)
{
    mContext = ctx;
    return NOERROR;
}

ECode CMenuInflater::Inflate(
    /* [in] */ Int32 menuRes,
    /* [in] */ IMenu* menu)
{
    AutoPtr<IXmlResourceParser> parser;
    //try {
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);
    assert(resources != NULL);
    resources->GetLayout(menuRes, (IXmlResourceParser**)&parser);
    assert(parser != NULL);
    AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);

    ParseMenu(parser, attrs, menu);
    //}
    // catch (XmlPullParserException e) {
    //     throw new InflateException("Error inflating menu XML", e);
    // } catch (IOException e) {
    //     throw new InflateException("Error inflating menu XML", e);
    // } finally {
    //     if (parser != null) parser.close();
    // }
    return NOERROR;
}

ECode CMenuInflater::ParseMenu(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ IMenu* menu)
{
    MenuState* menuState = new MenuState(menu, this);

    Int32 eventType = 0;
    parser->GetEventType(&eventType);
    String tagName;
    Boolean lookingForEndOfUnknownTag = FALSE;
    String unknownTagName;

    // This loop will skip to the menu start tag
    do {
        if (eventType == IXmlPullParser_START_TAG) {
            parser->GetName(&tagName);
            if (XML_MENU.Equals((const char*)tagName)) {
                // Go to next tag
                parser->Next(&eventType);
                break;
            }

            //throw new RuntimeException("Expecting menu, got " + tagName);
            if (menuState != NULL) delete menuState;
            return E_RUNTIME_EXCEPTION;
        }
        parser->Next(&eventType);
    } while (eventType != IXmlPullParser_END_DOCUMENT);

    Boolean reachedEndOfMenu = FALSE;
    while (!reachedEndOfMenu) {
        switch (eventType) {
            case IXmlPullParser_START_TAG:
                if (lookingForEndOfUnknownTag) {
                    break;
                }

                parser->GetName(&tagName);
                if (XML_GROUP.Equals((const char*)tagName)) {
                    menuState->ReadGroup(attrs);
                }
                else if (XML_ITEM.Equals((const char*)tagName)) {
                    menuState->ReadItem(attrs);
                }
                else if (XML_MENU.Equals((const char*)tagName)) {
                    // A menu start tag denotes a submenu for an item
                    AutoPtr<ISubMenu> subMenu = menuState->AddSubMenuItem();

                    // Parse the submenu into returned SubMenu
                    ParseMenu(parser, attrs, subMenu);
                }
                else {
                    lookingForEndOfUnknownTag = TRUE;
                    unknownTagName = tagName;
                }
                break;

            case IXmlPullParser_END_TAG:
                parser->GetName(&tagName);
                if (lookingForEndOfUnknownTag && tagName.Equals(unknownTagName)) {
                    lookingForEndOfUnknownTag = FALSE;
                    unknownTagName = NULL;
                }
                else if (XML_GROUP.Equals((const char*)tagName)) {
                    menuState->ResetGroup();
                }
                else if (XML_ITEM.Equals((const char*)tagName)) {
                    // Add the item if it hasn't been added (if the item was
                    // a submenu, it would have been added already)
                    if (!menuState->HasAddedItem()) {
                        menuState->AddItem();
                    }
                }
                else if (XML_MENU.Equals((const char*)tagName)) {
                    reachedEndOfMenu = TRUE;
                }
                break;

            case IXmlPullParser_END_DOCUMENT:
                //throw new RuntimeException("Unexpected end of document");
                if (menuState != NULL) delete menuState;
                return E_RUNTIME_EXCEPTION;
        }

        parser->Next(&eventType);
    }

    if (menuState != NULL) delete menuState;
    return NOERROR;
}
