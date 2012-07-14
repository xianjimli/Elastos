
#ifndef __LAYOUTINFLATER_H__
#define __LAYOUTINFLATER_H__

#include "ext/frameworkext.h"
#include <elastos.h>
#include <elastos/AutoPtr.h>
#include <elastos/Mutex.h>
#include <elastos/HashMap.h>

using namespace Elastos;
using namespace Elastos::Core::Threading;

class LayoutInflater
{
public:
    static const char* TAG;

    static const char* TAG_MERGE;
    static const char* TAG_INCLUDE;
    static const char* TAG_REQUEST_FOCUS;

public:
    LayoutInflater();

    ~LayoutInflater();

    /**
     * Obtains the LayoutInflater from the given context.
     */
    static CARAPI From(
        /* [in] */ IContext* context,
        /* [out] */ ILayoutInflater** inflater);

    /**
     * Return the context we are running in, for access to resources, class
     * loader, etc.
     */
    CARAPI_(IContext*) GetContext();

    CARAPI Inflate(
        /* [in] */ Int32 resource,
        /* [in] */ IViewGroup* root,
        /* [out] */ IView** view);

    CARAPI InflateEx(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IViewGroup* root,
        /* [out] */ IView** view);

    CARAPI InflateEx2(
        /* [in] */ Int32 resource,
        /* [in] */ IViewGroup* root,
        /* [in] */ Boolean attachToRoot,
        /* [out] */ IView** view);

    CARAPI InflateEx3(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IViewGroup* root,
        /* [in] */ Boolean attachToRoot,
        /* [out] */ IView** view);

    /**
     * Low-level function for instantiating a view by name. This attempts to
     * instantiate a view class of the given <var>name</var> found in this
     * LayoutInflater's ClassLoader.
     *
     * <p>
     * There are two things that can happen in an error case: either the
     * exception describing the error will be thrown, or a null will be
     * returned. You must deal with both possibilities -- the former will happen
     * the first time createView() is called for a class of a particular name,
     * the latter every time there-after for that class name.
     *
     * @param name The full name of the class to be instantiated.
     * @param attrs The XML attributes supplied for this instance.
     *
     * @return View The newly instantied view, or null.
     */
    CARAPI CreateView(
        /* [in] */ const String& name,
        /* [in] */ const String& prefix,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IView** view);

protected:
    /**
     * This routine is responsible for creating the correct subclass of View
     * given the xml element name. Override it to handle custom view objects. If
     * you override this in your subclass be sure to call through to
     * super.onCreateView(name) for names you do not recognize.
     *
     * @param name The fully qualified class name of the View to be create.
     * @param attrs An AttributeSet of attributes to apply to the View.
     *
     * @return View The View created.
     */
    CARAPI OnCreateView(
        /* [in] */ const String& name,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IView** view);

    /*
     * default visibility so the BridgeInflater can override it.
     */
    CARAPI CreateViewFromTag(
        /* [in] */ const String& name,
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IView** view);

    CARAPI Init(
        /* [in] */ IContext* context);

private:
    /**
     * Throw an excpetion because the specified class is not allowed to be inflated.
     */
    CARAPI FailNotAllowed(
        /* [in] */ const String& name,
        /* [in] */ const String& prefix,
        /* [in] */ IAttributeSet* attrs);

    /**
     * Recursive method used to descend down the xml hierarchy and instantiate
     * views, instantiate their children, and then call onFinishInflate().
     */
    CARAPI RInflate(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IView* parent,
        /* [in] */ IAttributeSet* attrs);

    CARAPI ParseRequestFocus(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IView* parent);

    CARAPI ParseInclude(
        /* [in] */ IXmlPullParser* parser,
        /* [in] */ IView* parent,
        /* [in] */ IAttributeSet* attrs);

private:
    Boolean DEBUG;

    AutoPtr<IContext> mContext;

    // these are optional, set by the caller
    Boolean mFactorySet;
    AutoPtr<ILayoutInflaterFactory> mFactory;
    AutoPtr<ILayoutInflaterFilter> mFilter;

    AutoPtr<IInterface> mConstructorArgs[2];
    Mutex mConstructorArgsLock;

    static CString mConstructorSignature;

    static HashMap<String, AutoPtr<IConstructorInfo> >* sConstructorMap;

    HashMap<String, Boolean>* mFilterMap;
};

#endif // __LAYOUTINFLATER_H__