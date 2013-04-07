
#include "widget/CViewGroupAction.h"

ECode CViewGroupAction::Apply(
    /* [in] */ IView* root)
{
    AutoPtr<IContext> context;
    FAIL_RETURN(root->GetContext((IContext**)&context));

    AutoPtr<IView> temp;
    FAIL_RETURN(root->FindViewById(mViewId, (IView**)&temp));
    IViewGroup* target = (IViewGroup*)temp->Probe(EIID_IViewGroup);
    if (target == NULL) return E_NO_INTERFACE;

    if (mNestedViews != NULL) {
        // Inflate nested views and add as children
        AutoPtr<IView> view;
        FAIL_RETURN(mNestedViews->Apply(context, target, (IView**)&view));
        FAIL_RETURN(target->AddView(view));
    } else if (target != NULL) {
        // Clear all children when nested views omitted
        target->RemoveAllViews();
    }

    return NOERROR;
}

ECode CViewGroupAction::ReadFromParcel(
    /* [in] */ IParcel * pSource)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CViewGroupAction::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(dest->WriteInt32(ViewGroupAction_TAG));
    FAIL_RETURN(dest->WriteInt32(mViewId));
    // TODO: ALEX need IParcel::writeParcelable
    return E_NOT_IMPLEMENTED;
    // dest.writeParcelable(nestedViews, 0 /* no flags */);
}

ECode CViewGroupAction::constructor(
    /* [in] */ Int32 viewId,
    /* [in] */ IRemoteViews* nestedViews)
{
    mViewId = viewId;
    mNestedViews = nestedViews;

    return NOERROR;
}

ECode CViewGroupAction::constructor(
    /* [in] */ IParcel* parcel)
{
    FAIL_RETURN(parcel->ReadInt32(&mViewId));
    // TODO: ALEX need IParcel::readParcelable
    return E_NOT_IMPLEMENTED;
    // nestedViews = parcel.readParcelable(null);
}

