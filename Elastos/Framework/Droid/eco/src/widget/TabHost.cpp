
#include "widget/TabHost.h"
#include "widget/CTabHostTabSpec.h"
#include "view/CViewGroupLayoutParams.h"

static const Int32 R_Layout_Tab_indicator = 0x01080309;
static const Int32 R_Drawable_Tab_indicator_v4 = 0x0108030a;
static const Int32 R_Color_Tab_indicator_text_v4 = 0x01060038;

TabHost::TabHost()
{
}

TabHost::TabHost(
    /* [in] */ IContext* context) : FrameLayout(context)
{
    InitTabHost();
}

TabHost::TabHost(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs) : FrameLayout(context, attrs)
{
    InitTabHost();
}

void TabHost::InitTabHost()
{
    SetFocusableInTouchMode(TRUE);
    SetDescendantFocusability(FOCUS_AFTER_DESCENDANTS);

    mCurrentTab = -1;
    mCurrentView = NULL;
}

/**
 * Get a new {@link TabSpec} associated with this tab host.
 * @param tag required tag of tab.
 */
AutoPtr<ITabHostTabSpec> TabHost::NewTabSpec(
    /* [in] */ String tag)
{
    AutoPtr<ITabHostTabSpec> tabSpec;
    CTabHostTabSpec::New(tag, (ITabHostTabSpec**)&tabSpec);

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
    //mTabWidget = FindViewById(0x01020013/* R.id.tab */);
    //if (mTabWidget.Get() == NULL) {
    //    /*throw new RuntimeException(
    //            "Your TabHost must have a TabWidget whose id attribute is 'android.R.id.tabs'");*/
    //}

    // KeyListener to attach to all tabs. Detects non-navigation keys
    // and relays them to the tab content.
    /*mTabKeyListener = new OnKeyListener() {
        public Boolean onKey(View v, Int32 keyCode, KeyEvent event) {
            switch (keyCode) {
                case KeyEvent.KEYCODE_DPAD_CENTER:
                case KeyEvent.KEYCODE_DPAD_LEFT:
                case KeyEvent.KEYCODE_DPAD_RIGHT:
                case KeyEvent.KEYCODE_DPAD_UP:
                case KeyEvent.KEYCODE_DPAD_DOWN:
                case KeyEvent.KEYCODE_ENTER:
                    return FALSE;

            }
            mTabContent.requestFocus(View.FOCUS_FORWARD);
            return mTabContent.dispatchKeyEvent(event);
        }

    };*/

    /*mTabWidget.setTabSelectionListener(new TabWidget.OnTabSelectionChanged() {
        public void onTabSelectionChanged(Int32 tabIndex, Boolean clicked) {
            setCurrentTab(tabIndex);
            if (clicked) {
                mTabContent.requestFocus(View.FOCUS_FORWARD);
            }
        }
    });*/

    //mTabContent = FindViewById(/*com.android.internal.R.id.tabcontent*/0x01020011);
    //if (mTabContent.Get() == NULL) {
    //    /*throw new RuntimeException(
    //            "Your TabHost must have a FrameLayout whose id attribute is "
    //                    + "'android.R.id.tabcontent'");*/
    //}

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
    if (treeObserver.Get() != NULL) {
        //treeObserver->AddOnTouchModeChangeListener(this);
    }
}

void TabHost::OnDetachedFromWindow()
{
    FrameLayout::OnDetachedFromWindow();
    AutoPtr<IViewTreeObserver> treeObserver = GetViewTreeObserver();
    if (treeObserver.Get() != NULL) {
        //treeObserver.removeOnTouchModeChangeListener(this);
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
        if (mCurrentView.Get() != NULL) {
            mCurrentView->HasFocus(&hasFocus);
            mCurrentView->IsFocused(&isFocused);
        }

        if (mCurrentView.Get() != NULL && (hasFocus || isFocused)) {
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
    /* [in] */ ITabHostTabSpec* tabSpec)
{
    CTabHostTabSpec* ts = (CTabHostTabSpec*)tabSpec;
    if (ts->mIndicatorStrategy.Get() == NULL) {
        //throw new IllegalArgumentException("you must specify a way to create the tab indicator.");
    }

    if (ts->mContentStrategy.Get() == NULL) {
        //throw new IllegalArgumentException("you must specify a way to create the tab content");
    }

    AutoPtr<IView> tabIndicator;
    ts->mIndicatorStrategy->CreateIndicatorView((IView**)&tabIndicator);

    tabIndicator->SetOnKeyListener(mTabKeyListener);

    // If this is a custom view, then do not draw the bottom strips for
    // the tab indicators.
    //if (ts->mIndicatorStrategy->Probe(EIID_IViewIndicatorStrategy)) {
        mTabWidget->SetStripEnabled(FALSE);
    //}
    mTabWidget->AddView(tabIndicator);
    //mTabSpecs->Add(tabSpec);

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
    //mTabSpecs->Clear();
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
    if (mCurrentTab >= 0 && mCurrentTab < mTabSpecs->GetLength()) {
        String str;
        (*mTabSpecs)[mCurrentTab]->GetTag(&str);
        return str;
    }
    //return NULL;
}

AutoPtr<IView> TabHost::GetCurrentTabView()
{
    if (mCurrentTab >= 0 && mCurrentTab < mTabSpecs->GetLength()) {
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
    Int32 i;

    String str;
    for (i = 0; i < mTabSpecs->GetLength(); i++) {
        (*mTabSpecs)[i]->GetTag(&str);
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
            && ((event->GetAction(&action), action) == KeyEvent_ACTION_DOWN)
            && ((event->GetKeyCode(&keycode), keycode) == KeyEvent_KEYCODE_DPAD_UP)
            && (mCurrentView.Get() != NULL)
            && (mCurrentView->IsRootNamespace(&result), result)
            && (mCurrentView->HasFocus(&result), result)
            && ((mCurrentView->FindFocus((IView**)&view), view->FocusSearch(View::FOCUS_UP, (IView**)&v)), v == NULL)) {

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
    if (mCurrentView.Get() != NULL){
        mCurrentView->DispatchWindowFocusChanged(hasFocus);
    }

    return NOERROR;
}

ECode TabHost::SetCurrentTab(
    /* [in] */ Int32 index)
{
    if (index < 0 || index >= mTabSpecs->GetLength()) {
        return NOERROR;
    }

    if (index == mCurrentTab) {
        return NOERROR;
    }

    // notify old tab content
    if (mCurrentTab != -1) {
        ((CTabHostTabSpec*)((*mTabSpecs)[mCurrentTab].Get()))->mContentStrategy->TabClosed();
    }

    mCurrentTab = index;
    AutoPtr<ITabHostTabSpec> spec = (*mTabSpecs)[index];

    // Call the tab widget's focusCurrentTab(), instead of just
    // selecting the tab.
    mTabWidget->FocusCurrentTab(mCurrentTab);

    // tab content
    ((CTabHostTabSpec*)spec.Get())->mContentStrategy->GetContentView((IView**)&mCurrentView);

    AutoPtr<IViewParent> view;
    mCurrentView->GetParent((IViewParent**)&view);
    if (view.Get() == NULL) {
        AutoPtr<IViewGroupLayoutParams> lp;
        CViewGroupLayoutParams::New(ViewGroupLayoutParams_MATCH_PARENT, ViewGroupLayoutParams_MATCH_PARENT, (IViewGroupLayoutParams**)&lp);
        mTabContent->AddViewEx3(
                        mCurrentView, lp);
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
    if (mOnTabChangeListener.Get() != NULL) {
        mOnTabChangeListener->OnTabChanged(GetCurrentTabTag());
    }
}

TabHost::LabelIndicatorStrategy::LabelIndicatorStrategy(
    /* [in] */ ICharSequence* label)
{
    mLabel = label;
}

ECode TabHost::LabelIndicatorStrategy::CreateIndicatorView(
    /* [out] */ IView** view)
{
    //AutoPtr<IContext> context = GetContext();
    //AutoPtr<ILayoutInflater> inflater;
    //context->GetSystemService(Context_LAYOUT_INFLATER_SERVICE, (IInterface**)&inflater);

    //AutoPtr<IView> tabIndicator;
    //inflater->InflateEx2(R_Layout_Tab_indicator,
    //        mTabWidget, // tab widget is the parent
    //        FALSE,
    //        (IView**)&tabIndicator); // no inflate params

    //AutoPtr<ITextView> tv;
    //tv = tabIndicator->FindViewById(/*R.id.title*/0x010101e1);
    //tv->SetText(mLabel);

    //AutoPtr<IApplicationInfo> ai;
    //context->GetApplicationInfo((IApplicationInfo**)&ai);
    //if (((CApplicationInfo*)ai.Get())->mTargetSdkVersion <= Build_VERSION_CODES_DONUT) {
    //    // Donut apps get old color scheme
    //    tabIndicator->SetBackgroundResource(R_Drawable_Tab_indicator_v4);

    //    AutoPtr<IResources> resources;
    //    context->GetResources((IResources**)&resources);

    //    AutoPtr<IColorStateList> sl;
    //    resources->GetColorStateList(R_Color_Tab_indicator_text_v4, (IColorStateList**)&sl);

    //    tv->SetTextColor(sl);
    //}

    //*view = tabIndicator.Get();

    return NOERROR;
}

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

TabHost::LabelAndIconIndicatorStrategy::LabelAndIconIndicatorStrategy(
    /* [in] */ ICharSequence* label,
    /* [in] */ IDrawable* icon)
{
    mLabel = label;
    mIcon = icon;
}

ECode TabHost::LabelAndIconIndicatorStrategy::CreateIndicatorView(
    /* [out] */ IView** view)
{
    //AutoPtr<IContext> context = GetContext();

    //AutoPtr<ILayoutInflater> inflater;
    //context->GetSystemService(Context_LAYOUT_INFLATER_SERVICE, (IInterface**)&inflater);
    //AutoPtr<IView> tabIndicator;
    //inflater->InflateEx2(R_Layout_Tab_indicator,
    //        mTabWidget, // tab widget is the parent
    //        FALSE, (IView**)&tabIndicator); // no inflate params

    //AutoPtr<ITextView> tv;
    //tv = tabIndicator->FindViewById(/*R.id.title*/0x010101e1);
    //tv->SetText(mLabel);

    //AutoPtr<IImageView> iconView;
    //iconView = tabIndicator->FindViewById(/*R.id.icon*/0x01010002);
    //iconView->SetImageDrawable(mIcon);

    //AutoPtr<IApplicationInfo> ai;
    //context->GetApplicationInfo((IApplicationInfo**)&ai);
    //if (((CApplicationInfo*)ai.Get())->mTargetSdkVersion <= Build_VERSION_CODES_DONUT) {
    //    // Donut apps get old color scheme
    //    tabIndicator->SetBackgroundResource(R_Drawable_Tab_indicator_v4);

    //    AutoPtr<IResources> resources;
    //    context->GetResources((IResources**)&resources);

    //    AutoPtr<IColorStateList> sl;
    //    resources->GetColorStateList(R_Color_Tab_indicator_text_v4, (IColorStateList**)&sl);

    //    tv->SetTextColor(sl);
    //}

    //*view = tabIndicator.Get();

    return NOERROR;
}

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

TabHost::ViewIndicatorStrategy::ViewIndicatorStrategy(
    /* [in] */ IView* view)
{
    mView = view;
}

ECode TabHost::ViewIndicatorStrategy::CreateIndicatorView(
    /* [out] */ IView** view)
{
    *view = mView;
    return NOERROR;
}

PInterface TabHost::ViewIndicatorStrategy::Probe(
    /* [in] */ REIID riid)
{
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

TabHost::ViewIdContentStrategy::ViewIdContentStrategy(
    /* [in] */ Int32 viewId)
{
    //mView = mTabContent->FindViewById(viewId);
    if (mView.Get() != NULL) {
        mView->SetVisibility(View::GONE);
    } else {
        /*throw new RuntimeException("Could not create tab content because " +
                "could not find view with id " + viewId);*/
    }
}

ECode TabHost::ViewIdContentStrategy::GetContentView(
    /* [out] */ IView** view)
{
    mView->SetVisibility(View::VISIBLE);
    *view = mView;
    return NOERROR;
}

ECode TabHost::ViewIdContentStrategy::TabClosed()
{
    return mView->SetVisibility(View::GONE);
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

TabHost::FactoryContentStrategy::FactoryContentStrategy(
    /* [in] */ ICharSequence* tag,
    /* [in] */ ITabHostTabContentFactory* factory)
{
    mTag = tag;
    mFactory = factory;
}

ECode TabHost::FactoryContentStrategy::GetContentView(
    /* [out] */ IView** view)
{
    if (mTabContent.Get() == NULL) {

        String str;
        mTag->ToString(&str);
        mFactory->CreateTabContent(str, (IView**)&mTabContent);
    }
    mTabContent->SetVisibility(View::VISIBLE);
    *view = mTabContent;

    return NOERROR;
}

ECode TabHost::FactoryContentStrategy::TabClosed()
{
    return mTabContent->SetVisibility(View::GONE);
}

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

TabHost::IntentContentStrategy::IntentContentStrategy(
    /* [in] */ String tag,
    /* [in] */ IIntent* intent)
{
    mTag = tag;
    mIntent = intent;
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
    if (mLaunchedView.Get() != NULL) {
        mLaunchedView->SetVisibility(View::GONE);
    }

    return NOERROR;
}

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
