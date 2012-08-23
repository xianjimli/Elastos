
#include "widget/TabHostTabSpec.h"
#include "widget/TabHost.h" 

TabHostTabSpec::TabHostTabSpec()
{

}

TabHostTabSpec::TabHostTabSpec(
    /* [in] */ String tag) 
{
    mTag = tag;
}

/**
 * Specify a label as the tab indicator.
 */
AutoPtr<ITabHostTabSpec> TabHostTabSpec::SetIndicator(
    /* [in] */ ICharSequence* label) 
{
    mIndicatorStrategy = new TabHost::LabelIndicatorStrategy(label);
    return NULL;
}

/**
 * Specify a label and icon as the tab indicator.
 */
AutoPtr<ITabHostTabSpec> TabHostTabSpec::SetIndicator(
    /* [in] */ ICharSequence* label, 
    /* [in] */ IDrawable* icon) 
{
    mIndicatorStrategy = new TabHost::LabelAndIconIndicatorStrategy(label, icon);
    return NULL;
}

/**
 * Specify a view as the tab indicator.
 */
AutoPtr<ITabHostTabSpec> TabHostTabSpec::SetIndicator(
    /* [in] */ IView* view) 
{
    mIndicatorStrategy = new TabHost::ViewIndicatorStrategy(view);
    return NULL;
}

/**
 * Specify the id of the view that should be used as the content
 * of the tab.
 */
AutoPtr<ITabHostTabSpec> TabHostTabSpec::SetContent(
    /* [in] */ Int32 viewId) 
{
    mContentStrategy = new TabHost::ViewIdContentStrategy(viewId);
    return NULL;
}

/**
 * Specify a {@link android.widget.TabHost.TabContentFactory} to use to
 * create the content of the tab.
 */
AutoPtr<ITabHostTabSpec> TabHostTabSpec::SetContent(
    /* [in] */ ITabHostTabContentFactory* contentFactory) 
{
    mContentStrategy = new TabHost::FactoryContentStrategy(/*mTag*/NULL, contentFactory);
    return NULL;
}

/**
 * Specify an intent to use to launch an activity as the tab content.
 */
AutoPtr<ITabHostTabSpec> TabHostTabSpec::SetContent(
    /* [in] */ IIntent* intent) 
{
    mContentStrategy = new TabHost::IntentContentStrategy(mTag, intent);
    return NULL;
}


String TabHostTabSpec::GetTag() 
{
    return mTag;
}
