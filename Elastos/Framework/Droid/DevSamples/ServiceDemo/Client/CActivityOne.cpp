
#include "CActivityOne.h"
#include "CUri.h"
#include<elastos/AutoPtr.h>
#include <stdio.h>

ECode CActivityOne::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    SetContentView("/res/layout/layout1.xml");
    AutoPtr<CButton> startButton = (CButton *)GetControlById("btn_start");
    CButton::AddClickCallback(
            startButton, this, &CActivityOne::OnClick);

    AutoPtr<CButton> stopButton = (CButton *)GetControlById("btn_stop");
    CButton::AddClickCallback(
            stopButton, this, &CActivityOne::OnClick1);

    AutoPtr<CButton> bindButton = (CButton *)GetControlById("btn_bind");
    CButton::AddClickCallback(
            bindButton, this, &CActivityOne::OnClick2);

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

ECode CActivityOne::OnClick(
    /* [in] */ Handle32 serverHandle)
{
    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);
    intent->SetClassName("MyService", "CMyService");
    AutoPtr<IComponentName>  comName;
    return StartService(intent, (IComponentName**)&comName);
}

ECode CActivityOne::OnClick1(
    /* [in] */ Handle32 serverHandle)
{
    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);
    intent->SetClassName("MyService", "CMyService");
    Boolean successed;
    return StopService(intent, &successed);
}

ECode CActivityOne::OnClick2(
    /* [in] */ Handle32 serverHandle)
{
    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);
    intent->SetClassName("MyService", "CMyService");

    AutoPtr<CActivityOneServiceConnection> con;
    CActivityOneServiceConnection::New((IServiceConnection**)&con);
    Boolean successed;
    return BindService(intent, con, Context_BIND_AUTO_CREATE, &successed);
}

ECode CActivityOne::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent *data)
{
    return NOERROR;
}
