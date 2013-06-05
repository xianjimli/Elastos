
#include "CActivityOne.h"
#include "Uri.h"
#include <elastos/AutoPtr.h>
#include <stdio.h>

const Int32 CONTEXTMENU_ID_BASE =  0x00000100;
const Int32 OPTIONSMENU_ID_BASE =  0x00001000;

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

ECode CActivityOne::OnOptionsItemSelected(
        /* [in] */ IMenuItem* item,
        /* [out] */ Boolean* res)
{
    AutoPtr<ICharSequence> title;
    assert(item != NULL);
    item->GetTitle((ICharSequence**) &title);
    assert(title != NULL);
    String str;
    title->ToString(&str);
printf("File=[%s], Line = [%d], FUNC = [%s], str=[%s]\n", __FILE__, __LINE__, __FUNCTION__, (const char*)str);
    return Activity::OnOptionsItemSelected(item, res);
}

ECode CActivityOne::OnMenuItemSelected(
    /* [in] */ Int32 featureId,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* toFinish)
{
    AutoPtr<ICharSequence> title;
    assert(item != NULL);
    item->GetTitle((ICharSequence**) &title);
    assert(title != NULL);
    String str;
    title->ToString(&str);
printf("File=[%s], Line = [%d], FUNC = [%s], str=[%s]\n", __FILE__, __LINE__, __FUNCTION__, (const char*)str);
    return Activity::OnMenuItemSelected(featureId, item, toFinish);
}

ECode CActivityOne::OnContextItemSelected(
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* res)
{
    AutoPtr<ICharSequence> title;
    assert(item != NULL);
    item->GetTitle((ICharSequence**) &title);
    assert(title != NULL);
    String str;
    title->ToString(&str);
printf("File=[%s], Line = [%d], FUNC = [%s], str=[%s]\n", __FILE__, __LINE__, __FUNCTION__, (const char*)str);
    return NOERROR;
}

ECode CActivityOne::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu,
    /* [in] */ IView* v,
    /* [in] */ IContextMenuInfo* menuInfo)
{
printf("==== File: %s, Line: %d ====, FUNC : %s.\n", __FILE__, __LINE__, __FUNCTION__);
    AutoPtr<IMenuItem> item = NULL;
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(String("ctxItem1"), (ICharSequence**)&csq);
    menu->AddEx2(0, CONTEXTMENU_ID_BASE + 1, CONTEXTMENU_ID_BASE + 1, csq, (IMenuItem**) &item);

    CStringWrapper::New(String("ctxItem2"), (ICharSequence**)&csq);
    menu->AddEx2(0, CONTEXTMENU_ID_BASE + 2, CONTEXTMENU_ID_BASE + 2, csq, (IMenuItem**) &item);

    CStringWrapper::New(String("ctxItem3"), (ICharSequence**)&csq);
    menu->AddEx2(0, CONTEXTMENU_ID_BASE + 3, CONTEXTMENU_ID_BASE + 3, csq, (IMenuItem**) &item);

    CStringWrapper::New(String("ctxItem4"), (ICharSequence**)&csq);
    menu->AddEx2(0, CONTEXTMENU_ID_BASE + 4, CONTEXTMENU_ID_BASE + 4, csq, (IMenuItem**) &item);

    CStringWrapper::New(String("ctxItem5"), (ICharSequence**)&csq);
    menu->AddEx2(0, CONTEXTMENU_ID_BASE + 5, CONTEXTMENU_ID_BASE + 5, csq, (IMenuItem**) &item);

    CStringWrapper::New(String("ctxItem6"), (ICharSequence**)&csq);
    menu->AddEx2(0, CONTEXTMENU_ID_BASE + 6, CONTEXTMENU_ID_BASE + 6, csq, (IMenuItem**) &item);

    CStringWrapper::New(String("ctxItem7"), (ICharSequence**)&csq);
    menu->AddEx2(0, CONTEXTMENU_ID_BASE + 7, CONTEXTMENU_ID_BASE + 7, csq, (IMenuItem**) &item);

    return NOERROR;
}

ECode CActivityOne::OnCreateOptionsMenu(
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* b)
{
printf("File=[%s], Line = [%d], FUNC = [%s]\n", __FILE__, __LINE__, __FUNCTION__);
    AutoPtr<ICharSequence> text;
    AutoPtr<IMenuItem> item;
    AutoPtr<ISubMenu> subMenu;

    CStringWrapper::New(String("delete"), (ICharSequence**)&text);
    menu->AddEx2(0, OPTIONSMENU_ID_BASE + 1, OPTIONSMENU_ID_BASE + 1, text, (IMenuItem**)&item);
    item->SetIconEx(0x7f020000 /*icon*/);

    CStringWrapper::New(String("save"), (ICharSequence**)&text);
    menu->AddEx2(0, OPTIONSMENU_ID_BASE + 2, OPTIONSMENU_ID_BASE + 2, text, (IMenuItem**)&item);

    CStringWrapper::New(String("submenu"), (ICharSequence**)&text);
    menu->AddSubMenuEx2(0, OPTIONSMENU_ID_BASE + 3, OPTIONSMENU_ID_BASE + 3, text, (ISubMenu**)&subMenu);
    CStringWrapper::New(String("SubmenuItem1"), (ICharSequence**)&text);
    subMenu->AddEx2(1, OPTIONSMENU_ID_BASE + 4, OPTIONSMENU_ID_BASE + 4, text, (IMenuItem**)&item);
    CStringWrapper::New(String("SubmenuItem2"), (ICharSequence**)&text);
    subMenu->AddEx2(1, OPTIONSMENU_ID_BASE + 5, OPTIONSMENU_ID_BASE + 5, text, (IMenuItem**)&item);

    CStringWrapper::New(String("detail"), (ICharSequence**)&text);
    menu->AddEx2(0, OPTIONSMENU_ID_BASE + 6, OPTIONSMENU_ID_BASE + 6, text, (IMenuItem**)&item);

    CStringWrapper::New(String("send"), (ICharSequence**)&text);
    menu->AddEx2(0, OPTIONSMENU_ID_BASE + 7, OPTIONSMENU_ID_BASE + 7, text, (IMenuItem**)&item);

    CStringWrapper::New(String("read"), (ICharSequence**)&text);
    menu->AddEx2(0, OPTIONSMENU_ID_BASE + 8, OPTIONSMENU_ID_BASE + 8, text, (IMenuItem**)&item);

    CStringWrapper::New(String("add"), (ICharSequence**)&text);
    menu->AddEx2(0, OPTIONSMENU_ID_BASE + 9, OPTIONSMENU_ID_BASE + 9, text, (IMenuItem**)&item);
printf("File=[%s], Line = [%d], FUNC = [%s]\n", __FILE__, __LINE__, __FUNCTION__);
    return NOERROR;
}

ECode CActivityOne::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
printf("File=[%s], Line = [%d], FUNC = [%s]\n", __FILE__, __LINE__, __FUNCTION__);
    SetContentView(0x7f030000);

    AutoPtr<IView> view = FindViewById(0x7f050000 /*textView*/);
    assert(view != NULL);
    RegisterForContextMenu(view);

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
