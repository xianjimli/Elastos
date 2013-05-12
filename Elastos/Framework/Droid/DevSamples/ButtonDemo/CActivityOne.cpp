
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
    printf("==== File: %s, Line: %d ====\n", __FILE__, __LINE__);


    return NOERROR;
}

ECode CActivityOne::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    SetContentView(0x7f030000);

    AutoPtr<IView> view = FindViewById(0x7f050000);
    assert(view != NULL);

    AutoPtr<MyListener> l = new MyListener(this);
    view->SetOnClickListener(l.Get());

    AutoPtr<ILocalLocationManager> lm;
    GetSystemService(
        Context_LOCATION_SERVICE, (IInterface**)&lm);

    AutoPtr<ILocation> loc;
    lm->GetLastKnownLocation(String("gps"), (ILocation**)&loc);
    Double latitude, longitude;
    loc->GetLatitude(&latitude);
    loc->GetLongitude(&longitude);
printf("==== File: %s, Line: %d, latitude: %f, longitude: %f ====\n", __FILE__, __LINE__, latitude, longitude);

    AutoPtr<ILocalCapsuleManager> cm;
    GetCapsuleManager((ILocalCapsuleManager**)&cm);

    AutoPtr<IObjectContainer> caps;
    cm->GetInstalledCapsules(0, (IObjectContainer**)&caps);
    AutoPtr<IObjectEnumerator> it;
    caps->GetObjectEnumerator((IObjectEnumerator**)&it);
    Boolean succeeded;
    while (it->MoveNext(&succeeded), succeeded) {
        AutoPtr<IInterface> obj;
        it->Current((IInterface**)&obj);
        String capName;
        ICapsuleInfo::Probe(obj)->GetCapsuleName(&capName);
        printf("==== File: %s, Line: %d, capName: %s ====\n", __FILE__, __LINE__, capName.string());
    }

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
