
#include "widget/TabHost.h"
#include "widget/CTabWidget.h"
#include "view/CViewGroupLayoutParams.h"
#include "content/CApplicationInfo.h"
#include "os/Build.h"


// {68D263F8-0F1F-4AE6-BBD7-998BF0B1358A}
const InterfaceID EIID_ViewIndicatorStrategy =
{ 0x68d263f8, 0xf1f, 0x4ae6, { 0xbb, 0xd7, 0x99, 0x8b, 0xf0, 0xb1, 0x35, 0x8a } };


TabHost::TabSpec::TabSpec(
    /* [in] */ const String& tag,
    /* [in] */ TabHost* owner)
    : mTag(tag)
    , mOwner(owner)
{
}

PInterface TabHost::TabSpec::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_ITabSpec) {
        return (ITabSpec*)this;
    }

    return NULL;
}

UInt32 TabHost::TabSpec::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TabHost::TabSpec::Release()
{
    return ElRefBase::Release();
}

ECode TabHost::TabSpec::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) return E_ILLEGAL_ARGUMENT_EXCEPTION;

    if (pObject == (IInterface*)this) {
        *pIID = EIID_ITabSpec;
        return NOERROR;
    }

    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

/**
 * Specify a label as the tab indicator.
 */
ECode TabHost::TabSpec::SetIndicator(
    /* [in] */ ICharSequence* label)
{
    mIndicatorStrategy = new TabHost::LabelIndicatorStrategy(label, mOwner);
    return NOERROR;
}

/**
 * Specify a label and icon as the tab indicator.
 */
ECode TabHost::TabSpec::SetIndicatorEx(
    /* [in] */ ICharSequence* label,
    /* [in] */ IDrawable* icon)
{
    mIndicatorStrategy = new TabHost::LabelAndIconIndicatorStrategy(label, icon, mOwner);
    return NOERROR;
}

/**
 * Specify a view as the tab indicator.
 */
ECode TabHost::TabSpec::SetIndicatorEx2(
    /* [in] */ IView* view)
{
    mIndicatorStrategy = new TabHost::ViewIndicatorStrategy(view);
    return NOERROR;
}

/**
 * Specify the id of the view that should be used as the content
 * of the tab.
 */
ECode TabHost::TabSpec::SetContent(
    /* [in] */ Int32 viewId)
{
    mContentStrategy = new TabHost::ViewIdContentStrategy(viewId, mOwner);
    return NOERROR;
}

/**
 * Specify a {@link android.widget.TabHost.TabContentFactory} to use to
 * create the content of the tab.
 */
ECode TabHost::TabSpec::SetContentEx(
    /* [in] */ ITabHostTabContentFactory* contentFactory)
{
    AutoPtr<ICharSequence> tag;
    CStringWrapper::New(mTag, (ICharSequence**)&tag);
    mContentStrategy = new TabHost::FactoryContentStrategy(tag, contentFactory);
    return NOERROR;
}

/**
 * Specify an intent to use to launch an activity as the tab content.
 */
ECode TabHost::TabSpec::SetContentEx2(
    /* [in] */ IIntent* intent)
{
    mContentStrategy = new TabHost::IntentContentStrategy(mTag, intent);
    return NOERROR;
}


ECode TabHost::TabSpec::GetTag(
    /* [out] */ String* tag)
{
    VALIDATE_NOT_NULL(tag);
    *tag = mTag;
    return NOERROR;
}


TabHost::LabelIndicatorStrategy::LabelIndicatorStrategy(
    /* [in] */ ICharSequence* label,
    /* [in] */ TabHost* owner)
    : mLabel(label)
    , mOwner(owner)
{}

PInterface TabHost::LabelIndicatorStrategy::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 TabHost::LabelIndicatorStrategy::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TabHost::LabelIndicatorStrategy::Release()
{
    return ElRefBase::Release();
}

ECode TabHost::LabelIndicatorStrategy::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode TabHost::LabelIndicatorStrategy::CreateIndicatorView(
    /* [out] */ IView** view)
{
    AutoPtr<IContext> context = mOwner->GetContext();
    AutoPtr<ILayoutInflater> inflater;
    context->GetSystemService(Context_LAYOUT_INFLATER_SERVICE, (IInterface**)&inflater);

    AutoPtr<IView> tabIndicator;
    inflater->InflateEx2(0x01090063/*R.layout.tab_indicator*/,
           mOwner->mTabWidget, // tab widget is the parent
           FALSE,
           (IView**)&tabIndicator); // no inflate params

    AutoPtr<IView> temp;
    tabIndicator->FindViewById(/*R.id.title*/0x01020016, (IView**)&temp);
    AutoPtr<ITextView> tv = ITextView::Probe(temp.Get());
    tv->SetText(mLabel);
    AutoPtr<IApplicationInfo> ai;
    context->GetApplicationInfo((IApplicationInfo**)&ai);
    if (((CApplicationInfo*)ai.Get())->mTargetSdkVersion <= Build::VERSION_CODES::DONUT) {
        // Donut apps get old color scheme
        tabIndicator->SetBackgroundResource(0x0108030a/*R.drawable.tab_indicator_v4*/);
        AutoPtr<IResources> resources;
        context->GetResources((IResources**)&resources);
        AutoPtr<IColorStateList> sl;
        resources->GetColorStateList(
            0x01060038/*R.color.tab_indicator_text_v4*/, (IColorStateList**)&sl);
        tv->SetTextColorEx(sl);
    }
    *view = tabIndicator.Get();
    if (*view) {
        (*view)->AddRef();
    }

    return NOERROR;
}

TabHost::LabelAndIconIndicatorStrategy::LabelAndIconIndicatorStrategy(
    /* [in] */ ICharSequence* label,
    /* [in] */ IDrawable* icon,
    /* [in] */ TabHost* owner)
    : mLabel(label)
    , mIcon(icon)
    , mOwner(owner)
{}

PInterface TabHost::LabelAndIconIndicatorStrategy::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 TabHost::LabelAndIconIndicatorStrategy::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TabHost::LabelAndIconIndicatorStrategy::Release()
{
    return ElRefBase::Release();
}

ECode TabHost::LabelAndIconIndicatorStrategy::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode TabHost::LabelAndIconIndicatorStrategy::CreateIndicatorView(
    /* [out] */ IView** view)
{
    AutoPtr<IContext> context = mOwner->GetContext();

    AutoPtr<ILayoutInflater> inflater;
    context->GetSystemService(Context_LAYOUT_INFLATER_SERVICE, (IInterface**)&inflater);
    AutoPtr<IView> tabIndicator;
    inflater->InflateEx2(0x01090063/*R.layout.tab_indicator*/,
           mOwner->mTabWidget, // tab widget is the parent
           FALSE, (IView**)&tabIndicator); // no inflate params

    AutoPtr<IView> temp;
    tabIndicator->FindViewById(/*R.id.title*/0x01020016, (IView**)&temp);
    assert(temp != NULL);
    AutoPtr<ITextView> tv = ITextView::Probe(temp.Get());
    tv->SetText(mLabel);

    temp = NULL;
    tabIndicator->FindViewById(/*R.id.icon*/0x01010002, (IView**)&temp);
    AutoPtr<IImageView> iconView = IImageView::Probe(temp.Get());
    iconView->SetImageDrawable(mIcon);

    AutoPtr<IApplicationInfo> ai;
    context->GetApplicationInfo((IApplicationInfo**)&ai);
    if (((CApplicationInfo*)ai.Get())->mTargetSdkVersion <= Build::VERSION_CODES::DONUT) {
        // Donut apps get old color scheme
        tabIndicator->SetBackgroundResource(0x0108030a/*R.drawable.tab_indicator_v4*/);

        AutoPtr<IResources> resources;
        context->GetResources((IResources**)&resources);

        AutoPtr<IColorStateList> sl;
        resources->GetColorStateList(
            0x01060038/*R.color.tab_indicator_text_v4*/, (IColorStateList**)&sl);

        tv->SetTextColorEx(sl);
    }

    *view = tabIndicator.Get();
    if (*view) {
        (*view)->AddRef();
    }

    return NOERROR;
}



TabHost::ViewIndicatorStrategy::ViewIndicatorStrategy(
    /* [in] */ IView* view)
    : mView(view)
{}

PInterface TabHost::ViewIndicatorStrategy::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_ViewIndicatorStrategy) {
    	return reinterpret_cast<PInterface>(this);
    }

    return NULL;
}

UInt32 TabHost::ViewIndicatorStrategy::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TabHost::ViewIndicatorStrategy::Release()
{
    return ElRefBase::Release();
}

ECode TabHost::ViewIndicatorStrategy::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode TabHost::ViewIndicatorStrategy::CreateIndicatorView(
    /* [out] */ IView** view)
{
    *view = mView;
    if (*view != NULL) (*view)->AddRef();

    return NOERROR;
}


TabHost::ViewIdContentStrategy::ViewIdContentStrategy(
    /* [in] */ Int32 viewId,
    /* [in] */ TabHost* owner)
    : mOwner(owner)
{
    mOwner->mTabContent->FindViewById(viewId, (IView**)&mView);
    if (mView != NULL) {
        mView->SetVisibility(View::GONE);
    }
    else {
        /*throw new RuntimeException("Could not create tab content because " +
                "could not find view with id " + viewId);*/
        assert(0);
    }
}

PInterface TabHost::ViewIdContentStrategy::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 TabHost::ViewIdContentStrategy::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TabHost::ViewIdContentStrategy::Release()
{
    return ElRefBase::Release();
}

ECode TabHost::ViewIdContentStrategy::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode TabHost::ViewIdContentStrategy::GetContentView(
    /* [out] */ IView** view)
{
    mView->SetVisibility(View::VISIBLE);
    *view = mView;
    (*view)->AddRef();
    return NOERROR;
}

ECode TabHost::ViewIdContentStrategy::TabClosed()
{
    return mView->SetVisibility(View::GONE);
}



TabHost::FactoryContentStrategy::FactoryContentStrategy(
    /* [in] */ ICharSequence* tag,
    /* [in] */ ITabHostTabContentFactory* factory)
    : mTag(tag)
    , mFactory(factory)
{}

PInterface TabHost::FactoryContentStrategy::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 TabHost::FactoryContentStrategy::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TabHost::FactoryContentStrategy::Release()
{
    return ElRefBase::Release();
}

ECode TabHost::FactoryContentStrategy::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode TabHost::FactoryContentStrategy::GetContentView(
    /* [out] */ IView** view)
{
    if (mTabContent == NULL) {
        String str;
        mTag->ToString(&str);
        mFactory->CreateTabContent(str, (IView**)&mTabContent);
    }
    mTabContent->SetVisibility(View::VISIBLE);
    *view = mTabContent;
    (*view)->AddRef();

    return NOERROR;
}

ECode TabHost::FactoryContentStrategy::TabClosed()
{
    return mTabContent->SetVisibility(View::GONE);
}


TabHost::IntentContentStrategy::IntentContentStrategy(
    /* [in] */ String tag,
    /* [in] */ IIntent* intent)
    : mTag(tag)
    , mIntent(intent)
{}

PInterface TabHost::IntentContentStrategy::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 TabHost::IntentContentStrategy::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TabHost::IntentContentStrategy::Release()
{
    return ElRefBase::Release();
}

ECode TabHost::IntentContentStrategy::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode TabHost::IntentContentStrategy::GetContentView(
    /* [in] */ IView** view)
{
    //if (mLocalActivityManager == NULL) {
    //    throw new IllegalStateException("Did you forget to call 'public void setup(LocalActivityManager activityGroup)'?");
    //}
    //final Window w = mLocalActivityManager.startActivity(
    //        mTag, mIntent);
    //final View wd = w != NULL ? w.getDecorView() : NULL;
    //if (mLaunchedView != wd && mLaunchedView != NULL) {
    //    if (mLaunchedView.getParent() != NULL) {
    //        mTabContent.removeView(mLaunchedView);
    //    }
    //}
    //mLaunchedView = wd;

    //// XXX Set FOCUS_AFTER_DESCENDANTS on embedded activities for now so they can get
    //// focus if none of their children have it. They need focus to be able to
    //// display menu items.
    ////
    //// Replace this with something better when Bug 628886 is fixed...
    ////
    //if (mLaunchedView != NULL) {
    //    mLaunchedView.setVisibility(View.VISIBLE);
    //    mLaunchedView.setFocusableInTouchMode(TRUE);
    //    ((ViewGroup) mLaunchedView).setDescendantFocusability(
    //            FOCUS_AFTER_DESCENDANTS);
    //}

    //*view = mLaunchedView;
    return NOERROR;
}

ECode TabHost::IntentContentStrategy::TabClosed()
{
    if (mLaunchedView != NULL) {
        mLaunchedView->SetVisibility(View::GONE);
    }

    return NOERROR;
}


TabHost::TabKeyListener::TabKeyListener(
    /* [in] */ TabHost* owner)
    : mOwner(owner)
{}

PInterface TabHost::TabKeyListener::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 TabHost::TabKeyListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TabHost::TabKeyListener::Release()
{
    return ElRefBase::Release();
}

ECode TabHost::TabKeyListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode TabHost::TabKeyListener::OnKey(
    /* [in] */ IView* v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    switch (keyCode) {
        case KeyEvent_KEYCODE_DPAD_CENTER:
        case KeyEvent_KEYCODE_DPAD_LEFT:
        case KeyEvent_KEYCODE_DPAD_RIGHT:
        case KeyEvent_KEYCODE_DPAD_UP:
        case KeyEvent_KEYCODE_DPAD_DOWN:
        case KeyEvent_KEYCODE_ENTER:
            return FALSE;

    }
    Boolean res;
    mOwner->mTabContent->RequestFocusEx(View::FOCUS_FORWARD, &res);
    return mOwner->mTabContent->DispatchKeyEvent(event, &res);
}


TabHost::TabSelectionListener::TabSelectionListener(
    /* [in] */ TabHost* owner)
    : mOwner(owner)
{}

PInterface TabHost::TabSelectionListener::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

UInt32 TabHost::TabSelectionListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 TabHost::TabSelectionListener::Release()
{
    return ElRefBase::Release();
}

ECode TabHost::TabSelectionListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode TabHost::TabSelectionListener::OnTabSelectionChanged(
    /* [in] */ Int32 tabIndex,
    /* [in] */ Boolean clicked)
{
    mOwner->SetCurrentTab(tabIndex);
    if (clicked) {
        Boolean res;
        mOwner->mTabContent->RequestFocusEx(View::FOCUS_FORWARD, &res);
    }
    return NOERROR;
}


TabHost::TabHost()
    : mCurrentTab(-1)
{}

TabHost::TabHost(
    /* [in] */ IContext* context)
    : FrameLayout(context)
    , mCurrentTab(-1)
{
    InitTabHost();
}

TabHost::TabHost(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : FrameLayout(context, attrs)
    , mCurrentTab(-1)
{
    InitTabHost();
}

void TabHost::InitTabHost()
{
    SetFocusableInTouchMode(TRUE);
    SetDescendantFocusability(FOCUS_AFTER_DESCENDANTS);
}

/**
 * Get a new {@link TabSpec} associated with this tab host.
 * @param tag required tag of tab.
 */
AutoPtr<ITabSpec> TabHost::NewTabSpec(
    /* [in] */ String tag)
{
    AutoPtr<ITabSpec> tabSpec = new TabHost::TabSpec(tag, this);
    return tabSpec;
}

/**
  * <p>Call setup() before adding tabs if loading TabHost using findViewById().
  * <i><b>However</i></b>: You do not need to call setup() after getTabHost()
  * in {@link android.app.TabActivity TabActivity}.
  * Example:</p>
<pre>mTabHost = (TabHost)findViewById(R.id.tabhost);
mTabHost.setup();
mTabHost.addTab(TAB_TAG_1, "Hello, world!", "Tab 1");
  */
ECode TabHost::Setup()
{
    mTabWidget = ITabWidget::Probe(FindViewById(0x01020013/*com.android.internal.R.id.tabs*/));
    if (mTabWidget == NULL) {
//        throw new RuntimeException(
//                "Your TabHost must have a TabWidget whose id attribute is 'android.R.id.tabs'");
        return E_RUNTIME_EXCEPTION;
    }

    // KeyListener to attach to all tabs. Detects non-navigation keys
    // and relays them to the tab content.
    mTabKeyListener = new TabKeyListener(this);

    AutoPtr<ITabWidgetOnTabSelectionChanged> listener = new TabSelectionListener(this);
    ((TabWidget*)(CTabWidget*)ITabWidget::Probe(mTabWidget))->SetTabSelectionListener(listener);

    mTabContent = IFrameLayout::Probe(FindViewById(0x01020011/*com.android.internal.R.id.tabcontent*/));
    if (mTabContent == NULL) {
//        throw new RuntimeException(
//                "Your TabHost must have a FrameLayout whose id attribute is "
//                        + "'android.R.id.tabcontent'");
        return E_RUNTIME_EXCEPTION;
    }

    return NOERROR;
}

/**
 * If you are using {@link TabSpec#setContent(android.content.Intent)}, this
 * must be called since the activityGroup is needed to launch the local activity.
 *
 * This is done for you if you extend {@link android.app.TabActivity}.
 * @param activityGroup Used to launch activities for tab content.
 */
//public void TabHost::setup(LocalActivityManager activityGroup) {
//    setup();
//    mLocalActivityManager = activityGroup;
//}

void TabHost::OnAttachedToWindow()
{
    FrameLayout::OnAttachedToWindow();
    AutoPtr<IViewTreeObserver> treeObserver = GetViewTreeObserver();
    if (treeObserver != NULL) {
        treeObserver->AddOnTouchModeChangeListener(
                (IOnTouchModeChangeListener*)this->Probe(EIID_IOnTouchModeChangeListener));
    }
}

void TabHost::OnDetachedFromWindow()
{
    FrameLayout::OnDetachedFromWindow();
    AutoPtr<IViewTreeObserver> treeObserver = GetViewTreeObserver();
    if (treeObserver != NULL) {
        treeObserver->RemoveOnTouchModeChangeListener(
                (IOnTouchModeChangeListener*)this->Probe(EIID_IOnTouchModeChangeListener));
    }
}

/**
 * {@inheritDoc}
 */
ECode TabHost::OnTouchModeChanged(
    /* [in] */ Boolean isInTouchMode)
{
    if (!isInTouchMode) {
        // leaving touch mode.. if nothing has focus, let's give it to
        // the indicator of the current tab

        Boolean hasFocus, isFocused;
        if (mCurrentView != NULL &&
                ((mCurrentView->HasFocus(&hasFocus), hasFocus) ||
                 (mCurrentView->IsFocused(&isFocused), isFocused))) {
            AutoPtr<IView> view;
            mTabWidget->GetChildTabViewAt(mCurrentTab, (IView**)&view);

            Boolean res;
            view->RequestFocus(&res);
        }
    }

    return NOERROR;
}

/**
 * Add a tab.
 * @param tabSpec Specifies how to create the indicator and content.
 */
ECode TabHost::AddTab(
    /* [in] */ ITabSpec* tabSpec)
{
    TabSpec* ts = (TabSpec*)tabSpec;
    if (ts->mIndicatorStrategy == NULL) {
//        throw new IllegalArgumentException("you must specify a way to create the tab indicator.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (ts->mContentStrategy == NULL) {
//        throw new IllegalArgumentException("you must specify a way to create the tab content");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IView> tabIndicator;
    ts->mIndicatorStrategy->CreateIndicatorView((IView**)&tabIndicator);
    tabIndicator->SetOnKeyListener(mTabKeyListener);
    // If this is a custom view, then do not draw the bottom strips for
    // the tab indicators.
    if (ts->mIndicatorStrategy->Probe(EIID_ViewIndicatorStrategy) != NULL) {
        mTabWidget->SetStripEnabled(FALSE);
    }
    mTabWidget->AddView(tabIndicator);
    mTabSpecs.PushBack(ts);
    if (mCurrentTab == -1) {
        SetCurrentTab(0);
    }

    return NOERROR;
}


/**
 * Removes all tabs from the tab widget associated with this tab host.
 */
ECode TabHost::ClearAllTabs()
{
    mTabWidget->RemoveAllViews();
    InitTabHost();
    mTabContent->RemoveAllViews();
    mTabSpecs.Clear();
    RequestLayout();
    Invalidate();

    return NOERROR;
}

AutoPtr<ITabWidget> TabHost::GetTabWidget()
{
    return mTabWidget;
}

Int32 TabHost::GetCurrentTab()
{
    return mCurrentTab;
}

String TabHost::GetCurrentTabTag()
{
    if (mCurrentTab >= 0 && mCurrentTab < (Int32)mTabSpecs.GetSize()) {
        String str;
        mTabSpecs[mCurrentTab]->GetTag(&str);
        return str;
    }
    return String(NULL);
}

AutoPtr<IView> TabHost::GetCurrentTabView()
{
    if (mCurrentTab >= 0 && mCurrentTab < (Int32)mTabSpecs.GetSize()) {
        AutoPtr<IView> view;
        mTabWidget->GetChildTabViewAt(mCurrentTab, (IView**)&view);
        return view;
    }
    return NULL;
}

AutoPtr<IView> TabHost::GetCurrentView()
{
    return mCurrentView;
}

ECode TabHost::SetCurrentTabByTag(
    /* [in] */ String tag)
{
    Int32 i = 0;
    String str;
    List< AutoPtr<TabSpec> >::Iterator it;
    for (it = mTabSpecs.Begin(); it != mTabSpecs.End(); ++it, ++i) {
        (*it)->GetTag(&str);
        if (str.Equals(tag)) {
            SetCurrentTab(i);
            break;
        }
    }

    return NOERROR;
}

/**
 * Get the FrameLayout which holds tab content
 */
AutoPtr<IFrameLayout> TabHost::GetTabContentView()
{
    return mTabContent;
}

Boolean TabHost::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    Boolean handled = FrameLayout::DispatchKeyEvent(event);

    Int32 action, keycode;
    Boolean result;
    AutoPtr<IView> view;
    AutoPtr<IView> v;
    // unhandled key ups change focus to tab indicator for embedded activities
    // when there is nothing that will take focus from default focus searching
    if (!handled
            && (event->GetAction(&action), action == KeyEvent_ACTION_DOWN)
            && (event->GetKeyCode(&keycode), keycode == KeyEvent_KEYCODE_DPAD_UP)
            && (mCurrentView != NULL)
            && (mCurrentView->IsRootNamespace(&result), result)
            && (mCurrentView->HasFocus(&result), result)
            && (mCurrentView->FindFocus((IView**)&view),
                view->FocusSearch(View::FOCUS_UP, (IView**)&v), v == NULL)) {

                AutoPtr<IView> view;
                mTabWidget->GetChildTabViewAt(mCurrentTab, (IView**)&view);

                Boolean res;
                view->RequestFocus(&res);
                PlaySoundEffect(SoundEffectConstants_NAVIGATION_UP);
                return TRUE;
    }

    return handled;
}


ECode TabHost::DispatchWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    if (mCurrentView != NULL){
        mCurrentView->DispatchWindowFocusChanged(hasFocus);
    }

    return NOERROR;
}

ECode TabHost::SetCurrentTab(
    /* [in] */ Int32 index)
{
    if (index < 0 || index >= (Int32)mTabSpecs.GetSize()) {
        return NOERROR;
    }

    if (index == mCurrentTab) {
        return NOERROR;
    }

    // notify old tab content
    if (mCurrentTab != -1) {
        mTabSpecs[mCurrentTab]->mContentStrategy->TabClosed();
    }

    mCurrentTab = index;
    AutoPtr<TabSpec> spec = mTabSpecs[index];

    // Call the tab widget's focusCurrentTab(), instead of just
    // selecting the tab.
    mTabWidget->FocusCurrentTab(mCurrentTab);

    // tab content
    spec->mContentStrategy->GetContentView((IView**)&mCurrentView);

    AutoPtr<IViewParent> view;
    mCurrentView->GetParent((IViewParent**)&view);
    if (view == NULL) {
        AutoPtr<IViewGroupLayoutParams> lp;
        CViewGroupLayoutParams::New(
                ViewGroupLayoutParams_MATCH_PARENT,
                ViewGroupLayoutParams_MATCH_PARENT,
                (IViewGroupLayoutParams**)&lp);
        mTabContent->AddViewEx3(mCurrentView, lp);
    }

    Boolean hasFocus;
    mTabWidget->HasFocus(&hasFocus);
    if (!hasFocus) {
        // if the tab widget didn't take focus (likely because we're in touch mode)
        // give the current tab content view a shot
        mCurrentView->RequestFocus(&hasFocus);
    }

    //mTabContent.requestFocus(View.FOCUS_FORWARD);
    InvokeOnTabChangeListener();
    return NOERROR;
}

/**
 * Register a callback to be invoked when the selected state of any of the items
 * in this list changes
 * @param l
 * The callback that will run
 */
ECode TabHost::SetOnTabChangedListener(
    /* [in] */ ITabHostOnTabChangeListener* l)
{
    mOnTabChangeListener = l;

    return NOERROR;
}

void TabHost::InvokeOnTabChangeListener()
{
    if (mOnTabChangeListener != NULL) {
        mOnTabChangeListener->OnTabChanged(GetCurrentTabTag());
    }
}

ECode TabHost::Init(
    /* [in] */ IContext* context)
{
    FrameLayout::Init(context);
    InitTabHost();

    return NOERROR;
}

ECode TabHost::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FrameLayout::Init(context, attrs);
    InitTabHost();

    return NOERROR;
}
