
#include "CActivityOne.h"
#include "Uri.h"
#include <elastos/AutoPtr.h>
#include <stdio.h>

CActivityOne::MyListener::MyListener(
    /* [in] */ CActivityOne* host) :
    mHost(host),
    mRef(0)
{
}

PInterface CActivityOne::MyListener::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IViewOnTouchListener*)this;
    }
    else if (riid == EIID_IViewOnTouchListener) {
        return (IViewOnTouchListener*)this;
    }
    else if (riid == EIID_IViewOnKeyListener) {
        return (IViewOnKeyListener*)this;
    }

    return NULL;
}

UInt32 CActivityOne::MyListener::AddRef()
{
    // atomic_inc of android bionic C library will return
    // the old value of mRef before it is increased
    Int32 ref = atomic_inc(&mRef);
    // so we should increase ref before return
    return ++ref;
}

UInt32 CActivityOne::MyListener::Release()
{
    // atomic_inc of android bionic C library will return
    // the old value of mRef before it is decreased
    Int32 ref = atomic_dec(&mRef);
    // so we should decrease ref
    if (--ref == 0) {
        delete this;
    }
    assert(ref >= 0);
    return ref;
}

ECode CActivityOne::MyListener::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IViewOnTouchListener*)this) {
        *pIID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CActivityOne::MyListener::OnClick(
    /* [in] */ IView* v)
{

    return NOERROR;
}

static const Int32 COUNTRIES_LEN = 15;
static const String COUNTRIES[] = {
    String("Afghanistan"), String("Albania"), String("Algeria"),
    String("American Samoa"), String("Andorra"), String("aaa"),
    String("aab"), String("aac"), String("aad"),
    String("aaa bbb"), String("aaa bbc"), String("aaa bbd"),
    String("aaa bbb ccc"), String("aaa bbb ccd"), String("aaa bbb cce")};

ECode CActivityOne::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    SetContentView(0x7f030001);

    AutoPtr<IView> view = FindViewById(0x7f050000);
    assert(view != NULL);

    AutoPtr<IObjectContainer> strs;
    CParcelableObjectContainer::New((IObjectContainer**)&strs);

    AutoPtr<ICharSequence> cs;
    for (Int32 i=0; i<COUNTRIES_LEN; i++) {
        CStringWrapper::New(COUNTRIES[i], (ICharSequence**)&cs);
        strs->Add(cs);
        cs = NULL;
    }

    AutoPtr<IArrayAdapter> adapter;
    CArrayAdapter::New(this, 0x7f030000, strs, (IArrayAdapter**)&adapter);
    assert(adapter != NULL);

    IAutoCompleteTextView::Probe(view)->SetAdapter(adapter);

    return NOERROR;
}

ECode CActivityOne::OnStart()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnResume()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnPause()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnStop()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnDestroy()
{
    // TODO: Add your code here
    return NOERROR;
}

ECode CActivityOne::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *data)
{
    return NOERROR;
}
