
#include "view/LayoutInflater.h"
#include "utils/Xml.h"
#include <Slogger.h>
#include <StringBuffer.h>

#include <stdio.h>

using namespace Elastos::Core;
using namespace Elastos::Utility::Logging;

#define LAYOUT_INFLATOR_CATCH_EXCEPTION1(expr) \
    do { \
        ECode ec = expr; \
        if (ec == (Int32)E_NO_SUCH_METHOD_EXCEPTION) { \
            /*InflateException ie = new InflateException(attrs.getPositionDescription()*/ \
                  /*+ ": Error inflating class "*/ \
                  /*+ (prefix != null ? (prefix + name) : name));*/ \
            /*ie.initCause(e);*/ \
            return ec; \
        } \
        else if (ec == (Int32)E_CLASS_NOT_FOUND_EXCEPTION) { \
            /* If loadClass fails, we should propagate the exception. */ \
            return ec; \
        } \
        else if (FAILED(ec)) { \
            /*InflateException ie = new InflateException(attrs.getPositionDescription()*/ \
                    /*+ ": Error inflating class "*/ \
                    /*+ (clazz == null ? "<unknown>" : clazz.getName()));*/ \
            /*ie.initCause(e);*/ \
            return ec;; \
        } \
    } while(0);


#define LAYOUT_INFLATOR_CATCH_EXCEPTION2(expr) \
    do { \
        ECode ec = expr; \
        if (ec == (Int32)E_XML_PULL_PARSER_EXCEPTION) { \
            /*InflateException ex = new InflateException(e.getMessage());*/ \
            /*ie.initCause(e);*/ \
            mConstructorArgs[0] = lastContext; \
            mConstructorArgs[1] = NULL; \
            return ec; \
        } \
        else if (ec == (Int32)E_IO_EXCEPTION) { \
            /*InflateException ie = new InflateException(*/ \
                    /*parser.getPositionDescription()*/ \
                    /*+ ": " + e.getMessage());*/ \
            /*ie.initCause(e);*/ \
            mConstructorArgs[0] = lastContext; \
            mConstructorArgs[1] = NULL; \
            return ec;; \
        } \
        else if (FAILED(ec)) { \
            mConstructorArgs[0] = lastContext; \
            mConstructorArgs[1] = NULL; \
            return ec;; \
        } \
    } while(0);


#define LAYOUT_INFLATOR_CATCH_EXCEPTION3(expr) \
    do { \
        ECode ec = expr; \
        if (ec == (Int32)E_INFLATE_EXCEPTION) { \
            return ec; \
        } \
        else if (ec == (Int32)E_CLASS_NOT_FOUND_EXCEPTION) { \
            /*InflateException ie = new InflateException(attrs.getPositionDescription()*/ \
                    /*+ ": Error inflating class " + name);*/ \
            /*ie.initCause(e);*/ \
            return ec;; \
        } \
        else if (FAILED(ec)) { \
            /*InflateException ie = new InflateException(attrs.getPositionDescription()*/ \
                    /*+ ": Error inflating class " + name);*/ \
            /*ie.initCause(e);*/ \
            return ec;; \
        } \
    } while(0);

#define FAIL_RETURN_WITH_CLOSE(expr) \
    do { \
        ECode ec = expr; \
        if (FAILED(ec)) { \
            childParser->Close(); \
            return ec; \
        } \
    } while(0);

const char* LayoutInflater::TAG  = "LayoutInflater";
const char* LayoutInflater::TAG_MERGE = "merge";
const char* LayoutInflater::TAG_INCLUDE = "include";
const char* LayoutInflater::TAG_REQUEST_FOCUS = "requestFocus";

/*
 * to be careful: the value of mConstructorSignature should be
 * be consistent with constructor methods of all widgets declared
 * in *.car.
 */
CString LayoutInflater::mConstructorSignature = "CtxAttrs";

HashMap<String, AutoPtr<IConstructorInfo> >* LayoutInflater::sConstructorMap = \
    new HashMap<String, AutoPtr<IConstructorInfo> >(30);

LayoutInflater::LayoutInflater() :
    DEBUG(FALSE)
{}

LayoutInflater::~LayoutInflater()
{}

ECode LayoutInflater::From(
    /* [in] */ IContext* context,
    /* [out] */ ILayoutInflater** inflater)
{
    if (inflater == NULL) return E_INVALID_ARGUMENT;

    ECode ec = context->GetSystemService(
            Context_LAYOUT_INFLATER_SERVICE, (IInterface**)inflater);
    if (FAILED(ec) || (*inflater == NULL)) {
        Slogger::W(TAG, "LayoutInflater not found.");
        return E_ASSERTION_ERROR;
    }

    return ec;
}

/**
 * Return the context we are running in, for access to resources, class
 * loader, etc.
 */
IContext* LayoutInflater::GetContext()
{
    return mContext;
}

/**
 * Inflate a new view hierarchy from the specified xml resource. Throws
 * {@link InflateException} if there is an error.
 *
 * @param resource ID for an XML layout resource to load (e.g.,
 *        <code>R.layout.main_page</code>)
 * @param root Optional view to be the parent of the generated hierarchy.
 * @return The root View of the inflated hierarchy. If root was supplied,
 *         this is the root View; otherwise it is the root of the inflated
 *         XML file.
 */
ECode LayoutInflater::Inflate(
    /* [in] */ Int32 resource,
    /* [in] */ IViewGroup* root,
    /* [out] */ IView** view)
{
    return InflateEx2(resource, root, root != NULL, view);
}

ECode LayoutInflater::InflateEx(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IViewGroup* root,
    /* [out] */ IView** view)
{
    return InflateEx3(parser, root, root != NULL, view);
}

ECode LayoutInflater::InflateEx2(
    /* [in] */ Int32 resource,
    /* [in] */ IViewGroup* root,
    /* [in] */ Boolean attachToRoot,
    /* [out] */ IView** view)
{
    assert(view != NULL);

    if (DEBUG) {
//        System.out.println("INFLATING from resource: " + resource);
    }

    AutoPtr<IResources> res;
    ASSERT_SUCCEEDED(mContext->GetResources((IResources**)&res));

    AutoPtr<IXmlResourceParser> parser;
    ASSERT_SUCCEEDED(res->GetLayout(resource, (IXmlResourceParser**)&parser));

    ECode ec = InflateEx3(parser, root, attachToRoot, view);
    parser->Close();
    return ec;
}

ECode LayoutInflater::InflateEx3(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IViewGroup* root,
    /* [in] */ Boolean attachToRoot,
    /* [out] */ IView** view)
{
    assert(view != NULL);

    Mutex::Autolock lock(mConstructorArgsLock);

    AutoPtr<IAttributeSet> attrs = Xml::AsAttributeSet(parser);
    AutoPtr<IContext> lastContext = (IContext*)mConstructorArgs[0].Get();
    mConstructorArgs[0] = (IInterface*)mContext;
    AutoPtr<IView> result = root != NULL? (IView*)root->Probe(EIID_IView) : NULL;

//    try {
    // Look for the root node.
    Int32 type;
    LAYOUT_INFLATOR_CATCH_EXCEPTION2(parser->Next(&type));
    while (type != IXmlPullParser_START_TAG &&
            type != IXmlPullParser_END_DOCUMENT) {
        LAYOUT_INFLATOR_CATCH_EXCEPTION2(parser->Next(&type));
    }

    if (type != IXmlPullParser_START_TAG) {
//        throw new InflateException(parser.getPositionDescription()
//                + ": No start tag found!");
        mConstructorArgs[0] = lastContext;
        mConstructorArgs[1] = NULL;
        return E_INFLATE_EXCEPTION;
    }

    String name;
    parser->GetName(&name);

    if (DEBUG) {
//        System.out.println("**************************");
//        System.out.println("Creating root view: "
//                + name);
//        System.out.println("**************************");
    }

    if (name.Equals(TAG_MERGE)) {
        if (root == NULL || !attachToRoot) {
//            throw new InflateException("<merge /> can be used only with a valid "
//                    + "ViewGroup root and attachToRoot=true");
            mConstructorArgs[0] = lastContext;
            mConstructorArgs[1] = NULL;
            return E_INFLATE_EXCEPTION;
        }

        LAYOUT_INFLATOR_CATCH_EXCEPTION2(RInflate(parser, root, attrs.Get()));
    }
    else {
        // Temp is the root view that was found in the xml
        AutoPtr<IView> temp;
        LAYOUT_INFLATOR_CATCH_EXCEPTION2(CreateViewFromTag(name, attrs, (IView**)&temp));

        AutoPtr<IViewGroupLayoutParams> params;

        if (root != NULL) {
            if (DEBUG) {
//                System.out.println("Creating params from root: " +
//                        root);
            }
            // Create layout params that match root, if supplied
            root->GenerateLayoutParams(attrs, (IViewGroupLayoutParams**)&params);
            assert(params.Get());
            if (!attachToRoot) {
                // Set the layout params for temp if we are not
                // attaching. (If we are, we use addView, below)
                temp->SetLayoutParams(params.Get());
            }
        }

        if (DEBUG) {
//            System.out.println("-----> start inflating children");
        }
        // Inflate all children under temp
        LAYOUT_INFLATOR_CATCH_EXCEPTION2(RInflate(parser, temp.Get(), attrs));
        if (DEBUG) {
//            System.out.println("-----> done inflating children");
        }

        // We are supposed to attach all the views we found (int temp)
        // to root. Do that now.
        if (root != NULL && attachToRoot) {
            root->AddViewEx3(temp.Get(), params.Get());
        }

        // Decide whether to return the root that was passed in or the
        // top view found in xml.
        if (root == NULL || !attachToRoot) {
            result = temp;
        }
    }

//    } catch (XmlPullParserException e) {
//        InflateException ex = new InflateException(e.getMessage());
//        ex.initCause(e);
//        throw ex;
//    } catch (IOException e) {
//        InflateException ex = new InflateException(
//                parser.getPositionDescription()
//                + ": " + e.getMessage());
//        ex.initCause(e);
//        throw ex;
//    } finally {
//        // Don't retain static reference on context.
//        mConstructorArgs[0] = lastContext;
//        mConstructorArgs[1] = null;
//    }

    *view = result.Get();
    if (*view != NULL) (*view)->AddRef();

    mConstructorArgs[0] = lastContext;
    mConstructorArgs[1] = NULL;

    return NOERROR;
}

ECode LayoutInflater::CreateView(
    /* [in] */ const String& name,
    /* [in] */ const String& prefix,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IView** view)
{
    AutoPtr<IConstructorInfo> constructor;
    AutoPtr<IClassInfo> clazz;
//zprintf("====== File: %s, Line: %d, Name: %s ======\n", __FILE__, __LINE__, (const char*)name);
//    try {
    HashMap<String, AutoPtr<IConstructorInfo> >::Iterator it = \
        sConstructorMap->Find(name);

    if (it == sConstructorMap->End()) {
        // Class not found in the cache, see if it's real, and try to add it
        AutoPtr<IClassLoader> cl;
        LAYOUT_INFLATOR_CATCH_EXCEPTION1(mContext->GetClassLoader((IClassLoader**)&cl));
        if (!prefix.IsNull()) {
            LAYOUT_INFLATOR_CATCH_EXCEPTION1(cl->LoadClass(prefix + name, (Handle32*)&clazz));
        }
        else {
            LAYOUT_INFLATOR_CATCH_EXCEPTION1(cl->LoadClass(name, (Handle32*)&clazz));
        }

        if (mFilter != NULL && clazz != NULL) {
            Boolean allowed;
            mFilter->OnLoadClass((Handle32)clazz.Get(), &allowed);
            if (!allowed) {
                LAYOUT_INFLATOR_CATCH_EXCEPTION1(FailNotAllowed(name, prefix, attrs));
            }
        }
        LAYOUT_INFLATOR_CATCH_EXCEPTION1(clazz->GetConstructorInfoByParamNames(
                mConstructorSignature, (IConstructorInfo**)&constructor));
        (*sConstructorMap)[name] = constructor;
    }
    else {
        constructor = it->mSecond;
        // If we have a filter, apply it to cached constructor
        if (mFilter != NULL) {
            // Have we seen this name before?
            Boolean allowedState;
            HashMap<String, Boolean>::Iterator iit = mFilterMap->Find(name);
            if (iit == mFilterMap->End()) {
                // New class -- remember whether it is allowed
                AutoPtr<IClassLoader> cl;
                LAYOUT_INFLATOR_CATCH_EXCEPTION1(mContext->GetClassLoader((IClassLoader**)&cl));
                if (!prefix.IsNull()) {
                    LAYOUT_INFLATOR_CATCH_EXCEPTION1(cl->LoadClass(prefix + name, (Handle32*)&clazz));
                }
                else {
                    LAYOUT_INFLATOR_CATCH_EXCEPTION1(cl->LoadClass(name, (Handle32*)&clazz));
                }

                Boolean allowed;
                mFilter->OnLoadClass((Handle32)clazz.Get(), &allowed);
                allowed = allowed && (clazz != NULL);
                (*mFilterMap)[name] = allowed;
                if (!allowed) {
                    LAYOUT_INFLATOR_CATCH_EXCEPTION1(FailNotAllowed(name, prefix, attrs));
                }
            }
            else {
                allowedState = iit->mSecond;
                if (!allowedState) {
                    LAYOUT_INFLATOR_CATCH_EXCEPTION1(FailNotAllowed(name, prefix, attrs));
                }
            }
        }
    }

    mConstructorArgs[1] = attrs;

    AutoPtr<IArgumentList> args;
    LAYOUT_INFLATOR_CATCH_EXCEPTION1(constructor->CreateArgumentList((IArgumentList**)&args));
    LAYOUT_INFLATOR_CATCH_EXCEPTION1(args->SetInputArgumentOfObjectPtr(0, mConstructorArgs[0].Get()));
    LAYOUT_INFLATOR_CATCH_EXCEPTION1(args->SetInputArgumentOfObjectPtr(1, mConstructorArgs[1].Get()));
    AutoPtr<IInterface> obj;
    LAYOUT_INFLATOR_CATCH_EXCEPTION1(constructor->CreateObject(args, (IInterface**)&obj));
    *view = (IView*)obj->Probe(EIID_IView);
    assert(*view != NULL);
    (*view)->AddRef();
    return NOERROR;

//    } catch (NoSuchMethodException e) {
//        InflateException ie = new InflateException(attrs.getPositionDescription()
//                + ": Error inflating class "
//                + (prefix != null ? (prefix + name) : name));
//        ie.initCause(e);
//        throw ie;
//
//    } catch (ClassNotFoundException e) {
//        // If loadClass fails, we should propagate the exception.
//        throw e;
//    } catch (Exception e) {
//        InflateException ie = new InflateException(attrs.getPositionDescription()
//                + ": Error inflating class "
//                + (clazz == null ? "<unknown>" : clazz.getName()));
//        ie.initCause(e);
//        throw ie;
//    }
}

ECode LayoutInflater::FailNotAllowed(
    /* [in] */ const String& name,
    /* [in] */ const String& prefix,
    /* [in] */ IAttributeSet* attrs)
{
//    InflateException ie = new InflateException(attrs.getPositionDescription()
//            + ": Class not allowed to be inflated "
//            + (prefix != null ? (prefix + name) : name));
//    throw ie;
    return E_INFLATE_EXCEPTION;
}

ECode LayoutInflater::OnCreateView(
    /* [in] */ const String& name,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IView** view)
{
    return CreateView(name, String("elastos.view."), attrs, view);
}

ECode LayoutInflater::CreateViewFromTag(
    /* [in] */ const String& _name,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IView** view)
{
    String name;

    if (!_name.Compare("view")) {
        attrs->GetAttributeValueEx(NULL, "class", &name);
    }
    else {
        name = _name;
    }

    if (DEBUG) {
//        System.out.println("******** Creating view: " + name);
    }

//    try {
    if (mFactory != NULL) {
        mFactory->OnCreateView(name, mContext.Get(), attrs, view);
    }
    else *view = NULL;

    if (*view == NULL) {
        if (-1 == name.IndexOf('.')) {
            LAYOUT_INFLATOR_CATCH_EXCEPTION3(OnCreateView(name, attrs, view));
        } else {
            LAYOUT_INFLATOR_CATCH_EXCEPTION3(CreateView(name, String(NULL), attrs, view));
        }
    }

    if (DEBUG) {
//        System.out.println("Created view is: " + view);
    }

    return NOERROR;

//    } catch (InflateException e) {
//        throw e;
//
//    } catch (ClassNotFoundException e) {
//        InflateException ie = new InflateException(attrs.getPositionDescription()
//                + ": Error inflating class " + name);
//        ie.initCause(e);
//        throw ie;
//
//    } catch (Exception e) {
//        InflateException ie = new InflateException(attrs.getPositionDescription()
//                + ": Error inflating class " + name);
//        ie.initCause(e);
//        throw ie;
//    }
}

ECode LayoutInflater::RInflate(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IView* parent,
    /* [in] */ IAttributeSet* attrs)
{
    Int32 orgDepth, depth;
    Int32 type;
    String name;

    FAIL_RETURN(parser->GetDepth(&orgDepth));
    FAIL_RETURN(parser->Next(&type));
    FAIL_RETURN(parser->GetDepth(&depth));
    while ((type != IXmlPullParser_END_TAG ||
            depth > orgDepth) && type != IXmlPullParser_END_DOCUMENT) {

        if (type != IXmlPullParser_START_TAG) {
            FAIL_RETURN(parser->Next(&type));
            FAIL_RETURN(parser->GetDepth(&depth));
            continue;
        }

        FAIL_RETURN(parser->GetName(&name));

        if (String(TAG_REQUEST_FOCUS).Equals(name)) {
            FAIL_RETURN(ParseRequestFocus(parser, parent));
        }
        else if (String(TAG_INCLUDE).Equals(name)) {
            Int32 d;
            FAIL_RETURN(parser->GetDepth(&d));
            if (d == 0) {
//                throw new InflateException("<include /> cannot be the root element");
                return E_INFLATE_EXCEPTION;
            }
            FAIL_RETURN(ParseInclude(parser, parent, attrs));
        }
        else if (String(TAG_MERGE).Equals(name)) {
//            throw new InflateException("<merge /> must be the root element");
            return E_INFLATE_EXCEPTION;
        }
        else {
            AutoPtr<IView> view;
            FAIL_RETURN(CreateViewFromTag(name, attrs, (IView**)&view));
            AutoPtr<IViewGroup> viewGroup = (IViewGroup*)parent->Probe(EIID_IViewGroup);
            AutoPtr<IViewGroupLayoutParams> params;
            viewGroup->GenerateLayoutParams(attrs, (IViewGroupLayoutParams**)&params);
            FAIL_RETURN(RInflate(parser, view, attrs));
            viewGroup->AddViewEx3(view, params);
        }

        FAIL_RETURN(parser->Next(&type));
        FAIL_RETURN(parser->GetDepth(&depth));
    }

    parent->OnFinishInflate();
    return NOERROR;
}

ECode LayoutInflater::ParseRequestFocus(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IView* parent)
{
    Int32 type;
    Boolean hasFocus;
    Int32 currentDepth, depth;

    parent->RequestFocus(&hasFocus);
    FAIL_RETURN(parser->GetDepth(&currentDepth));
    FAIL_RETURN(parser->Next(&type));
    FAIL_RETURN(parser->GetDepth(&depth));
    while ((type != IXmlPullParser_END_TAG ||
            depth > currentDepth) && type != IXmlPullParser_END_DOCUMENT) {
        // Empty
        FAIL_RETURN(parser->Next(&type));
        FAIL_RETURN(parser->GetDepth(&depth));
    }
    return NOERROR;
}

ECode LayoutInflater::ParseInclude(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IView* parent,
    /* [in] */ IAttributeSet* attrs)
{
    Int32 type;

    AutoPtr<IViewGroup> group = (IViewGroup*)parent->Probe(EIID_IViewGroup);
    if (group != NULL) {
        Int32 layout;
        attrs->GetAttributeResourceValue(String(NULL), String("layout"), 0, &layout);
        if (layout == 0) {
            String value;
            attrs->GetAttributeValueEx(NULL, "layout", &value);
            if (value.IsNull()) {
//                throw new InflateException("You must specifiy a layout in the"
//                        + " include tag: <include layout=\"@layout/layoutID\" />");
                return E_INFLATE_EXCEPTION;
            } else {
//                throw new InflateException("You must specifiy a valid layout "
//                        + "reference. The layout ID " + value + " is not valid.");
                return E_INFLATE_EXCEPTION;
            }
        } else {
            AutoPtr<IResources> res;
            ASSERT_SUCCEEDED(mContext->GetResources((IResources**)&res));

            AutoPtr<IXmlResourceParser> childParser;
            ASSERT_SUCCEEDED(res->GetLayout(layout, (IXmlResourceParser**)&childParser));

//            try {
            AutoPtr<IAttributeSet> childAttrs = Xml::AsAttributeSet(childParser);

            FAIL_RETURN_WITH_CLOSE(childParser->Next(&type));
            while (type != IXmlPullParser_START_TAG && type != IXmlPullParser_END_DOCUMENT) {
                // Empty.
                FAIL_RETURN_WITH_CLOSE(childParser->Next(&type));
            }

            if (type != IXmlPullParser_START_TAG) {
//                throw new InflateException(childParser.getPositionDescription() +
//                        ": No start tag found!");
                childParser->Close();
                return E_INFLATE_EXCEPTION;
            }

            String childName;
            FAIL_RETURN_WITH_CLOSE(childParser->GetName(&childName));

            if (!String(TAG_MERGE).Compare(childName)) {
                // Inflate all children.
                FAIL_RETURN_WITH_CLOSE(RInflate(childParser, parent, childAttrs));
            } else {
                AutoPtr<IView> view;
                FAIL_RETURN_WITH_CLOSE(CreateViewFromTag(childName, childAttrs, (IView**)&view));

                // We try to load the layout params set in the <include /> tag. If
                // they don't exist, we will rely on the layout params set in the
                // included XML file.
                // During a layoutparams generation, a runtime exception is thrown
                // if either layout_width or layout_height is missing. We catch
                // this exception and set localParams accordingly: true means we
                // successfully loaded layout params from the <include /> tag,
                // false means we need to rely on the included layout params.
                AutoPtr<IViewGroupLayoutParams> params;
//                try {
                ECode ec = group->GenerateLayoutParams(attrs, (IViewGroupLayoutParams**)&params);
                if (ec == (Int32)E_RUNTIME_EXCEPTION) {
                    group->GenerateLayoutParams(childAttrs, (IViewGroupLayoutParams**)&params);
                }
//                } catch (RuntimeException e) {
//                    params = group.generateLayoutParams(childAttrs);
//                } finally {
                if (params != NULL) {
                    view->SetLayoutParams(params.Get());
                }
//                }

                // Inflate all children.
                FAIL_RETURN_WITH_CLOSE(RInflate(childParser, view, childAttrs));

                // Attempt to override the included layout's android:id with the
                // one set on the <include /> tag itself.
                AutoPtr<ITypedArray> a;
                //todo:
                ArrayOf_<Int32, 1> arr;
                mContext->ObtainStyledAttributesEx3(attrs,
                    arr/*com.android.internal.R.styleable.View*/, 0, 0, (ITypedArray**)&a);
                Int32 id;
                a->GetResourceId(8/*com.android.internal.R.styleable.View_id*/, View_NO_ID, &id);
                // While we're at it, let's try to override android:visibility.
                Int32 visibility;
                a->GetInt32(0/*com.android.internal.R.styleable.View_visibility*/, -1, &visibility);
                a->Recycle();

                if (id != View_NO_ID) {
                    view->SetId(id);
                }

                switch (visibility) {
                    case 0:
                        view->SetVisibility(View_VISIBLE);
                        break;
                    case 1:
                        view->SetVisibility(View_INVISIBLE);
                        break;
                    case 2:
                        view->SetVisibility(View_GONE);
                        break;
                }

                group->AddView(view);
            }
//            } finally {
//                childParser.close();
//            }
        }
    } else {
//        throw new InflateException("<include /> can only be used inside of a ViewGroup");
        return E_INFLATE_EXCEPTION;
    }

    Int32 currentDepth, depth;
    FAIL_RETURN(parser->GetDepth(&currentDepth));
    FAIL_RETURN(parser->Next(&type));
    FAIL_RETURN(parser->GetDepth(&depth));
    while ((type != IXmlPullParser_END_TAG ||
            depth > currentDepth) &&
            type != IXmlPullParser_END_DOCUMENT) {
        // Empty
        FAIL_RETURN(parser->Next(&type));
        FAIL_RETURN(parser->GetDepth(&depth));
    }
    return NOERROR;
}

ECode LayoutInflater::Init(
    /* [in] */ IContext* context)
{
    mContext = context;
    return NOERROR;
}
