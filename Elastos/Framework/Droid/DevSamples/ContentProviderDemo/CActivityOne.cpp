
#include "CActivityOne.h"
#include<stdio.h>

#include <elastos/AutoPtr.h>
#include <frameworkext.h>
#include <curi.h>

ECode CActivityOne::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{

//	sp<IBar> bar;
//	sp<IFoo> bar2;
//	sp<IFoo> bar3;
//	CFooBar::New((IBar**)&bar);
//	bar2 = (IFoo*)bar->Probe(EIID_IFoo);
//	bar->Bar1(const_cast<IFoo*>((const IFoo*)bar2), (IFoo**)&bar3);

//    serviceManager->Test(ECLSID_CActivityOne);

    SetContentView("/res/layout/layout1.xml");

    CLabel *pLabel1 = (CLabel*)GetControlById("lab1");
    pLabel1->SetTextAlignment(TextAlign_HCenter | TextAlign_VCenter);

    CLabel *pLabel2 = (CLabel*)GetControlById("lab2");
    pLabel2->SetTextAlignment(TextAlign_HCenter | TextAlign_VCenter);

    CButton *pButton1 = (CButton *)GetControlById("btn1");
    CButton::AddClickCallback(
            pButton1, this, &CActivityOne::OnClick);

    CButton *pButton2 = (CButton *)GetControlById("btn2");
    CButton::AddClickCallback(
            pButton2, this, &CActivityOne::OnClick2);

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
    CButton *pButton1 = (CButton*)GetControlById("btn1");
    CButton::RemoveAllCallbacks(pButton1);

    CButton *pButton2 = (CButton *)GetControlById("btn2");
    CButton::RemoveAllCallbacks(pButton2);

    return NOERROR;
}

ECode CActivityOne::OnClick(
    /* [in] */ Handle32 serverHandle)
{
    AutoPtr<IContentResolver> resolver;
    ECode ec = GetContentResolver((IContentResolver**)&resolver);
    if (FAILED(ec)) {
        return ec;
    }

    AutoPtr<IUri> uri;
    AutoPtr<ICursor> cursor;
    String type;
    ec = CUri::Parse("content://CContentProviderOne", (IUri**)&uri);
    if (FAILED(ec)) {
        return ec;
    }

    ArrayOf_<String, 0> arr;
    ec = resolver->Query((IUri*)uri, arr, "", arr, "", (ICursor**)&cursor);
    if (FAILED(ec)) {
        return ec;
    }
//
//    Int32 count = 0;
//    cursor->GetCount(&count);
//    printf("!!!!!!!!!!!!!!!!! count:%d\n", count);
    return NOERROR;
}

ECode CActivityOne::OnClick2(
    /* [in] */ Handle32 serverHandle)
{
    IIntent *pIntent;
    CIntent::New(&pIntent);
    pIntent->SetAction("com.kortide.userevents.ALARM");
    return SendBroadcast(pIntent);
}

ECode CActivityOne::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *pData)
{
    IUri *pUri;
    String uriString;
    pData->GetData(&pUri);
    pUri->GetDescription(&uriString);
    CLabel *pLabel = (CLabel *)GetControlById("lab2");
    pLabel->SetText(uriString);

    return NOERROR;
}
