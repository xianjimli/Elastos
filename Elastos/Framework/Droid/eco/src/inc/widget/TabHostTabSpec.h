
#ifndef __TABHOSTTABSPEC_H__
#define __TABHOSTTABSPEC_H__

#include "ext/frameworkext.h"
#include <elastos/AutoPtr.h>

/**
 * A tab has a tab indicator, content, and a tag that is used to keep
 * track of it.  This builder helps choose among these options.
 *
 * For the tab indicator, your choices are:
 * 1) set a label
 * 2) set a label and an icon
 *
 * For the tab content, your choices are:
 * 1) the id of a {@link View}
 * 2) a {@link TabContentFactory} that creates the {@link View} content.
 * 3) an {@link Intent} that launches an {@link android.app.Activity}.
 */
class TabHostTabSpec
{
public:
    TabHostTabSpec();
     /**
     * Specify a label as the tab indicator.
     */
    virtual CARAPI_(AutoPtr<ITabHostTabSpec>) SetIndicator(
        /* [in] */ ICharSequence* label);

    /**
     * Specify a label and icon as the tab indicator.
     */
    virtual CARAPI_(AutoPtr<ITabHostTabSpec>) SetIndicator(
        /* [in] */ ICharSequence* label, 
        /* [in] */ IDrawable* icon);

    /**
     * Specify a view as the tab indicator.
     */
    virtual CARAPI_(AutoPtr<ITabHostTabSpec>) SetIndicator(
        /* [in] */ IView* view);

    /**
     * Specify the id of the view that should be used as the content
     * of the tab.
     */
    virtual CARAPI_(AutoPtr<ITabHostTabSpec>) SetContent(
        /* [in] */ Int32 viewId);

    /**
     * Specify a {@link android.widget.TabHost.TabContentFactory} to use to
     * create the content of the tab.
     */
    virtual CARAPI_(AutoPtr<ITabHostTabSpec>) SetContent(
        /* [in] */ ITabHostTabContentFactory* contentFactory);

    /**
     * Specify an intent to use to launch an activity as the tab content.
     */
    virtual CARAPI_(AutoPtr<ITabHostTabSpec>) SetContent(
        /* [in] */ IIntent* intent);

    String GetTag();

private:
    TabHostTabSpec(
        /* [in] */ String tag);

public:
    String mTag;

    AutoPtr<IIndicatorStrategy> mIndicatorStrategy;
    AutoPtr<IContentStrategy> mContentStrategy;
};

#endif
