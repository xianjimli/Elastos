
#include "impl/CPhoneLayoutInflater.h"
#include <elastos/AutoPtr.h>
#include <widget/CImageView.h>
#include "graphics/CBitmapDrawable.h"

ECode CPhoneLayoutInflater::constructor(
    /* [in] */ IContext* context)
{
    return LayoutInflater::Init(context);
}

ECode CPhoneLayoutInflater::CloneInContext(
    /* [in] */ IContext* newContext,
    /* [out] */ ILayoutInflater** newInflater)
{
    VALIDATE_NOT_NULL(newInflater);
    return CPhoneLayoutInflater::New(newContext, newInflater);
}

ECode CPhoneLayoutInflater::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context);

    *context = LayoutInflater::GetContext();
    if (*context != NULL) (*context)->AddRef();
    return NOERROR;
}

ECode CPhoneLayoutInflater::GetFactory(
    /* [out] */ ILayoutInflaterFactory** factory)
{
//    VALIDATE_NOT_NULL(factory);
//
//    return LayoutInflater::GetFactory(factory);
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneLayoutInflater::SetFactory(
    /* [in] */ ILayoutInflaterFactory* factory)
{
//    return LayoutInflater::SetFactory(factory);
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneLayoutInflater::GetFilter(
    /* [out] */ ILayoutInflaterFilter** filter)
{
//    VALIDATE_NOT_NULL(filter);
//
//    return LayoutInflater::GetFilter(filter);
    return E_NOT_IMPLEMENTED;
}

ECode CPhoneLayoutInflater::SetFilter(
    /* [in] */ ILayoutInflaterFilter* filter)
{
//    return LayoutInflater::SetFilter(filter);
    return E_NOT_IMPLEMENTED;
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
ECode CPhoneLayoutInflater::Inflate(
    /* [in] */ Int32 resource,
    /* [in] */ IViewGroup* root,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    return LayoutInflater::Inflate(resource, root, view);
}

ECode CPhoneLayoutInflater::InflateEx(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IViewGroup* root,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    return LayoutInflater::InflateEx(parser, root, view);
}

ECode CPhoneLayoutInflater::InflateEx2(
    /* [in] */ Int32 resource,
    /* [in] */ IViewGroup* root,
    /* [in] */ Boolean attachToRoot,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    return LayoutInflater::InflateEx2(resource, root, attachToRoot, view);
}

ECode CPhoneLayoutInflater::InflateEx3(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IViewGroup* root,
    /* [in] */ Boolean attachToRoot,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    return LayoutInflater::InflateEx3(parser, root, attachToRoot, view);
}
