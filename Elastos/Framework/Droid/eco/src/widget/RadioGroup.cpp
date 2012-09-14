
#include "widget/RadioGroup.h"
#include "widget/CRadioButton.h"
#include "widget/CRadioGroupLayoutParams.h"

RadioGroup::CheckedStateTracker::CheckedStateTracker(
    /* [in] */ RadioGroup* host)
    : mHost(host)
{}

PInterface RadioGroup::CheckedStateTracker::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ICompoundButtonOnCheckedChangeListener*)this;
    }
    else if (riid == EIID_ICompoundButtonOnCheckedChangeListener) {
        return (ICompoundButtonOnCheckedChangeListener*)this;
    }
    return NULL;
}

UInt32 RadioGroup::CheckedStateTracker::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 RadioGroup::CheckedStateTracker::Release()
{
    return ElRefBase::Release();
}

ECode RadioGroup::CheckedStateTracker::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(ICompoundButtonOnCheckedChangeListener*)this) {
        *pIID = EIID_ICompoundButtonOnCheckedChangeListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode RadioGroup::CheckedStateTracker::OnCheckedChanged(
    /* [in] */ ICompoundButton* buttonView,
    /* [in] */ Boolean isChecked)
{
    // prevents from infinite recursion
    if (mHost->mProtectFromCheckedChange) {
        return NOERROR;
    }

    mHost->mProtectFromCheckedChange = TRUE;
    if (mHost->mCheckedId != -1) {
        mHost->SetCheckedStateForView(mHost->mCheckedId, FALSE);
    }
    mHost->mProtectFromCheckedChange = FALSE;

    Int32 id;
    buttonView->GetId(&id);
    mHost->SetCheckedId(id);

    return NOERROR;
}

RadioGroup::PassThroughHierarchyChangeListener::PassThroughHierarchyChangeListener(
    /* [in] */ RadioGroup* host)
    : mHost(host)
{}

PInterface RadioGroup::PassThroughHierarchyChangeListener::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IViewGroupOnHierarchyChangeListener*)this;
    }
    else if (riid == EIID_IViewGroupOnHierarchyChangeListener) {
        return (IViewGroupOnHierarchyChangeListener*)this;
    }
    return NULL;
}

UInt32 RadioGroup::PassThroughHierarchyChangeListener::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 RadioGroup::PassThroughHierarchyChangeListener::Release()
{
    return ElRefBase::Release();
}

ECode RadioGroup::PassThroughHierarchyChangeListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IViewGroupOnHierarchyChangeListener*)this) {
        *pIID = EIID_IViewGroupOnHierarchyChangeListener;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

/**
 * {@inheritDoc}
 */
ECode RadioGroup::PassThroughHierarchyChangeListener::OnChildViewAdded(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    if (parent == (IView*)mHost->Probe(EIID_IView)
        && IRadioButton::Probe(child)) {
        Int32 id;
        child->GetId(&id);
        // generates an id if it's missing
        if (id == View_NO_ID) {
            //child->HashCode(&id);
            child->SetId(id);
        }
        ((CRadioButton*)IRadioButton::Probe(child))->SetOnCheckedChangeWidgetListener(
            mHost->mChildOnCheckedChangeListener);
    }

    if (mOnHierarchyChangeListener != NULL) {
        mOnHierarchyChangeListener->OnChildViewAdded(parent, child);
    }

    return NOERROR;
}

/**
 * {@inheritDoc}
 */
ECode RadioGroup::PassThroughHierarchyChangeListener::OnChildViewRemoved(
    /* [in] */ IView* parent,
    /* [in] */ IView* child)
{
    if (parent == (IView*)mHost->Probe(EIID_IView)
        && IRadioButton::Probe(child)) {
        ((CRadioButton*)IRadioButton::Probe(child))->SetOnCheckedChangeWidgetListener(NULL);
    }

    if (mOnHierarchyChangeListener != NULL) {
        mOnHierarchyChangeListener->OnChildViewRemoved(parent, child);
    }

    return NOERROR;
}

RadioGroup::RadioGroup()
    : mCheckedId(-1)
    , mProtectFromCheckedChange(FALSE)
{}

/**
 * {@inheritDoc}
 */
RadioGroup::RadioGroup(
    /* [in] */ IContext* context)
    : LinearLayout(context)
    , mCheckedId(-1)
    , mProtectFromCheckedChange(FALSE)
{
    SetOrientation(VERTICAL);
    InitInternal();
}

/**
 * {@inheritDoc}
 */
RadioGroup::RadioGroup(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mCheckedId(-1)
    , mProtectFromCheckedChange(FALSE)
{
    Init(context, attrs);
}

void RadioGroup::InitInternal()
{
    mChildOnCheckedChangeListener = new CheckedStateTracker(this);
    mPassThroughListener = new PassThroughHierarchyChangeListener(this);
    LinearLayout::SetOnHierarchyChangeListener(mPassThroughListener.Get());
}

/**
 * {@inheritDoc}
 */
//@Override
ECode RadioGroup::SetOnHierarchyChangeListener(
    /* [in] */ IViewGroupOnHierarchyChangeListener* listener)
{
    // the user listener is delegated to our pass-through listener
    mPassThroughListener->mOnHierarchyChangeListener = listener;
    return NOERROR;
}

/**
 * {@inheritDoc}
 */
//@Override
ECode RadioGroup::OnFinishInflate()
{
    LinearLayout::OnFinishInflate();

    // checks the appropriate radio button as requested in the XML file
    if (mCheckedId != -1) {
        mProtectFromCheckedChange = TRUE;
        SetCheckedStateForView(mCheckedId, TRUE);
        mProtectFromCheckedChange = FALSE;
        SetCheckedId(mCheckedId);
    }

    return NOERROR;
}

//@Override
ECode RadioGroup::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (IRadioButton::Probe(child)) {
        Boolean isChecked;
        ICheckable::Probe(child)->IsChecked(&isChecked);
        if (isChecked) {
            mProtectFromCheckedChange = TRUE;
            if (mCheckedId != -1) {
                SetCheckedStateForView(mCheckedId, FALSE);
            }
            mProtectFromCheckedChange = FALSE;
            Int32 id;
            IRadioButton::Probe(child)->GetId(&id);
            SetCheckedId(id);
        }
    }

    return LinearLayout::AddView(child, index, params);
}

/**
 * <p>Sets the selection to the radio button whose identifier is passed in
 * parameter. Using -1 as the selection identifier clears the selection;
 * such an operation is equivalent to invoking {@link #clearCheck()}.</p>
 *
 * @param id the unique id of the radio button to select in this group
 *
 * @see #getCheckedRadioButtonId()
 * @see #clearCheck()
 */
ECode RadioGroup::Check(
    /* [in] */ Int32 id)
{
    // don't even bother
    if (id != -1 && (id == mCheckedId)) {
        return NOERROR;
    }

    if (mCheckedId != -1) {
        SetCheckedStateForView(mCheckedId, FALSE);
    }

    if (id != -1) {
        SetCheckedStateForView(id, TRUE);
    }

    SetCheckedId(id);

    return NOERROR;
}

void RadioGroup::SetCheckedId(
    /* [in] */ Int32 id)
{
    mCheckedId = id;
    if (mOnCheckedChangeListener != NULL) {
        mOnCheckedChangeListener->OnCheckedChanged(
            (IRadioGroup*)this->Probe(EIID_IRadioGroup), mCheckedId);
    }
}

void RadioGroup::SetCheckedStateForView(
    /* [in] */ Int32 viewId,
    /* [in] */ Boolean checked)
{
    AutoPtr<IView> checkedView = FindViewById(viewId);
    if (checkedView != NULL && IRadioButton::Probe(checkedView))
    {
        ICheckable::Probe(checkedView)->SetChecked(checked);
    }
}

/**
 * <p>Returns the identifier of the selected radio button in this group.
 * Upon empty selection, the returned value is -1.</p>
 *
 * @return the unique id of the selected radio button in this group
 *
 * @see #check(Int32)
 * @see #clearCheck()
 */
Int32 RadioGroup::GetCheckedRadioButtonId()
{
    return mCheckedId;
}

/**
 * <p>Clears the selection. When the selection is cleared, no radio button
 * in this group is selected and {@link #getCheckedRadioButtonId()} returns
 * null.</p>
 *
 * @see #check(Int32)
 * @see #getCheckedRadioButtonId()
 */
ECode RadioGroup::ClearCheck()
{
    return Check(-1);
}

/**
 * <p>Register a callback to be invoked when the checked radio button
 * changes in this group.</p>
 *
 * @param listener the callback to call on checked state change
 */
ECode RadioGroup::SetOnCheckedChangeListener(
    /* [in] */ IRadioGroupOnCheckedChangeListener* listener)
{
    mOnCheckedChangeListener = listener;

    return NOERROR;
}

/**
 * {@inheritDoc}
 */
//@Override
AutoPtr<IViewGroupLayoutParams> RadioGroup::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<IRadioGroupLayoutParams> params;
    CRadioGroupLayoutParams::New(
        GetContext(), attrs, (IRadioGroupLayoutParams**)&params);
    return IViewGroupLayoutParams::Probe(params.Get());
}

/**
 * {@inheritDoc}
 */
//@Override
Boolean RadioGroup::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return p != NULL && IRadioGroupLayoutParams::Probe(p) != NULL;
}

//@Override
ECode RadioGroup::GenerateDefaultLayoutParams(
    /* [out] */ ILinearLayoutLayoutParams** params)
{
    AutoPtr<IRadioGroupLayoutParams> p;
    CRadioGroupLayoutParams::New(
        ViewGroupLayoutParams_WRAP_CONTENT, ViewGroupLayoutParams_WRAP_CONTENT,
        (IRadioGroupLayoutParams**)&p);

    *params = ILinearLayoutLayoutParams::Probe(p.Get());
    if (*params) {
        (*params)->AddRef();
    }

    return NOERROR;
}

ECode RadioGroup::Init(
    /* [in] */ IContext* context)
{
    FAIL_RETURN(LinearLayout::Init(context));
    SetOrientation(VERTICAL);
    InitInternal();

    return NOERROR;
}

static Int32 R_Styleable_RadioGroup[] = {
    0x010100c4, 0x01010148
};

ECode RadioGroup::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(LinearLayout::Init(context, attrs));

    // retrieve selected radio button as requested by the user in the
    // XML layout file
    AutoPtr<ITypedArray> attributes;
    context->ObtainStyledAttributesEx3(
            attrs, ArrayOf<Int32>(R_Styleable_RadioGroup, 2)/*com.android.internal.R.styleable.RadioGroup*/,
            0x0101007e/*com.android.internal.R.attr.radioButtonStyle*/, 0,
            (ITypedArray**)&attributes);

    Int32 value;
    attributes->GetResourceId(
        1/*R.styleable.RadioGroup_checkedButton*/, View_NO_ID, &value);
    if (value != View_NO_ID) {
        mCheckedId = value;
    }

    Int32 index;
    attributes->GetInt32(
        0/*com.android.internal.R.styleable.RadioGroup_orientation*/,
        VERTICAL, &index);
    SetOrientation(index);

    attributes->Recycle();
    InitInternal();

    return NOERROR;
}
